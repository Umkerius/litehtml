#include "html.h"
#include "el_td.h"


litehtml::el_td::el_td(const std::shared_ptr<litehtml::document>& doc) : html_tag(doc)
{

}

litehtml::el_td::~el_td()
{

}

void litehtml::el_td::parse_attributes()
{
	tstring_view str = get_attr(_t("width"));
	if(!str.empty())
	{
		m_style.add_property(_t("width"), str, 0, false);
	}
	str = get_attr(_t("background"));
    if (!str.empty())
	{
        tstring url = tstring_view(_t("url('")) + str + _t("')"); // UTODO: remove this shit
		m_style.add_property(_t("background-image"), url, 0, false);
	}
	str = get_attr(_t("align"));
    if (!str.empty())
	{
		m_style.add_property(_t("text-align"), str, 0, false);
	}

	str = get_attr(_t("bgcolor"));
    if (!str.empty())
	{
		m_style.add_property(_t("background-color"), str, 0, false);
	}

	str = get_attr(_t("valign"));
    if (!str.empty())
	{
		m_style.add_property(_t("vertical-align"), str, 0, false);
	}
	html_tag::parse_attributes();
}

