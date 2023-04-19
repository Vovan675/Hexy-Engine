#include "EditorScene.h"
#include "EditorLayer.h"

namespace Hexy
{
	namespace Editor
	{
		EditorScene::EditorScene()
		{
			editorCamera = EditorCamera();
		}

		void EditorScene::OnUpdate()
		{
			SceneRenderer::Render(editorCamera, editorCamera.GetView());
		}

		void EditorScene::OnImgui()
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
			ImGui::Begin("Viewport");

			ImGuiIO& io = ImGui::GetIO();
			
			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 windowSize = ImGui::GetWindowSize();
			ImVec2 regionPos = ImGui::GetCursorPos();
			ImVec2 regionSize = ImGui::GetContentRegionAvail();

			SceneRenderer::SetViewportSize(regionSize.x, regionSize.y);
			editorCamera.SetViewportSize(regionSize.x, regionSize.y);
			glViewport(0, 0, regionSize.x, regionSize.y);

			ImGui::Image((ImTextureID)editorCamera.GetFramebuffer()->GetColorAttachment(), regionSize, { 0, 1 }, { 1, 0 });

			//Controlling camera
			if (ImGui::IsWindowHovered() || editorCamera.isUsing) 
			{
				if (!ImGuizmo::IsUsing()) 
				{
					editorCamera.Update(io.DeltaTime);
					if (Input::IsKeyDown('F') && !EditorLayer::SelectionContext.empty())
					{
						glm::vec3 center(0);
						AABB aabb(glm::vec3(0), glm::vec3(0));
						for (auto selection : EditorLayer::SelectionContext)
						{
							Entity entity = Entity(selection, SceneManager::GetCurrentScene());
							auto& transform = entity.GetComponent<TransformComponent>();
							center += transform.position;

							if (entity.HasComponent<MeshComponent>())
							{
								auto submeshes = entity.GetComponent<MeshComponent>().mesh->GetSubmeshes();
								for (auto& submesh : submeshes)
								{
									if (aabb == AABB(glm::vec3(0), glm::vec3(0)))
									{
										aabb = submesh.boundingBox * transform.matrix;
									}
									else
									{
										aabb |= submesh.boundingBox * transform.matrix;
									}
								}
							}
							else
							{
								aabb |= AABB(transform.position, transform.position);
							}
						}
						center /= (float)EditorLayer::SelectionContext.size();
						

						editorCamera.SetFocalPoint(center);

						glm::vec3 size = aabb.max - aabb.min;
						editorCamera.SetDistance(glm::compMax(size) * 3.1415926);
					}
				}
			}

			//Ray Selection
			if (Input::IsKeyDown('C'))
			{
				EditorLayer::Selection.clear();
				EditorLayer::SelectionContext.clear();
			}

			static bool leftMouseDown = false;
			if (Input::IsMouseButtonDown(0) == 0)
			{
				leftMouseDown = false;
			}

			if (ImGui::IsWindowHovered() && Input::IsMouseButtonDown(0) && !leftMouseDown && !ImGuizmo::IsUsing() && !ImGuizmo::IsOver())
			{
				leftMouseDown = true;

				float closestT = std::numeric_limits<float>::max();
				entt::entity closestEntity = entt::null;

				auto entities = SceneManager::GetCurrentScene()->GetEntitiesWith<MeshComponent, TransformComponent>();

				glm::vec2 mousePos = { io.MousePos.x, io.MousePos.y };
				mousePos -= glm::vec2(windowPos.x + regionPos.x, windowPos.y + regionPos.y);
				mousePos /= glm::vec2(regionSize.x, regionSize.y);
				mousePos = mousePos * 2.0f - 1.0f;
				mousePos.y *= -1;

				auto [pos, dir] = CastRay(mousePos);
				for (auto entity : entities)
				{
					auto [mesh, transform] = entities.get<MeshComponent, TransformComponent>(entity);

					auto& submeshes = mesh.mesh->GetSubmeshes();
					for (auto& submesh : submeshes) 
					{
						AABB bbox = submesh.boundingBox;
						Ray ray;
						ray.pos = glm::inverse(transform.matrix) * glm::vec4(pos, 1.0);
						ray.dir = glm::inverse(glm::mat3(transform.matrix)) * dir;

						float t;
						bool intersects = ray.IntersectsAABB(bbox, t);
						if (intersects)
						{
							if (t < closestT) 
							{
								closestT = t;
								closestEntity = entity;
							}
						}
					}
				}

				if (closestEntity != entt::null)
				{
					if (Input::IsKeyDown(GLFW_KEY_LEFT_CONTROL))
					{
						auto entity = std::find(EditorLayer::SelectionContext.begin(), EditorLayer::SelectionContext.end(), closestEntity);
						if (entity != EditorLayer::SelectionContext.end()) 
						{
							EditorLayer::SelectionContext.erase(entity);
							EditorLayer::Selection[closestEntity] = false;
						} 
						else
						{
							EditorLayer::SelectionContext.push_back(closestEntity);
							EditorLayer::Selection[closestEntity] = true;
						}
					}
					else
					{
						EditorLayer::SelectionContext.clear();
						EditorLayer::Selection.clear();
						EditorLayer::SelectionContext.push_back(closestEntity);
						EditorLayer::Selection[closestEntity] = true;
					}
				}
				else 
				{
					EditorLayer::SelectionContext.clear();
					EditorLayer::Selection.clear();
				}
				
			}
			
