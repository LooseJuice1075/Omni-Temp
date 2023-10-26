#include "SandboxApp.h"
#include "utils/PlatformUtils.h"
#include "math/Math.h"

#include <glad/glad.h>

namespace Omni
{
	SandboxLayer::SandboxLayer()
	{
		app.GetWindow().SetMSAA(msaaSamples);
		app.GetWindow().SetVSync(vsyncOn);
		app.m_CapUpdateInterval = false;

		PhysicsManager::InitDebugDraw(DrawFlag::e_shapeBit
			//| DrawFlag::e_aabbBit
			| DrawFlag::e_centerOfMassBit
			| DrawFlag::e_jointBit
			//| DrawFlag::e_pairBit
		);

		//ImGuiIO& io = ImGui::GetIO();
		//io.Fonts->AddFontFromFileTTF("fonts/MS Gothic.ttf", 14);

	#pragma region Textures
		//textures[0] = Texture2D::Create("joosesprites0.dds");
		//textures[1] = Texture2D::Create("checkerboard.dds", false);
		//textures[2] = Texture2D::Create("tilemap.dds", false);
		//textures[3] = Texture2D::Create("tilemap1.dds", false);
	#pragma endregion

	#pragma region Entities
		//sprites[0].Create(1, "Dynamic Body", textures[0], { 16, 16 }, spriteIndex);
		//sprites[0].m_Transform = { 0.0f, 0.0f, 0.0f };
	#pragma endregion

	#pragma region Camera
		//m_Camera.m_Transform = { 0.0f, 0.0f, -8.0f };
		//m_Camera.m_Rotation = { 0.0f, 0.0f, 90.0f };
		//if (m_Camera.m_CamType == CameraType::Orthographic)
		//{
		//	m_Camera.m_FOV = 2.0f;
		//	m_CamModeLabel = "Orthographic";
		//}
		//else if (m_Camera.m_CamType == CameraType::Perspective)
		//{
		//	m_Camera.m_FOV = 60.0f;
		//	m_CamModeLabel = "Perspective";
		//}
	#pragma endregion

	#pragma region Physics
		//m_PlayerBody.Create({ 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, 1.0f, 0.3f);
		//m_PlayerBody.SetGravityScale(0.0f);
		//m_PlayerBody.SetAllowSleep(false);
		//m_PlayerBody.SetFixedRot(true);
	#pragma endregion

		//------------------------

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8 };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);
		
		//------------------------

		//------------------------
		m_VertexArray = VertexArray::Create();

		float vertices[4 * 5] =
		{
			-0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
			 0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
			 0.75f,  0.75f, 0.0f, 1.0f, 1.0f,
			-0.75f,  0.75f, 0.0f, 0.0f, 1.0f,
		};

		m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

		m_IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		//------------------------

		m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Camera = Camera(CameraType::Orthographic);

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		m_Camera.SetFOV(m_CameraFOV);
		m_Camera.SetResolution(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
		m_Camera.SetProjection();

		m_Texture = Texture2D::Create("assets/textures/joosesprites0.png");
		m_CheckerboardTexture = Texture2D::Create("assets/textures/checkerboard.png");

		const Ref<Shader>& shader = m_ShaderLibrary.Get("Texture");
		shader->Bind();
		shader->SetInt("u_Texture", 0);

		glm::mat4 rotation = glm::toMat4(glm::quat({ 0.0f, 0.0f, 0.0f, 0.0f }));
		m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3({ 0.0f, 0.0f, 1.0f }))
			* rotation
			* glm::scale(glm::mat4(1.0f), glm::vec3({ 1.0f, 1.0f, 1.0f }));
	}

	SandboxLayer::~SandboxLayer()
	{
		//for (size_t i = 0; i < sizeof(textures) / sizeof(textures[0]); i++)
		//{
		//	if (textures[i] != nullptr)
		//	{
		//		textures[i]->Destroy();
		//	}
		//}

		//for (size_t i = 0; i < sizeof(sprites) / sizeof(sprites[0]); i++)
		//{
		//	sprites[i].Destroy();
		//}
	}

	void SandboxLayer::OnAttach()
	{
	}

	void SandboxLayer::OnDetach()
	{
	}

	double lastTime = glfwGetTime();
	int nbFrames = 0;

	void SandboxLayer::OnUpdate(Timestep ts)
	{
		OM_PROFILE_FUNCTION();

		float time = ts;

		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= fpsUpdateInterval)
		{
			currentFPS = 1000.0f / (1000.0f / double(nbFrames));
			msPF = 1000.0f / double(nbFrames);
			nbFrames = 0;
			lastTime += 1.0f;
		}

	#pragma region Input
		//------Character-Controls------
		if (Input::IsKeyPressed(OM_KEY_D))
		{
			//m_PlayerBody.SetLinearVelocity({ m_CameraMoveSpeed, m_PlayerBody.GetLinearVelocity().y });
		}
		else if (Input::IsKeyPressed(OM_KEY_A))
		{
			//m_PlayerBody.SetLinearVelocity({ -m_CameraMoveSpeed, m_PlayerBody.GetLinearVelocity().y });
		}
		else
		{
			//m_PlayerBody.SetLinearVelocity({ 0.0f, m_PlayerBody.GetLinearVelocity().y });
		}

		if (Input::IsKeyPressed(OM_KEY_W))
		{
			//m_PlayerBody.SetLinearVelocity({ m_PlayerBody.GetLinearVelocity().x, m_CameraMoveSpeed });
		}
		else if (Input::IsKeyPressed(OM_KEY_S))
		{
			//m_PlayerBody.SetLinearVelocity({ m_PlayerBody.GetLinearVelocity().x, -m_CameraMoveSpeed });
		}
		else
		{
			//m_PlayerBody.SetLinearVelocity({ m_PlayerBody.GetLinearVelocity().x, 0.0f });
		}
		//------Character-Controls------
	#pragma endregion

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_Camera.SetResolution(m_ViewportSize.x, m_ViewportSize.y);
		}

		Renderer::ResetStats();
		m_Framebuffer->Bind();

		Renderer::BeginScene(m_Camera);
		{
			RenderCommand::SetClearColor({ 0.2, 0.2, 0.2, 1 });
			RenderCommand::Clear();

			RenderCommand::EnableDepthTesting(false);

			const Ref<Shader>& shader = m_ShaderLibrary.Get("Texture");

			glm::mat4 rotation = glm::toMat4(glm::quat({ 0.0f, 0.0f, 0.0f, 0.0f }));
			m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3({ 0.0f, 0.0f, 1.0f }))
				* rotation
				* glm::scale(glm::mat4(1.0f), glm::vec3({ 1.0f, 1.0f, 1.0f }));

			float x = 0.0f;
			float y = 0.0f;
			for (size_t r = 0; r < m_SpriteRowsColumns.x; r++)
			{
				for (size_t c = 0; c < m_SpriteRowsColumns.y; c++)
				{
					glm::mat4 rotation = glm::toMat4(glm::quat({ 0.0f, 0.0f, 0.0f, 0.0f }));
					m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3({ x, y, 1.0f }))
						* rotation
						* glm::scale(glm::mat4(1.0f), glm::vec3({ 1.0f, 1.0f, 1.0f }));

					Renderer::DrawSprite(m_Transform, m_Texture,
						{ 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, { 16.0f, 16.0f }, { r, c });

					x += 1.5f;
				}
				x = 0.0f;
				y += 1.5f;
			}
		}
		Renderer::EndScene();

		m_Framebuffer->Unbind();

		frameCounter++;
	}

	void SandboxLayer::OnFixedUpdate()
	{
	#pragma region Physics
		//m_Camera.m_Transform.x = sprites[0].m_Transform.x;
		//m_Camera.m_Transform.y = sprites[0].m_Transform.y;

		PhysicsManager::Step(0.01666666667f);

		{
			//glm::vec2 pos = m_PlayerBody.GetTransform();
			//float angle = m_PlayerBody.GetAngle();
			//sprites[0].m_Transform.x = pos.x;
			//sprites[0].m_Transform.y = pos.y;
			//sprites[0].m_Rotation.z = -glm::degrees(angle);
		}
	#pragma endregion
	}

	void SandboxLayer::OnImGuiRender()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		io.FontGlobalScale = 1.3f;
		io.FontAllowUserScaling = true;
		style.Alpha = 1.0f;

		ImGui::DockSpaceOverViewport(0);

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Begin("Stats");

		ImGui::Text("FPS:     ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(currentFPS).c_str());

		ImGui::Text("ms/frame:");
		ImGui::SameLine();
		ImGui::Text(std::to_string(msPF).c_str());

		ImGui::Separator();

		ImGui::Text("Draw Calls:");
		ImGui::SameLine();
		ImGui::Text(std::to_string(Renderer::GetStats().DrawCalls).c_str());

		ImGui::Text("Sprite Count:");
		ImGui::SameLine();
		ImGui::Text(std::to_string(Renderer::GetStats().SpriteCount).c_str());

		ImGui::End();

		#pragma region Settings
		ImGui::Begin("Settings");

		ImGui::InputFloat2("Sprite Position", glm::value_ptr(m_SpritePosition));

		ImGui::InputFloat2("Rows / Columns", glm::value_ptr(m_SpriteRowsColumns));

		ImGui::Separator();

		if (ImGui::InputFloat3("Cam Pos", glm::value_ptr(m_CameraPosition)))
		{
			m_Camera.SetPosition(m_CameraPosition);
		}

		if (ImGui::DragFloat("Cam Pitch", &m_CameraRotation.x))
		{
			m_Camera.SetRotation(m_CameraRotation);
		}
		if (ImGui::DragFloat("Cam Yaw", &m_CameraRotation.y))
		{
			m_Camera.SetRotation(m_CameraRotation);
		}
		if (ImGui::DragFloat("Cam Roll", &m_CameraRotation.z))
		{
			m_Camera.SetRotation(m_CameraRotation);
		}

		if (ImGui::DragFloat("Cam FOV", &m_CameraFOV))
		{
			m_Camera.SetFOV(m_CameraFOV);
		}

		ImGui::End();
		#pragma endregion

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void SandboxLayer::OnEvent(Omni::Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(OM_BIND_EVENT_FN(SandboxLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseScrolledEvent>(OM_BIND_EVENT_FN(SandboxLayer::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(OM_BIND_EVENT_FN(SandboxLayer::OnWindowResized));
	}

	bool SandboxLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		//----------Shortcuts-----------
		if (e.GetKeyCode() == OM_KEY_F11)
		{
			fullscreen = !fullscreen;
			app.GetWindow().SetFullscreen(fullscreen);
		}
		//----------Shortcuts-----------
		return false;
	}

	bool SandboxLayer::OnMouseScrolled(MouseScrolledEvent& e)
	{
		//m_Camera.m_FOV += (e.GetYOffset() * 0.25f);
		return false;
	}

	bool SandboxLayer::OnWindowResized(WindowResizeEvent& e)
	{
		return false;
	}
}
