#pragma once
#include "Component.h"
#include "Rendering/Texture.h"

namespace Hexy
{
	class SpriteComponent : public Component 
	{
	private:
		std::shared_ptr<Texture2D> m_texture;
	public:
		SpriteComponent();

		void SetSprite(char* path);


	};


}