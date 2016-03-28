#pragma once
#include "style.h"
#include "types.h"
#include "context.h"

//forwarding declaration
typedef struct GumboInternalNode GumboNode;

namespace litehtml
{
	struct css_text
	{
		using vector = lite_vector<css_text>;

		tstring	text;
		tstring	baseurl;
		tstring	media;
		
        css_text() = default;
		css_text(tstring_view txt, tstring_view url, tstring_view media_str)
            : text(txt.to_string()), baseurl(url.to_string()), media(media_str.to_string()) {}
	};

	class html_tag;

	class document : public std::enable_shared_from_this<document>
	{
	public:
		using ptr = std::shared_ptr<document>;
		using weak_ptr = std::weak_ptr<document>;
	private:
		std::shared_ptr<element>			m_root;
		document_container*					m_container;
		fonts_map							m_fonts;
		css_text::vector					m_css;
		litehtml::css						m_styles;
		litehtml::web_color					m_def_color;
		litehtml::context*					m_context;
		litehtml::size						m_size;
		position::vector					m_fixed_boxes;
		media_query_list::vector			m_media_lists;
		element::ptr						m_over_element;
		elements_vector						m_tabular_elements;
		media_features						m_media;
		tstring                             m_lang;
		tstring                             m_culture;
	public:
		document(litehtml::document_container* objContainer, litehtml::context* ctx);
		virtual ~document();

		litehtml::document_container*	container()	{ return m_container; }
		uint_ptr						get_font(tstring_view name, int size, tstring_view weight, tstring_view style, tstring_view decoration, font_metrics* fm);
		int								render(int max_width, render_type rt = render_all);
		void							draw(uint_ptr hdc, int x, int y, const position* clip);
		web_color						get_def_color()	{ return m_def_color; }
		int								cvt_units(tstring_view str, int fontSize, bool* is_percent = 0) const;
		int								cvt_units(css_length& val, int fontSize, int size = 0) const;
		int								width() const;
		int								height() const;
		void							add_stylesheet(tstring_view str, tstring_view baseurl, tstring_view media);
		bool							on_mouse_over(int x, int y, int client_x, int client_y, position::vector& redraw_boxes);
		bool							on_lbutton_down(int x, int y, int client_x, int client_y, position::vector& redraw_boxes);
		bool							on_lbutton_up(int x, int y, int client_x, int client_y, position::vector& redraw_boxes);
		bool							on_mouse_leave(position::vector& redraw_boxes);
        litehtml::element::ptr			create_element(tstring_view tag_name, const string_hash_map& attributes);
		element::ptr					root();
		void							get_fixed_boxes(position::vector& fixed_boxes);
		void							add_fixed_box(const position& pos);
		void							add_media_list(media_query_list::ptr list);
		bool							media_changed();
		bool							lang_changed();
		bool                            match_lang(tstring_view lang);
		void							add_tabular(const element::ptr& el);

		static litehtml::document::ptr createFromString(tstring_view str, litehtml::document_container* objPainter, litehtml::context* ctx, litehtml::css* user_styles = 0);
	
	private:
		litehtml::uint_ptr	add_font(tstring_view name, int size, tstring_view weight, tstring_view style, tstring_view decoration, font_metrics* fm);

        void create_node(GumboNode* node, elements_vector& elements);
		bool update_media_lists(const media_features& features);
		void fix_tables_layout();
		void fix_table_children(element::ptr& el_ptr, style_display disp, tstring_view disp_str);
		void fix_table_parent(element::ptr& el_ptr, style_display disp, tstring_view disp_str);
	};

	inline element::ptr document::root()
	{
		return m_root;
	}
	inline void document::add_tabular(const element::ptr& el)
	{
		m_tabular_elements.push_back(el);
	}
	inline bool document::match_lang(tstring_view lang)
	{
		return lang == m_lang || lang == m_culture;
	}
}
