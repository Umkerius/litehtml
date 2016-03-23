#include "html.h"
#include "types.h"
#include "html_tag.h"

litehtml::tstring_view litehtml::trim(tstring_view str)
{
    tstring_view::size_type space_pos = str.find_first_not_of(_t(" \n\r\t"));
    if (space_pos != 0 && space_pos != tstring_view::npos)
        str.remove_prefix(space_pos);
    else if (space_pos == tstring_view::npos)
        return tstring_view();

    space_pos = str.find_last_not_of(_t(" \n\r\t")) + 1;
    if (space_pos != tstring_view::npos && space_pos != str.size())
        str.remove_suffix(str.size() - space_pos);

    return str;
}

void litehtml::lcase(tstring &s)
{
    for (auto& x : s)
    {
        x = t_tolower(x);
    }
}

litehtml::tstring litehtml::lcase_copy(tstring_view s)
{
    tstring result(s.c_str(), s.size());
    lcase(result);

    return result;
}

litehtml::tstring_view::size_type litehtml::find_close_bracket(tstring_view s, tstring_view::size_type off, tchar_t open_b, tchar_t close_b)
{
	int cnt = 0;
	for(tstring_view::size_type i = off; i < s.length(); i++)
	{
		if(s[i] == open_b)
		{
			cnt++;
		} else if(s[i] == close_b)
		{
			cnt--;
			if(!cnt)
			{
				return i;
			}
		}
	}
	return tstring_view::npos;
}

int litehtml::value_index( tstring_view val, tstring_view strings, int defValue, tchar_t delim )
{
	if(val.empty() || strings.empty() || !delim)
	{
		return defValue;
	}

	int idx = 0;
	tstring_view::size_type delim_start	= 0;
	tstring_view::size_type delim_end	= strings.find(delim, delim_start);
	tstring_view::size_type item_len		= 0;
	while(true)
	{
		if(delim_end == tstring_view::npos)
		{
			item_len = strings.length() - delim_start;
		} else
		{
			item_len = delim_end - delim_start;
		}
		if(item_len == val.length())
		{
			if(val == strings.substr(delim_start, item_len))
			{
				return idx;
			}
		}
		idx++;
		delim_start = delim_end;
		if(delim_start == tstring_view::npos) break;
		delim_start++;
		if(delim_start == strings.length()) break;
		delim_end = strings.find(delim, delim_start);
	}
	return defValue;
}

bool litehtml::value_in_list( tstring_view val, tstring_view strings, tchar_t delim )
{
	int idx = value_index(val, strings, -1, delim);
	if(idx >= 0)
	{
		return true;
	}
	return false;
}

void litehtml::split_string(tstring_view str, string_view_vector& tokens, tstring_view delims, tstring_view delims_preserve, tstring_view quote)
{
	if(str.empty() || (delims.empty() && delims_preserve.empty()))
	{
		return;
	}

    tstring all_delims = delims + delims_preserve + quote;

	tstring_view::size_type token_start	= 0;
	tstring_view::size_type token_end	= str.find_first_of(all_delims, token_start);
	tstring_view::size_type token_len	= 0;
	tstring_view token;
	while(true)
	{
		while( token_end != tstring_view::npos && quote.find_first_of(str[token_end]) != tstring_view::npos )
		{
			if(str[token_end] == _t('('))
			{
				token_end = find_close_bracket(str, token_end, _t('('), _t(')'));
			} else if(str[token_end] == _t('['))
			{
				token_end = find_close_bracket(str, token_end, _t('['), _t(']'));
			} else if(str[token_end] == _t('{'))
			{
				token_end = find_close_bracket(str, token_end, _t('{'), _t('}'));
			} else
			{
				token_end = str.find_first_of(str[token_end], token_end + 1);
			}
			if(token_end != tstring_view::npos)
			{
				token_end = str.find_first_of(all_delims, token_end + 1);
			}
		}

		if(token_end == tstring_view::npos)
		{
			token_len = tstring_view::npos;
		} else
		{
			token_len = token_end - token_start;
		}

		token = str.substr(token_start, token_len);
		if(!token.empty())
		{
			tokens.push_back( token );
		}
		if(token_end != tstring_view::npos && !delims_preserve.empty() && delims_preserve.find_first_of(str[token_end]) != tstring_view::npos)
		{
			tokens.push_back( str.substr(token_end, 1) );
		}

		token_start = token_end;
		if(token_start == tstring_view::npos) break;
		token_start++;
		if(token_start == str.length()) break;
		token_end = str.find_first_of(all_delims, token_start);
	}
}

litehtml::string_vector litehtml::to_string_vector(const litehtml::string_view_vector& views)
{
    litehtml::string_vector result;
    result.reserve(views.size());

    for (const auto& x : views)
    {
        result.emplace_back(x.to_string());
    }

    return result;
}