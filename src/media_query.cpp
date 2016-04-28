#include "html.h"
#include "media_query.h"
#include "document.h"


litehtml::media_query::media_query()
{
	m_media_type	= media_type_all;
	m_not			= false;
}

litehtml::media_query::media_query( const media_query& val )
{
	m_not			= val.m_not;
	m_expressions	= val.m_expressions;
	m_media_type	= val.m_media_type;
}

litehtml::media_query::ptr litehtml::media_query::create_from_string(tstring_view str, const std::shared_ptr<document>& doc)
{
    media_query::ptr query = make_lite_shared<media_query>();

	string_view_deque tokens;
	split_string(str, tokens, _Q(" \t\r\n"), _Q(""), _Q("("));

	for(auto tok = tokens.begin(); tok != tokens.end(); tok++)
	{
		if((*tok) == _Q("not"))
		{
			query->m_not = true;
		} else if(tok->front() == '(')
		{
			tok->remove_prefix(1);
			if(tok->back() == ')')
			{
                tok->remove_suffix(1);
			}
			media_query_expression expr;
			string_view_deque expr_tokens;
			split_string((*tok), expr_tokens, _Q(":"));
			if(!expr_tokens.empty())
			{
                expr_tokens[0] = trim(expr_tokens[0]);
				expr.feature = (media_feature) value_index(expr_tokens[0], media_feature_strings, media_feature_none);
				if(expr.feature != media_feature_none)
				{
					if(expr_tokens.size() == 1)
					{
						expr.check_as_bool = true;
					} else
					{
                        expr_tokens[1] = trim(expr_tokens[1]);
						expr.check_as_bool = false;
						if(expr.feature == media_feature_orientation)
						{
							expr.val = value_index(expr_tokens[1], media_orientation_strings, media_orientation_landscape);
						} else
						{
							tstring_view::size_type slash_pos = expr_tokens[1].find('/');
							if( slash_pos != tstring_view::npos )
							{
                                tstring_view val1 = trim(expr_tokens[1].substr(0, slash_pos));
								tstring_view val2 = trim(expr_tokens[1].substr(slash_pos + 1));
								expr.val = std::stoi(val1.to_string());
                                expr.val2 = std::stoi(val2.to_string());
							} else
							{
								css_length length;
								length.fromString(expr_tokens[1]);
								if(length.units() == css_units_dpcm)
								{
									expr.val = (int) (length.val() * 2.54);
								} else if(length.units() == css_units_dpi)
								{
									expr.val = (int) (length.val() * 2.54);
								} else
								{
									if(doc)
									{
										doc->cvt_units(length, doc->container()->get_default_font_size());
									}
									expr.val = (int) length.val();
								}
							}
						}
					}
					query->m_expressions.push_back(expr);
				}
			}
		} else
		{
			query->m_media_type = (media_type) value_index((*tok), media_type_strings, media_type_all);

		}
	}

	return query;
}

