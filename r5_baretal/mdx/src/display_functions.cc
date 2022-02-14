
#include "display_functions.h"

#include "MyDisp.h"

#include <map>
#include <vector>

typedef void  (*fnptr)(std::string,bool*);
std::map<std::string,fnptr> function;


void function_call (std::string name, std::string args, bool* ret){
	function[name] (args,ret);
}


void tokenize(std::string const &str,std::vector<std::string> &out)
{
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(ARGUMENTS_DELIMITER, end)) != std::string::npos)
    {
        end = str.find(ARGUMENTS_DELIMITER, start);
        out.push_back(str.substr(start, end - start));
    }
}
void tokenize_function(std::string const &str, std::vector<std::string> &out)
{
    size_t start;
    size_t end = 0;
    const char delim = '-';
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}


void begin(std::string args, bool* ret) {
	*ret = mydisp.begin();
}

void end(std::string args, bool* ret) {
	mydisp.end();
}

void display_on(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	bool fEN =  out[0] == "1";
	*ret = mydisp.displayOn(fEN);
}

void clear_display(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	int clr = std::stoi( out[0]);
	*ret = mydisp.clearDisplay(clr);
}
void set_backlight(std::string args, bool* ret){
	std::vector <std::string> out;  tokenize(args, out);
	int val = std::stoi( out[0]);
	*ret = mydisp.setBacklight(val);
}
void display_hds(std::string args, HDS* ret) {
	*ret = mydisp.displayHds();

}

void set_pen(std::string args, bool* ret){
	std::vector <std::string> out;  tokenize(args, out);
	int pen = std::stoi( out[0]);
	*ret = mydisp.setPen(pen);
}

void set_foreground(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	int clr = std::stoi( out[0]);
	*ret = mydisp.setForeground(clr);
}

void set_background(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	int clr = std::stoi( out[0]);
	*ret = mydisp.setBackground(clr);
}

void set_transparency(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	bool fTrans =  out[0] == "1";
	int clr = std::stoi( out[1]);
	*ret = mydisp.setTransparency(fTrans, clr);
}

void set_intensity(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	int inty = std::stoi( out[0]);
	*ret = mydisp.setIntensity(inty);
}

void set_font(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	int idFont = std::stoi( out[0]);
//	*ret = mydisp.setFont(idFont);
}

void load_image(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	char * name = (char *)out[0].c_str();
	*ret = mydisp.loadImage(name);
}

void delete_image(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	int hbmp = std::stoi( out[0]);
	*ret = mydisp.deleteImage(hbmp);
}

void set_pixel(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);

	int x = std::stoi( out[0]);
	int y = std::stoi( out[1]);
	int clr = std::stoi( out[2]);

	xil_printf("this is my argument");
	xil_printf("x,y,z %s, %s, %s",out[0],out[1],out[2]);
	*ret = mydisp.setPixel(x, y, clr);
}

void get_pixel(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	int x = std::stoi( out[0]);
	int y = std::stoi( out[1]);
	*ret = mydisp.getPixel(x, y);
}

void draw_line(std::string args, bool *ret) {
	std::vector <std::string> out;  tokenize(args, out);
	int x1 = std::stoi( out[0]);
	int y1 = std::stoi( out[1]);
	int x2 = std::stoi( out[2]);
	int y2 = std::stoi( out[3]);

	*ret =  mydisp.drawLine(x1, y1, x2, y2);
}

void draw_arc(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	int x1 = std::stoi( out[0]);
	int y1 = std::stoi( out[1]);

	int x2 = std::stoi( out[2]);
	int y2 = std::stoi( out[3]);

	int xr1 = std::stoi( out[4]);
	int yr1 = std::stoi( out[5]);

	int xr2 = std::stoi( out[6]);
	int yr2 = std::stoi( out[7]);

	*ret =  mydisp.drawArc(x1, y1, x2, y2, xr1, yr1, xr2, yr2);
}

void draw_rectangle(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	bool fill =  out[0] == "1";

	int x1 = std::stoi( out[1]);
	int y1 = std::stoi( out[2]);
	int x2 = std::stoi( out[3]);
	int y2 = std::stoi( out[4]);

	*ret =  mydisp.drawRectangle(fill, x1, y1, x2, y2);
}

