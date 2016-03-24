#pragma once

#include "string_view.h"

namespace litehtml
{
#if defined( WIN32 )

	#define _t(quote)			quote

	#define t_strncmp			strncmp
	#define t_strncasecmp		_strnicmp
	#define t_tolower			tolower
	#define t_isdigit			isdigit
    #define t_num_to_string     std::to_string

#else

	#define _t(quote)			quote

	#define t_strncmp			strncmp
	#define t_strncasecmp		strncasecmp

	#define t_tolower			tolower
	#define t_isdigit			isdigit
    #define t_num_to_string     std::to_string

#endif

    using tchar_t = char;
    using uint_ptr = uintptr_t;

    using tstring_view = string_view<tchar_t>;
    using tstring = std::basic_string<tchar_t>;
}
