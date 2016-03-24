#pragma once

#include "string_view.h"

namespace litehtml
{
    template <typename T, size_t N>
    size_t array_size(T(&)[N])
    {
        return N;
    }

    using tchar_t = char;
    using uint_ptr = uintptr_t;

    using tstring_view = string_view<tchar_t>;
    using tstring = std::basic_string<tchar_t>;
}

#if defined( WIN32 )
#define t_strncasecmp _strnicmp
#else
#define t_strncasecmp strncasecmp
#endif

//small optimization: _Q("string") creates string_view with known size at runtime
#define _Q(str) litehtml::tstring_view(str, litehtml::array_size(str) - 1)
