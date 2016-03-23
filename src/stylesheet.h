#pragma once
#include "style.h"
#include "css_selector.h"

namespace litehtml
{
	class document_container;

	class css
	{
		css_selector::vector	m_selectors;
	public:
		css()
		{

		}
		
		~css()
		{

		}

		const css_selector::vector& selectors() const
		{
			return m_selectors;
		}

		void clear()
		{
			m_selectors.clear();
		}

        void	parse_stylesheet(tstring_view str, const tchar_t* baseurl, const std::shared_ptr <document>& doc, const media_query_list::ptr& media);
		void	sort_selectors();
		static void	parse_css_url(const tstring_view& str, tstring& url);

	private:
		void	parse_atrule(const tstring_view& text, const tchar_t* baseurl, const std::shared_ptr<document>& doc, const media_query_list::ptr& media);
		void	add_selector(css_selector::ptr selector);
		bool	parse_selectors(const tstring_view& txt, const litehtml::style::ptr& styles, const media_query_list::ptr& media);

	};

	inline void litehtml::css::add_selector( css_selector::ptr selector )
	{
		selector->m_order = (int) m_selectors.size();
		m_selectors.push_back(selector);
	}

}
