#pragma once

#include <algorithm>
#include "types.h"
#include "background.h"
#include "borders.h"
#include "html_tag.h"
#include "web_color.h"
#include "media_query.h"

namespace litehtml
{
	struct list_marker
	{
		tstring			image;
		tstring_view	baseurl;
		list_style_type	marker_type;
		web_color		color;
		position		pos;
	};

    struct text_node
    {
        tstring str;
        bool is_text; // true for text, false for space
    };

	// call back interface to draw text, images and other elements
	class document_container
	{
	public:
		virtual litehtml::uint_ptr	create_font(litehtml::tstring_view faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* fm) = 0;
		virtual void				delete_font(litehtml::uint_ptr hFont) = 0;
		virtual int					text_width(litehtml::tstring_view text, litehtml::uint_ptr hFont) = 0;
		virtual void				draw_text(litehtml::uint_ptr hdc, litehtml::tstring_view text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos) = 0;
		virtual int					pt_to_px(int pt) = 0;
		virtual int					get_default_font_size() const = 0;
		virtual litehtml::tstring_view	get_default_font_name() const = 0;
		virtual void				draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker) = 0;
		virtual void				load_image(litehtml::tstring_view src, litehtml::tstring_view baseurl, bool redraw_on_ready) = 0;
		virtual void				get_image_size(litehtml::tstring_view src, litehtml::tstring_view baseurl, litehtml::size& sz) = 0;
		virtual void				draw_background(litehtml::uint_ptr hdc, const litehtml::background_paint& bg) = 0;
		virtual void				draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root) = 0;

		virtual	void				set_caption(litehtml::tstring_view caption) = 0;
		virtual	void				set_base_url(litehtml::tstring_view base_url) = 0;
		virtual void				link(const std::shared_ptr<litehtml::document>& doc, const litehtml::element::ptr& el) = 0;
		virtual void				on_anchor_click(litehtml::tstring_view url, const litehtml::element::ptr& el) = 0;
		virtual	void				set_cursor(litehtml::tstring_view cursor) = 0;
		virtual	void				transform_text(litehtml::tstring& text, litehtml::text_transform tt) = 0;
		virtual void				import_css(litehtml::tstring& text, litehtml::tstring_view url, litehtml::tstring& baseurl) = 0;
		virtual void				set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius, bool valid_x, bool valid_y) = 0;
		virtual void				del_clip() = 0;
		virtual void				get_client_rect(litehtml::position& client) const = 0;
		virtual std::shared_ptr<litehtml::element>	create_element(litehtml::tstring_view tag_name,
																   const litehtml::string_hash_map &attributes,
																   const std::shared_ptr<litehtml::document> &doc) = 0;

		virtual void				get_media_features(litehtml::media_features& media) const = 0;
		virtual void				get_language(litehtml::tstring& language, litehtml::tstring& culture) const = 0;

        // split UTF8 text on text nodes. returns false for default behavior (UTF8 -> Unicode -> split by spaces)
        virtual bool                split_text(litehtml::tstring_view text, litehtml::lite_deque<text_node>& result) const { return false; }
	};

    tstring_view trim(tstring_view s);
	void lcase(tstring &s);
    tstring lcase_copy(tstring_view s);

    int	 value_index(tstring_view val, tstring_view strings, int defValue = -1, tchar_t delim = ';');
	bool value_in_list(tstring_view val, tstring_view strings, tchar_t delim = ';');
	tstring_view::size_type find_close_bracket(tstring_view s, tstring_view::size_type off, tchar_t open_b = '(', tchar_t close_b = ')');
    void split_string(tstring_view str, string_view_deque& tokens, tstring_view delims, tstring_view delims_preserve = _Q(""), tstring_view quote = _Q("\""));

    template <typename StringT>
    void join_string(tstring& str, const lite_vector<StringT>& tokens, tstring_view delims)
    {
        str.clear();

        for (size_t i = 0; i < tokens.size(); ++i)
        {
            if (i != 0)
            {
                str.append(delims.data(), delims.size());
            }
            str += tokens[i];
        }
    }

    string_vector to_string_vector(const string_view_deque& views);

	inline int round_f(float val)
	{
		int int_val = (int) val;
		if(val - int_val >= 0.5)
		{
			int_val++;
		}
		return int_val;
	}

	inline int round_d(double val)
	{
		int int_val = (int) val;
		if(val - int_val >= 0.5)
		{
			int_val++;
		}
		return int_val;
	}
}
