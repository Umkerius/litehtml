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
	tstring_view str = get_attr(_Q("width"));
	if(!str.empty())
	{
		m_style.add_property(_Q("width"), str, 0, false);
	}
	str = get_attr(_Q("background"));
    if (!str.empty())
	{
        tstring url = _Q("url('") + str + _Q("')");
		m_style.add_property(_Q("background-image"), url, 0, false);
	}
	str = get_attr(_Q("align"));
    if (!str.empty())
	{
		m_style.add_property(_Q("text-align"), str, 0, false);
	}

	str = get_attr(_Q("bgcolor"));
    if (!str.empty())
	{
		m_style.add_property(_Q("background-color"), str, 0, false);
	}

	str = get_attr(_Q("valign"));
    if (!str.empty())
	{
		m_style.add_property(_Q("vertical-align"), str, 0, false);
	}
	html_tag::parse_attributes();
}

