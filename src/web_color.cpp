#include "html.h"
#include "web_color.h"
#include <string.h>

struct def_color
{
    litehtml::tstring_view name;
    litehtml::tstring_view rgb;
};

def_color g_def_colors[] = 
{
	{_Q("transparent"),_Q("rgba(0, 0, 0, 0)")},
	{_Q("AliceBlue"),_Q("#F0F8FF")},
	{_Q("AntiqueWhite"),_Q("#FAEBD7")},
	{_Q("Aqua"),_Q("#00FFFF")},
	{_Q("Aquamarine"),_Q("#7FFFD4")},
	{_Q("Azure"),_Q("#F0FFFF")},
	{_Q("Beige"),_Q("#F5F5DC")},
	{_Q("Bisque"),_Q("#FFE4C4")},
	{_Q("Black"),_Q("#000000")},
	{_Q("BlanchedAlmond"),_Q("#FFEBCD")},
	{_Q("Blue"),_Q("#0000FF")},
	{_Q("BlueViolet"),_Q("#8A2BE2")},
	{_Q("Brown"),_Q("#A52A2A")},
	{_Q("BurlyWood"),_Q("#DEB887")},
	{_Q("CadetBlue"),_Q("#5F9EA0")},
	{_Q("Chartreuse"),_Q("#7FFF00")},
	{_Q("Chocolate"),_Q("#D2691E")},
	{_Q("Coral"),_Q("#FF7F50")},
	{_Q("CornflowerBlue"),_Q("#6495ED")},
	{_Q("Cornsilk"),_Q("#FFF8DC")},
	{_Q("Crimson"),_Q("#DC143C")},
	{_Q("Cyan"),_Q("#00FFFF")},
	{_Q("DarkBlue"),_Q("#00008B")},
	{_Q("DarkCyan"),_Q("#008B8B")},
	{_Q("DarkGoldenRod"),_Q("#B8860B")},
	{_Q("DarkGray"),_Q("#A9A9A9")},
	{_Q("DarkGrey"),_Q("#A9A9A9")},
	{_Q("DarkGreen"),_Q("#006400")},
	{_Q("DarkKhaki"),_Q("#BDB76B")},
	{_Q("DarkMagenta"),_Q("#8B008B")},
	{_Q("DarkOliveGreen"),_Q("#556B2F")},
	{_Q("Darkorange"),_Q("#FF8C00")},
	{_Q("DarkOrchid"),_Q("#9932CC")},
	{_Q("DarkRed"),_Q("#8B0000")},
	{_Q("DarkSalmon"),_Q("#E9967A")},
	{_Q("DarkSeaGreen"),_Q("#8FBC8F")},
	{_Q("DarkSlateBlue"),_Q("#483D8B")},
	{_Q("DarkSlateGray"),_Q("#2F4F4F")},
	{_Q("DarkSlateGrey"),_Q("#2F4F4F")},
	{_Q("DarkTurquoise"),_Q("#00CED1")},
	{_Q("DarkViolet"),_Q("#9400D3")},
	{_Q("DeepPink"),_Q("#FF1493")},
	{_Q("DeepSkyBlue"),_Q("#00BFFF")},
	{_Q("DimGray"),_Q("#696969")},
	{_Q("DimGrey"),_Q("#696969")},
	{_Q("DodgerBlue"),_Q("#1E90FF")},
	{_Q("FireBrick"),_Q("#B22222")},
	{_Q("FloralWhite"),_Q("#FFFAF0")},
	{_Q("ForestGreen"),_Q("#228B22")},
	{_Q("Fuchsia"),_Q("#FF00FF")},
	{_Q("Gainsboro"),_Q("#DCDCDC")},
	{_Q("GhostWhite"),_Q("#F8F8FF")},
	{_Q("Gold"),_Q("#FFD700")},
	{_Q("GoldenRod"),_Q("#DAA520")},
	{_Q("Gray"),_Q("#808080")},
	{_Q("Grey"),_Q("#808080")},
	{_Q("Green"),_Q("#008000")},
	{_Q("GreenYellow"),_Q("#ADFF2F")},
	{_Q("HoneyDew"),_Q("#F0FFF0")},
	{_Q("HotPink"),_Q("#FF69B4")},
	{_Q("Ivory"),_Q("#FFFFF0")},
	{_Q("Khaki"),_Q("#F0E68C")},
	{_Q("Lavender"),_Q("#E6E6FA")},
	{_Q("LavenderBlush"),_Q("#FFF0F5")},
	{_Q("LawnGreen"),_Q("#7CFC00")},
	{_Q("LemonChiffon"),_Q("#FFFACD")},
	{_Q("LightBlue"),_Q("#ADD8E6")},
	{_Q("LightCoral"),_Q("#F08080")},
	{_Q("LightCyan"),_Q("#E0FFFF")},
	{_Q("LightGoldenRodYellow"),_Q("#FAFAD2")},
	{_Q("LightGray"),_Q("#D3D3D3")},
	{_Q("LightGrey"),_Q("#D3D3D3")},
	{_Q("LightGreen"),_Q("#90EE90")},
	{_Q("LightPink"),_Q("#FFB6C1")},
	{_Q("LightSalmon"),_Q("#FFA07A")},
	{_Q("LightSeaGreen"),_Q("#20B2AA")},
	{_Q("LightSkyBlue"),_Q("#87CEFA")},
	{_Q("LightSlateGray"),_Q("#778899")},
	{_Q("LightSlateGrey"),_Q("#778899")},
	{_Q("LightSteelBlue"),_Q("#B0C4DE")},
	{_Q("LightYellow"),_Q("#FFFFE0")},
	{_Q("Lime"),_Q("#00FF00")},
	{_Q("LimeGreen"),_Q("#32CD32")},
	{_Q("Linen"),_Q("#FAF0E6")},
	{_Q("Magenta"),_Q("#FF00FF")},
	{_Q("Maroon"),_Q("#800000")},
	{_Q("MediumAquaMarine"),_Q("#66CDAA")},
	{_Q("MediumBlue"),_Q("#0000CD")},
	{_Q("MediumOrchid"),_Q("#BA55D3")},
	{_Q("MediumPurple"),_Q("#9370D8")},
	{_Q("MediumSeaGreen"),_Q("#3CB371")},
	{_Q("MediumSlateBlue"),_Q("#7B68EE")},
	{_Q("MediumSpringGreen"),_Q("#00FA9A")},
	{_Q("MediumTurquoise"),_Q("#48D1CC")},
	{_Q("MediumVioletRed"),_Q("#C71585")},
	{_Q("MidnightBlue"),_Q("#191970")},
	{_Q("MintCream"),_Q("#F5FFFA")},
	{_Q("MistyRose"),_Q("#FFE4E1")},
	{_Q("Moccasin"),_Q("#FFE4B5")},
	{_Q("NavajoWhite"),_Q("#FFDEAD")},
	{_Q("Navy"),_Q("#000080")},
	{_Q("OldLace"),_Q("#FDF5E6")},
	{_Q("Olive"),_Q("#808000")},
	{_Q("OliveDrab"),_Q("#6B8E23")},
	{_Q("Orange"),_Q("#FFA500")},
	{_Q("OrangeRed"),_Q("#FF4500")},
	{_Q("Orchid"),_Q("#DA70D6")},
	{_Q("PaleGoldenRod"),_Q("#EEE8AA")},
	{_Q("PaleGreen"),_Q("#98FB98")},
	{_Q("PaleTurquoise"),_Q("#AFEEEE")},
	{_Q("PaleVioletRed"),_Q("#D87093")},
	{_Q("PapayaWhip"),_Q("#FFEFD5")},
	{_Q("PeachPuff"),_Q("#FFDAB9")},
	{_Q("Peru"),_Q("#CD853F")},
	{_Q("Pink"),_Q("#FFC0CB")},
	{_Q("Plum"),_Q("#DDA0DD")},
	{_Q("PowderBlue"),_Q("#B0E0E6")},
	{_Q("Purple"),_Q("#800080")},
	{_Q("Red"),_Q("#FF0000")},
	{_Q("RosyBrown"),_Q("#BC8F8F")},
	{_Q("RoyalBlue"),_Q("#4169E1")},
	{_Q("SaddleBrown"),_Q("#8B4513")},
	{_Q("Salmon"),_Q("#FA8072")},
	{_Q("SandyBrown"),_Q("#F4A460")},
	{_Q("SeaGreen"),_Q("#2E8B57")},
	{_Q("SeaShell"),_Q("#FFF5EE")},
	{_Q("Sienna"),_Q("#A0522D")},
	{_Q("Silver"),_Q("#C0C0C0")},
	{_Q("SkyBlue"),_Q("#87CEEB")},
	{_Q("SlateBlue"),_Q("#6A5ACD")},
	{_Q("SlateGray"),_Q("#708090")},
	{_Q("SlateGrey"),_Q("#708090")},
	{_Q("Snow"),_Q("#FFFAFA")},
	{_Q("SpringGreen"),_Q("#00FF7F")},
	{_Q("SteelBlue"),_Q("#4682B4")},
	{_Q("Tan"),_Q("#D2B48C")},
	{_Q("Teal"),_Q("#008080")},
	{_Q("Thistle"),_Q("#D8BFD8")},
	{_Q("Tomato"),_Q("#FF6347")},
	{_Q("Turquoise"),_Q("#40E0D0")},
	{_Q("Violet"),_Q("#EE82EE")},
	{_Q("Wheat"),_Q("#F5DEB3")},
	{_Q("White"),_Q("#FFFFFF")},
	{_Q("WhiteSmoke"),_Q("#F5F5F5")},
	{_Q("Yellow"),_Q("#FFFF00")},
	{_Q("YellowGreen"),_Q("#9ACD32")}
};

