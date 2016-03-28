#pragma once
#include "attributes.h"
#include <string>

namespace litehtml
{
	class property_value
	{
	public:
		tstring	m_value;
		bool    m_important;

		property_value()
		{
			m_important = false;
		}
		property_value(tstring_view val, bool imp)
		{
            m_value = val.to_string();
            m_important = imp;
		}
		property_value(const property_value& val)
		{
			m_value		= val.m_value;
			m_important	= val.m_important;
		}

		property_value& operator=(const property_value& val)
		{
			m_value		= val.m_value;
			m_important	= val.m_important;
			return *this;
		}
	};

	typedef std::map<tstring, property_value>	props_map;

	class style
	{
	public:
		typedef std::shared_ptr<style>		ptr;
		typedef std::vector<style::ptr>		vector;
	private:
		props_map			m_properties;
		static string_hash_map	m_valid_values;
	public:
		style();
		style(const style& val);
		virtual ~style();

		void operator=(const style& val)
		{
			m_properties = val.m_properties;
		}

		void add(tstring_view txt, tstring_view baseurl)
		{
			parse(txt, baseurl);
		}

		void add_property(tstring_view name, tstring_view val, tstring_view baseurl, bool important);

		tstring_view get_property(tstring_view name) const
		{
			if(!name.empty())
			{
				props_map::const_iterator f = m_properties.find(name.to_string());
				if(f != m_properties.end())
				{
					return f->second.m_value;
				}
			}
            return tstring_view();
		}

		void combine(const litehtml::style& src);
		void clear()
		{
			m_properties.clear();
		}

	private:
		void parse_property(tstring_view txt, tstring_view baseurl);
		void parse(tstring_view txt, tstring_view baseurl);
		void parse_short_border(tstring_view prefix, tstring_view val, bool important);
		void parse_short_background(tstring_view val, tstring_view baseurl, bool important);
		void parse_short_font(tstring_view val, bool important);
		void add_parsed_property(tstring_view name, tstring_view val, bool important);
		void remove_property(tstring_view name, bool important);
	};
}