bool litehtml::media_query::check( const media_features& features ) const
{
	bool res = false;
	if(m_media_type == media_type_all || m_media_type == features.type)
	{
		res = true;
		for(auto expr = m_expressions.begin(); expr != m_expressions.end() && res; ++expr)
		{
			if(!expr->check(features))
			{
				res = false;
			}
		}
	}

	if(m_not)
	{
		res = !res;
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////

litehtml::media_query_list::ptr litehtml::media_query_list::create_from_string(tstring_view str, const std::shared_ptr<document>& doc)
{
    media_query_list::ptr list = make_lite_shared<media_query_list>();

	string_view_deque tokens;
	split_string(str, tokens, _Q(","));

	for(auto tok = tokens.begin(); tok != tokens.end(); tok++)
	{
        tstring media = lcase_copy(trim(*tok));

		litehtml::media_query::ptr query = media_query::create_from_string(media, doc);
		if(query)
		{
			list->m_queries.push_back(query);
		}
	}
	if(list->m_queries.empty())
	{
		list = 0;
	}

	return list;
}

bool litehtml::media_query_list::apply_media_features( const media_features& features )
{
	bool apply = false;
	
	for(media_query::vector::iterator iter = m_queries.begin(); iter != m_queries.end() && !apply; ++iter)
	{
		if((*iter)->check(features))
		{
			apply = true;
		}
	}

	bool ret = (apply != m_is_used);
	m_is_used = apply;
	return ret;
}

bool litehtml::media_query_expression::check( const media_features& features ) const
{
	switch(feature)
	{
	case media_feature_width:
		if(check_as_bool)
		{
			return (features.width != 0);
		} else if(features.width == val)
		{
			return true;
		}
		break;
	case media_feature_min_width:
		if(features.width >= val)
		{
			return true;
		}
		break;
	case media_feature_max_width:
		if(features.width <= val)
		{
			return true;
		}
		break;
	case media_feature_height:
		if(check_as_bool)
		{
			return (features.height != 0);
		} else if(features.height == val)
		{
			return true;
		}
		break;
	case media_feature_min_height:
		if(features.height >= val)
		{
			return true;
		}
		break;
	case media_feature_max_height:
		if(features.height <= val)
		{
			return true;
		}
		break;

	case media_feature_device_width:
		if(check_as_bool)
		{
			return (features.device_width != 0);
		} else if(features.device_width == val)
		{
			return true;
		}
		break;
	case media_feature_min_device_width:
		if(features.device_width >= val)
		{
			return true;
		}
		break;
	case media_feature_max_device_width:
		if(features.device_width <= val)
		{
			return true;
		}
		break;
	case media_feature_device_height:
		if(check_as_bool)
		{
			return (features.device_height != 0);
		} else if(features.device_height == val)
		{
			return true;
		}
		break;
	case media_feature_min_device_height:
		if(features.device_height <= val)
		{
			return true;
		}
		break;
	case media_feature_max_device_height:
		if(features.device_height <= val)
		{
			return true;
		}
		break;

	case media_feature_orientation:
		if(features.height >= features.width)
		{
			if(val == media_orientation_portrait)
			{
				return true;
			}
		} else
		{
			if(val == media_orientation_landscape)
			{
				return true;
			}
		}
		break;
	case media_feature_aspect_ratio:
		if(features.height && val2)
		{
			int ratio_this = round_d( (double) val / (double) val2 * 100 );
			int ratio_feat = round_d( (double) features.width / (double) features.height * 100.0 );
			if(ratio_this == ratio_feat)
			{
				return true;
			}
		}
		break;
	case media_feature_min_aspect_ratio:
		if(features.height && val2)
		{
			int ratio_this = round_d( (double) val / (double) val2 * 100 );
			int ratio_feat = round_d( (double) features.width / (double) features.height * 100.0 );
			if(ratio_feat >= ratio_this)
			{
				return true;
			}
		}
		break;
	case media_feature_max_aspect_ratio:
		if(features.height && val2)
		{
			int ratio_this = round_d( (double) val / (double) val2 * 100 );
			int ratio_feat = round_d( (double) features.width / (double) features.height * 100.0 );
			if(ratio_feat <= ratio_this)
			{
				return true;
			}
		}
		break;

	case media_feature_device_aspect_ratio:
		if(features.device_height && val2)
		{
			int ratio_this = round_d( (double) val / (double) val2 * 100 );
			int ratio_feat = round_d( (double) features.device_width / (double) features.device_height * 100.0 );
			if(ratio_feat == ratio_this)
			{
				return true;
			}
		}
		break;
	case media_feature_min_device_aspect_ratio:
		if(features.device_height && val2)
		{
			int ratio_this = round_d( (double) val / (double) val2 * 100 );
			int ratio_feat = round_d( (double) features.device_width / (double) features.device_height * 100.0 );
			if(ratio_feat >= ratio_this)
			{
				return true;
			}
		}
		break;
	case media_feature_max_device_aspect_ratio:
		if(features.device_height && val2)
		{
			int ratio_this = round_d( (double) val / (double) val2 * 100 );
			int ratio_feat = round_d( (double) features.device_width / (double) features.device_height * 100.0 );
			if(ratio_feat <= ratio_this)
			{
				return true;
			}
		}
		break;

	case media_feature_color:
		if(check_as_bool)
		{
			return (features.color != 0);
		} else if(features.color == val)
		{
			return true;
		}
		break;
	case media_feature_min_color:
		if(features.color >= val)
		{
			return true;
		}
		break;
	case media_feature_max_color:
		if(features.color <= val)
		{
			return true;
		}
		break;

	case media_feature_color_index:
		if(check_as_bool)
		{
			return (features.color_index != 0);
		} else if(features.color_index == val)
		{
			return true;
		}
		break;
	case media_feature_min_color_index:
		if(features.color_index >= val)
		{
			return true;
		}
		break;
	case media_feature_max_color_index:
		if(features.color_index <= val)
		{
			return true;
		}
		break;

	case media_feature_monochrome:
		if(check_as_bool)
		{
			return (features.monochrome != 0);
		} else if(features.monochrome == val)
		{
			return true;
		}
		break;
	case media_feature_min_monochrome:
		if(features.monochrome >= val)
		{
			return true;
		}
		break;
	case media_feature_max_monochrome:
		if(features.monochrome <= val)
		{
			return true;
		}
		break;

	case media_feature_resolution:
		if(features.resolution == val)
		{
			return true;
		}
		break;
	case media_feature_min_resolution:
		if(features.resolution >= val)
		{
			return true;
		}
		break;
	case media_feature_max_resolution:
		if(features.resolution <= val)
		{
			return true;
		}
		break;
	default:
		return false;
	}

	return false;
}
