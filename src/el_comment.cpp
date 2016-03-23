#include "html.h"
#include "el_comment.h"

litehtml::el_comment::el_comment(const std::shared_ptr<litehtml::document>& doc) : litehtml::element(doc)
{
	m_skip = true;
}

litehtml::el_comment::~el_comment()
{

}

void litehtml::el_comment::get_text( tstring& text )
{
	text += m_text;
}

void litehtml::el_comment::set_data( tstring_view data )
{
	m_text += data.to_string();
}