void draw_ellipse(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	bool fill = true;
	if( std::stoi( out[0])==0)
		fill = false;

	int x1 = std::stoi( out[1]);
	int y1 = std::stoi( out[2]);
	int x2 = std::stoi( out[3]);
	int y2 = std::stoi( out[4]);

	*ret =  mydisp.drawEllipse(fill, x1, y1, x2, y2);
}

void draw_image(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);

	char * name = (char *) out[0].c_str();

	int x = std::stoi( out[1]);
	int y = std::stoi( out[2]);

	*ret =  mydisp.drawImage(name, x, y);

}

void draw_text(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	char * txt = (char *) out[0].c_str();

	int x = std::stoi( out[1]);
	int y = std::stoi( out[2]);

	*ret =  mydisp.drawText(txt, x, y);
}

void create_button(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	int id = std::stoi( out[0]);
	char * txt = (char *) out[1].c_str();

	int x = std::stoi( out[2]);
	int y = std::stoi( out[3]);

	*ret =  mydisp.drawText(txt, x, y);
}

void delete_button(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	int id = std::stoi( out[0]);

	*ret =  mydisp.deleteButton(id);
}

void enable_button(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	int id = std::stoi( out[0]);
	bool fEn =  out[0] == "1";
	*ret =  mydisp.enableButton(id, fEn);
}

void draw_button(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	int id = std::stoi( out[0]);
	int st = std::stoi( out[1]);
	*ret =  mydisp.drawButton(id, st);
}

void is_enabled(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	int id = std::stoi( out[0]);
	*ret =  mydisp.isEnabled(id);
}

void is_touched(std::string args, bool* ret) {
	std::vector <std::string> out;  tokenize(args, out);
	int id = std::stoi( out[0]);

	*ret =  mydisp.isTouched(id);
}

void get_finger(std::string args, bool* ret) {

}

void get_button(std::string args, int* ret) {
	*ret = mydisp.getButton();
}

void check_touch(std::string args, bool* ret) {
	mydisp.checkTouch();
}

bool function_map (){


	function["begin"] = begin;
	function["end"] = end;
	function["display_on"] = display_on;
	function["clear_display"] = clear_display;
	function["set_backlight"] = set_backlight;
//	function["display_hds"] = display_hds;
	function["set_pen"] = set_pen;
	function["set_foreground"] = set_foreground;
	function["set_Background"] = set_background;
	function["set_transparency"] = set_transparency;
	function["set_intensity"] = set_intensity;
	function["set_font"] = set_font;
	function["load_image"] = load_image;
	function["delete_image"] = delete_image;
	function["set_pixel"] = set_pixel;
	function["get_pixel"] = get_pixel;
	function["draw_line"] = draw_line;
	function["draw_arc"] = draw_arc;
	function["draw_rectangle"] = draw_rectangle;
	function["draw_ellipse"] = draw_ellipse;
	function["draw_image"] = draw_image;
	function["draw_text"] = draw_text;
	function["create_button"] = create_button;
	function["delete_button"] = delete_button;
	function["enable_button"] = enable_button;
	function["draw_button"] = draw_button;
	function["is_enabled"] = is_enabled;
	function["is_touched"] = is_touched;
	function["get_finger"] = get_finger;
//	function["get_button"] = get_button;
	function["check_touch"] = check_touch;

	return true;
}


/*
void  function_mapper(std::string function_name, void * args){
	switch(function_name){
		case "begin":
			break;
		case "end":
			break;
		case "display_on":
			break;
		case "clear_display":
			break;
		case "display_hds":
			break;
		case "set_pen":
			break;
		case "set_foreground":
			break;
		case "set_Background":
			break;
		case "set_transparency":
			break;
		case "set_intensity":
			break;
		case "set_font":
			break;
		case "load_image":
			break;
		case "delete_image":
			break;
		case "set_pixel":
			break;
		case "get_pixel":
			break;
		case "draw_line":
			break;
		case "draw_arc":
			break;
		case "draw_rectangle":
			break;
		case "draw_ellipse":
			break;
		case "draw_image":
			break;
		case "draw_image":
			break;
		case "draw_text":
			break;
		case "create_button":
			break;
		case "delete_button":
			break;
		case "enable_button":
			break;
		case "draw_button":
			break;
		case "is_enabled":
			break;
		case "is_touched":
			break;
		case "get_finger":
			break;
		case "get_button":
			break;
		case "check_touch":
			break;


	}
}

*/
