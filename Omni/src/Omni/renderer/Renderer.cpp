#include "ompch.h"
#include "Renderer.h"

#include "UniformBuffer.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Omni
{
	struct SpriteVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// Editor Only
		int EntityID;
	};

	struct RendererData
	{
		static const uint32_t MaxSprites = 20000;
		static const uint32_t MaxVertices = MaxSprites * 4;
		static const uint32_t MaxIndices = MaxSprites * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> SpriteVertexArray;
		Ref<VertexBuffer> SpriteVertexBuffer;
		Ref<Shader> SpriteShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t SpriteIndexCount = 0;
		SpriteVertex* SpriteVertexBufferBase = nullptr;
		SpriteVertex* SpriteVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 SpriteVertexPositions[4];

		Renderer::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		OM_PROFILE_FUNCTION();

		RenderCommand::Init();

		s_Data.SpriteVertexArray = VertexArray::Create();

		s_Data.SpriteVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(SpriteVertex));
		s_Data.SpriteVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			{ ShaderDataType::Float,  "a_TilingFactor" },
			{ ShaderDataType::Int,    "a_EntityID"     },
			});
		s_Data.SpriteVertexArray->AddVertexBuffer(s_Data.SpriteVertexBuffer);

		s_Data.SpriteVertexBufferBase = new SpriteVertex[s_Data.MaxVertices];

		uint32_t* spriteIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			spriteIndices[i + 0] = offset + 0;
			spriteIndices[i + 1] = offset + 1;
			spriteIndices[i + 2] = offset + 2;

			spriteIndices[i + 3] = offset + 2;
			spriteIndices[i + 4] = offset + 3;
			spriteIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> spriteIB = IndexBuffer::Create(spriteIndices, s_Data.MaxIndices);
		s_Data.SpriteVertexArray->SetIndexBuffer(spriteIB);
		delete[] spriteIndices;

		s_Data.WhiteTexture = Texture2D::Create(TextureSpecification());
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.SpriteShader = Shader::Create("assets/shaders/Omni_Sprite.glsl");

		// Set first texture slot to 0
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.SpriteVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.SpriteVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.SpriteVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.SpriteVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);
	}

	void Renderer::Shutdown()
	{
		OM_PROFILE_FUNCTION();

		delete[] s_Data.SpriteVertexBufferBase;
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Camera& camera)
	{
		OM_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));

		StartBatch();
	}

	void Renderer::EndScene()
	{
		OM_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer::Flush()
	{
		if (s_Data.SpriteIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.SpriteVertexBufferPtr - (uint8_t*)s_Data.SpriteVertexBufferBase);
			s_Data.SpriteVertexBuffer->SetData(s_Data.SpriteVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);

			s_Data.SpriteShader->Bind();
			RenderCommand::DrawIndexed(s_Data.SpriteVertexArray, s_Data.SpriteIndexCount);
			s_Data.Stats.DrawCalls++;
		}
	}

	void Renderer::SetClearColor(const glm::vec4& clearColor)
	{
		RenderCommand::SetClearColor(clearColor);
		s_ClearColor = clearColor;
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::DrawSprite(
		const glm::vec3& position,
		const glm::vec3& rotation,
		const glm::vec3& scale,
		const glm::vec4& color,
		int entityID)
	{
		glm::mat4 calculatedRotation = glm::toMat4(glm::quat(-glm::radians(rotation)));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* calculatedRotation
			* glm::scale(glm::mat4(1.0f), scale);

		DrawSprite(transform, color, entityID);
	}

	void Renderer::DrawSprite(
		const glm::vec3& position,
		const glm::vec3& rotation,
		const glm::vec3& scale,
		const Ref<Texture2D>& texture,
		const glm::vec4& color,
		float tilingFactor,
		const glm::vec2& spriteSize,
		const glm::vec2& spritePosition,
		int entityID)
	{
		glm::mat4 calculatedRotation = glm::toMat4(glm::quat(-glm::radians(rotation)));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* calculatedRotation
			* glm::scale(glm::mat4(1.0f), scale);

		DrawSprite(transform, texture, color, tilingFactor, spriteSize, spritePosition, entityID);
	}

	void Renderer::DrawSprite(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		OM_PROFILE_FUNCTION();

		constexpr size_t spriteVertexCount = 4;
		const float textureIndex = 0.0f; // This is the White Texture ID
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (s_Data.SpriteIndexCount >= RendererData::MaxIndices)
			NextBatch();

		for (size_t i = 0; i < spriteVertexCount; i++)
		{
			s_Data.SpriteVertexBufferPtr->Position = transform * s_Data.SpriteVertexPositions[i];
			s_Data.SpriteVertexBufferPtr->Color = color;
			s_Data.SpriteVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.SpriteVertexBufferPtr->TexIndex = textureIndex;
			s_Data.SpriteVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.SpriteVertexBufferPtr->EntityID = entityID;
			s_Data.SpriteVertexBufferPtr++;
		}

		s_Data.SpriteIndexCount += 6;
		s_Data.Stats.SpriteCount++;
	}

	void Renderer::DrawSprite(
		const glm::mat4& transform,
		const Ref<Texture2D>& texture,
		const glm::vec4& color,
		float tilingFactor,
		const glm::vec2& spriteSize,
		const glm::vec2& spritePosition,
		int entityID)
	{
		OM_PROFILE_FUNCTION();

		float sheetWidth = texture->GetWidth();
		float sheetHeight = texture->GetHeight();

		// Adding this offset to the uv coords fixes the apparent texture bleeding
		float offset = 0.0000001f;

		constexpr size_t spriteVertexCount = 4;
		glm::vec2 textureCoords[] = {
			{ ((spritePosition.x * spriteSize.x) / sheetWidth) + offset, ((spritePosition.y * spriteSize.y) / sheetHeight) + offset },
			{ (((spritePosition.x + 1) * spriteSize.x) / sheetWidth) + offset, ((spritePosition.y * spriteSize.y) / sheetHeight) + offset },
			{ (((spritePosition.x + 1) * spriteSize.x) / sheetWidth) + offset, (((spritePosition.y + 1) * spriteSize.y) / sheetHeight) + offset },
			{ ((spritePosition.x * spriteSize.x) / sheetWidth) + offset, (((spritePosition.y + 1) * spriteSize.y) / sheetHeight) + offset }
		};

		if (s_Data.SpriteIndexCount >= RendererData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= RendererData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < spriteVertexCount; i++)
		{
			s_Data.SpriteVertexBufferPtr->Position = transform * s_Data.SpriteVertexPositions[i];
			s_Data.SpriteVertexBufferPtr->Color = color;
			s_Data.SpriteVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.SpriteVertexBufferPtr->TexIndex = textureIndex;
			s_Data.SpriteVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.SpriteVertexBufferPtr->EntityID = entityID;
			s_Data.SpriteVertexBufferPtr++;
		}

		s_Data.SpriteIndexCount += 6;

		s_Data.Stats.SpriteCount++;
	}

	void Renderer::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer::Statistics Renderer::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer::StartBatch()
	{
		s_Data.SpriteIndexCount = 0;
		s_Data.SpriteVertexBufferPtr = s_Data.SpriteVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer::NextBatch()
	{
		Flush();
		StartBatch();
	}
}
