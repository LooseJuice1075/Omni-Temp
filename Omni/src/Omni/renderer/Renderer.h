#pragma once

#include "core/Core.h"
#include "RenderCommand.h"

#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Omni
{
	class Renderer
	{
	public:

		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Camera& camera);
		static void EndScene();
		static void Flush();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static glm::vec4 GetClearColor() { return s_ClearColor; }
		static void SetClearColor(const glm::vec4& clearColor);

		// 2D Rendering

		static void DrawSprite(
			const glm::vec3& position,
			const glm::vec3& rotation,
			const glm::vec3& scale,
			const glm::vec4& color,
			int entityID = -1);
		static void DrawSprite(
			const glm::vec3& position,
			const glm::vec3& rotation,
			const glm::vec3& scale,
			const Ref<Texture2D>& texture,
			const glm::vec4& color,
			float tilingFactor,
			const glm::vec2& spriteSize,
			const glm::vec2& spritePosition,
			int entityID = -1);

		static void DrawSprite(
			const glm::mat4& transform,
			const glm::vec4& color,
			int entityID = -1);
		static void DrawSprite(
			const glm::mat4& transform,
			const Ref<Texture2D>& texture,
			const glm::vec4& color,
			float tilingFactor,
			const glm::vec2& spriteSize,
			const glm::vec2& spritePosition,
			int entityID = -1);

		// 2D Rendering

		// Stats

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t SpriteCount = 0;

			uint32_t GetTotalSpriteVertexCount() const { return SpriteCount * 4; }
			uint32_t GetTotalSpriteIndexCount() const { return SpriteCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();

		// Stats

	private:

		static void StartBatch();
		static void NextBatch();

	private:

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
		inline static glm::vec4 s_ClearColor;
	};
}
