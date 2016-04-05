#include "html.h"
#include "el_tr.h"


litehtml::el_tr::el_tr(const std::shared_ptr<litehtml::document>& doc) : html_tag(doc)
{
    
}

litehtml::el_tr::~el_tr()
{

}

bool litehtml::el_tr::appendChild(const element::ptr & el)
{
    //if (get_direction() == direction_rtl)
    {
        if (el)
        {
            el->parent(shared_from_this());
            m_children.insert(m_children.begin(), el);
            return true;
        }
        return false;
    }
    return html_tag::appendChild(el);
}

void litehtml::el_tr::parse_attributes()
{
	tstring_view str = get_attr(_Q("align"));
	if(!str.empty())
	{
		m_style.add_property(_Q("text-align"), str, 0, false);
	}
	str = get_attr(_Q("valign"));
    if (!str.empty())
	{
		m_style.add_property(_Q("vertical-align"), str, 0, false);
	}
	str = get_attr(_Q("bgcolor"));
    if (!str.empty())
	{
		m_style.add_property(_Q("background-color"), str, 0, false);
	}
	html_tag::parse_attributes();
}

void litehtml::el_tr::get_inline_boxes( position::vector& boxes )
{
	position pos;
    for(auto& el : m_children)
	{
		if(el->get_display() == display_table_cell)
		{
			pos.x		= el->left() + el->margin_left();
			pos.y		= el->top() - m_padding.top - m_borders.top;

			pos.width	= el->right() - pos.x - el->margin_right() - el->margin_left();
			pos.height	= el->height() + m_padding.top + m_padding.bottom + m_borders.top + m_borders.bottom;

			boxes.push_back(pos);
		}
	}
}