// parse rgb[a] & hsl[a]
void parse_color(litehtml::tstring_view str, litehtml::string_view_deque& colors)
{
    using namespace litehtml;

    tstring_view::size_type pos = str.find_first_of(_Q("("));
    if (pos != tstring_view::npos)
    {
        str.remove_prefix(pos + 1);
    }
    pos = str.find_last_of(_Q(")"));
    if (pos != tstring_view::npos)
    {
        str.remove_suffix(str.size() - pos);
    }

    string_view_deque tokens;
    split_string(str, tokens, _Q(","));

    for (tstring_view& token : tokens)
    {
        colors.push_back(trim(token));
    }
}

double to_double(litehtml::tstring_view view)
{
    return std::stod(view.to_string());
}

double percent_value(litehtml::tstring_view value)
{
    assert(value.back() == '%');
    value.remove_suffix(1);
    return to_double(value);
}

// string to value
double to_abs_value(litehtml::tstring_view value, double max_value)
{
    double result;

    if (value.back() == '%')
    {
        result = percent_value(value);
        result = (result / 100.0) * max_value;
    }
    else
    {
        result = to_double(value);
    }

    assert(result <= max_value);
    return result;
}

double hue_to_color_val(double v1, double v2, double vH)
{
    if (vH < 0.0)
        vH += 1.0;

    if (vH > 1.0)
        vH -= 1.0;

    if ((6.0 * vH) < 1.0)
        return (v1 + (v2 - v1) * 6.0 * vH);

    if ((2.0 * vH) < 1.0)
        return v2;

    if ((3.0 * vH) < 2.0)
        return (v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6.0);

    return v1;
}

