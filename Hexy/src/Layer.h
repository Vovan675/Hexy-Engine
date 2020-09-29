#pragma once


namespace Hexy 
{
	class Layer
	{
	private:

	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(double deltaTime) {}
		virtual void OnImGui() {}

		virtual bool OnWindowSizeEvent(int width, int height) { return 0; }
		virtual bool OnKeyEvent(int key, int action) { return 0; }
		virtual bool OnMouseButtonEvent(int button, int action) { return 0; }
		virtual bool OnScrollEvent(int xScroll, int yScroll) { return 0; }
	};
}