#include "pch.h"
#include "SceneManager.h"
#include "SceneSerializer.h"

namespace Hexy 
{
	Scene* SceneManager::m_currentScene;

	Scene* SceneManager::CreateScene()
	{
		delete m_currentScene;
		m_currentScene = new Scene();
		return m_currentScene;
	}

	void SceneManager::SetScene(Scene* scene)
	{
		delete m_currentScene;
		m_currentScene = scene;
	}

	Scene* SceneManager::LoadScene(const std::string& path, bool binary)
	{
		delete m_currentScene;
		m_currentScene = SceneSerializer::Deserialize(path, binary);
		return m_currentScene;
	}
}