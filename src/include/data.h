#ifndef __DATA_H__
#define __DATA_H__

using namespace std;
#include<string>

static char *dump_parameters[] = {
        "MODULE_NAME",
        "REGISTER_NAME",
        "ADDRESS"
};

static char *input_parameters[] = {
        "NUMBER_OF_REGISTER_DUMP",
        "CCS_CHAIN_POISTION",
        "MODULE_NAME"
};

static char *dump_info[] = {
        "REGISTER_NAME",
        "ADDRESS"
        "ENDIANESS",
};

struct input_params {
	char *input_file;
	char *output_file;
	char *out_type;
	char *interface;
	int ccs_chain_position;
};

class general_data {
	public:
		int num_of_reg_dumps;
		general_data(){
			num_of_reg_dumps = 0;
		}
};

class info_reg_dump{
	public:
		string module_name;
		string reg_name;
		string addr;
		string endianess;
		int val;
		string offset;
		int size;
		bool valid_bit;
		info_reg_dump() {
			module_name = "";
			endianess = "LE";
			reg_name = "";
			addr = "";
			val = 0;
			offset = "";
			size = 0;
			valid_bit = 0;
		}
};

#endif
