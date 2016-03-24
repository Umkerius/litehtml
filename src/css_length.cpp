#include "html.h"
#include "css_length.h"

void litehtml::css_length::fromString( tstring_view str, tstring_view predefs, int defValue )
{
	// TODO: Make support for calc
	if(str.substr(0, 4) == _Q("calc"))
	{
		m_is_predefined = true;
		m_predef		= 0;
		return;
	}

	int predef = value_index(str, predefs, -1);
	if(predef >= 0)
	{
		m_is_predefined = true;
		m_predef		= predef;
	} else
	{
		m_is_predefined = false;

		tstring num;
        tstring un;
		bool is_unit = false;
		for(tstring_view::const_iterator chr = str.begin(); chr != str.end(); chr++)
		{
			if(!is_unit)
			{
				if(isdigit(*chr) || *chr == '.' || *chr == '+' || *chr == '-')
				{
					num += *chr;
				} else
				{
					is_unit = true;
				}
			}
			if(is_unit)
			{
				un += *chr;
			}
		}
		if(!num.empty())
		{
			m_value = std::stof(num);
			m_units	= (css_units) value_index(un, css_units_strings, css_units_none);
		} else
		{
			// not a number so it is predefined
			m_is_predefined = true;
			m_predef = defValue;
		}
	}
}
