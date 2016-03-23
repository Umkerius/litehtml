#include "html.h"
#include "css_selector.h"
#include "document.h"

void litehtml::css_element_selector::parse( const tstring_view& txt )
{
	tstring_view::size_type el_end = txt.find_first_of(_t(".#[:"));
	m_tag = litehtml::lcase_copy(txt.substr(0, el_end));

	while(el_end != tstring_view::npos)
	{
		if(txt[el_end] == _t('.'))
		{
			css_attribute_selector attribute;

			tstring_view::size_type pos = txt.find_first_of(_t(".#[:"), el_end + 1);
			attribute.val		= txt.substr(el_end + 1, pos - el_end - 1).to_string();

            string_view_vector class_val;
            split_string(attribute.val, class_val, _t(" "));
            attribute.class_val = std::move(to_string_vector(class_val));

			attribute.condition	= select_equal;
			attribute.attribute	= _t("class");
			m_attrs.push_back(attribute);
			el_end = pos;
		} else if(txt[el_end] == _t(':'))
		{
			css_attribute_selector attribute;

			if(txt[el_end + 1] == _t(':'))
			{
				tstring_view::size_type pos = txt.find_first_of(_t(".#[:"), el_end + 2);
				attribute.val		= litehtml::lcase_copy(txt.substr(el_end + 2, pos - el_end - 2));
				attribute.condition	= select_pseudo_element;
				attribute.attribute	= _t("pseudo-el");
				m_attrs.push_back(attribute);
				el_end = pos;
			} else
			{
				tstring_view::size_type pos = txt.find_first_of(_t(".#[:("), el_end + 1);
				if(pos != tstring_view::npos && txt.at(pos) == _t('('))
				{
					pos = find_close_bracket(txt, pos);
					if(pos != tstring_view::npos)
					{
						pos++;
					} else
					{
						int iii = 0;
						iii++;
					}
				}
				if(pos != tstring_view::npos)
				{
					attribute.val		= txt.substr(el_end + 1, pos - el_end - 1).to_string();
				} else
				{
					attribute.val		= txt.substr(el_end + 1).to_string();
				}
				litehtml::lcase(attribute.val);
				if(attribute.val == _t("after") || attribute.val == _t("before"))
				{
					attribute.condition	= select_pseudo_element;
				} else
				{
					attribute.condition	= select_pseudo_class;
				}
				attribute.attribute	= _t("pseudo");
				m_attrs.push_back(attribute);
				el_end = pos;
			}
		} else if(txt[el_end] == _t('#'))
		{
			css_attribute_selector attribute;

			tstring_view::size_type pos = txt.find_first_of(_t(".#[:"), el_end + 1);
			attribute.val		= txt.substr(el_end + 1, pos - el_end - 1).to_string();
			attribute.condition	= select_equal;
			attribute.attribute	= _t("id");
			m_attrs.push_back(attribute);
			el_end = pos;
		} else if(txt[el_end] == _t('['))
		{
			css_attribute_selector attribute;

			tstring_view::size_type pos = txt.find_first_of(_t("]~=|$*^"), el_end + 1);
            tstring attr = trim(txt.substr(el_end + 1, pos - el_end - 1)).to_string();
			litehtml::lcase(attr);
			if(pos != tstring_view::npos)
			{
				if(txt[pos] == _t(']'))
				{
					attribute.condition = select_exists;
				} else if(txt[pos] == _t('='))
				{
					attribute.condition = select_equal;
					pos++;
				} else if(txt.substr(pos, 2) == _t("~="))
				{
					attribute.condition = select_contain_str;
					pos += 2;
				} else if(txt.substr(pos, 2) == _t("|="))
				{
					attribute.condition = select_start_str;
					pos += 2;
				} else if(txt.substr(pos, 2) == _t("^="))
				{
					attribute.condition = select_start_str;
					pos += 2;
				} else if(txt.substr(pos, 2) == _t("$="))
				{
					attribute.condition = select_end_str;
					pos += 2;
				} else if(txt.substr(pos, 2) == _t("*="))
				{
					attribute.condition = select_contain_str;
					pos += 2;
				} else
				{
					attribute.condition = select_exists;
					pos += 1;
				}
				pos = txt.find_first_not_of(_t(" \t"), pos);
				if(pos != tstring_view::npos)
				{
					if(txt[pos] == _t('"'))
					{
						tstring_view::size_type pos2 = txt.find_first_of(_t("\""), pos + 1);
						attribute.val = txt.substr(pos + 1, pos2 == tstring_view::npos ? pos2 : (pos2 - pos - 1)).to_string();
						pos = pos2 == tstring_view::npos ? pos2 : (pos2 + 1);
					} else if(txt[pos] == _t(']'))
					{
						pos ++;
					} else
					{
						tstring_view::size_type pos2 = txt.find_first_of(_t("]"), pos + 1);
                        attribute.val = trim(txt.substr(pos, pos2 == tstring_view::npos ? pos2 : (pos2 - pos))).to_string();
						pos = pos2 == tstring_view::npos ? pos2 : (pos2 + 1);
					}
				}
			} else
			{
				attribute.condition = select_exists;
			}
			attribute.attribute	= attr;
			m_attrs.push_back(attribute);
			el_end = pos;
		} else
		{
			el_end++;
		}
		el_end = txt.find_first_of(_t(".#[:"), el_end);
	}
}


