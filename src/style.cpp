#include "html.h"
#include "style.h"
#include <functional>
#include <algorithm>
#ifndef WINCE
#include <locale>
#endif

litehtml::string_hash_map litehtml::style::m_valid_values =
{
	{ "white-space", (white_space_strings).to_string() }
};

litehtml::style::style()
{
}

litehtml::style::style( const style& val )
{
	m_properties = val.m_properties;
}

litehtml::style::~style()
{

}

void litehtml::style::parse( tstring_view txt, tstring_view baseurl )
{
	string_view_deque properties;
	split_string(txt, properties, _Q(";"));

    for (auto property : properties)
	{
        parse_property(property, baseurl);
	}
}

void litehtml::style::parse_property( tstring_view txt, tstring_view baseurl )
{
	tstring_view::size_type pos = txt.find_first_of(_Q(":"));
	if(pos != tstring_view::npos)
	{
		tstring name = lcase_copy(trim(txt.substr(0, pos)));
		tstring_view val = trim(txt.substr(pos + 1));

		if(!name.empty() && !val.empty())
		{
			string_view_deque vals;
			split_string(val, vals, _Q("!"));

			if(vals.size() == 1)
			{
				add_property(name, val, baseurl, false);
			} 
            else if(vals.size() > 1)
			{
                vals[0] = trim(vals[0]);
                tstring val_type = lcase_copy(vals[1]);

                if (val_type == _Q("important"))
				{
					add_property(name, vals[0], baseurl, true);
				} else
				{
					add_property(name, vals[0], baseurl, false);
				}
			}
		}
	}
}

void litehtml::style::combine( const litehtml::style& src )
{
	for(props_map::const_iterator i = src.m_properties.begin(); i != src.m_properties.end(); i++)
	{
		add_parsed_property(i->first, i->second.m_value, i->second.m_important);
	}
}

