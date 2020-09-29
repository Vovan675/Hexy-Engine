#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Component.h"

namespace Hexy 
{
	class TransformComponent : Component 
	{
	public:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::mat4 matrix;

		TransformComponent();

		void UpdateMatrix();
		glm::quat GetOrientation();

		void RotateAround(glm::vec3 origin, glm::vec3 axis, float angle);
		void LookAt(glm::vec3 point);

		glm::vec3 Forward();
		glm::vec3 Right();
		glm::vec3 Up();
	private:
		/*
		friend class cereal::access;
		
		template <class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("Position", position));
			archive(cereal::make_nvp("Rotation", rotation));
			archive(cereal::make_nvp("Scale", scale));
		}

		template <class Archive>
		void load(Archive& archive) 
		{
			archive(cereal::make_nvp("Position", position));
			archive(cereal::make_nvp("Rotation", rotation));
			archive(cereal::make_nvp("Scale", scale));
			UpdateMatrix();
		}
		*/
	};
}