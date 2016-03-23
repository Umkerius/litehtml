#include "html.h"
#include "style.h"
#include <functional>
#include <algorithm>
#ifndef WINCE
#include <locale>
#endif

litehtml::string_map litehtml::style::m_valid_values =
{
	{ _t("white-space"), white_space_strings }
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
	string_view_vector properties;
	split_string(txt, properties, _t(";"));

	for(auto i = properties.begin(); i != properties.end(); i++)
	{
		parse_property(*i, baseurl);
	}
}

void litehtml::style::parse_property( const tstring_view& txt, tstring_view baseurl )
{
	tstring_view::size_type pos = txt.find_first_of(_t(":"));
	if(pos != tstring_view::npos)
	{
		tstring name	= trim(txt.substr(0, pos)).to_string();
		tstring_view val	= trim(txt.substr(pos + 1));
		lcase(name);

		if(!name.empty() && !val.empty())
		{
			string_view_vector vals;
			split_string(val, vals, _t("!"));

			if(vals.size() == 1)
			{
				add_property(name, val, baseurl, false);
			} 
            else if(vals.size() > 1)
			{
                vals[0] = trim(vals[0]);
                tstring val_type = vals[1].to_string();
                lcase(val_type);

                if (val_type == _t("important"))
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
	if(name == _t("background-image"))
	{
		add_parsed_property(name, val, important);
		if(!baseurl.empty())
		{
			add_parsed_property(_t("background-image-baseurl"), baseurl, important);
		}
	} else

	// Parse border spacing properties 
	if(name == _t("border-spacing"))
	{
		string_view_vector tokens;
        split_string(val, tokens, _t(" "));
		if(tokens.size() == 1)
		{
			add_property(_t("-litehtml-border-spacing-x"), tokens[0], baseurl, important);
			add_property(_t("-litehtml-border-spacing-y"), tokens[0], baseurl, important);
		} else if(tokens.size() == 2)
		{
			add_property(_t("-litehtml-border-spacing-x"), tokens[0], baseurl, important);
			add_property(_t("-litehtml-border-spacing-y"), tokens[1], baseurl, important);
		}
	} else

	// Parse borders shorthand properties 

	if(name == _t("border"))
	{
        string_view_vector tokens;
        split_string(val, tokens, _t(" "), _t(""), _t("("));
		int idx;
		tstring_view str;
		for(auto tok : tokens)
		{
			idx = value_index(tok, border_style_strings, -1);
			if(idx >= 0)
			{
				add_property(_t("border-left-style"), tok, baseurl, important);
                add_property(_t("border-right-style"), tok, baseurl, important);
                add_property(_t("border-top-style"), tok, baseurl, important);
                add_property(_t("border-bottom-style"), tok, baseurl, important);
			} else
			{
                if (web_color::is_color(tok))
				{
                    add_property(_t("border-left-color"), tok, baseurl, important);
                    add_property(_t("border-right-color"), tok, baseurl, important);
                    add_property(_t("border-top-color"), tok, baseurl, important);
                    add_property(_t("border-bottom-color"), tok, baseurl, important);
				} else
				{
                    add_property(_t("border-left-width"), tok, baseurl, important);
                    add_property(_t("border-right-width"), tok, baseurl, important);
                    add_property(_t("border-top-width"), tok, baseurl, important);
                    add_property(_t("border-bottom-width"), tok, baseurl, important);
				}
			}
		}
	} else if(name == _t("border-left")  ||
		      name == _t("border-right") ||
		      name == _t("border-top")   ||
		      name == _t("border-bottom"))
	{
        string_view_vector tokens;
        split_string(val, tokens, _t(" "), _t(""), _t("("));
		int idx;
		tstring str;
        for (auto tok : tokens)
		{
			idx = value_index(tok, border_style_strings, -1);
			if(idx >= 0)
			{
                str = name + _t("-style");
				add_property(str, tok, baseurl, important);
			} else
			{
                if (web_color::is_color(tok))
				{
                    str = name + _t("-color");
                    add_property(str, tok, baseurl, important);
				} else
				{
					str = name + _t("-width");
                    add_property(str, tok, baseurl, important);
				}
			}
		}
	} else 

	// Parse border radius shorthand properties 
	if(name == _t("border-bottom-left-radius"))
	{
		string_view_vector tokens;
        split_string(val, tokens, _t(" "));
		if(tokens.size() >= 2)
		{
			add_property(_t("border-bottom-left-radius-x"), tokens[0], baseurl, important);
			add_property(_t("border-bottom-left-radius-y"), tokens[1], baseurl, important);
		} else if(tokens.size() == 1)
		{
			add_property(_t("border-bottom-left-radius-x"), tokens[0], baseurl, important);
			add_property(_t("border-bottom-left-radius-y"), tokens[0], baseurl, important);
		}

	} else if(name == _t("border-bottom-right-radius"))
	{
        string_view_vector tokens;
        split_string(val, tokens, _t(" "));
		if(tokens.size() >= 2)
		{
			add_property(_t("border-bottom-right-radius-x"), tokens[0], baseurl, important);
			add_property(_t("border-bottom-right-radius-y"), tokens[1], baseurl, important);
		} else if(tokens.size() == 1)
		{
			add_property(_t("border-bottom-right-radius-x"), tokens[0], baseurl, important);
			add_property(_t("border-bottom-right-radius-y"), tokens[0], baseurl, important);
		}

	} else if(name == _t("border-top-right-radius"))
	{
        string_view_vector tokens;
        split_string(val, tokens, _t(" "));
		if(tokens.size() >= 2)
		{
			add_property(_t("border-top-right-radius-x"), tokens[0], baseurl, important);
			add_property(_t("border-top-right-radius-y"), tokens[1], baseurl, important);
		} else if(tokens.size() == 1)
		{
			add_property(_t("border-top-right-radius-x"), tokens[0], baseurl, important);
			add_property(_t("border-top-right-radius-y"), tokens[0], baseurl, important);
		}

	} else if(name == _t("border-top-left-radius"))
	{
        string_view_vector tokens;
        split_string(val, tokens, _t(" "));
		if(tokens.size() >= 2)
		{
			add_property(_t("border-top-left-radius-x"), tokens[0], baseurl, important);
			add_property(_t("border-top-left-radius-y"), tokens[1], baseurl, important);
		} else if(tokens.size() == 1)
		{
			add_property(_t("border-top-left-radius-x"), tokens[0], baseurl, important);
			add_property(_t("border-top-left-radius-y"), tokens[0], baseurl, important);
		}

	} else 

	// Parse border-radius shorthand properties 
	if(name == _t("border-radius"))
	{
        string_view_vector tokens;
        split_string(val, tokens, _t("/"));
		if(tokens.size() == 1)
		{
			add_property(_t("border-radius-x"), tokens[0], baseurl, important);
			add_property(_t("border-radius-y"), tokens[0], baseurl, important);
		} else if(tokens.size() >= 2)
		{
			add_property(_t("border-radius-x"), tokens[0], baseurl, important);
			add_property(_t("border-radius-y"), tokens[1], baseurl, important);
		}
	} else if(name == _t("border-radius-x"))
	{
        string_view_vector tokens;
        split_string(val, tokens, _t(" "));
		if(tokens.size() == 1)
		{
			add_property(_t("border-top-left-radius-x"),		tokens[0], baseurl, important);
			add_property(_t("border-top-right-radius-x"),		tokens[0], baseurl, important);
			add_property(_t("border-bottom-right-radius-x"),	tokens[0], baseurl, important);
			add_property(_t("border-bottom-left-radius-x"),	tokens[0], baseurl, important);
		} else if(tokens.size() == 2)
		{
			add_property(_t("border-top-left-radius-x"),		tokens[0], baseurl, important);
			add_property(_t("border-top-right-radius-x"),		tokens[1], baseurl, important);
			add_property(_t("border-bottom-right-radius-x"),	tokens[0], baseurl, important);
			add_property(_t("border-bottom-left-radius-x"),	tokens[1], baseurl, important);
		} else if(tokens.size() == 3)
		{
			add_property(_t("border-top-left-radius-x"),		tokens[0], baseurl, important);
			add_property(_t("border-top-right-radius-x"),		tokens[1], baseurl, important);
			add_property(_t("border-bottom-right-radius-x"),	tokens[2], baseurl, important);
			add_property(_t("border-bottom-left-radius-x"),	tokens[1], baseurl, important);
		} else if(tokens.size() == 4)
		{
			add_property(_t("border-top-left-radius-x"),		tokens[0], baseurl, important);
			add_property(_t("border-top-right-radius-x"),		tokens[1], baseurl, important);
			add_property(_t("border-bottom-right-radius-x"),	tokens[2], baseurl, important);
			add_property(_t("border-bottom-left-radius-x"),	tokens[3], baseurl, important);
		}
	} else if(name == _t("border-radius-y"))
	{
        string_view_vector tokens;
		split_string(val, tokens, _t(" "));
		if(tokens.size() == 1)
		{
			add_property(_t("border-top-left-radius-y"),		tokens[0], baseurl, important);
			add_property(_t("border-top-right-radius-y"),		tokens[0], baseurl, important);
			add_property(_t("border-bottom-right-radius-y"),	tokens[0], baseurl, important);
			add_property(_t("border-bottom-left-radius-y"),	tokens[0], baseurl, important);
		} else if(tokens.size() == 2)
		{
			add_property(_t("border-top-left-radius-y"),		tokens[0], baseurl, important);
			add_property(_t("border-top-right-radius-y"),		tokens[1], baseurl, important);
			add_property(_t("border-bottom-right-radius-y"),	tokens[0], baseurl, important);
			add_property(_t("border-bottom-left-radius-y"),	tokens[1], baseurl, important);
		} else if(tokens.size() == 3)
		{
			add_property(_t("border-top-left-radius-y"),		tokens[0], baseurl, important);
			add_property(_t("border-top-right-radius-y"),		tokens[1], baseurl, important);
			add_property(_t("border-bottom-right-radius-y"),	tokens[2], baseurl, important);
			add_property(_t("border-bottom-left-radius-y"),	tokens[1], baseurl, important);
		} else if(tokens.size() == 4)
		{
			add_property(_t("border-top-left-radius-y"),		tokens[0], baseurl, important);
			add_property(_t("border-top-right-radius-y"),		tokens[1], baseurl, important);
			add_property(_t("border-bottom-right-radius-y"),	tokens[2], baseurl, important);
			add_property(_t("border-bottom-left-radius-y"),	tokens[3], baseurl, important);
		}
	}
	

	// Parse list-style shorthand properties 
	if(name == _t("list-style"))
	{
		add_parsed_property(_t("list-style-type"),			_t("disc"),		important);
		add_parsed_property(_t("list-style-position"),		_t("outside"),	important);
		add_parsed_property(_t("list-style-image"),			_t(""),			important);
		add_parsed_property(_t("list-style-image-baseurl"),	_t(""),			important);

        string_view_vector tokens;
		split_string(val, tokens, _t(" "), _t(""), _t("("));
		for(auto tok : tokens)
		{
			int idx = value_index(tok, list_style_type_strings, -1);
			if(idx >= 0)
			{
				add_parsed_property(_t("list-style-type"), tok, important);
			} else
			{
				idx = value_index(tok, list_style_position_strings, -1);
				if(idx >= 0)
				{
					add_parsed_property(_t("list-style-position"), tok, important);
				} else if(val.substr(0, 3) == _t("url"))
				{
					add_parsed_property(_t("list-style-image"), tok, important);
                    if (!baseurl.empty())
					{
						add_parsed_property(_t("list-style-image-baseurl"), baseurl, important);
					}
				}
			}
		}
	} else 

	// Add baseurl for background image 
	if(name == _t("list-style-image"))
	{
		add_parsed_property(name, val, important);
        if (!baseurl.empty())
		{
			add_parsed_property(_t("list-style-image-baseurl"), baseurl, important);
		}
	} else
		
	// Parse background shorthand properties 
	if(name == _t("background"))
	{
		parse_short_background(val, baseurl, important);

	} else 
		
	// Parse margin and padding shorthand properties 
	if(name == _t("margin") || name == _t("padding"))
	{
        string_view_vector tokens;
		split_string(val, tokens, _t(" "));
		if(tokens.size() >= 4)
		{
			add_parsed_property(name + _t("-top"),	  tokens[0], important);
            add_parsed_property(name + _t("-right"),  tokens[1], important);
            add_parsed_property(name + _t("-bottom"), tokens[2], important);
            add_parsed_property(name + _t("-left"),   tokens[3], important);
		} else if(tokens.size() == 3)
		{
            add_parsed_property(name + _t("-top"), tokens[0], important);
            add_parsed_property(name + _t("-right"), tokens[1], important);
            add_parsed_property(name + _t("-left"), tokens[1], important);
            add_parsed_property(name + _t("-bottom"), tokens[2], important);
		} else if(tokens.size() == 2)
		{
            add_parsed_property(name + _t("-top"), tokens[0], important);
            add_parsed_property(name + _t("-bottom"), tokens[0], important);
            add_parsed_property(name + _t("-right"), tokens[1], important);
            add_parsed_property(name + _t("-left"), tokens[1], important);
		} else if(tokens.size() == 1)
		{
            add_parsed_property(name + _t("-top"), tokens[0], important);
            add_parsed_property(name + _t("-bottom"), tokens[0], important);
            add_parsed_property(name + _t("-right"), tokens[0], important);
            add_parsed_property(name + _t("-left"), tokens[0], important);
		}
	} else 
		
		
	// Parse border-* shorthand properties 
	if(name == _t("border-left")   || 
	   name == _t("border-right") ||
	   name == _t("border-top")   || 
	   name == _t("border-bottom"))
	{
		parse_short_border(name, val, important);
	} else 
		
	// Parse border-width/style/color shorthand properties 
	if(name == _t("border-width") || name == _t("border-style") || name == _t("border-color"))
	{
        string_view_vector nametokens;
		split_string(name, nametokens, _t("-"));

        string_view_vector tokens;
		split_string(val, tokens, _t(" "));

		if(tokens.size() >= 4)
		{
            add_parsed_property(nametokens[0] + _t("-top-") + nametokens[1], tokens[0], important);
            add_parsed_property(nametokens[0] + _t("-right-") + nametokens[1], tokens[1], important);
            add_parsed_property(nametokens[0] + _t("-bottom-") + nametokens[1], tokens[2], important);
            add_parsed_property(nametokens[0] + _t("-left-") + nametokens[1], tokens[3], important);
		} else if(tokens.size() == 3)
		{
            add_parsed_property(nametokens[0] + _t("-top-") + nametokens[1], tokens[0], important);
            add_parsed_property(nametokens[0] + _t("-right-") + nametokens[1], tokens[1], important);
            add_parsed_property(nametokens[0] + _t("-left-") + nametokens[1], tokens[1], important);
            add_parsed_property(nametokens[0] + _t("-bottom-") + nametokens[1], tokens[2], important);
		} else if(tokens.size() == 2)
		{
            add_parsed_property(nametokens[0] + _t("-top-") + nametokens[1], tokens[0], important);
            add_parsed_property(nametokens[0] + _t("-bottom-") + nametokens[1], tokens[0], important);
            add_parsed_property(nametokens[0] + _t("-right-") + nametokens[1], tokens[1], important);
            add_parsed_property(nametokens[0] + _t("-left-") + nametokens[1], tokens[1], important);
		} else if(tokens.size() == 1)
		{
            add_parsed_property(nametokens[0] + _t("-top-") + nametokens[1], tokens[0], important);
            add_parsed_property(nametokens[0] + _t("-bottom-") + nametokens[1], tokens[0], important);
            add_parsed_property(nametokens[0] + _t("-right-") + nametokens[1], tokens[0], important);
            add_parsed_property(nametokens[0] + _t("-left-") + nametokens[1], tokens[0], important);
		}
	} else 
		
	// Parse font shorthand properties 
	if(name == _t("font"))
	{
		parse_short_font(val, important);
	} else 
	{
		add_parsed_property(name, val, important);
	}
}

void litehtml::style::parse_short_border( const tstring_view& prefix, const tstring_view& val, bool important )
{
    string_view_vector tokens;
	split_string(val, tokens, _t(" "), _t(""), _t("("));
	if(tokens.size() >= 3)
	{
		add_parsed_property(prefix + _t("-width"),	tokens[0], important);
		add_parsed_property(prefix + _t("-style"),	tokens[1], important);
		add_parsed_property(prefix + _t("-color"),	tokens[2], important);
	} else if(tokens.size() == 2)
	{
		if(iswdigit(tokens[0][0]) || value_index(val, border_width_strings) >= 0)
		{
			add_parsed_property(prefix + _t("-width"),	tokens[0], important);
			add_parsed_property(prefix + _t("-style"),	tokens[1], important);
		} else
		{
			add_parsed_property(prefix + _t("-style"),	tokens[0], important);
			add_parsed_property(prefix + _t("-color"),	tokens[1], important);
		}
	}
}

void litehtml::style::parse_short_background( const tstring_view& val, tstring_view baseurl, bool important )
{
	add_parsed_property(_t("background-color"),			_t("transparent"),	important);
	add_parsed_property(_t("background-image"),			_t(""),				important);
	add_parsed_property(_t("background-image-baseurl"), _t(""),				important);
	add_parsed_property(_t("background-repeat"),		_t("repeat"),		important);
	add_parsed_property(_t("background-origin"),		_t("padding-box"),	important);
	add_parsed_property(_t("background-clip"),			_t("border-box"),	important);
	add_parsed_property(_t("background-attachment"),	_t("scroll"),		important);

	if(val == _t("none"))
	{
		return;
	}

	string_view_vector tokens;
	split_string(val, tokens, _t(" "), _t(""), _t("("));
	bool origin_found = false;
	for(auto tok : tokens)
	{
		if(web_color::is_color(tok))
		{
			add_parsed_property(_t("background-color"), tok, important);
		} else if(tok.substr(0, 3) == _t("url"))
		{
			add_parsed_property(_t("background-image"), tok, important);
			if(!baseurl.empty())
			{
				add_parsed_property(_t("background-image-baseurl"), baseurl, important);
			}

		} else if( value_in_list(tok, background_repeat_strings) )
		{
			add_parsed_property(_t("background-repeat"), tok, important);
		} else if( value_in_list(tok, background_attachment_strings) )
		{
			add_parsed_property(_t("background-attachment"), tok, important);
		} else if( value_in_list(tok, background_box_strings) )
		{
			if(!origin_found)
			{
				add_parsed_property(_t("background-origin"), tok, important);
				origin_found = true;
			} else
			{
				add_parsed_property(_t("background-clip"), tok, important);
			}
		} else if(	value_in_list(tok, _t("left;right;top;bottom;center")) ||
					iswdigit(tok[0]) ||
					tok[0] == _t('-')	||
					tok[0] == _t('.')	||
					tok[0] == _t('+'))
		{
			if(m_properties.find(_t("background-position")) != m_properties.end())
			{
				m_properties[_t("background-position")].m_value = m_properties[_t("background-position")].m_value + _t(" ") + tok.to_string();
			} else
			{
				add_parsed_property(_t("background-position"), tok, important);
			}
		}
	}
}

void litehtml::style::parse_short_font( const tstring_view& val, bool important )
{
	add_parsed_property(_t("font-style"),	_t("normal"),	important);
	add_parsed_property(_t("font-variant"),	_t("normal"),	important);
	add_parsed_property(_t("font-weight"),	_t("normal"),	important);
	add_parsed_property(_t("font-size"),		_t("medium"),	important);
	add_parsed_property(_t("line-height"),	_t("normal"),	important);

    string_view_vector tokens;
	split_string(val, tokens, _t(" "), _t(""), _t("\""));

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
					add_parsed_property(_t("font-weight"),		tok, important);
					add_parsed_property(_t("font-variant"),		tok, important);
					add_parsed_property(_t("font-style"),		tok, important);
				} else
				{
					add_parsed_property(_t("font-style"),		tok, important);
				}
			} else
			{
				if(value_in_list(tok, font_weight_strings))
				{
					add_parsed_property(_t("font-weight"),		tok, important);
				} else
				{
					if(value_in_list(tok, font_variant_strings))
					{
						add_parsed_property(_t("font-variant"),	tok, important);
					} else if( iswdigit(tok[0]) )
					{
                        string_view_vector szlh;
						split_string(tok, szlh, _t("/"));

						if(szlh.size() == 1)
						{
							add_parsed_property(_t("font-size"),	szlh[0], important);
						} else	if(szlh.size() >= 2)
						{
							add_parsed_property(_t("font-size"),	szlh[0], important);
							add_parsed_property(_t("line-height"),	szlh[1], important);
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
	add_parsed_property(_t("font-family"), font_family, important);
}

void litehtml::style::add_parsed_property( const tstring_view& name, const tstring_view& val, bool important )
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

void litehtml::style::remove_property( const tstring_view& name, bool important )
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
