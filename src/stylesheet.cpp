#include "html.h"
#include "stylesheet.h"
#include <algorithm>
#include "document.h"


void litehtml::css::parse_stylesheet(tstring_view str, tstring_view baseurl, const std::shared_ptr<document>& doc, const media_query_list::ptr& media)
{
	// remove comments
    tstring_view stylesheet;
    tstring stylesheet_commentless; //if style has comments, remove it and store modified string
    tstring_view::size_type c_start = str.find(_t("/*"));

    if (c_start != tstring::npos)
    {
        stylesheet_commentless = str.to_string();
        while (c_start != tstring::npos)
        {
            tstring::size_type c_end = stylesheet_commentless.find(_t("*/"), c_start + 2);
            stylesheet_commentless.erase(c_start, c_end - c_start + 2);
            c_start = stylesheet_commentless.find(_t("/*"));
        }

        stylesheet = stylesheet_commentless;
    }
    else
    {
        stylesheet = str;
    }

	tstring_view::size_type pos = stylesheet.find_first_not_of(_t(" \n\r\t"));
	while(pos != tstring_view::npos)
	{
		while(pos != tstring_view::npos && stylesheet[pos] == _t('@'))
		{
			tstring_view::size_type sPos = pos;
			pos = stylesheet.find_first_of(_t("{"), pos);
			if(pos != tstring_view::npos && stylesheet[pos] == _t('{'))
			{
				pos = find_close_bracket(stylesheet, pos, _t('{'), _t('}'));
			}
			if(pos != tstring_view::npos)
			{
				parse_atrule(stylesheet.substr(sPos, pos - sPos + 1), baseurl, doc, media);
			} else
			{
				parse_atrule(stylesheet.substr(sPos), baseurl, doc, media);
			}

			if(pos != tstring_view::npos)
			{
				pos = stylesheet.find_first_not_of(_t(" \n\r\t"), pos + 1);
			}
		}

		if(pos == tstring_view::npos)
		{
			break;
		}

		tstring_view::size_type style_start = stylesheet.find(_t("{"), pos);
		tstring_view::size_type style_end	= stylesheet.find(_t("}"), pos);
		if(style_start != tstring_view::npos && style_end != tstring_view::npos)
		{
			style::ptr st = std::make_shared<style>();
			st->add(stylesheet.substr(style_start + 1, style_end - style_start - 1).c_str(), baseurl);

			parse_selectors(stylesheet.substr(pos, style_start - pos), st, media);

			if(media && doc)
			{
				doc->add_media_list(media);
			}

			pos = style_end + 1;
		} else
		{
			pos = tstring_view::npos;
		}

		if(pos != tstring_view::npos)
		{
			pos = stylesheet.find_first_not_of(_t(" \n\r\t"), pos);
		}
	}
}

void litehtml::css::parse_css_url(const tstring_view& str, tstring& url)
{
	url = _t("");
	size_t pos1 = str.find(_t('('));
	size_t pos2 = str.find(_t(')'));
	if(pos1 != tstring_view::npos && pos2 != tstring_view::npos)
	{
		url = str.substr(pos1 + 1, pos2 - pos1 - 1).to_string();
		if(url.length())
		{
			if(url[0] == _t('\'') || url[0] == _t('"'))
			{
				url.erase(0, 1);
			}
		}
		if(url.length())
		{
			if(url[url.length() - 1] == _t('\'') || url[url.length() - 1] == _t('"'))
			{
				url.erase(url.length() - 1, 1);
			}
		}
	}
}

bool litehtml::css::parse_selectors( const tstring_view& txt, const litehtml::style::ptr& styles, const media_query_list::ptr& media )
{
    tstring_view selector = trim(txt);
	string_view_vector tokens;
	split_string(selector, tokens, _t(","));

	bool added_something = false;

	for(auto tok = tokens.begin(); tok != tokens.end(); tok++)
	{
		css_selector::ptr selector = std::make_shared<css_selector>(media);
		selector->m_style = styles;
		trim(*tok);
		if(selector->parse(*tok))
		{
			selector->calc_specificity();
			add_selector(selector);
			added_something = true;
		}
	}

	return added_something;
}

void litehtml::css::sort_selectors()
{
	std::sort(m_selectors.begin(), m_selectors.end(),
		 [](const css_selector::ptr& v1, const css_selector::ptr& v2)
		 {
			 return (*v1) < (*v2);
		 }
	);
}

void litehtml::css::parse_atrule(const tstring_view& text, tstring_view baseurl, const std::shared_ptr<document>& doc, const media_query_list::ptr& media)
{
	if(text.substr(0, 7) == _t("@import"))
	{
		int sPos = 7;
		tstring_view iStr;
		iStr = text.substr(sPos);
		if(iStr.back() == _t(';'))
		{
            iStr.remove_suffix(1);
		}
        iStr = trim(iStr);
        string_view_vector tokens;
		split_string(iStr, tokens, _t(" "), _t(""), _t("(\""));
		if(!tokens.empty())
		{
			tstring url;
			parse_css_url(tokens.front(), url);
			if(url.empty())
			{
				url = tokens.front().to_string();
			}
			tokens.erase(tokens.begin());
			if(doc)
			{
				document_container* doc_cont = doc->container();
				if(doc_cont)
				{
					tstring css_text;
					tstring css_baseurl;
                    css_baseurl = baseurl.to_string();
					doc_cont->import_css(css_text, url, css_baseurl);
					if(!css_text.empty())
					{
						media_query_list::ptr new_media = media;
						if(!tokens.empty())
						{
							tstring media_str;
							for(auto iter = tokens.begin(); iter != tokens.end(); iter++)
							{
								if(iter != tokens.begin())
								{
									media_str += _t(" ");
								}
								media_str += iter->to_string();
							}
							new_media = media_query_list::create_from_string(media_str, doc);
							if(!new_media)
							{
								new_media = media;
							}
						}
						parse_stylesheet(css_text.c_str(), css_baseurl.c_str(), doc, new_media);
					}
				}
			}
		}
	} else if(text.substr(0, 6) == _t("@media"))
	{
		tstring_view::size_type b1 = text.find_first_of(_t('{'));
        tstring_view::size_type b2 = text.find_last_of(_t('}'));
		if(b1 != tstring_view::npos)
		{
            tstring_view media_type = trim(text.substr(6, b1 - 6));
			media_query_list::ptr new_media = media_query_list::create_from_string(media_type, doc);

			tstring_view media_style;
			if(b2 != tstring_view::npos)
			{
				media_style = text.substr(b1 + 1, b2 - b1 - 1);
			} else
			{
				media_style = text.substr(b1 + 1);
			}

			parse_stylesheet(media_style.c_str(), baseurl, doc, new_media);
		}
	}
}