litehtml::web_color hsl_to_rgb(double hue, double saturation, double lightness)
{
    using namespace litehtml;
    web_color rgb;

    if (saturation == 0.0)
    {
        rgb.red   = byte(lightness * 255.0);
        rgb.green = byte(lightness * 255.0);
        rgb.blue  = byte(lightness * 255.0);
        return rgb;
    }

    double v1, v2;
    double h = hue / 360.0;

    v2 = (lightness < 0.5) ? (lightness * (1 + saturation)) : ((lightness + saturation) - (lightness * saturation));
    v1 = 2 * lightness - v2;

    rgb.red = byte(hue_to_color_val(v1, v2, hue + (1.0 / 3.0)) * 255.0);
    rgb.green = byte(hue_to_color_val(v1, v2, hue) * 255.0);
    rgb.blue = byte(hue_to_color_val(v1, v2, hue - (1.0 / 3.0)) * 255.0);

    return rgb;
}

litehtml::web_color litehtml::web_color::from_string(tstring_view str)
{
	if(str.empty())
	{
		return web_color(0, 0, 0);
	}

	if(str[0] == '#')
	{
		std::string red;
        std::string green;
        std::string blue;
		if(str.size() == 4) // #fff
		{
			red		+= str[1];
			red		+= str[1];
			green	+= str[2];
			green	+= str[2];
			blue	+= str[3];
			blue	+= str[3];
		} else if(str.size() == 7) //#ffffff
		{
			red		+= str[1];
			red		+= str[2];
			green	+= str[3];
			green	+= str[4];
			blue	+= str[5];
			blue	+= str[6];
		}
		web_color clr;
		clr.red		= byte(std::stol(red,	nullptr, 16));
		clr.green	= byte(std::stol(green,	nullptr, 16));
		clr.blue	= byte(std::stol(blue,	nullptr, 16));
		return clr;
	} 
    else if(starts_with(str, _Q("rgb")) || starts_with(str, _Q("hsl")))
	{
        string_view_deque color_tokens;
        parse_color(str, color_tokens);

        if (color_tokens.size() != 3 && color_tokens.size() != 4)
            return web_color(0, 0, 0);

        web_color clr;

        // rgb[a](red[%], green[%], blue[%][, alpha])
        if (starts_with(str, _Q("rgb")))
        {
            clr.red   = byte(to_abs_value(color_tokens[0], 255.0));
            clr.green = byte(to_abs_value(color_tokens[1], 255.0));
            clr.blue  = byte(to_abs_value(color_tokens[2], 255.0));
        }
        // hsl[a](hue, saturate%, lightness%[, alpha])
        else
        {
            double hue = to_double(color_tokens[0]);
            double saturation = percent_value(color_tokens[1]);
            double lightness = percent_value(color_tokens[2]);

            clr = hsl_to_rgb(hue, saturation, lightness);
        }

        // [alpha]
        if (color_tokens.size() == 4)
            clr.alpha = byte(std::stod(color_tokens[3].to_string()) * 255.0);

		return clr;
	} else
	{
		tstring_view rgb = resolve_name(str);
		if(!rgb.empty())
		{
			return from_string(rgb);
		}
	}
	return web_color(0, 0, 0);
}

litehtml::tstring_view litehtml::web_color::resolve_name(tstring_view name)
{
    tstring lcase_name = lcase_copy(name);

    for (const def_color& color : g_def_colors)
    {
        if (lcase_name == lcase_copy(color.name))
        {
            return color.rgb;
        }
    }

    return tstring_view();
}

bool litehtml::web_color::is_color(tstring_view str)
{
    if ((lcase_copy(str).find("rgb") == 0) || str[0] == '#')
	{
		return true;
	}

    return !resolve_name(str).empty();
}
