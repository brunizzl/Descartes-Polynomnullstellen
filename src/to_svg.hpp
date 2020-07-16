#pragma once

#include <string>
#include <fstream>
#include <vector>

constexpr double pi = 3.14159265358979323846264338327950288419716939937510582097494;

struct RGB
{
	unsigned char red, green, blue;

	std::string to_sting() const;
};

namespace rgb {

	//stolen from here https://www.rapidtables.com/web/color/RGB_Color.html
	constexpr RGB maroon                   { 128,  0,  0 };
	constexpr RGB dark_red                 { 139,  0,  0 };
	constexpr RGB brown                    { 165, 42, 42 };
	constexpr RGB firebrick                { 178, 34, 34 };
	constexpr RGB crimson                  { 220, 20, 60 };
	constexpr RGB red                      { 255,  0,  0 };
	constexpr RGB tomato                   { 255, 99, 71 };
	constexpr RGB coral                    { 255,127, 80 };
	constexpr RGB indian_red               { 205, 92, 92 };
	constexpr RGB light_coral              { 240,128,128 };
	constexpr RGB dark_salmon              { 233,150,122 };
	constexpr RGB salmon                   { 250,128,114 };
	constexpr RGB light_salmon             { 255,160,122 };
	constexpr RGB orange_red               { 255, 69,  0 };
	constexpr RGB dark_orange              { 255,140,  0 };
	constexpr RGB orange                   { 255,165,  0 };
	constexpr RGB gold                     { 255,215,  0 };
	constexpr RGB dark_golden_rod          { 184,134, 11 };
	constexpr RGB golden_rod               { 218,165, 32 };
	constexpr RGB pale_golden_rod          { 238,232,170 };
	constexpr RGB dark_khaki               { 189,183,107 };
	constexpr RGB khaki                    { 240,230,140 };
	constexpr RGB olive                    { 128,128,  0 };
	constexpr RGB yellow                   { 255,255,  0 };
	constexpr RGB yellow_green             { 154,205, 50 };
	constexpr RGB dark_olive_green         {  85,107, 47 };
	constexpr RGB olive_drab               { 107,142, 35 };
	constexpr RGB lawn_green               { 124,252,  0 };
	constexpr RGB chart_reuse              { 127,255,  0 };
	constexpr RGB green_yellow             { 173,255, 47 };
	constexpr RGB dark_green               {   0,100,  0 };
	constexpr RGB green                    {   0,128,  0 };
	constexpr RGB forest_green             {  34,139, 34 };
	constexpr RGB lime                     {   0,255,  0 };
	constexpr RGB lime_green               {  50,205, 50 };
	constexpr RGB light_green              { 144,238,144 };
	constexpr RGB pale_green               { 152,251,152 };
	constexpr RGB dark_sea_green           { 143,188,143 };
	constexpr RGB medium_spring_green      {   0,250,154 };
	constexpr RGB spring_green             {   0,255,127 };
	constexpr RGB sea_green                {  46,139, 87 };
	constexpr RGB medium_aqua_marine       { 102,205,170 };
	constexpr RGB medium_sea_green         {  60,179,113 };
	constexpr RGB light_sea_green          {  32,178,170 };
	constexpr RGB dark_slate_gray          {  47, 79, 79 };
	constexpr RGB teal                     {   0,128,128 };
	constexpr RGB dark_cyan                {   0,139,139 };
	constexpr RGB aqua                     {   0,255,255 };
	constexpr RGB cyan                     {   0,255,255 };
	constexpr RGB light_cyan               { 224,255,255 };
	constexpr RGB dark_turquoise           {   0,206,209 };
	constexpr RGB turquoise                {  64,224,208 };
	constexpr RGB medium_turquoise         {  72,209,204 };
	constexpr RGB pale_turquoise           { 175,238,238 };
	constexpr RGB aqua_marine              { 127,255,212 };
	constexpr RGB powder_blue              { 176,224,230 };
	constexpr RGB cadet_blue               {  95,158,160 };
	constexpr RGB steel_blue               {  70,130,180 };
	constexpr RGB corn_flower_blue         { 100,149,237 };
	constexpr RGB deep_sky_blue            {   0,191,255 };
	constexpr RGB dodger_blue              {  30,144,255 };
	constexpr RGB light_blue               { 173,216,230 };
	constexpr RGB sky_blue                 { 135,206,235 };
	constexpr RGB light_sky_blue           { 135,206,250 };
	constexpr RGB midnight_blue            {  25, 25,112 };
	constexpr RGB navy                     {   0,  0,128 };
	constexpr RGB dark_blue                {   0,  0,139 };
	constexpr RGB medium_blue              {   0,  0,205 };
	constexpr RGB blue                     {   0,  0,255 };
	constexpr RGB royal_blue               {  65,105,225 };
	constexpr RGB blue_violet              { 138, 43,226 };
	constexpr RGB indigo                   {  75,  0,130 };
	constexpr RGB dark_slate_blue          {  72, 61,139 };
	constexpr RGB slate_blue               { 106, 90,205 };
	constexpr RGB medium_slate_blue        { 123,104,238 };
	constexpr RGB medium_purple            { 147,112,219 };
	constexpr RGB dark_magenta             { 139,  0,139 };
	constexpr RGB dark_violet              { 148,  0,211 };
	constexpr RGB dark_orchid              { 153, 50,204 };
	constexpr RGB medium_orchid            { 186, 85,211 };
	constexpr RGB purple                   { 128,  0,128 };
	constexpr RGB thistle                  { 216,191,216 };
	constexpr RGB plum                     { 221,160,221 };
	constexpr RGB violet                   { 238,130,238 };
	constexpr RGB magenta                  { 255,  0,255 };
	constexpr RGB fuchsia                  { 255,  0,255 };
	constexpr RGB orchid                   { 218,112,214 };
	constexpr RGB medium_violet_red        { 199, 21,133 };
	constexpr RGB pale_violet_red          { 219,112,147 };
	constexpr RGB deep_pink                { 255, 20,147 };
	constexpr RGB hot_pink                 { 255,105,180 };
	constexpr RGB light_pink               { 255,182,193 };
	constexpr RGB pink                     { 255,192,203 };
	constexpr RGB antique_white            { 250,235,215 };
	constexpr RGB beige                    { 245,245,220 };
	constexpr RGB bisque                   { 255,228,196 };
	constexpr RGB blanched_almond          { 255,235,205 };
	constexpr RGB wheat                    { 245,222,179 };
	constexpr RGB corn_silk                { 255,248,220 };
	constexpr RGB lemon_chiffon            { 255,250,205 };
	constexpr RGB light_golden_rod_yellow  { 250,250,210 };
	constexpr RGB light_yellow             { 255,255,224 };
	constexpr RGB saddle_brown             { 139, 69, 19 };
	constexpr RGB sienna                   { 160, 82, 45 };
	constexpr RGB chocolate                { 210,105, 30 };
	constexpr RGB peru                     { 205,133, 63 };
	constexpr RGB sandy_brown              { 244,164, 96 };
	constexpr RGB burly_wood               { 222,184,135 };
	constexpr RGB tan                      { 210,180,140 };
	constexpr RGB rosy_brown               { 188,143,143 };
	constexpr RGB moccasin                 { 255,228,181 };
	constexpr RGB navajo_white             { 255,222,173 };
	constexpr RGB peach_puff               { 255,218,185 };
	constexpr RGB misty_rose               { 255,228,225 };
	constexpr RGB lavender_blush           { 255,240,245 };
	constexpr RGB linen                    { 250,240,230 };
	constexpr RGB old_lace                 { 253,245,230 };
	constexpr RGB papaya_whip              { 255,239,213 };
	constexpr RGB sea_shell                { 255,245,238 };
	constexpr RGB mint_cream               { 245,255,250 };
	constexpr RGB slate_gray               { 112,128,144 };
	constexpr RGB light_slate_gray         { 119,136,153 };
	constexpr RGB light_steel_blue         { 176,196,222 };
	constexpr RGB lavender                 { 230,230,250 };
	constexpr RGB floral_white             { 255,250,240 };
	constexpr RGB alice_blue               { 240,248,255 };
	constexpr RGB ghost_white              { 248,248,255 };
	constexpr RGB honeydew                 { 240,255,240 };
	constexpr RGB ivory                    { 255,255,240 };
	constexpr RGB azure                    { 240,255,255 };
	constexpr RGB snow                     { 255,250,250 };
	constexpr RGB black                    {   0,  0,  0 };
	constexpr RGB dim_grey                 { 105,105,105 };
	constexpr RGB grey                     { 128,128,128 };
	constexpr RGB dark_grey                { 169,169,169 };
	constexpr RGB silver                   { 192,192,192 };
	constexpr RGB light_grey               { 211,211,211 };
	constexpr RGB gainsboro                { 220,220,220 };
	constexpr RGB white_smoke              { 245,245,245 };
	constexpr RGB white                    { 255,255,255 };
}

