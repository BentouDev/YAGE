//
// Created by Bentoo on 2017-04-16.
//

#ifndef YAGE_OBJPARSER_H
#define YAGE_OBJPARSER_H

#include <map>
#include <string>
#include <type_traits>

namespace Resources
{
	template <typename T, typename P>
	class ObjParser
	{
	protected:
		std::map<std::string, bool(*)(std::string, P&, T&)> lookupMap;

		void ProcessLine(std::string line, P& parser, T& obj)
		{
			if(line.empty() || line[0] == '#')
				return;

			// Todo: refactor to slice
			auto separatorPos = line.find(' ');
			std::string token = line.substr(0, separatorPos);

			if(!token.empty() && lookupMap.find(token) != lookupMap.end())
			{
				lookupMap[token](line.substr(separatorPos), parser, obj);
			}
		}
	};
}

#endif //YAGE_OBJPARSER_H
