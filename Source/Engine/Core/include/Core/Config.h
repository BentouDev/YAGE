//
// Created by MrJaqbq on 2016-03-26.
//

#ifndef YAGE_CONFIG_H
#define YAGE_CONFIG_H

#include <nlohmann/json.hpp>
#include <Utils/Handle.h>
#include <Utils/BorrowedPtr.h>
#include <Utils/List.h>
#include <Utils/MemoryBlock.h>
#include <string>
#include <EASTL/string.h>

namespace Core
{
	class Engine;

	class Logger;

	class Config;

	class PropertyBase
	{
		friend class Config;

	protected:
		Config& _config;
		eastl::string _name;

		PropertyBase(Config* config, const eastl::string& name)
			: _config(*config), _name(name)
		{ }

		PropertyBase(PropertyBase&& other)
			: _config(other._config), _name(other._name)
		{ }

		virtual auto reload() -> void = 0;

	public:
		auto name() -> const char* { return _name.c_str(); }
	};

	template <typename T>
	class ConfigProperty : public PropertyBase
	{
		T _value;

		void reload();

	public:
		explicit ConfigProperty(Config* config, const eastl::string& name, T def);
		ConfigProperty(ConfigProperty&&);

		ConfigProperty<T>& operator = (const T& new_value);

		explicit operator T*() const
		{
			return nullptr;
		}

		operator const T&() const
		{
			return _value;
		}
	};

	class Config
	{
		friend class Engine;

		Memory::IMemoryBlock& _memory;
		Utils::List<PropertyBase*> _properties;

		nlohmann::json json;

	public:
		explicit Config(Memory::IMemoryBlock& memory);
		Config(Config&&);

		auto Load(const eastl::string& path) -> bool;

		auto Save(const eastl::string& path) -> bool;

		auto Has(const eastl::string&) -> bool;

		template <typename T>
		auto Set(const eastl::string&, T& value) -> void;

		template <typename T>
		auto Get(const eastl::string&, T) -> T;

		template <typename T>
		auto Register(ConfigProperty<T>* prop) -> void;

		auto ReloadProperties() -> void;

		ConfigProperty<std::string> RenderingApi;
		ConfigProperty<std::string> Subsystem;
		ConfigProperty<std::string> WindowTitle;
		ConfigProperty<uint32_t> WindowWidth;
		ConfigProperty<uint32_t> WindowHeight;
		ConfigProperty<bool> DrawDebug;
		ConfigProperty<bool> DrawFPS;
	};

	template <typename T>
	ConfigProperty<T>::ConfigProperty(Config* config, const eastl::string& name, T def)
		: PropertyBase(config, name), _value(def)
	{
		_config.Register(this);
	}

	template <typename T>
	ConfigProperty<T>::ConfigProperty(ConfigProperty<T>&& other)
		: PropertyBase(std::move(other)), _value(std::move(other._value))
	{ }

	template <typename T>
	ConfigProperty<T>& ConfigProperty<T>::operator = (const T& new_value)
	{
		_value = new_value;
		_config.Set(_name, _value);
		return *this;
	}

	template <typename T>
	void ConfigProperty<T>::reload()
	{
		_value = _config.Get<T>(_name, _value);
	}
}

#endif //YAGE_CONFIG_H
