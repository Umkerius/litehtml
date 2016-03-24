#include "html.h"
#include "el_para.h"
#include "document.h"

litehtml::el_para::el_para(const std::shared_ptr<litehtml::document>& doc) : litehtml::html_tag(doc)
{
}

litehtml::el_para::~el_para()
{

}

void litehtml::el_para::parse_attributes()
{
	tstring_view str = get_attr(_Q("align"));
	if(!str.empty())
	{
		m_style.add_property(_Q("text-align"), str, 0, false);
	}

	html_tag::parse_attributes();
}
