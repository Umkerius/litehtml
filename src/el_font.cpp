#include "html.h"
#include "el_font.h"


litehtml::el_font::el_font(const std::shared_ptr<litehtml::document>& doc) : html_tag(doc)
{

}

litehtml::el_font::~el_font()
{

}

void litehtml::el_font::parse_attributes()
{
	tstring_view str = get_attr(_t("color"));
	if(!str.empty())
	{
		m_style.add_property(_t("color"), str, 0, false);
	}

	str = get_attr(_t("face"));
    if (!str.empty())
	{
		m_style.add_property(_t("font-face"), str, 0, false);
	}

	str = get_attr(_t("size"));
    if (!str.empty())
	{
		int sz = std::stoi(str.to_string());
		if(sz <= 1)
		{
			m_style.add_property(_t("font-size"), _t("x-small"), 0, false);
		} else if(sz >= 6)
		{
			m_style.add_property(_t("font-size"), _t("xx-large"), 0, false);
		} else
		{
			switch(sz)
			{
			case 2:
				m_style.add_property(_t("font-size"), _t("small"), 0, false);
				break;
			case 3:
				m_style.add_property(_t("font-size"), _t("medium"), 0, false);
				break;
			case 4:
				m_style.add_property(_t("font-size"), _t("large"), 0, false);
				break;
			case 5:
				m_style.add_property(_t("font-size"), _t("x-large"), 0, false);
				break;
			}
		}
	}

	html_tag::parse_attributes();
}
