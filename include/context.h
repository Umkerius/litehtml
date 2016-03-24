#pragma once
#include "stylesheet.h"

namespace litehtml
{
	class context
	{
		litehtml::css	m_master_css;
	public:
		void			load_master_stylesheet(tstring_view str);
		litehtml::css&	master_css()
		{
			return m_master_css;
		}
	};
}