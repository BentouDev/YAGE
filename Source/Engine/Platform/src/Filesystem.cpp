#include "Platform/Filesystem.h"

#ifdef YAGE_FILESYSTEM_EXPERIMENTAL

namespace std::experimental::filesystem
{
	namespace detail
	{
		::fs::path& append(::fs::path& that, typename path::iterator begin, typename path::iterator end)
		{
			for (; begin != end; ++begin)
				that /= *begin;
			return that;
		}
	}

	// Return path when appended to a_From will resolve to same as a_To
	::fs::path relative(::fs::path a_To, ::fs::path a_From, std::error_code code)
	{
		a_From = ::fs::absolute(a_From);
		a_To = ::fs::absolute(a_To);

		a_From = ::fs::canonical(a_From);
		a_To = ::fs::canonical(a_To);

		::fs::path ret;
		::fs::path::const_iterator itrFrom(a_From.begin()), itrTo(a_To.begin());

		// Find common base
		for (::fs::path::const_iterator toEnd(a_To.end()), fromEnd(a_From.end()); itrFrom != fromEnd && itrTo != toEnd && *itrFrom == *itrTo; ++itrFrom, ++itrTo);

		// Navigate backwards in directory to reach previously found base
		for (::fs::path::const_iterator fromEnd(a_From.end()); itrFrom != fromEnd; ++itrFrom)
		{
			if ((*itrFrom) != ".")
				ret /= "..";
		}

		// Now navigate down the directory branch
		detail::append(ret, itrTo, a_To.end());
		return ret;
	}
}

#endif // YAGE_FILESYSTEM_EXPERIMENTAL