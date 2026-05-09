
/*
     ██╗  ██╗  █████╗  ████████╗  █████╗  ███╗   ██╗  █████╗ 
	 ██║ ██╔╝ ██╔══██╗ ╚══██╔══╝ ██╔══██╗ ████╗  ██║ ██╔══██╗
	 █████╔╝  ███████║    ██║    ███████║ ██╔██╗ ██║ ███████║
	 ██╔═██╗  ██╔══██║    ██║    ██╔══██║ ██║╚██╗██║ ██╔══██║
	 ██║  ██╗ ██║  ██║    ██║    ██║  ██║ ██║ ╚████║ ██║  ██║
	 ╚═╝  ╚═╝ ╚═╝  ╚═╝/\  ╚═╝    ╚═╝  ╚═╝ ╚═╝  ╚═══╝ ╚═╝  ╚═╝
   /vvvvvvvvvvvvvvvvvvv \=========================================,
   `^^^^^^^^^^^^^^^^^^^ /---------------------------------------"
        Katana Engine \/ © 2012 - Shuriken Studios LLC
			    http://www.shurikenstudios.com
*/

#pragma once
#include <allegro5/allegro.h>
namespace KatanaEngine
{
	/** @brief Represents a four-component color using red, green, blue, and alpha data. */
	class Color
	{

	public:

		/** @brief Instantiate a color object.
			@param red The red value of the color.
			@param green The green value of the color.
			@param blue The blue value of the color.
			@param alpha The alpha value (opacity) of the color. */	
		Color(const float red = 1, const float green = 1, const float blue = 1, const float alpha = 1)
			: Red(red), Green(green), Blue(blue), Alpha(alpha) { }

		/** @brief Linearly interpolate between two colors.
			@param start The result when the value is zero.
			@param end The result when the value is one.
			@param value A value between 0 and 1 that will indicate the resulting weight between start and end.
			@return Returns the interpolated color value between start and end. */
		static Color Lerp(const Color& start, const Color& end, const float value);

		/** @brief Multiplies each component by a scale factor.
			@param scalar The scale factor.
			@return Returns the scaled color. */
		const Color operator*(const float scalar) const;
		/** @brief Determines if two colors are equal.
			@param color The color to compare.
			@return Returns true if the colors are equal, false otherwise. */
		bool operator==(const Color& color) const;

		/** @brief Determines if two colors are not equal.
			@param color The color to compare.
			@return Returns true if the colors are not equal, false otherwise. */
		bool operator!=(const Color& color) const;

		/** @brief Get the color-equivalent allegro color.
			@return Returns the mapped allegro color. */
		ALLEGRO_COLOR GetAllegroColor() const { return al_map_rgba_f(Red, Green, Blue, Alpha); }


		float Red;		/**< @brief The red component of the color. */
		float Green;	/**< @brief The green component of the color. */
		float Blue;		/**< @brief The blue component of the color. */
		float Alpha;	/**< @brief The alpha component of the color. */


		static const Color TRANSPARENT;

