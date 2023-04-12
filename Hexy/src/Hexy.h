#pragma once

#include "Log.h"

#include "Math/AABB.h"
#include "Math/Ray.h"
#include "Math/Transform.h"

#include "Utils/Random.h"
#include "Utils/FileSystem.h"
#include "Utils/SceneManager.h"
#include "DebugInfo.h"

#include "Application.h"
#include "Input.h"

#include "Rendering/Shader.h"
#include "Rendering/Buffers.h"
#include "Rendering/VertexArray.h"
#include "Rendering/FrameBuffer.h"
#include "Rendering/Texture.h"
#include "Rendering/Material.h"

#include "Rendering/Renderer.h"
#include "Rendering/Renderer2D.h"
#include "Rendering/SceneRenderer.h"
#include "Rendering/Mesh.h"
#include "Rendering/MeshFactory.h"

#include "Rendering/Camera.h"

#include "ECS/Components/Component.h"
#include "ECS/Components/NameComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/MeshComponent.h"
#include "ECS/Components/SpriteComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/ScriptComponent.h"

#include "ECS/Entity.h"

#include "Rendering/Scene.h"

#include "Scripting/ScriptEngine.h"