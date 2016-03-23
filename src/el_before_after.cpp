#include "html.h"
#include "el_before_after.h"
#include "el_text.h"
#include "el_space.h"
#include "el_image.h"

litehtml::el_before_after_base::el_before_after_base(const std::shared_ptr<litehtml::document>& doc, bool before) : html_tag(doc)
{
	if(before)
	{
		set_tagName(_t("::before"));
	} else
	{
		set_tagName(_t("::after"));
	}
}

litehtml::el_before_after_base::~el_before_after_base()
{

}

void litehtml::el_before_after_base::add_style(const litehtml::style& st)
{
	html_tag::add_style(st);

	tstring_view content = get_style_property(_t("content"), false, _t(""));
	if(!content.empty())
	{
		int idx = value_index(content, content_property_string);
		if(idx < 0)
		{
			tstring fnc;
			tstring_view::size_type i = 0;
			while(i < content.length() && i != tstring_view::npos)
			{
				if(content.at(i) == _t('"'))
				{
					fnc.clear();
					i++;
					tstring_view::size_type pos = content.find(_t('"'), i);
					tstring_view txt;
					if(pos == tstring_view::npos)
					{
						txt = content.substr(i);
						i = tstring_view::npos;
					} else
					{
						txt = content.substr(i, pos - i);
						i = pos + 1;
					}
					add_text(txt);
				} else if(content.at(i) == _t('('))
				{
					i++;
                    fnc = litehtml::trim(fnc).to_string();
					litehtml::lcase(fnc);
					tstring_view::size_type pos = content.find(_t(')'), i);
					tstring_view params;
					if(pos == tstring_view::npos)
					{
						params = content.substr(i);
						i = tstring_view::npos;
					} else
					{
						params = content.substr(i, pos - i);
						i = pos + 1;
					}
					add_function(fnc, params);
					fnc.clear();
				} else
				{
					fnc += content.at(i);
					i++;
				}
			}
		}
	}
}

void litehtml::el_before_after_base::add_text( tstring_view txt )
{
	tstring word;
    tstring esc;
    for (tstring::size_type i = 0; i < txt.length(); i++)
	{
		if( (txt.at(i) == _t(' ')) || (txt.at(i) == _t('\t')) || (txt.at(i) == _t('\\') && !esc.empty()) )
		{
			if(esc.empty())
			{
				if(!word.empty())
				{
					element::ptr el = std::make_shared<el_text>(std::move(word), get_document());
					appendChild(el);
				}

				element::ptr el = std::make_shared<el_space>(txt.substr(i, 1), get_document());
				appendChild(el);
			} else
			{
				word += convert_escape(esc.c_str() + 1);
				esc.clear();
				if(txt.at(i) == _t('\\'))
				{
					esc += txt.at(i);
				}
			}
		} else
		{
			if(!esc.empty() || txt.at(i) == _t('\\'))
			{
				esc += txt.at(i);
			} else
			{
				word += txt.at(i);
			}
		}
	}

	if(!esc.empty())
	{
		word += convert_escape(esc.c_str() + 1);
	}
	if(!word.empty())
	{
		element::ptr el = std::make_shared<el_text>(std::move(word), get_document());
		appendChild(el);
	}
}

void litehtml::el_before_after_base::add_function( tstring_view fnc, tstring_view params )
{
	int idx = value_index(fnc, _t("attr;counter;url"));
	switch(idx)
	{
	// attr
	case 0:
		{
            tstring p_name = lcase_copy(trim(params));
			element::ptr el_parent = parent();
			if (el_parent)
			{
				tstring_view attr_value = el_parent->get_attr(p_name);
				if (!attr_value.empty())
				{
					add_text(attr_value);
				}
			}
		}
		break;
	// counter
	case 1:
		break;
	// url
	case 2:
		{
            tstring_view p_url = trim(params);
			if(!p_url.empty())
			{
				if(p_url.front() == _t('\'') || p_url.front() == _t('\"'))
				{
                    p_url.remove_prefix(1);
				}
			}
			if(!p_url.empty())
			{
				if(p_url.back() == _t('\'') || p_url.back() == _t('\"'))
				{
                    p_url.remove_suffix(1);
				}
			}
			if(!p_url.empty())
			{
				element::ptr el = std::make_shared<el_image>(get_document());
				el->set_attr(_t("src"), p_url);
				el->set_attr(_t("style"), _t("display:inline-block"));
				el->set_tagName(_t("img"));
				appendChild(el);
				el->parse_attributes();
			}
		}
		break;
	}
}

litehtml::tchar_t litehtml::el_before_after_base::convert_escape( tstring_view txt )
{
    long esc = std::stol(txt.to_string(), nullptr, 16);
	return tchar_t(esc);
}

void litehtml::el_before_after_base::apply_stylesheet( const litehtml::css& stylesheet )
{
}
