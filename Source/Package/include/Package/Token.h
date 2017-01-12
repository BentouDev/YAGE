//
// Created by Bentoo on 2017-01-12.
//

#ifndef YAGE_RECORDTOKEN_H
#define YAGE_RECORDTOKEN_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <algorithm>

namespace Yage
{
	namespace Package
	{
		template <std::uint32_t token_size>
		class Token
		{
			static_assert(token_size > 0, "Token : token_size must be larger than 0!");

			char _token[token_size + 1];

		public:
			explicit Token()
			{
				memset(_token, '\0', token_size + 1);
			}

			explicit Token(const char* token)
			{
				// +1, to copy with null terminator
				memcpy(_token, token, std::min(token_size, strlen(token) + 1));

				_token[token_size] = '\0';
			}

			virtual ~Token()
			{ }

			static bool isValid(const Token& token) const
			{
				return token._token[0] != '\0';
			}

			operator bool() const
			{
				return isValid(*this);
			}

			const char* c_str() const
			{
				return _token;
			}

			const char* c_str()
			{
				return _token;
			}
		};
	}
}

#endif //YAGE_RECORDTOKEN_H
