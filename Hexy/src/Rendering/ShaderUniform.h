#pragma once

namespace Hexy 
{
	//Just describes the uniform type and name to upload into shader. Also holds offset to data in buffer
	struct ShaderUniform 
	{
		enum Type 
		{
			Int,
			Float,
			Vec2,
			Vec3,
			Vec4
		};

		std::string name;
		Type type;
		unsigned int offset = 0;
		unsigned int size = 0;

		ShaderUniform(const std::string& name, Type type, unsigned int offset);
	};
}