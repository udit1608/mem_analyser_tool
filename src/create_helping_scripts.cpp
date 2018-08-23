#include <iostream>
#include <data.h>
#include <string.h>
#include <utilities.h>
#include<common.h>

using namespace std;
extern struct input_params in_prm;
extern vector<info_reg_dump> vector_reg_info;


int create_helpful_scripts() {

	ofstream myfile;
	int cpos = in_prm.ccs_chain_position;
	myfile.open ("ccs_help_file.csh");
	myfile << "\n";

	for (vector<info_reg_dump>::iterator it = vector_reg_info.begin() ; it != vector_reg_info.end(); ++it){
		class info_reg_dump p = *it;
		myfile<<"ccs::read_mem ";
		myfile<<cpos;
		myfile<<" ";
		myfile<<p.addr;		                
		myfile<<" ";
		myfile<<(p.size/4);
		myfile<<" 0 1";
		myfile<<";\n";

	}
	myfile.close();
	return 1;
}
