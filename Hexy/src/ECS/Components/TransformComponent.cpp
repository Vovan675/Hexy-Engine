#include "pch.h"
#include "TransformComponent.h"

namespace Hexy 
{
	TransformComponent::TransformComponent() 
	{
		position = glm::vec3(0, 0, 0);
		rotation = glm::vec3(0, 0, 0);
		scale = glm::vec3(1, 1, 1);
		matrix = glm::mat4(1);
	}

	void TransformComponent::UpdateMatrix()
	{
		glm::mat4 result = glm::mat4(1);
		result = glm::translate(result, position);
		result *= glm::toMat4(GetOrientation());
		result = glm::scale(result, scale);
		matrix = result;
	}

	glm::quat TransformComponent::GetOrientation()
	{
		return glm::quat(glm::radians(rotation));
	}

	void TransformComponent::RotateAround(glm::vec3 origin, glm::vec3 axis, float angle)
	{
		position = position * glm::angleAxis(glm::radians(angle), glm::normalize(axis));
		LookAt(origin);
	}

	void TransformComponent::LookAt(glm::vec3 point)
	{
		glm::vec3 direction = glm::normalize(point - position);
		rotation = glm::degrees(glm::eulerAngles(glm::quatLookAt(direction, Up())));
	}

	glm::vec3 TransformComponent::Forward()
	{
		return glm::rotate(GetOrientation(), glm::vec3(0, 0, -1));
	}

	glm::vec3 TransformComponent::Right()
	{
		return glm::rotate(GetOrientation(), glm::vec3(1, 0, 0));
	}

	glm::vec3 TransformComponent::Up()
	{
		return glm::rotate(GetOrientation(), glm::vec3(0, 1, 0));
	}
}
