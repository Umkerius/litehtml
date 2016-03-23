#pragma once

#include "string_view.h"

namespace litehtml
{
#if defined( WIN32 ) || defined( WINCE )

#ifndef LITEHTML_UTF8

	typedef wchar_t				tchar_t;

	#define _t(quote)			L##quote

	#define t_strncmp			wcsncmp
	#define t_strncasecmp		_wcsnicmp
	#define t_tolower			towlower
	#define t_isdigit			iswdigit
    #define t_num_to_string     std::to_wstring

#else

	typedef char				tchar_t;

	#define _t(quote)			quote

	#define t_strncmp			strncmp
	#define t_strncasecmp		_strnicmp
	#define t_tolower			tolower
	#define t_isdigit			isdigit
    #define t_num_to_string     std::to_string

#endif

	#ifdef _WIN64
		typedef unsigned __int64 uint_ptr;
	#else
		typedef unsigned int	uint_ptr;
	#endif

#else
	#define LITEHTML_UTF8

	typedef char				tchar_t;
	typedef void*				uint_ptr;

	#define _t(quote)			quote

	#define t_strncmp			strncmp
	#define t_strncasecmp		strncasecmp

	#define t_tolower			tolower
	#define t_isdigit			isdigit
    #define t_num_to_string     std::to_string

#endif

    typedef litehtml::string_view<tchar_t>   tstring_view;
    typedef std::basic_string<tchar_t>       tstring;
    typedef std::basic_stringstream<tchar_t> tstring_viewstream;
}
