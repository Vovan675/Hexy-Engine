#include "pch.h"
#include "SceneRenderer.h"
#include "Renderer2D.h"
#include "Renderer.h"

namespace Hexy 
{
	struct SceneRendererData 
	{
		std::shared_ptr<Shader> basicShader;
		std::shared_ptr<TextureCube> skybox;
		std::shared_ptr<Shader> skyboxShader;
		std::shared_ptr<VertexArray> cubeVA;

		std::shared_ptr<Framebuffer> gBuffer;

		std::shared_ptr<Shader> compositeShader;


		struct MeshDrawCommand
		{
			std::shared_ptr<Mesh> mesh = nullptr;
			glm::mat4 transform = glm::mat4(1);
		};

		std::list<MeshDrawCommand> selectedMeshQueue;
		std::list<MeshDrawCommand> meshQueue;
	};
	static SceneRendererData m_data;


	SceneRenderingSettings SceneRenderer::m_settings;

	void SceneRenderer::Init()
	{
		m_data.basicShader = ShaderLibrary::Use("assets/shaders/SimpleShader.vert", "assets/shaders/SimpleShader.frag");
		m_data.skybox = TextureCube::Create("assets/textures/sky.hdr");
		m_data.skyboxShader = ShaderLibrary::Use("assets/shaders/Skybox.vert", "assets/shaders/Skybox.frag");
		m_data.cubeVA = MeshFactory::Cube();

		auto size = Application::Instance->GetWindowSize();
		m_data.gBuffer = Framebuffer::Create(size.x, size.y, 8);
		m_data.compositeShader = ShaderLibrary::Use("assets/shaders/PostProcessingShader.vert", "assets/shaders/PostProcessingShader.frag");
	}

	void SceneRenderer::Begin()
	{

	}

	void SceneRenderer::End()
	{

	}

	void SceneRenderer::SetViewportSize(int width, int height)
	{
		m_data.gBuffer->Resize(width, height);
	}


	void SceneRenderer::Render(Camera& camera, glm::mat4& viewMatrix)
	{
		m_data.gBuffer->BindAndClear(ClearFlags::COLOR | ClearFlags::DEPTH | ClearFlags::STENCIL);
		glStencilMask(0);
		
		for (auto command : m_data.meshQueue)
		{
			//TODO: Multiple shaders support
			auto shader = command.mesh->GetMaterials()[0]->GetShader();
			shader->Bind();

			shader->SetMat4("u_cameraMatrix", camera.GetProjection() * viewMatrix);
			shader->SetVec3("u_cameraPosition", glm::inverse(viewMatrix)[3]);

			shader->SetVec3("u_color", { 1, 1, 1 });

			glBindTextureUnit(10, m_data.skybox->GetIrradiance());
			shader->SetInt("u_irradianceMap", 10);

			glBindTextureUnit(11, m_data.skybox->GetPrefilter());
			shader->SetInt("u_prefilterMap", 11);

			if (Input::IsKeyDown('T')) 
				TextureLibrary::Load("assets/textures/brdf.tga")->Bind(12);
			else
				glBindTextureUnit(12, m_data.skybox->GetBRDF());
			shader->SetInt("u_brdfMap", 12);
			Renderer::DrawMesh(command.mesh, command.transform);
		}

		m_data.skyboxShader->Bind();
		m_data.skyboxShader->SetMat4("u_projection", camera.GetProjection());
		m_data.skyboxShader->SetMat4("u_view", viewMatrix);
		m_data.skybox->Bind(0);
		//Draw background skybox
		glDepthFunc(GL_LEQUAL);
		m_data.cubeVA->Draw();
		m_data.skyboxShader->Unbind(); //Without unbinding there will be warning in opengl
		glDepthFunc(GL_LESS);

		//Draw debug stuff
		if (m_settings.ShowBoundingBoxes)
		{
			if (!m_data.selectedMeshQueue.empty()) 
			{
				/*
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
				*/

				for (auto command : m_data.selectedMeshQueue) 
				{
					/*
					auto shader = command.material->GetShader();
					shader->Bind();

					shader->SetMat4("u_cameraMatrix", camera.GetProjection() * transform);

					shader->SetVec3("u_color", { 1, 0, 0 });

					shader->SetTexture(10, m_data.skybox->GetIrradiance());
					shader->SetInt("u_irradianceMap", 10);

					shader->SetTexture(11, m_data.skybox->GetPrefilter());
					shader->SetInt("u_prefilterMap", 11);

					shader->SetTexture(12, m_data.skybox->GetBRDF());
					shader->SetInt("u_brdfMap", 12);
					Renderer::DrawMesh(command.va, command.transform);
					*/
					Renderer::m_lineShader->Bind();
					Renderer::m_lineShader->SetMat4("u_cameraMatrix", camera.GetProjection() * viewMatrix);

					auto submeshes = command.mesh->GetSubmeshes();
					for (auto submesh : submeshes)
						Renderer::DrawAABB(submesh.boundingBox, command.transform);

				}
			}
			//glStencilMask(0);
		}

		m_data.gBuffer->Unbind();


		camera.GetFramebuffer()->BindAndClear(ClearFlags::COLOR | ClearFlags::DEPTH);

		m_data.compositeShader->Bind();
		glBindTextureUnit(0, m_data.gBuffer->GetColorAttachment());

		Renderer2D::DrawFulscreenQuad();
		camera.GetFramebuffer()->Unbind();
	}

	void SceneRenderer::SubmitMesh(std::shared_ptr<Mesh> mesh, glm::mat4 transform)
	{
		m_data.meshQueue.push_back({ mesh, transform });
	}

	void SceneRenderer::SubmitSelectedMesh(std::shared_ptr<Mesh> mesh, glm::mat4 transform)
	{
		m_data.selectedMeshQueue.push_back({ mesh, transform});
	}

	void SceneRenderer::ClearRenderingQueue()
	{
		m_data.meshQueue.clear();
		m_data.selectedMeshQueue.clear();
	}

	SceneRenderingSettings& SceneRenderer::GetSettings()
	{
		return m_settings;
	}

	void SceneRenderer::SetSettings(SceneRenderingSettings settings)
	{
		m_settings = settings;
	}
}