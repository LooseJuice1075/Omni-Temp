#include "EditorLayer.h"
#include "panels/Icons.h"
#include "Omni/utils/PlatformUtils.h"
#include "panels/PanelAccessor.h"
#include "math/Math.h"
#include <glm/gtx/rotate_vector.hpp>
#include <yaml-cpp/yaml.h>

namespace Omni
{
	EditorLayer::EditorLayer()
	{
		app.GetWindow().SetMSAA(m_MSAASamples);
		app.GetWindow().SetVSync(m_VsyncEnabled);
		app.m_CapUpdateInterval = false;
	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnAttach()
	{
		{
			ImGuiIO& io = ImGui::GetIO();
			io.IniFilename = "assets/imgui.ini";
			io.Fonts->AddFontFromFileTTF("assets/fonts/MS Gothic.ttf", m_FontSize);
			io.ConfigWindowsMoveFromTitleBarOnly = true;

			ImGui::GetStyle().FrameRounding = 6.0f;

			ImVec4* colors = ImGui::GetStyle().Colors;
			colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.23f, 0.13f, 0.44f, 0.86f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.39f, 0.22f, 0.74f, 0.86f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.48f, 0.27f, 0.90f, 0.86f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.05f, 0.14f, 1.00f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.08f, 0.27f, 1.00f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.13f, 0.13f, 0.13f, 0.97f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.54f, 0.30f, 1.00f, 0.86f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.59f, 0.37f, 1.00f, 0.86f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.54f, 0.30f, 1.00f, 0.86f);
			colors[ImGuiCol_Button] = ImVec4(0.59f, 0.37f, 1.00f, 0.86f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.61f, 0.42f, 1.00f, 0.86f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.54f, 0.30f, 1.00f, 0.86f);
			colors[ImGuiCol_Header] = ImVec4(0.54f, 0.30f, 1.00f, 0.86f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.61f, 0.42f, 1.00f, 0.86f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.54f, 0.30f, 1.00f, 0.86f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.38f, 0.21f, 0.71f, 0.86f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.38f, 0.21f, 0.71f, 0.86f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.54f, 0.30f, 1.00f, 0.86f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.54f, 0.30f, 1.00f, 0.86f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(0.54f, 0.30f, 1.00f, 0.86f);
			colors[ImGuiCol_Tab] = ImVec4(0.23f, 0.13f, 0.44f, 0.86f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.61f, 0.42f, 1.00f, 0.86f);
			colors[ImGuiCol_TabActive] = ImVec4(0.54f, 0.30f, 1.00f, 0.86f);
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.54f, 0.30f, 1.00f, 0.86f);
			colors[ImGuiCol_DockingPreview] = ImVec4(0.54f, 0.30f, 1.00f, 0.86f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.61f, 0.42f, 1.00f, 0.86f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(0.54f, 0.30f, 1.00f, 0.86f);
			colors[ImGuiCol_NavHighlight] = ImVec4(0.76f, 0.65f, 1.00f, 0.86f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.54f, 0.30f, 1.00f, 0.86f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.07f, 0.05f, 0.14f, 1.00f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.26f, 0.16f, 0.54f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.29f, 0.20f, 0.54f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.24f, 0.13f, 0.54f, 1.00f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.50f, 0.22f, 0.97f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.69f, 0.52f, 0.97f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.36f, 0.27f, 1.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.45f, 0.37f, 1.00f, 1.00f);
		}

		{
			Icons();
		}

		{
			m_EditorCamera = CameraController(CameraType::Orthographic);
			m_EditorCamera.SetPosition({ 0.0f, 0.0f, 2.0f });
			m_EditorCamera.SetFOV(m_CameraFOV);
			m_CameraNearClip = m_EditorCamera.GetNearClip();
			m_CameraFarClip = m_EditorCamera.GetFarClip();
			m_EditorCamera.SetMoveSpeed(m_CameraMoveSpeed);
			m_EditorCamera.SetRotationSpeed(m_CameraRotationSpeed);
		}

		{
			FramebufferSpecification fbSpec;
			fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
			fbSpec.Width = 1280;
			fbSpec.Height = 720;
			m_ViewportFrameBuffer = Framebuffer::Create(fbSpec);
		}

		m_ActiveScene = CreateRef<Scene>();

		PanelAccessor::m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		SceneSerializer serializer(m_ActiveScene);
		serializer.Deserialize("assets/scenes/Test_Scene.omscene");

		//OpenProject();

		//{
		//	YAML::Emitter out;
		//	{
		//		out << YAML::BeginMap;

		//		out << YAML::Key << "Project" << YAML::Value;
		//		{
		//			out << YAML::BeginMap;

		//			out << YAML::Key << "Name" << YAML::Value << "TestProject";
		//			out << YAML::Key << "StartScene" << YAML::Value << "Test_Scene.omscene";
		//			out << YAML::Key << "AssetDirectory" << YAML::Value << "assets";

		//			out << YAML::EndMap;
		//		}

		//		out << YAML::EndMap;
		//	}

		//	std::ofstream fout("TestProject/TestProject.omproj");
		//	if (fout.is_open())
		//	{
		//		fout << out.c_str();
		//		fout.close();
		//	}
		//}
	}

	void EditorLayer::OnDetach()
	{
		if (m_ViewportFrameBuffer)
		{
			m_ViewportFrameBuffer->~Framebuffer();
		}

		for (size_t i = 0; i < sizeof(m_Textures) / sizeof(m_Textures[0]); i++)
		{
			if (m_Textures[i] != nullptr)
			{
				m_Textures[i]->~Texture2D();
			}
		}
	}

	double lastTime = glfwGetTime();
	int nbFrames = 0;

	void EditorLayer::OnUpdate(Timestep ts)
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

		AssetManager::OnUpdate(ts);
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		if (FramebufferSpecification spec = m_ViewportFrameBuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_ViewportFrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetResolution((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		if (m_ViewportHovered || m_EditorCamera.IsInDragRotateMode())
		{
			m_EditorCamera.OnUpdate(ts);
		}

		Renderer::ResetStats();
		RenderCommand::SetViewport(0, 0, m_EditorCamera.GetResolutionX(), m_EditorCamera.GetResolutionY());
		m_ViewportFrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::Clear();

		// Clear entity ID attatchment to -1
		m_ViewportFrameBuffer->ClearAttachment(1, -1);

		m_ActiveScene->RenderScene(m_EditorCamera.GetCamera());

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_ViewportFrameBuffer->ReadPixel(1, mouseX, mouseY);
			if (pixelData == -1)
				m_HoveredEntity = {};
			else
				m_HoveredEntity = { (entt::entity)pixelData, m_ActiveScene.get() };
		}

		m_ViewportFrameBuffer->Unbind();

		// Update Scene
		m_ActiveScene->OnUpdate(ts);

		m_FrameCount++;
	}

	void EditorLayer::OnFixedUpdate()
	{
		AssetManager::OnFixedUpdate();

		// Update Scene
		m_ActiveScene->OnFixedUpdate();
		PanelAccessor::m_AssetBrowserPanel.OnFixedUpdate();

		m_FixedUpdateCount++;
	}

	void EditorLayer::OnImGuiRender()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		io.FontGlobalScale = 1.0f;
		style.Alpha = 1.0f;

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
				{
				}

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Open Scene..."))
					OpenScene();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::Checkbox("FullScreen", &m_FullscreenEnabled))
					app.GetWindow().SetFullscreen(m_FullscreenEnabled);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				ImGui::Text("View Type");
				if (m_EditorCamera.GetCameraType() == CameraType::Orthographic)
				{
					if (ImGui::Button("Perspective View"))
					{
						m_EditorCamera.SetCameraType(CameraType::Perspective);
						m_CameraFOV = 60.0f;
						m_EditorCamera.SetFOV(m_CameraFOV);
						if (m_CameraNearClip <= 0.0f)
						{
							m_CameraNearClip = 0.001f;
							m_EditorCamera.SetNearClip(m_CameraNearClip);
						}
					}
				}
				else if (m_EditorCamera.GetCameraType() == CameraType::Perspective)
				{
					if (ImGui::Button("Orthographic View"))
					{
						m_EditorCamera.SetCameraType(CameraType::Orthographic);
						m_CameraFOV = 0.2f;
						m_EditorCamera.SetFOV(m_CameraFOV);
						m_CameraNearClip = -1.0f;
						m_EditorCamera.SetNearClip(m_CameraNearClip);
						m_EditorCamera.SetRotation(glm::vec3(0.0f));
					}
				}

				ImGui::Separator();

				ImGui::Text("Move Speed");
				ImGui::PushItemWidth(ImGui::GetContentRegionMax().x / 1.3f);
				if (ImGui::InputFloat("##Move Speed", &m_CameraMoveSpeed, 1.0f, 5.0f, "%.1f"))
				{
					m_EditorCamera.SetMoveSpeed(m_CameraMoveSpeed);
				}
				ImGui::PopItemWidth();

				ImGui::Text("Rotation Speed");
				ImGui::PushItemWidth(ImGui::GetContentRegionMax().x / 1.3f);
				if (ImGui::InputFloat("##Rotation Speed", &m_CameraRotationSpeed, 1.0f, 3.0f, "%.1f"))
				{
					m_EditorCamera.SetRotationSpeed(m_CameraRotationSpeed);
				}
				ImGui::PopItemWidth();

				ImGui::Separator();

				ImGui::Text("FOV");
				ImGui::PushItemWidth(ImGui::GetContentRegionMax().x / 1.3f);
				if (m_EditorCamera.GetCameraType() == CameraType::Orthographic)
				{
					if (ImGui::InputFloat("##FOV", &m_CameraFOV, 0.1f, 0.5f, "%.2f"))
					{
						if (m_CameraFOV < 0.001f)
							m_CameraFOV = 0.001f;
						m_EditorCamera.SetFOV(m_CameraFOV);
					}
				}
				else if (m_EditorCamera.GetCameraType() == CameraType::Perspective)
				{
					if (ImGui::InputFloat("##FOV", &m_CameraFOV, 1.0f, 5.0f, "%.2f"))
					{
						if (m_CameraFOV < 0.001f)
							m_CameraFOV = 0.001f;
						m_EditorCamera.SetFOV(m_CameraFOV);
					}
				}
				ImGui::PopItemWidth();

				ImGui::Separator();

				ImGui::Text("Near Clip");
				ImGui::PushItemWidth(ImGui::GetContentRegionMax().x / 1.3f);
				if (ImGui::InputFloat("##Near Clip", &m_CameraNearClip, 0.1f, 1.0f, "%.3f"))
				{
					m_EditorCamera.SetNearClip(m_CameraNearClip);
				}
				ImGui::PopItemWidth();

				ImGui::Text("Far Clip");
				ImGui::PushItemWidth(ImGui::GetContentRegionMax().x / 1.3f);
				if (ImGui::InputFloat("##Far Clip", &m_CameraFarClip, 0.1f, 1.0f, "%.3f"))
				{
					m_EditorCamera.SetFarClip(m_CameraFarClip);
				}
				ImGui::PopItemWidth();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem("Stats", NULL, &m_StatsWindowOpen);
				ImGui::MenuItem("Viewport", NULL, &m_ViewportWindowOpen);
				ImGui::MenuItem("Game", NULL, &m_GameWindowOpen);
				ImGui::MenuItem("Hierarchy", NULL, &PanelAccessor::m_SceneHierarchyPanel.m_Open);
				ImGui::MenuItem("Properties", NULL, &PanelAccessor::m_PropertiesPanel.m_Open);
				ImGui::MenuItem("Asset Browser", NULL, &PanelAccessor::m_AssetBrowserPanel.m_Open);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				if (ImGui::MenuItem("Translate", "T"))
				{
					if (!m_UsingGizmo)
						m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				}

				if (ImGui::MenuItem("Rotate", "R"))
				{
					if (!m_UsingGizmo)
						m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				}

				if (ImGui::MenuItem("Scale", "E"))
				{
					if (!m_UsingGizmo)
						m_GizmoType = ImGuizmo::OPERATION::SCALE;
				}

				ImGui::Separator();

				ImGui::Text("Translate Snap");
				ImGui::SameLine();
				ImGui::PushItemWidth(ImGui::GetContentRegionMax().x / 2.3f);
				ImGui::InputFloat("##Translate Snap", &m_TranslateSnap, 0.5f, 1.0f, "%.1f");
				ImGui::PopItemWidth();

				ImGui::Text("Rotate Snap");
				ImGui::SameLine();
				ImGui::PushItemWidth(ImGui::GetContentRegionMax().x / 2.3f);
				ImGui::InputFloat("##Rotate Snap", &m_RotateSnap, 5.0f, 15.0f, "%.1f");
				ImGui::PopItemWidth();

				ImGui::Text("Scale Snap");
				ImGui::SameLine();
				ImGui::PushItemWidth(ImGui::GetContentRegionMax().x / 2.3f);
				ImGui::InputFloat("##Scale Snap", &m_ScaleSnap, 0.5f, 1.0f, "%.1f");
				ImGui::PopItemWidth();

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		PanelAccessor::m_SceneHierarchyPanel.OnImGuiRender();
		PanelAccessor::m_PropertiesPanel.OnImGuiRender();
		PanelAccessor::m_AssetBrowserPanel.OnImGuiRender();

		ImGui::ShowDemoWindow();

		if (m_ViewportWindowOpen)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,3 });
			ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2{ 320.0f,320.0f });
			if (ImGui::Begin("Viewport", &m_ViewportWindowOpen, m_ViewportWindowFlags))
			{
				auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
				auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
				auto viewportOffset = ImGui::GetWindowPos();
				m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
				m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

				m_ViewportFocused = ImGui::IsWindowFocused();
				m_ViewportHovered = ImGui::IsWindowHovered();

				// TODO : Make sure inputs from viewport windows can be propagated to the editor layer

				if (!m_EditorCamera.IsInDragRotateMode())
				{
					app.GetImGuiLayer()->BlockEvents(!m_ViewportHovered);
					app.GetImGuiLayer()->EnableMouse(true);
				}
				else
				{
					app.GetImGuiLayer()->BlockEvents(false);
					app.GetImGuiLayer()->EnableMouse(false);
				}

				ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

				uint64_t textureID = m_ViewportFrameBuffer->GetColorAttachmentRendererID();
				ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM_OMSCENE"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						OpenScene(AssetManager::GetAssetPath() / path);
					}

					ImGui::EndDragDropTarget();
				}

				// Gizmos
				{
					Entity selectedEntity = PanelAccessor::m_SceneHierarchyPanel.GetSelectedEntity();
					if (selectedEntity && m_GizmoType != -1)
					{
						if (m_EditorCamera.GetCameraType() == CameraType::Orthographic)
						{
							ImGuizmo::SetOrthographic(true);
						}
						else if (m_EditorCamera.GetCameraType() == CameraType::Perspective)
						{
							ImGuizmo::SetOrthographic(false);
						}
						ImGuizmo::SetDrawlist();

						ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

						const glm::mat4& cameraProj = m_EditorCamera.GetProjectionMatrix();
						glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

						// Get as mat4
						auto& tc = selectedEntity.GetComponent<TransformComponent>();
						bool entityHasParent = selectedEntity.HasParent();
						TransformComponent tcCopy = tc;
						glm::mat4 transform;
						tcCopy.Rotation = { glm::radians(-tc.Rotation) };
						transform = tcCopy.ToMat4();

						// Snapping
						bool snap = Input::IsKeyPressed(OM_KEY_LEFT_CONTROL) ||
									Input::IsKeyPressed(OM_KEY_RIGHT_CONTROL);
						float snapValue = 0.0f;
						if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE)
							snapValue = m_TranslateSnap;
						else if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
							snapValue = m_RotateSnap;
						else if (m_GizmoType == ImGuizmo::OPERATION::SCALE)
							snapValue = m_ScaleSnap;

						float snapValues[3] = { snapValue, snapValue, snapValue };

						ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProj),
							(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
							nullptr, snap ? snapValues : nullptr);

						if (ImGuizmo::IsUsing())
						{
							glm::vec3 translation, rotation, scale;
							Math::DecomposeTransform(transform, translation, rotation, scale);

							glm::vec3 deltaRotation;

							deltaRotation = glm::degrees(-rotation) - glm::degrees(-tcCopy.Rotation);
							tc.Translation = translation;
							tc.Rotation += deltaRotation;
							tc.Scale = scale;

							if (entityHasParent)
							{
								m_ActiveScene->CalculateEntityLocalTransform(selectedEntity);
							}

							selectedEntity.SetDescendantsWorldTransformDirty();

							m_UsingGizmo = true;
						}
						else
						{
							m_UsingGizmo = false;
						}
					}
				}

				ImGui::PopStyleVar(3);
				ImGui::End();
			}
			else
			{
				ImGui::PopStyleVar(3);
				ImGui::End();
			}
		}

		if (m_GameWindowOpen)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,3 });
			ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2{ 320.0f,320.0f });
			if (ImGui::Begin("Game", &m_GameWindowOpen, m_GameWindowFlags))
			{
				ImVec2 gameWindowSize = ImGui::GetContentRegionAvail();
				m_GameWindowSize = { gameWindowSize.x, gameWindowSize.y };

				if (m_ActiveScene->GetMainCamera())
				{
					CameraComponent* camera = m_ActiveScene->GetMainCamera();
					if (!camera->FixedAspectRatio)
					{
						if (camera->Camera.GetResolutionX() != (uint32_t)m_GameWindowSize.x ||
							camera->Camera.GetResolutionY() != (uint32_t)m_GameWindowSize.y)
						{
							camera->Camera.SetResolution(m_GameWindowSize.x, m_GameWindowSize.y);
						}
					}
					uint64_t textureID = camera->Camera.GetFramebuffer()->GetColorAttachmentRendererID();
					ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ (float)camera->Camera.GetResolutionX(), (float)camera->Camera.GetResolutionY() }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				}
				else
				{
					float size = ImGui::GetContentRegionAvailWidth() - 20.0f;
					if (size > 256.0f)
						size = 256.0f;

					float availX = ImGui::GetContentRegionAvail().x;
					float offX = (availX - size) * 0.5f;
					if (offX > 0.0f)
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offX);

					float availY = ImGui::GetContentRegionAvail().y;
					float offY = (availY - size) * 0.5f;
					if (offY > 0.0f)
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offY);

					ImGui::Image(reinterpret_cast<void*>(Icons::m_MissingCameraIcon->GetRendererID()), ImVec2{ size, size }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				}

				ImGui::PopStyleVar(3);
				ImGui::End();
			}
			else
			{
				ImGui::PopStyleVar(3);
				ImGui::End();
			}
		}

		if (m_StatsWindowOpen)
		{
			if (ImGui::Begin("Stats", &m_StatsWindowOpen, ImGuiWindowFlags_NoCollapse))
			{
				ImGui::Text("FPS:     ");
				ImGui::SameLine();
				ImGui::Text(std::to_string(currentFPS).c_str());
				ImGui::Text("ms/frame:");
				ImGui::SameLine();
				ImGui::Text(std::to_string(msPF).c_str());

				ImGui::Separator();

				ImGui::Text("Vsync");
				ImGui::SameLine();
				if (ImGui::Checkbox("##Vsync", &m_VsyncEnabled))
				{
					app.GetWindow().SetVSync(m_VsyncEnabled);
				}

				ImGui::Text("Target FPS");
				ImGui::SameLine();
				ImGui::InputDouble("##Target FPS", &app.m_UpdateInterval, 1.0f, 2.0f, "%.2f");

				ImGui::Separator();

				ImGui::Text("Draw Calls:");
				ImGui::SameLine();
				ImGui::Text(std::to_string(Renderer::GetStats().DrawCalls).c_str());

				ImGui::Text("Sprite Count:");
				ImGui::SameLine();
				ImGui::Text(std::to_string(Renderer::GetStats().SpriteCount).c_str());

				ImGui::Separator();

				if (ImGui::Button("Save Layout"))
				{
					ImGui::SaveIniSettingsToDisk("assets/imgui.ini");
				}

				ImGui::End();
			}
			else
			{
				ImGui::End();
			}
		}
	}

	void EditorLayer::OnEvent(Omni::Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<KeyPressedEvent>(OM_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(OM_BIND_EVENT_FN(EditorLayer::OnKeyReleased));

		dispatcher.Dispatch<MouseButtonPressedEvent>(OM_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(OM_BIND_EVENT_FN(EditorLayer::OnMouseButtonReleased));
		dispatcher.Dispatch<MouseScrolledEvent>(OM_BIND_EVENT_FN(EditorLayer::OnMouseScrolled));

		dispatcher.Dispatch<WindowResizeEvent>(OM_BIND_EVENT_FN(EditorLayer::OnWindowResized));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		//----------Shortcuts-----------
		if (e.GetRepeatCount() > 0)
			return false;

		bool ctrl = Input::IsKeyPressed(OM_KEY_LEFT_CONTROL) || Input::IsKeyPressed(OM_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(OM_KEY_LEFT_SHIFT) || Input::IsKeyPressed(OM_KEY_RIGHT_SHIFT);
		switch (e.GetKeyCode())
		{
		case OM_KEY_F11:
			m_FullscreenEnabled = !m_FullscreenEnabled;
			app.GetWindow().SetFullscreen(m_FullscreenEnabled);
			break;

		case OM_KEY_N:
			if (ctrl)
				NewScene();
			break;

		case OM_KEY_S:
			if (ctrl && shift)
				SaveSceneAs();
			break;

			// TODO : Open project shortcut
		case OM_KEY_O:
			if (ctrl)
				//OpenScene();
			break;

		case OM_KEY_T:
			if (!m_UsingGizmo && m_ViewportHovered)
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;

		case OM_KEY_R:
			if (!m_UsingGizmo && m_ViewportHovered)
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;

		case OM_KEY_E:
			if (!m_UsingGizmo && m_ViewportHovered)
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;

		default:
			break;
		}
		//----------Shortcuts-----------
		return false;
	}

	bool EditorLayer::OnKeyReleased(KeyReleasedEvent& e)
	{
		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		m_EditorCamera.OnMouseButtonPressed(e);

		if (e.GetMouseButton() == OM_MOUSE_BUTTON_LEFT)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver())
			{
				PanelAccessor::m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		m_EditorCamera.OnMouseButtonReleased(e);

		return false;
	}

	bool EditorLayer::OnMouseScrolled(MouseScrolledEvent& e)
	{
		if (m_ViewportHovered)
		{
			m_EditorCamera.OnMouseScrolled(e);
			m_CameraFOV = m_EditorCamera.GetFOV();
		}

		return false;
	}

	bool EditorLayer::OnWindowResized(WindowResizeEvent& e)
	{
		return false;
	}

	void EditorLayer::NewProject()
	{
		Project::New();
	}

	bool EditorLayer::OpenProject()
	{
		std::string filepath = FileDialogs::OpenFile("Omni Project (*.omproj)\0*.omproj\0");
		if (filepath.empty())
			return false;

		OpenProject(filepath);
		return true;
	}

	void EditorLayer::OpenProject(const std::filesystem::path& path)
	{
		if (Project::Load(path))
		{
			auto startScenePath = AssetManager::GetAssetPath() / Project::GetActive()->GetConfig().StartScene;
			OpenScene(startScenePath);
			PanelAccessor::m_AssetBrowserPanel = AssetBrowserPanel();
		}
	}

	void EditorLayer::SaveProject()
	{
		//Project::SaveActive();
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		PanelAccessor::m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Omni Scene (*.omscene)\0*.omscene\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Omni Scene (*.omscene)\0*.omscene\0");
		if (!filepath.empty())
			OpenScene(filepath);
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		PanelAccessor::m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		SceneSerializer serializer(m_ActiveScene);
		serializer.Deserialize(path.string());
	}
}