void litehtml::style::add_property(tstring_view name, tstring_view val, tstring_view baseurl, bool important)
{
	if(name.empty() || val.empty())
	{
		return;
	}

	// Add baseurl for background image 
	if(name == _Q("background-image"))
	{
		add_parsed_property(name, val, important);
		if(!baseurl.empty())
		{
			add_parsed_property(_Q("background-image-baseurl"), baseurl, important);
		}
	} else

	// Parse border spacing properties 
	if(name == _Q("border-spacing"))
	{
		string_view_deque tokens;
        split_string(val, tokens, _Q(" "));
		if(tokens.size() == 1)
		{
			add_property(_Q("-litehtml-border-spacing-x"), tokens[0], baseurl, important);
			add_property(_Q("-litehtml-border-spacing-y"), tokens[0], baseurl, important);
		} else if(tokens.size() == 2)
		{
			add_property(_Q("-litehtml-border-spacing-x"), tokens[0], baseurl, important);
			add_property(_Q("-litehtml-border-spacing-y"), tokens[1], baseurl, important);
		}
	} else

	// Parse borders shorthand properties 

	if(name == _Q("border"))
	{
        string_view_deque tokens;
        split_string(val, tokens, _Q(" "), _Q(""), _Q("("));
		int idx;
		tstring_view str;
		for(auto tok : tokens)
		{
			idx = value_index(tok, border_style_strings, -1);
			if(idx >= 0)
			{
				add_property(_Q("border-left-style"), tok, baseurl, important);
                add_property(_Q("border-right-style"), tok, baseurl, important);
                add_property(_Q("border-top-style"), tok, baseurl, important);
                add_property(_Q("border-bottom-style"), tok, baseurl, important);
			} else
			{
                if (web_color::is_color(tok))
				{
                    add_property(_Q("border-left-color"), tok, baseurl, important);
                    add_property(_Q("border-right-color"), tok, baseurl, important);
                    add_property(_Q("border-top-color"), tok, baseurl, important);
                    add_property(_Q("border-bottom-color"), tok, baseurl, important);
				} else
				{
                    add_property(_Q("border-left-width"), tok, baseurl, important);
                    add_property(_Q("border-right-width"), tok, baseurl, important);
                    add_property(_Q("border-top-width"), tok, baseurl, important);
                    add_property(_Q("border-bottom-width"), tok, baseurl, important);
				}
			}
		}
	} else if(name == _Q("border-left")  ||
		      name == _Q("border-right") ||
		      name == _Q("border-top")   ||
		      name == _Q("border-bottom"))
	{
        string_view_deque tokens;
        split_string(val, tokens, _Q(" "), _Q(""), _Q("("));
		int idx;
		tstring str;
        for (auto tok : tokens)
		{
			idx = value_index(tok, border_style_strings, -1);
			if(idx >= 0)
			{
                str = name + _Q("-style");
				add_property(str, tok, baseurl, important);
			} else
			{
                if (web_color::is_color(tok))
				{
                    str = name + _Q("-color");
                    add_property(str, tok, baseurl, important);
				} else
				{
					str = name + _Q("-width");
                    add_property(str, tok, baseurl, important);
				}
			}
		}
	} else 

	// Parse border radius shorthand properties 
	if(name == _Q("border-bottom-left-radius"))
	{
		string_view_deque tokens;
        split_string(val, tokens, _Q(" "));
		if(tokens.size() >= 2)
		{
			add_property(_Q("border-bottom-left-radius-x"), tokens[0], baseurl, important);
			add_property(_Q("border-bottom-left-radius-y"), tokens[1], baseurl, important);
		} else if(tokens.size() == 1)
		{
			add_property(_Q("border-bottom-left-radius-x"), tokens[0], baseurl, important);
			add_property(_Q("border-bottom-left-radius-y"), tokens[0], baseurl, important);
		}

	} else if(name == _Q("border-bottom-right-radius"))
	{
        string_view_deque tokens;
        split_string(val, tokens, _Q(" "));
		if(tokens.size() >= 2)
		{
			add_property(_Q("border-bottom-right-radius-x"), tokens[0], baseurl, important);
			add_property(_Q("border-bottom-right-radius-y"), tokens[1], baseurl, important);
		} else if(tokens.size() == 1)
		{
			add_property(_Q("border-bottom-right-radius-x"), tokens[0], baseurl, important);
			add_property(_Q("border-bottom-right-radius-y"), tokens[0], baseurl, important);
		}

	} else if(name == _Q("border-top-right-radius"))
	{
        string_view_deque tokens;
        split_string(val, tokens, _Q(" "));
		if(tokens.size() >= 2)
		{
			add_property(_Q("border-top-right-radius-x"), tokens[0], baseurl, important);
			add_property(_Q("border-top-right-radius-y"), tokens[1], baseurl, important);
		} else if(tokens.size() == 1)
		{
			add_property(_Q("border-top-right-radius-x"), tokens[0], baseurl, important);
			add_property(_Q("border-top-right-radius-y"), tokens[0], baseurl, important);
		}

	} else if(name == _Q("border-top-left-radius"))
	{
        string_view_deque tokens;
        split_string(val, tokens, _Q(" "));
		if(tokens.size() >= 2)
		{
			add_property(_Q("border-top-left-radius-x"), tokens[0], baseurl, important);
			add_property(_Q("border-top-left-radius-y"), tokens[1], baseurl, important);
		} else if(tokens.size() == 1)
		{
			add_property(_Q("border-top-left-radius-x"), tokens[0], baseurl, important);
			add_property(_Q("border-top-left-radius-y"), tokens[0], baseurl, important);
		}

	} else 

	// Parse border-radius shorthand properties 
	if(name == _Q("border-radius"))
	{
        string_view_deque tokens;
        split_string(val, tokens, _Q("/"));
		if(tokens.size() == 1)
		{
			add_property(_Q("border-radius-x"), tokens[0], baseurl, important);
			add_property(_Q("border-radius-y"), tokens[0], baseurl, important);
		} else if(tokens.size() >= 2)
		{
			add_property(_Q("border-radius-x"), tokens[0], baseurl, important);
			add_property(_Q("border-radius-y"), tokens[1], baseurl, important);
		}
	} else if(name == _Q("border-radius-x"))
	{
        string_view_deque tokens;
        split_string(val, tokens, _Q(" "));
		if(tokens.size() == 1)
		{
			add_property(_Q("border-top-left-radius-x"),		tokens[0], baseurl, important);
			add_property(_Q("border-top-right-radius-x"),		tokens[0], baseurl, important);
			add_property(_Q("border-bottom-right-radius-x"),	tokens[0], baseurl, important);
			add_property(_Q("border-bottom-left-radius-x"),	tokens[0], baseurl, important);
		} else if(tokens.size() == 2)
		{
			add_property(_Q("border-top-left-radius-x"),		tokens[0], baseurl, important);
			add_property(_Q("border-top-right-radius-x"),		tokens[1], baseurl, important);
			add_property(_Q("border-bottom-right-radius-x"),	tokens[0], baseurl, important);
			add_property(_Q("border-bottom-left-radius-x"),	tokens[1], baseurl, important);
		} else if(tokens.size() == 3)
		{
			add_property(_Q("border-top-left-radius-x"),		tokens[0], baseurl, important);
			add_property(_Q("border-top-right-radius-x"),		tokens[1], baseurl, important);
			add_property(_Q("border-bottom-right-radius-x"),	tokens[2], baseurl, important);
			add_property(_Q("border-bottom-left-radius-x"),	tokens[1], baseurl, important);
		} else if(tokens.size() == 4)
		{
			add_property(_Q("border-top-left-radius-x"),		tokens[0], baseurl, important);
			add_property(_Q("border-top-right-radius-x"),		tokens[1], baseurl, important);
			add_property(_Q("border-bottom-right-radius-x"),	tokens[2], baseurl, important);
			add_property(_Q("border-bottom-left-radius-x"),	tokens[3], baseurl, important);
		}
	} else if(name == _Q("border-radius-y"))
	{
        string_view_deque tokens;
		split_string(val, tokens, _Q(" "));
		if(tokens.size() == 1)
		{
			add_property(_Q("border-top-left-radius-y"),		tokens[0], baseurl, important);
			add_property(_Q("border-top-right-radius-y"),		tokens[0], baseurl, important);
			add_property(_Q("border-bottom-right-radius-y"),	tokens[0], baseurl, important);
			add_property(_Q("border-bottom-left-radius-y"),	tokens[0], baseurl, important);
		} else if(tokens.size() == 2)
		{
			add_property(_Q("border-top-left-radius-y"),		tokens[0], baseurl, important);
			add_property(_Q("border-top-right-radius-y"),		tokens[1], baseurl, important);
			add_property(_Q("border-bottom-right-radius-y"),	tokens[0], baseurl, important);
			add_property(_Q("border-bottom-left-radius-y"),	tokens[1], baseurl, important);
		} else if(tokens.size() == 3)
		{
			add_property(_Q("border-top-left-radius-y"),		tokens[0], baseurl, important);
			add_property(_Q("border-top-right-radius-y"),		tokens[1], baseurl, important);
			add_property(_Q("border-bottom-right-radius-y"),	tokens[2], baseurl, important);
			add_property(_Q("border-bottom-left-radius-y"),	tokens[1], baseurl, important);
		} else if(tokens.size() == 4)
		{
			add_property(_Q("border-top-left-radius-y"),		tokens[0], baseurl, important);
			add_property(_Q("border-top-right-radius-y"),		tokens[1], baseurl, important);
			add_property(_Q("border-bottom-right-radius-y"),	tokens[2], baseurl, important);
			add_property(_Q("border-bottom-left-radius-y"),	tokens[3], baseurl, important);
		}
	}
	

	// Parse list-style shorthand properties 
	if(name == _Q("list-style"))
	{
		add_parsed_property(_Q("list-style-type"),			_Q("disc"),		important);
		add_parsed_property(_Q("list-style-position"),		_Q("outside"),	important);
		add_parsed_property(_Q("list-style-image"),			_Q(""),			important);
		add_parsed_property(_Q("list-style-image-baseurl"),	_Q(""),			important);

        string_view_deque tokens;
		split_string(val, tokens, _Q(" "), _Q(""), _Q("("));
		for(auto tok : tokens)
		{
			int idx = value_index(tok, list_style_type_strings, -1);
			if(idx >= 0)
			{
				add_parsed_property(_Q("list-style-type"), tok, important);
			} else
			{
				idx = value_index(tok, list_style_position_strings, -1);
				if(idx >= 0)
				{
					add_parsed_property(_Q("list-style-position"), tok, important);
				} else if(val.substr(0, 3) == _Q("url"))
				{
					add_parsed_property(_Q("list-style-image"), tok, important);
                    if (!baseurl.empty())
					{
						add_parsed_property(_Q("list-style-image-baseurl"), baseurl, important);
					}
				}
			}
		}
	} else 

	// Add baseurl for background image 
	if(name == _Q("list-style-image"))
	{
		add_parsed_property(name, val, important);
        if (!baseurl.empty())
		{
			add_parsed_property(_Q("list-style-image-baseurl"), baseurl, important);
		}
	} else
		
	// Parse background shorthand properties 
	if(name == _Q("background"))
	{
		parse_short_background(val, baseurl, important);

	} else 
		
	// Parse margin and padding shorthand properties 
	if(name == _Q("margin") || name == _Q("padding"))
	{
        string_view_deque tokens;
		split_string(val, tokens, _Q(" "));
		if(tokens.size() >= 4)
		{
			add_parsed_property(name + _Q("-top"),	  tokens[0], important);
            add_parsed_property(name + _Q("-right"),  tokens[1], important);
            add_parsed_property(name + _Q("-bottom"), tokens[2], important);
            add_parsed_property(name + _Q("-left"),   tokens[3], important);
		} else if(tokens.size() == 3)
		{
            add_parsed_property(name + _Q("-top"), tokens[0], important);
            add_parsed_property(name + _Q("-right"), tokens[1], important);
            add_parsed_property(name + _Q("-left"), tokens[1], important);
            add_parsed_property(name + _Q("-bottom"), tokens[2], important);
		} else if(tokens.size() == 2)
		{
            add_parsed_property(name + _Q("-top"), tokens[0], important);
            add_parsed_property(name + _Q("-bottom"), tokens[0], important);
            add_parsed_property(name + _Q("-right"), tokens[1], important);
            add_parsed_property(name + _Q("-left"), tokens[1], important);
		} else if(tokens.size() == 1)
		{
            add_parsed_property(name + _Q("-top"), tokens[0], important);
            add_parsed_property(name + _Q("-bottom"), tokens[0], important);
            add_parsed_property(name + _Q("-right"), tokens[0], important);
            add_parsed_property(name + _Q("-left"), tokens[0], important);
		}
	} else 
		
		
	// Parse border-* shorthand properties 
	if(name == _Q("border-left")   || 
	   name == _Q("border-right") ||
	   name == _Q("border-top")   || 
	   name == _Q("border-bottom"))
	{
		parse_short_border(name, val, important);
	} else 
		
	// Parse border-width/style/color shorthand properties 
	if(name == _Q("border-width") || name == _Q("border-style") || name == _Q("border-color"))
	{
        string_view_deque nametokens;
		split_string(name, nametokens, _Q("-"));

        string_view_deque tokens;
		split_string(val, tokens, _Q(" "));

		if(tokens.size() >= 4)
		{
            add_parsed_property(nametokens[0] + _Q("-top-") + nametokens[1], tokens[0], important);
            add_parsed_property(nametokens[0] + _Q("-right-") + nametokens[1], tokens[1], important);
            add_parsed_property(nametokens[0] + _Q("-bottom-") + nametokens[1], tokens[2], important);
            add_parsed_property(nametokens[0] + _Q("-left-") + nametokens[1], tokens[3], important);
		} else if(tokens.size() == 3)
		{
            add_parsed_property(nametokens[0] + _Q("-top-") + nametokens[1], tokens[0], important);
            add_parsed_property(nametokens[0] + _Q("-right-") + nametokens[1], tokens[1], important);
            add_parsed_property(nametokens[0] + _Q("-left-") + nametokens[1], tokens[1], important);
            add_parsed_property(nametokens[0] + _Q("-bottom-") + nametokens[1], tokens[2], important);
		} else if(tokens.size() == 2)
		{
            add_parsed_property(nametokens[0] + _Q("-top-") + nametokens[1], tokens[0], important);
            add_parsed_property(nametokens[0] + _Q("-bottom-") + nametokens[1], tokens[0], important);
            add_parsed_property(nametokens[0] + _Q("-right-") + nametokens[1], tokens[1], important);
            add_parsed_property(nametokens[0] + _Q("-left-") + nametokens[1], tokens[1], important);
		} else if(tokens.size() == 1)
		{
            add_parsed_property(nametokens[0] + _Q("-top-") + nametokens[1], tokens[0], important);
            add_parsed_property(nametokens[0] + _Q("-bottom-") + nametokens[1], tokens[0], important);
            add_parsed_property(nametokens[0] + _Q("-right-") + nametokens[1], tokens[0], important);
            add_parsed_property(nametokens[0] + _Q("-left-") + nametokens[1], tokens[0], important);
		}
	} else 
		
	// Parse font shorthand properties 
	if(name == _Q("font"))
	{
		parse_short_font(val, important);
	} else 
	{
		add_parsed_property(name, val, important);
	}
}

