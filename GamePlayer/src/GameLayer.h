#pragma once
#include "Hexy.h"
#include "EditorCamera.h"

class GameLayer : public Hexy::Layer
{
	Hexy::Scene* m_scene;
	Hexy::Editor::EditorCamera m_editorCamera;
	
public:
	GameLayer();
	void OnUpdate(double deltaTime);
	bool OnWindowSizeEvent(int width, int height);
};