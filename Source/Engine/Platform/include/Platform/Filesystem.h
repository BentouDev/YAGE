//
// Created by bentoo on 24.03.18.
//

#ifndef YAGE_FILESYSTEM_H
#define YAGE_FILESYSTEM_H

#if __cpp_lib_filesystem || __has_include(<filesystem>)
#define YAGE_FILESYSTEM
#include <filesystem>
namespace fs = std::filesystem;
#elif __cpp_lib_experimental_filesystem || __has_include(<experimental/filesystem>)
#define YAGE_FILESYSTEM_EXPERIMENTAL
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace std::experimental::filesystem
{
	namespace detail
	{
		::fs::path& append(::fs::path& that, typename path::iterator begin, typename path::iterator end);
	}

	// Return path when appended to a_From will resolve to same as a_To
	::fs::path relative(::fs::path a_To, ::fs::path a_From = ::fs::current_path(), std::error_code code = std::error_code{});
}

#else
#error "AnnotationGenerator requires filesystem TS which seems to be lacking!"
#endif

using cstring = const char*;

#endif //YAGE_FILESYSTEM_H
