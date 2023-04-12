#include "GameLayer.h"

std::shared_ptr<Hexy::Shader> basicShader;
std::shared_ptr<Hexy::VertexArray> cubeVA;
GameLayer::GameLayer()
{
	Hexy::ScriptEngine::LoadAssembly("assets/scripts/ScriptCustom.dll");
	m_scene = Hexy::SceneManager::LoadScene("MainScene.hexy");	
	m_editorCamera = Hexy::Editor::EditorCamera();

	basicShader = Hexy::ShaderLibrary::Use("assets/shaders/FlatShader.vert", "assets/shaders/FlatShader.frag");
	cubeVA = Hexy::MeshFactory::Cube();
	m_scene->OnRuntimeStart();
}

void GameLayer::OnUpdate(double deltaTime)
{
	m_scene->OnUpdate(deltaTime);
	m_scene->OnRenderRuntime(deltaTime);
	Hexy::SceneRenderer::Render(m_editorCamera, m_editorCamera.GetView());
	m_editorCamera.Update(deltaTime);

	Hexy::SceneRenderer::ClearRenderingQueue();
}

bool GameLayer::OnWindowSizeEvent(int width, int height)
{
	m_editorCamera.SetViewportSize(width, height);
	Hexy::SceneRenderer::SetViewportSize(width, height);
	return false;
}

