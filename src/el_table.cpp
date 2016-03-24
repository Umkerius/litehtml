#include "html.h"
#include "el_table.h"
#include "document.h"
#include "iterators.h"
#include <algorithm>


litehtml::el_table::el_table(const std::shared_ptr<litehtml::document>& doc) : html_tag(doc)
{
	m_border_spacing_x	= 0;
	m_border_spacing_y	= 0;
	m_border_collapse	= border_collapse_separate;
}


litehtml::el_table::~el_table()
{

}

bool litehtml::el_table::appendChild(const litehtml::element::ptr& el)
{
	if(!el)	return false;

    tstring_view tagName = el->get_tagName();

    if (tagName == _Q("tbody") || tagName == _Q("thead") || tagName == _Q("tfoot"))
	{
		return html_tag::appendChild(el);
	}
	return false;
}

void litehtml::el_table::parse_styles(bool is_reparse)
{
	html_tag::parse_styles(is_reparse);

	m_border_collapse = (border_collapse) value_index(get_style_property(_Q("border-collapse"), true, _Q("separate")), border_collapse_strings, border_collapse_separate);

	if(m_border_collapse == border_collapse_separate)
	{
		m_css_border_spacing_x.fromString(get_style_property(_Q("-litehtml-border-spacing-x"), true, _Q("0px")));
		m_css_border_spacing_y.fromString(get_style_property(_Q("-litehtml-border-spacing-y"), true, _Q("0px")));

		int fntsz = get_font_size();
		document::ptr doc = get_document();
		m_border_spacing_x = doc->cvt_units(m_css_border_spacing_x, fntsz);
		m_border_spacing_y = doc->cvt_units(m_css_border_spacing_y, fntsz);
	} else
	{
		m_border_spacing_x	= 0;
		m_border_spacing_y	= 0;
		m_padding.bottom	= 0;
		m_padding.top		= 0;
		m_padding.left		= 0;
		m_padding.right		= 0;
		m_css_padding.bottom.set_value(0, css_units_px);
		m_css_padding.top.set_value(0, css_units_px);
		m_css_padding.left.set_value(0, css_units_px);
		m_css_padding.right.set_value(0, css_units_px);
	}
}

void litehtml::el_table::parse_attributes()
{
	tstring_view str = get_attr(_Q("width"));
	if(!str.empty())
	{
		m_style.add_property(_Q("width"), str, 0, false);
	}

	str = get_attr(_Q("align"));
    if (!str.empty())
	{
		int align = value_index(str, _Q("left;center;right"));
		switch(align)
		{
		case 1:
			m_style.add_property(_Q("margin-left"), _Q("auto"), 0, false);
			m_style.add_property(_Q("margin-right"), _Q("auto"), 0, false);
			break;
		case 2:
			m_style.add_property(_Q("margin-left"), _Q("auto"), 0, false);
			m_style.add_property(_Q("margin-right"), _Q("0"), 0, false);
			break;
		}
	}

	str = get_attr(_Q("cellspacing"));
    if (!str.empty())
	{
		tstring val = str + _Q(" ") + str;
		m_style.add_property(_Q("border-spacing"), val, 0, false);
	}
	
	str = get_attr(_Q("border"));
    if (!str.empty())
	{
		m_style.add_property(_Q("border-width"), str, 0, false);
	}

	str = get_attr(_Q("bgcolor"));
    if (!str.empty())
	{
		m_style.add_property(_Q("background-color"), str, 0, false);
	}

	html_tag::parse_attributes();
}
