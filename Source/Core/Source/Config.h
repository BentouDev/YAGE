//
// Created by MrJaqbq on 2016-03-26.
//

#ifndef VOLKHVY_CONFIG_H
#define VOLKHVY_CONFIG_H

#include <string>
#include <json.hpp>

namespace Core
{
	class Config;

	template <typename T>
	class ConfigProperty
	{
		Config& _config;
		const std::string _name;
		T _value;
	public:
		explicit ConfigProperty(Config* config, std::string name, T def) :
				_config(*config),
				_value(def),
				_name(name)
		{ }

		ConfigProperty<T>& operator = (const T& new_value);

		operator const T&() const
		{
			return _value;
		}
	};

	class Config
	{
		nlohmann::json json;

		Config();

	public:
		auto static get() -> Config& {
			static Config config;
			return config;
		}

		auto Load(std::string path) -> bool;

		auto Save(std::string path) -> bool;

		template <typename T>
		auto Set(std::string, T& value) -> void;

		ConfigProperty<std::string> RenderingApi;
		ConfigProperty<std::string> WindowTitle;
		ConfigProperty<uint32_t> WindowWidth;
		ConfigProperty<uint32_t> WindowHeight;
		ConfigProperty<bool> DrawDebug;
		ConfigProperty<bool> DrawFPS;
	};

	template <typename T>
	ConfigProperty<T>& ConfigProperty<T>::operator = (const T& new_value)
	{
		_value = new_value;
		_config.Set(_name, _value);
		return *this;
	}
}


#endif //VOLKHVY_CONFIG_H