void litehtml::style::parse_short_border( tstring_view prefix, tstring_view val, bool important )
{
    string_view_deque tokens;
	split_string(val, tokens, _Q(" "), _Q(""), _Q("("));
	if(tokens.size() >= 3)
	{
		add_parsed_property(prefix + _Q("-width"),	tokens[0], important);
		add_parsed_property(prefix + _Q("-style"),	tokens[1], important);
		add_parsed_property(prefix + _Q("-color"),	tokens[2], important);
	} else if(tokens.size() == 2)
	{
		if(iswdigit(tokens[0][0]) || value_index(val, border_width_strings) >= 0)
		{
			add_parsed_property(prefix + _Q("-width"),	tokens[0], important);
			add_parsed_property(prefix + _Q("-style"),	tokens[1], important);
		} else
		{
			add_parsed_property(prefix + _Q("-style"),	tokens[0], important);
			add_parsed_property(prefix + _Q("-color"),	tokens[1], important);
		}
	}
}

void litehtml::style::parse_short_background( tstring_view val, tstring_view baseurl, bool important )
{
	add_parsed_property(_Q("background-color"),			_Q("transparent"),	important);
	add_parsed_property(_Q("background-image"),			_Q(""),				important);
	add_parsed_property(_Q("background-image-baseurl"), _Q(""),				important);
	add_parsed_property(_Q("background-repeat"),		_Q("repeat"),		important);
	add_parsed_property(_Q("background-origin"),		_Q("padding-box"),	important);
	add_parsed_property(_Q("background-clip"),			_Q("border-box"),	important);
	add_parsed_property(_Q("background-attachment"),	_Q("scroll"),		important);

	if(val == _Q("none"))
	{
		return;
	}

	string_view_deque tokens;
	split_string(val, tokens, _Q(" "), _Q(""), _Q("("));
	bool origin_found = false;
	for(auto tok : tokens)
	{
		if(web_color::is_color(tok))
		{
			add_parsed_property(_Q("background-color"), tok, important);
		} else if(tok.substr(0, 3) == _Q("url"))
		{
			add_parsed_property(_Q("background-image"), tok, important);
			if(!baseurl.empty())
			{
				add_parsed_property(_Q("background-image-baseurl"), baseurl, important);
			}

		} else if( value_in_list(tok, background_repeat_strings) )
		{
			add_parsed_property(_Q("background-repeat"), tok, important);
		} else if( value_in_list(tok, background_attachment_strings) )
		{
			add_parsed_property(_Q("background-attachment"), tok, important);
		} else if( value_in_list(tok, background_box_strings) )
		{
			if(!origin_found)
			{
				add_parsed_property(_Q("background-origin"), tok, important);
				origin_found = true;
			} else
			{
				add_parsed_property(_Q("background-clip"), tok, important);
			}
		} else if(	value_in_list(tok, _Q("left;right;top;bottom;center")) ||
					iswdigit(tok[0]) ||
					tok[0] == '-'	||
					tok[0] == '.'	||
					tok[0] == '+')
		{
			if(m_properties.find("background-position") != m_properties.end())
			{
				m_properties["background-position"].m_value = m_properties["background-position"].m_value + _Q(" ") + tok;
			} else
			{
				add_parsed_property(_Q("background-position"), tok, important);
			}
		}
	}
}

