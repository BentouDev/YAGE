//
// Created by bentoo on 10/3/16.
//

#include "Utils/String.h"

namespace Utils::StringUtils
{
    void Tokenize(const eastl::string& string, eastl::vector<eastl::string_view>& list, const char* divider)
    {
        if (string.size() == 0)
            return;

        const char* begin = string.begin();
        for (auto itr = string.begin() + 1; itr != string.end(); itr++)
        {
            if (strchr(divider, *begin) != nullptr)
            {
                begin++;
            }
            else
            {
                if (strchr(divider, *itr) != nullptr)
                {
                    list.emplace_back(begin, eastl::distance(begin, itr));
                    begin = itr + 1;
                }
            }
        }

        if (strchr(divider, *begin) == nullptr)
        {
            list.emplace_back(begin, eastl::distance(begin, string.end()));
        }
    }

    //void _broken_String::Tokenize(const _broken_String& string, Utils::List<Utils::Slice<char>>& list, const char* divider)
    //{
    //    if (string.size() == 0)
    //        return;

    //    char* begin = string.begin();
    //    for (auto itr = string.begin() + 1; itr != string.end(); itr++)
    //    {
    //        if (strchr(divider, *begin) != nullptr)
    //        {
    //            begin++;
    //        }
    //        else
    //        {
    //            if (strchr(divider, *itr) != nullptr)
    //            {
    //                list.emplace(begin, itr);
    //                begin = itr + 1;
    //            }
    //        }
    //    }

    //    if (strchr(divider, *begin) == nullptr)
    //    {
    //        list.emplace(begin, string.end());
    //    }
    //}
}
