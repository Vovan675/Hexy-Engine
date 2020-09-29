#include "pch.h"
#include "SpriteComponent.h"

namespace Hexy 
{
	SpriteComponent::SpriteComponent()
	{
		m_texture = Texture2D::Create();
	}

	void SpriteComponent::SetSprite(char* path)
	{
		m_texture->SetTexture(path);
	}

	/*
	void SpriteComponent::OnRenderRuntime(float deltaTime)
	{
		///Renderer2D::DrawQuad(owner->transform.matrix, m_texture);
	}

	void SpriteComponent::OnImgui()
	{
		ImGui::Image((ImTextureID)m_texture->GetTexture(), { 64, 64 }, { 0, 1 }, { 1, 0 });
		if (ImGui::IsItemClicked()) 
		{
			std::string file = Application::Instance->OpenFileDialog();
			if (file != "")
			{
				m_texture = TextureLibrary::Load(file);
			}
		}
	}
	*/
	
}
