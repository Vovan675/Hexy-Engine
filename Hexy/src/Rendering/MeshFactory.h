#pragma once
#include "VertexArray.h"

namespace Hexy
{
	enum PrimitiveType 
	{
		PLANE,
		TRIANGLE,
		CUBE,
		FULL_SCREEN_QUAD
	};

	class MeshFactory 
	{
	private:

	public:
		static std::shared_ptr<VertexArray> Plane();
		static std::shared_ptr<VertexArray> Triangle();
		static std::shared_ptr<VertexArray> Cube();
		static std::shared_ptr<VertexArray> FullScreenQuad();

		static std::shared_ptr<VertexArray> CreatePrimitive(PrimitiveType type);
	};
}