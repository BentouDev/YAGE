//
// Created by Bentoo on 2017-01-12.
//

#ifndef YAGE_VIRTUALFILE_H
#define YAGE_VIRTUALFILE_H

#include <cstdint>

namespace Yage
{
	namespace Package
	{
		class VirtualFile
		{
		private:
			void*			_data;
			std::uint32_t	_size;

		public:
			explicit VirtualFile(void* data, std::uint32_t size);
			virtual ~VirtualFile();

			inline auto getData() const -> void*
			{
				return _data;
			}

			inline auto getSize() const -> std::uint32_t
			{
				return _size;
			}
		};
	}
}

#endif //YAGE_VIRTUALFILE_H