			//ImGuizmos
			if (!EditorLayer::SelectionContext.empty() && (io.MousePos.x != -FLT_MAX && io.MousePos.y != -FLT_MAX))
			{
				glm::vec3 center(0);

				for (auto selection : EditorLayer::SelectionContext) 
				{
					Entity entity = Entity(selection, SceneManager::GetCurrentScene());
					center += entity.GetComponent<TransformComponent>().position;
				}
				center /= (float)EditorLayer::SelectionContext.size();
				glm::mat4 transform = glm::translate(glm::mat4(1), center);
				glm::mat4 dtransform;
				///ImGuizmo::SetOrthographic(1);
				ImGuizmo::SetDrawlist();

				static ImGuizmo::OPERATION curOperation = ImGuizmo::TRANSLATE;
				if (Input::IsKeyDown('E') && !ImGuizmo::IsUsing()) 
				{
					curOperation = ImGuizmo::TRANSLATE;
				}
				else if (Input::IsKeyDown('R') && !ImGuizmo::IsUsing())
				{
					curOperation = ImGuizmo::ROTATE;
				}
				else if (Input::IsKeyDown('T') && !ImGuizmo::IsUsing())
				{
					curOperation = ImGuizmo::SCALE;
				}

				float snap[3] = { 0.1f, 0.1f, 0.1f };
				ImGuizmo::SetRect(windowPos.x + regionPos.x, windowPos.y + regionPos.y, regionSize.x, regionSize.y);

				ImGuizmo::Manipulate(glm::value_ptr(editorCamera.GetView()), glm::value_ptr(editorCamera.GetProjection()),
									 curOperation, ImGuizmo::LOCAL, glm::value_ptr(transform), glm::value_ptr(dtransform));

				glm::vec3 dposition, drotation, dscale;
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(dtransform), glm::value_ptr(dposition), glm::value_ptr(drotation), glm::value_ptr(dscale));

				for (auto selection : EditorLayer::SelectionContext)
				{
					Entity entity = Entity(selection, SceneManager::GetCurrentScene());
					auto& entityTransform = entity.GetComponent<TransformComponent>();
					switch (curOperation)
					{
					case ImGuizmo::TRANSLATE:
						entityTransform.position += dposition;
						break;
					case ImGuizmo::ROTATE:
						entityTransform.rotation += drotation;
						break;
					case ImGuizmo::SCALE:
						//TODO SCALING
						/*
						static glm::vec3 original_scale;
						if (ImGuizmo::IsUsing()) {
							selectedEntity->transform.scale = original_scale * dscale;
						}
						else {
							original_scale = selectedEntity->transform.scale;
						}
						*/
						break;
					}
				}
			}
			ImGui::PopStyleVar();
			ImGui::End();
		}

		std::pair<glm::vec3, glm::vec3> EditorScene::CastRay(glm::vec2 mousePos)
		{
			glm::vec3 pos = editorCamera.GetPosition();

			glm::vec4 rayClip = glm::vec4(mousePos.x, mousePos.y, -1.0, 1.0);
			glm::vec4 rayEye = glm::inverse(editorCamera.GetProjection()) * rayClip;

			glm::vec3 dir = glm::inverse(glm::mat3(editorCamera.GetView())) * glm::vec3(rayEye);
			dir = glm::normalize(dir);
			return { pos, dir };
		}
	}
}

