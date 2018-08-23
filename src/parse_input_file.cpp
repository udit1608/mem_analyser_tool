#include<common.h>
#include<list>
#include<regex>
using namespace std;

extern struct input_params in_prm;
extern class general_data gd;
list<string> module_parsed;
extern vector<info_reg_dump> vector_reg_info;
extern int scripts;
/*
   int get_header_data(char **input_parameters, char *input_file) {

   char *file = input_file;
   int ret;

   FILE *fp;

   fp = fopen(input_file, "r");
   if (fp == NULL) {
   cout<<"Error in opening the file: %s => "<< file<<endl;
   return 0;
   } else {
   cout <<"Parsing header from file: "<<file<<endl;
   }

   ret = fill_int_data(&gd.num_of_reg_dumps, input_parameters[0], file);
   if (ret == 0) {
   cout<<"\tNumber of register to be dumped is: "<<ret<<endl;
   return 0;
   }

   if(strcmp(in_prm.interface,"kernel") != 0){
   ret = fill_int_data(&gd.ccs_chain_pos, input_parameters[1], file);
   if (ret == 0) {
   cout<<"\tCCS chain position cannot be: "<<ret<<endl;
   return 0;
   }
   }

   ret = fill_string_data(&gd.module_name, input_parameters[2], file);
   if (ret == 0) {
   cout<<"\tModule name not specified "<<endl;
   return 0;
   }

#ifdef DEBUG
cout<<"Parsed parameters are"<<endl;
cout<<"NUMBER_OF_REGISTER_DUMP "<<gd.num_of_reg_dumps<<endl;
cout<<"CCS_CHAIN_POSITION "<<gd.ccs_chain_pos<<endl;
cout<<"MODULE_NAME"<<gd.module_name<<endl;
#endif
}
 */

// To fill the reg_info struct based on th input line
// fill the temp field removing the spaces.
// Parse the temp field for the specified format and fill the reg_info class object
int get_reg_info(class info_reg_dump *reg_info, string temp){

	string tmp,t;
	int start, end, len;

	tmp = temp;
	// tmp has following string format = {Module_name,Reg_name,0x1000000,LE/BE}
	// First parse from { to , delimiters and then update tmp starting from
	// loc of (,) + 1 to end then addr is from start of modified tmp string
	// to , delimiter and endianess is from loc , + 1 to } delimiter end.  
	start = tmp.find("{");
	end = tmp.find(",");
	len = end - (start + 1);
	t = tmp.substr(start+1,len);
	transform(t.begin(), t.end(), t.begin(), ::toupper);
	reg_info->module_name = t;

	// get string from first , to end }
	start = end +  1;
	end = tmp.find("}");
	len = end - start;
	tmp = tmp.substr(start,len+1);

	start = 0; 
	end = tmp.find(",");
	len = end - start;
	t = tmp.substr(start,len);
	transform(t.begin(), t.end(), t.begin(), ::toupper);
	reg_info->reg_name = t;

	// get string from second, to end }
	start = end +  1;
	end = tmp.find("}");
	len = end - start;
	tmp = tmp.substr(start,len+1);

	start = 0; 
	end = tmp.find(",");
	len = end - start;
	t = tmp.substr(start,len);
	transform(t.begin(), t.end(), t.begin(), ::toupper);
	reg_info->addr = t;

	start = end;
	end = tmp.find("}");
	len = end - (start + 1);
	t = tmp.substr(start+1,len);
	if ((t=="LE") || (t=="BE")) {
		transform(t.begin(), t.end(), t.begin(), ::toupper);
		reg_info->endianess = t;
	} else {
		cout<<"\tPlease use specified format to depict Endianess"<<endl;
		return 0;
	}


return 1;
}

int check_command_format(string line, regex b)
{
	if (regex_match(line, b))
		return 1;
	else
		return 0;
}

