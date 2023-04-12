#pragma once

namespace Hexy 
{
	class ScriptEngineBinder 
	{
	public:
		static void BindCalls();
	private:
		static void GetPosition_Internal(uint32_t entityID, glm::vec3& vec);
		static void SetPosition_Internal(uint32_t entityID, const glm::vec3& vec);
	};
}