bool litehtml::css_selector::parse( const tstring_view& text )
{
	if(text.empty())
	{
		return false;
	}
	string_view_vector tokens;
	split_string(text, tokens, _t(""), _t(" \t>+~"), _t("(["));

	if(tokens.empty())
	{
		return false;
	}

	tstring_view left;
	tstring_view right = tokens.back();
	tchar_t combinator = 0;

	tokens.pop_back();
	while(!tokens.empty() && (tokens.back() == _t(" ") || tokens.back() == _t("\t") || tokens.back() == _t("+") || tokens.back() == _t("~") || tokens.back() == _t(">")))
	{
		if(combinator == _t(' ') || combinator == 0)
		{
			combinator = tokens.back()[0];
		}
		tokens.pop_back();
	}

    tstring joined_tokens;
	for(auto i = tokens.begin(); i != tokens.end(); i++)
	{
        joined_tokens.append(i->c_str(), i->size());
	}

    left = trim(joined_tokens);
    right = trim(right);

	if(right.empty())
	{
		return false;
	}

	m_right.parse(right);

	switch(combinator)
	{
	case _t('>'):
		m_combinator	= combinator_child;
		break;
	case _t('+'):
		m_combinator	= combinator_adjacent_sibling;
		break;
	case _t('~'):
		m_combinator	= combinator_general_sibling;
		break;
	default:
		m_combinator	= combinator_descendant;
		break;
	}

	m_left = 0;

	if(!left.empty())
	{
		m_left = std::make_shared<css_selector>(media_query_list::ptr(0));
		if(!m_left->parse(left))
		{
			return false;
		}
	}

	return true;
}

void litehtml::css_selector::calc_specificity()
{
	if(!m_right.m_tag.empty() && m_right.m_tag != _t("*"))
	{
		m_specificity.d = 1;
	}
	for(css_attribute_selector::vector::iterator i = m_right.m_attrs.begin(); i != m_right.m_attrs.end(); i++)
	{
		if(i->attribute == _t("id"))
		{
			m_specificity.b++;
		} else
		{
			if(i->attribute == _t("class"))
			{
				m_specificity.c += (int) i->class_val.size();
			} else
			{
				m_specificity.c++;
			}
		}	
	}
	if(m_left)
	{
		m_left->calc_specificity();
		m_specificity += m_left->m_specificity;
	}
}

void litehtml::css_selector::add_media_to_doc( document* doc ) const
{
	if(m_media_query && doc)
	{
		doc->add_media_list(m_media_query);
	}
}

