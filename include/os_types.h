#pragma once

#include "string_view.h"

namespace litehtml
{
    template <typename T, size_t N>
    size_t array_size(T(&)[N])
    {
        return N;
    }
}

//small optimization: _Q("string") creates string_view with known size at compile-time
#define _Q(str) litehtml::tstring_view(str, litehtml::array_size(str) - 1)
