#pragma once
#include "html_tag.h"
#include "el_text.h"

namespace litehtml
{
	class el_space : public el_text
	{
	public:
        explicit el_space(tstring_view text, const std::shared_ptr<litehtml::document>& doc);
        explicit el_space(tstring&& text, const std::shared_ptr<litehtml::document>& doc);
		virtual ~el_space() = default;

		virtual bool	is_white_space() const override;
		virtual bool	is_break() const override;
	};
}