#pragma once

// For use by Omni applications

#include "core/Application.h"
#include "core/Layer.h"
#include "core/Log.h"
#include "core/Timestep.h"
#include "core/UUID.h"
#include "asset/AssetManager.h"
#include "project/Project.h"
#include "project/ProjectSerializer.h"
#include "debug/Instrumentor.h"

//------------Input------------
#include "events/Input.h"
#include "events/KeyCodes.h"
#include "events/MouseButtonCodes.h"
//-----------------------------

//----------Rendering----------
#include "renderer/Renderer.h"
#include "renderer/RenderCommand.h"
#include "renderer/Shader.h"
#include "renderer/UniformBuffer.h"
#include "renderer/Texture.h"
#include "renderer/Framebuffer.h"
#include "renderer/EmbeddedShaders.h"
#include "renderer/EmbeddedTextures.h"
#include "renderer/Buffer.h"
#include "renderer/Camera.h"
#include "renderer/GraphicsContext.h"
#include <glm.hpp>
//-----------------------------

//------------Scene------------
#include "scene/Scene.h"
#include "scene/SceneSerializer.h"
#include "scene/Entity.h"
#include "scene/Components.h"
#include "scene/ScriptableEntity.h"
//-----------------------------

//-------------GUI-------------
#include "ImGui/ImGuiLayer.h"
//-----------------------------

//--------------UI-------------
#include "ui/FontRenderer.h"
//-----------------------------

//-----------Physics-----------
#include "physics/Physics.h"
//-----------------------------
