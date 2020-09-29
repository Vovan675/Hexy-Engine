#include "pch.h"
#include "ShaderUniform.h"

namespace Hexy 
{
	unsigned int GetUniformTypeSize(ShaderUniform::Type type) {
		if (type == ShaderUniform::Type::Int)
			return 4;
		else if (type == ShaderUniform::Type::Float)
			return 4;
		else if (type == ShaderUniform::Type::Vec2)
			return 4 * 2;
		else if (type == ShaderUniform::Type::Vec3)
			return 4 * 3;
		else if (type == ShaderUniform::Type::Vec4)
			return 4 * 4;
		return 0;
	}

	ShaderUniform::ShaderUniform(const std::string& name, Type type, unsigned int offset) : name(name), type(type), offset(offset), size(GetUniformTypeSize(type))
	{
	}
}