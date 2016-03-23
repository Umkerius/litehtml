#include "html.h"
#include "context.h"
#include "stylesheet.h"


void litehtml::context::load_master_stylesheet( tstring_view str )
{
	media_query_list::ptr media;

	m_master_css.parse_stylesheet(str, 0, std::shared_ptr<litehtml::document>(), media_query_list::ptr());
	m_master_css.sort_selectors();
}