void litehtml::style::parse_short_font( tstring_view val, bool important )
{
	add_parsed_property(_Q("font-style"),	_Q("normal"),	important);
	add_parsed_property(_Q("font-variant"),	_Q("normal"),	important);
	add_parsed_property(_Q("font-weight"),	_Q("normal"),	important);
	add_parsed_property(_Q("font-size"),		_Q("medium"),	important);
	add_parsed_property(_Q("line-height"),	_Q("normal"),	important);

    string_view_deque tokens;
	split_string(val, tokens, _Q(" "), _Q(""), _Q("\""));

	int idx = 0;
	bool was_normal = false;
	bool is_family = false;
	tstring font_family;
	for(auto tok : tokens)
	{
		idx = value_index(tok, font_style_strings);
		if(!is_family)
		{
			if(idx >= 0)
			{
				if(idx == 0 && !was_normal)
				{
					add_parsed_property(_Q("font-weight"),		tok, important);
					add_parsed_property(_Q("font-variant"),		tok, important);
					add_parsed_property(_Q("font-style"),		tok, important);
				} else
				{
					add_parsed_property(_Q("font-style"),		tok, important);
				}
			} else
			{
				if(value_in_list(tok, font_weight_strings))
				{
					add_parsed_property(_Q("font-weight"),		tok, important);
				} else
				{
					if(value_in_list(tok, font_variant_strings))
					{
						add_parsed_property(_Q("font-variant"),	tok, important);
					} else if( iswdigit(tok[0]) )
					{
                        string_view_deque szlh;
						split_string(tok, szlh, _Q("/"));

						if(szlh.size() == 1)
						{
							add_parsed_property(_Q("font-size"),	szlh[0], important);
						} else	if(szlh.size() >= 2)
						{
							add_parsed_property(_Q("font-size"),	szlh[0], important);
							add_parsed_property(_Q("line-height"),	szlh[1], important);
						}
					} else
					{
						is_family = true;
						font_family += tok.to_string();
					}
				}
			}
		} else
		{
			font_family += tok.to_string();
		}
	}
	add_parsed_property(_Q("font-family"), font_family, important);
}

void litehtml::style::add_parsed_property( tstring_view name, tstring_view val, bool important )
{
	bool is_valid = true;
	auto vals = m_valid_values.find(name.to_string());
	if (vals != m_valid_values.end())
	{
		if (!value_in_list(val, vals->second))
		{
			is_valid = false;
		}
	}

	if (is_valid)
	{
		props_map::iterator prop = m_properties.find(name.to_string());
		if (prop != m_properties.end())
		{
			if (!prop->second.m_important || (important && prop->second.m_important))
			{
				prop->second.m_value = val.to_string();
				prop->second.m_important = important;
			}
		}
		else
		{
			m_properties[name.to_string()] = property_value(val, important);
		}
	}
}

void litehtml::style::remove_property( tstring_view name, bool important )
{
    props_map::iterator prop = m_properties.find(name.to_string());
	if(prop != m_properties.end())
	{
		if( !prop->second.m_important || (important && prop->second.m_important) )
		{
			m_properties.erase(prop);
		}
	}
}
