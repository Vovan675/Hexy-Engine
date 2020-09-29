#include <iostream>
#include <Hexy.h>
#include "Editor/EditorLayer.h"

class MyApplication : public Hexy::Application 
{
public:
	MyApplication() 
	{
		PushLayer(new Hexy::Editor::EditorLayer());
	}
};

int main()
{
	MyApplication* app = new MyApplication();
	app->Run();
	return 0;
}