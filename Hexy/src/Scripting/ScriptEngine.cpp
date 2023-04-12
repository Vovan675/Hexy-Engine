#include <pch.h>
#include "ScriptEngine.h"
#include "ScriptEngineBinder.h"
#include "ECS/Components/ScriptComponent.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>

namespace Hexy
{
	static MonoDomain* s_Domain;
	static MonoAssembly* s_AssemblyCore;

	static MonoAssembly* s_AssemblyRuntime;
	static MonoImage* s_ImageRuntime;

	static void* GetMethod(const std::string& description)
	{
		auto mDesc = mono_method_desc_new(description.c_str(), false);
		auto method = mono_method_desc_search_in_image(mDesc, s_ImageRuntime);
		return mono_method_get_unmanaged_thunk(method);
	}

	struct EntityScriptInstance
	{
		MonoClass* klass = nullptr;
		MonoObject* instance = nullptr;

		std::string name;
		std::string namespaceName;
		std::string className;

		typedef void (*OnStartFunc)(MonoObject*, MonoException**);
		OnStartFunc OnStart;

		typedef void (*OnUpdateFunc)(MonoObject*, float, MonoException**);
		OnUpdateFunc OnUpdate;

		void Init()
		{
			OnStart = (OnStartFunc)GetMethod(name + ":OnStart()");
			OnUpdate = (OnUpdateFunc)GetMethod(name + ":OnUpdate(single)");
		}
	};
	static std::unordered_map<unsigned int, EntityScriptInstance> s_Scripts;

	void ScriptEngine::Init()
	{
		mono_set_assemblies_path("mono/lib/");
		s_Domain = mono_jit_init("HexyScripting");
		s_AssemblyCore = mono_domain_assembly_open(s_Domain, "assets/scripts/ScriptCore.dll");
		ScriptEngineBinder::BindCalls();
	}

	void ScriptEngine::LoadAssembly(const std::string& assembly)
	{
		s_AssemblyRuntime = mono_domain_assembly_open(s_Domain, assembly.c_str());
		s_ImageRuntime = mono_assembly_get_image(s_AssemblyRuntime);
	}

	bool ScriptEngine::Exists(const std::string& name)
	{
		std::string className, namespaceName;

		if (name.find('.') != std::string::npos)
		{
			auto pos = name.find('.');
			namespaceName = name.substr(0, pos);
			className = name.substr(pos + 1);
		}
		else
		{
			className = name;
		}

		auto klass = mono_class_from_name(s_ImageRuntime, namespaceName.c_str(), className.c_str());
		return klass;
	}

	void ScriptEngine::RegisterEntity(const Entity& entity)
	{
		EntityScriptInstance scriptInstance;
		std::string script = entity.GetComponent<ScriptComponent>().script;

		if (script.empty())
			return;

		scriptInstance.name = script;

		if (script.find('.') != std::string::npos)
		{
			auto pos = script.find('.');
			scriptInstance.namespaceName = script.substr(0, pos);
			scriptInstance.className = script.substr(pos + 1);
		}
		else
		{
			scriptInstance.className = script;
		}
		scriptInstance.klass = mono_class_from_name(s_ImageRuntime, scriptInstance.namespaceName.c_str(), scriptInstance.className.c_str());

		scriptInstance.Init();
		s_Scripts[(uint32_t)entity] = scriptInstance;
	}

	void ScriptEngine::DestroyScript(const Entity& entity)
	{
		s_Scripts.erase((uint32_t)entity);
	}

	void ScriptEngine::InstantiateClass(const Entity& entity)
	{
		auto& script = s_Scripts[(uint32_t)entity];
		script.instance = mono_object_new(s_Domain, script.klass);
		mono_runtime_object_init(script.instance);

		// Set ID property
		auto* id_prop = mono_class_get_property_from_name(script.klass, "ID");
		uint32_t id = (uint32_t)entity;
		void* params[] = { &id };
		mono_property_set_value(id_prop, script.instance, params, nullptr);
	}

	void ScriptEngine::OnStart(const Entity& entity)
	{
		MonoException* exception = nullptr;
		auto script = s_Scripts[(uint32_t)entity];
		if (script.instance == nullptr)
		{
			InstantiateClass(entity);
		}
		script.OnStart(script.instance, &exception);
	}

	void ScriptEngine::OnUpdate(const Entity& entity, float deltaTime)
	{
		MonoException* exception = nullptr;
		auto script = s_Scripts[(uint32_t)entity];
		if (script.instance == nullptr)
		{
			InstantiateClass(entity);
		}
		script.OnUpdate(script.instance, deltaTime, &exception);
	}

	
}