// Fill reg dump details:
// Read file failed return and display error.
// Read the inpu file line by line.
// Skip the line if first char is # in line.
// Check for valid format of line i.e. REG={Module,Reg,Addr,Endianess}
// Fill the reg_info struct for the same line. 
int fill_reg_dump_details(char *file)
{

	FILE *fp;
	fp = fopen(file, "r");

	if (fp == NULL) {
		cout<<"Error in opening the file: %s => "<< file<<endl;
		return 0;
	} else {
		cout <<"Parsing data from file: "<<file<<endl;
	}

	string module_name, line ="";
	int line_num, num_objs, ret;
	line_num = 0; ret = 0; num_objs = 0;
	regex b("(REG=\\{)(.*)(,)(.*)(,)(.*)(,)(.*)\\}");

	int num = gd.num_of_reg_dumps;

	ifstream infile(file);	

	while(getline(infile,line)) {
		if(line[0] != '#') {
			// remove any space from lines
			char *data = new char[line.size() + 1];
			std::copy(line.begin(), line.end(), data);
			data[line.size()] = '\0'; // don't forget the terminating 0

			ret = fill_string_data(&line, data);
			if (ret != 1) {
				cout<<"\t Some thing is seriously wrong. Aborted.";
				return 0;
			}
			ret = check_command_format(line, b);
			if (ret == 0) {
				cout<<"\t Line number: "<<line_num+1<<" not in specified format. Skipping it."<<endl;
			} else {
				class info_reg_dump reg_info;
				get_reg_info(&reg_info, line);
				int found = 0;
				// check for the module name if it is already parsed for all registers
				for (list<string>::iterator it = module_parsed.begin(); it != module_parsed.end(); ++it) {
					if (*it == reg_info.module_name) {
						found = 1;
					}
				}
				// do the following of the module is not parsed for all registers.
				if (found == 0) {
					if(reg_info.reg_name == "ALL") {
						// Add the module name to the list for  reg_name == ALL
						module_parsed.push_back(reg_info.module_name);
						ret = fetch_module_data(reg_info.module_name, reg_info.endianess, reg_info.addr);
						if (ret != 1) {
							cout<<"\t Some thing is seriously wrong. Aborted.";
							return 0;
						}

					} else {
						ret = fetch_register_data(reg_info.module_name, reg_info.endianess, reg_info.addr, reg_info.reg_name);
						if (ret != 1) {
							cout<<"\t Some thing is seriously wrong. Aborted.";
							return 0;
						}

					}
				}
			}
		}
		line_num++;
	}
	return 1;
}

int parse_input_file() {
	int num_of_dumps, ret;

	/*	
		int ret = get_header_data(input_parameters, in_prm.input_file);
		if (ret == 0){
		return 0;
		}
	 */
	ret = fill_reg_dump_details(in_prm.input_file);
	if (ret == 0){
		return 0;
	}
	if (scripts == 0) {
		if(strcmp(in_prm.interface,"kernel") == 0){
			int ret = get_regs_value();
			if (ret != 1) {
				cout<<"\t Some thing is seriously wrong. Aborted.";
				return 0;
			}
		} else if((strcmp(in_prm.interface,"ccs") == 0) || (strcmp(in_prm.interface,"uboot") == 0)){
			char *file = "temp_memdump";
			int ret = read_and_get_regs_value(file);
			if (ret != 1) {
				cout<<"\t Some thing is seriously wrong. Aborted.";
				return 0;
			}

		} else {
			cout <<"\t Unkown interface given."<<endl;
			return 0;
		}
	}

#ifdef DEBUG
	for (vector<info_reg_dump>::iterator it = vector_reg_info.begin() ; it != vector_reg_info.end(); ++it){
                class info_reg_dump p = *it;
                cout <<p.module_name<<" "<<p.reg_name<<" "<<p.endianess<<" "<<p.addr<<" "<<p.size<<" "<<p.val<<" "<<p.valid_bit<<endl;
        }
#endif
	return 1;
}
