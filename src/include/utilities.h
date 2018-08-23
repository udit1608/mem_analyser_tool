#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <common.h>
int parse_input_file();
int fill_string_data(string *, char *);
int fill_int_data(int *, char *, char *);
int stoi(string, int *);
string NumberToString (int);
char* string_to_char(string);
int fetch_reg_data(void);
int fetch_module_data(string, string, string);
int fetch_register_data(string, string, string, string); 
int get_regs_value();
int read_and_get_regs_value(char *);
int check_command_format(string, regex);
int create_output_xml_file(void);
int create_helpful_scripts();

#endif
