#pragma once

namespace litehtml
{
	struct web_color
	{
        union
        {
            uint32_t color;
            struct
            {
                byte    blue;
                byte    green;
                byte    red;
                byte    alpha;
            };
        };

        web_color(byte r, byte g, byte b, byte a = 255)
		{
			blue	= b;
			green	= g;
			red		= r;
			alpha	= a;
		}

        web_color() : web_color(0, 0, 0) {}

		static web_color		from_string(tstring_view str);
		static tstring_view	resolve_name(tstring_view name);
		static bool				is_color(tstring_view str);
	};

    inline bool operator==(web_color lhs, web_color rhs)
    {
        return lhs.color == rhs.color;
    }

    inline bool operator!=(web_color lhs, web_color rhs)
    {
        return !(lhs == rhs);
    }

    inline bool operator<(web_color lhs, web_color rhs)
    {
        return lhs.color < rhs.color;
    }

    inline bool operator<=(web_color lhs, web_color rhs)
    {
        return lhs < rhs || lhs == rhs;
    }

    inline bool operator>(web_color lhs, web_color rhs)
    {
        return lhs.color > rhs.color;
    }

    inline bool operator>=(web_color lhs, web_color rhs)
    {
        return lhs > rhs || lhs == rhs;
    }
}