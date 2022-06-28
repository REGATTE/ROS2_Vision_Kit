#ifndef VISION_BRIDGE__RGB_COLORS_H_
#define VISION_BRIDGE__RGB_COLORS_H_

#include <opencv2/opencv.hpp>
#include <vision_bridge/vision_bridge_export.h>


namespace vision_bridge
{

namespace rgb_colors
{

/**
 * @brief
 * 146 rgb colors
 */
enum Colors
{
  ALICEBLUE,
  ANTIQUEWHITE,
  AQUA,
  AQUAMARINE,
  AZURE,
  BEIGE,
  BISQUE,
  BLACK,
  BLANCHEDALMOND,
  BLUE,
  BLUEVIOLET,
  BROWN,
  BURLYWOOD,
  CADETBLUE,
  CHARTREUSE,
  CHOCOLATE,
  CORAL,
  CORNFLOWERBLUE,
  CORNSILK,
  CRIMSON,
  CYAN,
  DARKBLUE,
  DARKCYAN,
  DARKGOLDENROD,
  DARKGRAY,
  DARKGREEN,
  DARKGREY,
  DARKKHAKI,
  DARKMAGENTA,
  DARKOLIVEGREEN,
  DARKORANGE,
  DARKORCHID,
  DARKRED,
  DARKSALMON,
  DARKSEAGREEN,
  DARKSLATEBLUE,
  DARKSLATEGRAY,
  DARKSLATEGREY,
  DARKTURQUOISE,
  DARKVIOLET,
  DEEPPINK,
  DEEPSKYBLUE,
  DIMGRAY,
  DIMGREY,
  DODGERBLUE,
  FIREBRICK,
  FLORALWHITE,
  FORESTGREEN,
  FUCHSIA,
  GAINSBORO,
  GHOSTWHITE,
  GOLD,
  GOLDENROD,
  GRAY,
  GREEN,
  GREENYELLOW,
  GREY,
  HONEYDEW,
  HOTPINK,
  INDIANRED,
  INDIGO,
  IVORY,
  KHAKI,
  LAVENDER,
  LAVENDERBLUSH,
  LAWNGREEN,
  LEMONCHIFFON,
  LIGHTBLUE,
  LIGHTCORAL,
  LIGHTCYAN,
  LIGHTGOLDENRODYELLOW,
  LIGHTGRAY,
  LIGHTGREEN,
  LIGHTGREY,
  LIGHTPINK,
  LIGHTSALMON,
  LIGHTSEAGREEN,
  LIGHTSKYBLUE,
  LIGHTSLATEGRAY,
  LIGHTSLATEGREY,
  LIGHTSTEELBLUE,
  LIGHTYELLOW,
  LIME,
  LIMEGREEN,
  LINEN,
  MAGENTA,
  MAROON,
  MEDIUMAQUAMARINE,
  MEDIUMBLUE,
  MEDIUMORCHID,
  MEDIUMPURPLE,
  MEDIUMSEAGREEN,
  MEDIUMSLATEBLUE,
  MEDIUMSPRINGGREEN,
  MEDIUMTURQUOISE,
  MEDIUMVIOLETRED,
  MIDNIGHTBLUE,
  MINTCREAM,
  MISTYROSE,
  MOCCASIN,
  NAVAJOWHITE,
  NAVY,
  OLDLACE,
  OLIVE,
  OLIVEDRAB,
  ORANGE,
  ORANGERED,
  ORCHID,
  PALEGOLDENROD,
  PALEGREEN,
  PALEVIOLETRED,
  PAPAYAWHIP,
  PEACHPUFF,
  PERU,
  PINK,
  PLUM,
  POWDERBLUE,
  PURPLE,
  RED,
  ROSYBROWN,
  ROYALBLUE,
  SADDLEBROWN,
  SALMON,
  SANDYBROWN,
  SEAGREEN,
  SEASHELL,
  SIENNA,
  SILVER,
  SKYBLUE,
  SLATEBLUE,
  SLATEGRAY,
  SLATEGREY,
  SNOW,
  SPRINGGREEN,
  STEELBLUE,
  TAN,
  TEAL,
  THISTLE,
  TOMATO,
  TURQUOISE,
  VIOLET,
  WHEAT,
  WHITE,
  WHITESMOKE,
  YELLOW,
  YELLOWGREEN,
};

/**
 * @brief
 * get rgb color with enum.
 */
VISION_BRIDGE_EXPORT cv::Vec3d getRGBColor(const int color);

}  // namespace rgb_colors

}  // namespace vision_bridge

#endif  // VISION_BRIDGE__RGB_COLORS_H_