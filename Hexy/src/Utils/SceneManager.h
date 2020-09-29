#pragma once
#include "Rendering/Scene.h"

namespace Hexy
{
	class SceneManager 
	{
	private:
		static Scene* m_currentScene;
	public:
		static Scene* CreateScene();
		static void SetScene(Scene* scene);
		static Scene* LoadScene(const std::string& path, bool binary = false);

		static inline Scene* GetCurrentScene() { return m_currentScene; }
	};
}