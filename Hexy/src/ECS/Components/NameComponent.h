#pragma once
namespace Hexy
{
	struct NameComponent
	{
		std::string Name;
		operator std::string& () { return Name; }
	};
}