		static const Color ALICEBLUE;
		static const Color ANTIQUEWHITE;
		static const Color AQUA;
		static const Color AQUAMARINE;
		static const Color AZURE;
		static const Color BEIGE;
		static const Color BISQUE;
		static const Color BLACK;
		static const Color BLANCHEDALMOND;
		static const Color BLUE;
		static const Color BLUEVIOLET;
		static const Color BROWN;
		static const Color BURLYWOOD;
		static const Color CADETBLUE;
		static const Color CHARTREUSE;
		static const Color CHOCOLATE;
		static const Color CORAL;
		static const Color CORNFLOWER;
		static const Color CORNSILK;
		static const Color CRIMSON;
		static const Color CYAN;
		static const Color DARKBLUE;
		static const Color DARKCYAN;
		static const Color DARKGOLDENROD;
		static const Color DARKGRAY;
		static const Color DARKGREEN;
		static const Color DARKKHAKI;
		static const Color DARKMAGENTA;
		static const Color DARKOLIVEGREEN;
		static const Color DARKORANGE;
		static const Color DARKORCHID;
		static const Color DARKRED;
		static const Color DARKSALMON;
		static const Color DARKSEAGREEN;
		static const Color DARKSLATEBLUE;
		static const Color DARKSLATEGRAY;
		static const Color DARKTURQUOISE;
		static const Color DARKVIOLET;
		static const Color DEEPPINK;
		static const Color DEEPSKYBLUE;
		static const Color DIMGRAY;
		static const Color DODGERBLUE;
		static const Color FIREBRICK;
		static const Color FLORALWHITE;
		static const Color FORESTGREEN;
		static const Color FUCHSIA;
		static const Color GAINSBORO;
		static const Color GHOSTWHITE;
		static const Color GOLD;
		static const Color GOLDENROD;
		static const Color GRAY;
		static const Color WEBGRAY;
		static const Color GREEN;
		static const Color WEBGREEN;
		static const Color GREENYELLOW;
		static const Color HONEYDEW;
		static const Color HOTPINK;
		static const Color INDIANRED;
		static const Color INDIGO;
		static const Color IVORY;
		static const Color KHAKI;
		static const Color LAVENDER;
		static const Color LAVENDERBLUSH;
		static const Color LAWNGREEN;
		static const Color LEMONCHIFFON;
		static const Color LIGHTBLUE;
		static const Color LIGHTCORAL;
		static const Color LIGHTCYAN;
		static const Color LIGHTGOLDENROD;
		static const Color LIGHTGRAY;
		static const Color LIGHTGREEN;
		static const Color LIGHTPINK;
		static const Color LIGHTSALMON;
		static const Color LIGHTSEAGREEN;
		static const Color LIGHTSKYBLUE;
		static const Color LIGHTSLATEGRAY;
		static const Color LIGHTSTEELBLUE;
		static const Color LIGHTYELLOW;
		static const Color LIME;
		static const Color LIMEGREEN;
		static const Color LINEN;
		static const Color MAGENTA;
		static const Color MAROON;
		static const Color WEBMAROON;
		static const Color MEDIUMAQUAMARINE;
		static const Color MEDIUMBLUE;
		static const Color MEDIUMORCHID;
		static const Color MEDIUMPURPLE;
		static const Color MEDIUMSEAGREEN;
		static const Color MEDIUMSLATEBLUE;
		static const Color MEDIUMSPRINGGREEN;
		static const Color MEDIUMTURQUOISE;
		static const Color MEDIUMVIOLETRED;
		static const Color MIDNIGHTBLUE;
		static const Color MINTCREAM;
		static const Color MISTYROSE;
		static const Color MOCCASIN;
		static const Color NAVAJOWHITE;
		static const Color NAVYBLUE;
		static const Color OLDLACE;
		static const Color OLIVE;
		static const Color OLIVEDRAB;
		static const Color ORANGE;
		static const Color ORANGERED;
		static const Color ORCHID;
		static const Color PALEGOLDENROD;
		static const Color PALEGREEN;
		static const Color PALETURQUOISE;
		static const Color PALEVIOLETRED;
		static const Color PAPAYAWHIP;
		static const Color PEACHPUFF;
		static const Color PERU;
		static const Color PINK;
		static const Color PLUM;
		static const Color POWDERBLUE;
		static const Color PURPLE;
		static const Color WEBPURPLE;
		static const Color REBECCAPURPLE;
		static const Color RED;
		static const Color ROSYBROWN;
		static const Color ROYALBLUE;
		static const Color SADDLEBROWN;
		static const Color SALMON;
		static const Color SANDYBROWN;
		static const Color SEAGREEN;
		static const Color SEASHELL;
		static const Color SIENNA;
		static const Color SILVER;
		static const Color SKYBLUE;
		static const Color SLATEBLUE;
		static const Color SLATEGRAY;
		static const Color SNOW;
		static const Color SPRINGGREEN;
		static const Color STEELBLUE;
		static const Color TAN;
		static const Color TEAL;
		static const Color THISTLE;
		static const Color TOMATO;
		static const Color TURQUOISE;
		static const Color VIOLET;
		static const Color WHEAT;
		static const Color WHITE;
		static const Color WHITESMOKE;
		static const Color YELLOW;
		static const Color YELLOWGREEN;

	};
}