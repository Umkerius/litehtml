#include "html.h"
#include "el_link.h"
#include "document.h"


litehtml::el_link::el_link(const std::shared_ptr<litehtml::document>& doc) : litehtml::html_tag(doc)
{

}

litehtml::el_link::~el_link()
{

}

void litehtml::el_link::parse_attributes()
{
	bool processed = false;

	document::ptr doc = get_document();

    tstring_view rel = get_attr(_Q("rel"));
	if(rel == _Q("stylesheet"))
	{
		tstring_view media = get_attr(_Q("media"));
		tstring_view href = get_attr(_Q("href"));
		if(!href.empty())
		{
            tstring css_text;
            tstring css_baseurl;
            doc->container()->import_css(css_text, href, css_baseurl);
			if(!css_text.empty())
			{
				doc->add_stylesheet(css_text, css_baseurl, media);
				processed = true;
			}
		}
	}

	if(!processed)
	{
		doc->container()->link(doc, shared_from_this());
	}
}
