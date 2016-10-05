//
// Created by MrJaqbq on 2016-03-26.
//

#ifndef VOLKHVY_CONFIG_H
#define VOLKHVY_CONFIG_H

#include <string>
#include <json.hpp>
#include <Utils/Handle.h>
#include <Utils/BorrowedPtr.h>

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
		const std::string _name;

		PropertyBase(Config* config, std::string name) :
			_config(*config),
			_name(name) {}

		virtual auto reload() -> void = 0;

	public:
		auto name() -> std::string { return _name; }
	};

	template <typename T>
	class ConfigProperty : public PropertyBase
	{
		T _value;

		void reload();

	public:
		explicit ConfigProperty(Config* config, std::string name, T def);

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

		Utils::borrowed_ptr<Logger> logger;

		auto setLogger(Utils::borrowed_ptr<Logger> log) -> void
		{
			logger.reset(log.release());
		}

		std::vector<PropertyBase*> _properties;
		nlohmann::json json;

		Config();

	public:

		/*auto static get() -> Config& {
			static Config config;
			return config;
		}*/

		auto Load(std::string path) -> bool;

		auto Save(std::string path) -> bool;

		auto Has(std::string) -> bool;

		template <typename T>
		auto Set(std::string, T& value) -> void;

		template <typename T>
		auto Get(std::string, T) -> T;

		template <typename T>
		auto Register(ConfigProperty<T>* prop) -> void;

		auto ReloadProperties() -> void;

		ConfigProperty<std::string> RenderingApi;
		ConfigProperty<std::string> WindowTitle;
		ConfigProperty<uint32_t> WindowWidth;
		ConfigProperty<uint32_t> WindowHeight;
		ConfigProperty<bool> DrawDebug;
		ConfigProperty<bool> DrawFPS;
	};

	template <typename T>
	ConfigProperty<T>::ConfigProperty(Config* config, std::string name, T def) :
		_value(def),
		PropertyBase(config, name)
	{
		_config.Register(this);
	}

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


#endif //VOLKHVY_CONFIG_H