struct HSV
{
	double hue;			//assumed to be in [0, 2 * pi]
	double saturation;	//assumed to be in [0, 1]
	double value;		//assumed to be in [0, 1]

	RGB to_rgb() const;
};


struct Vec2D
{
	double x, y;
};


class SVG
{
public:
	struct Style
	{
		double stroke_width;
		RGB stroke_color;
		RGB fill_color;
		bool fill;
		double transparency;
	};

	struct Text_Style
	{
		std::string font;
		double size;
		RGB color;
	};


	SVG(const std::string& name, Vec2D view_box_min, Vec2D view_box_max);
	~SVG();

	void add_line(Vec2D start, Vec2D end, const Style& style);
	void add_line_path(const std::vector<Vec2D>& path, bool close, const Style& style);
	void add_text(Vec2D position, const std::string& content, const Text_Style& style);

private:
	std::ofstream document;
};

namespace svg_style {

	constexpr SVG::Style line(const RGB& color, double stroke = 5) {
		return { stroke, color, rgb::black, false, 0 };
	}
	constexpr SVG::Style fill(const RGB& color) { 
		return { 0, rgb::black, color, true, 0.2 }; 
	}

	using namespace rgb;
	constexpr SVG::Style red_line = { 0.5, red, black, false, 0 };
	constexpr SVG::Style green_line = { 0.5, green, black, false, 0 };
	constexpr SVG::Style blue_line = { 0.5, blue, black, false, 0 };

	constexpr SVG::Style black_fill = { 0, black, black, true, 0.2 };
}

namespace svg_text {
	const SVG::Text_Style small  = { "Veranda", 2, rgb::black };
	const SVG::Text_Style medium = { "Veranda", 4, rgb::black };
	const SVG::Text_Style large  = { "Veranda", 6, rgb::black };
}