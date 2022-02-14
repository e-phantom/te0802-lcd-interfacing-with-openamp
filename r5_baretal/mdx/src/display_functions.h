/*
 * display_functions.h
 *
 *  Created on: Feb 4, 2022
 *      Author: hamza
 */

#ifndef SRC_DISPLAY_FUNCTIONS_H_
#define SRC_DISPLAY_FUNCTIONS_H_
#include <string>
#include <vector>
#define ARGUMENTS_DELIMITER  ";"

void begin(std::string args, bool* ret);

void end(std::string args, bool* ret);

void display_on(std::string args, bool* ret);

void clear_display(std::string args, bool* ret);

void display_hds(std::string args, bool* ret);

void set_pen(std::string args, bool* ret);

void set_foreground(std::string args, bool* ret);

void set_Background(std::string args, bool* ret);

void set_transparency(std::string args, bool* ret);

void set_intensity(std::string args, bool* ret);

void set_font(std::string args, bool* ret);

void load_image(std::string args, bool* ret);

void delete_image(std::string args, bool* ret);

void set_pixel(std::string args, bool* ret);

void get_pixel(std::string args, bool* ret);

void draw_line(std::string args, bool* ret);

void draw_arc(std::string args, bool* ret);

void draw_rectangle(std::string args, bool* ret);

void draw_ellipse(std::string args, bool* ret);

void draw_image(std::string args, bool* ret);

void draw_image(std::string args, bool* ret);

void draw_text(std::string args, bool* ret);

void create_button(std::string args, bool* ret);

void delete_button(std::string args, bool* ret);

void enable_button(std::string args, bool* ret);

void draw_button(std::string args, bool* ret);

void is_enabled(std::string args, bool* ret);

void is_touched(std::string args, bool* ret);

void get_finger(std::string args, bool* ret);

void get_button(std::string args, bool* ret);

void check_touch(std::string args, bool* ret);

bool function_map();
void function_call (std::string name, std::string args, bool* ret);

void tokenize(std::string const &str,std::vector<std::string> &out);
void tokenize_function(std::string const &str, std::vector<std::string> &out);


#endif /* SRC_DISPLAY_FUNCTIONS_H_ */
