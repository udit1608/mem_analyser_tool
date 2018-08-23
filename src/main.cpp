#include <iostream>
#include <data.h>
#include <string.h>
#include <utilities.h>
#include<common.h>

using namespace std;


struct input_params in_prm;
struct input_params *ip = &in_prm;;
class general_data gd;
vector<info_reg_dump> vector_reg_info;
int scripts;
void convert_to_lower(char *p) {

	for (int i = 0; p[i]!= '\0'; i++)
		p[i] = tolower(p[i]);
}
int main(int argc, char *argv[])
{

	int ret  = 1;
	scripts = 0;

	cout<<"#########Start#######"<<endl<<endl;

	if (argc < 5) {
		cout<< "Invalid command to create helpful debugger scripts."<<endl;
		cout<< "\t Usage: ./dump_analyze -i <input_file_name> -c <chain_position>"<<endl;
		cout<< "\t\t -c Debugger chain position."<<endl;
		ret = 0; 

	} else if ((argc == 6 || argc > 7) && (argc != 5)) {
		cout<< "Invalid number of options"<<endl;
		cout<< "\t Usage: ./dump_analyze -i <INPUT_FILE_NAME> -m <ccs/kernel> -o <txt/xml>"<<endl;
		cout<< "\t\t -i Input file name"<<endl;
		cout<< "\t\t -m Mode in which to operate <ccs/kernel>"<<endl;
		cout<< "\t\t -o Output form: display/xml"<<endl;
		ret = 0; 
	}

	if (argc == 5) {
		scripts = 1;
	} else if (argc == 7) {
		scripts = 0;
	}

	if (scripts == 0) {
		for (int count=1; count < argc; ++count) {
			if(strcmp(argv[count], "-i") == 0) {
				in_prm.input_file = argv[count+1];
				count++;
			} else if(strcmp(argv[count], "-m") == 0) {
				in_prm.interface = argv[count+1];
				count++;
			} else if(strcmp(argv[count], "-o") == 0) {
				in_prm.out_type = argv[count+1];
				count++;
			} else {
				cout<< "Incorrect usage of command"<<endl;
				cout<< "\t Usage: ./dump_analyze -i <INPUT_FILE_NAME> -m <ccs/kernel> -o <txt/xml>"<<endl;
				cout<< "\t\t -i Input file name"<<endl;
				cout<< "\t\t -m Mode in which to operate <ccs/kernel>"<<endl;
				cout<< "\t\t -o Output form: display/xml"<<endl;

				ret =  0;
			}
		}
		convert_to_lower(in_prm.interface);
		convert_to_lower(in_prm.out_type);
	} else if(scripts == 1) {
		for (int count=1; count < argc; ++count) {
			if(strcmp(argv[count], "-c") == 0) {
				char *ccs_pos = argv[count+1];
				in_prm.ccs_chain_position = strtoul(ccs_pos, NULL, 0);
				count++;
			} else  if(strcmp(argv[count], "-i") == 0) {
				in_prm.input_file = argv[count+1];
				count++;
			} else {
				cout<<"Incorrect command format. Usage: ./dump_analyze -i <input_file_name> -c <chain_position>"<<endl;
				ret = 0;
			}
		}
	}

	if (ret == 0) {
		cout<<endl<<"#########Exit#######"<<endl;
		return 1; 
	}


	//	char fn[] = "input_file";
	//	in_prm.input_file = fn;
	//	char interface[] = "ccs";
	//	in_prm.interface = interface;

	char output_file[] = "memtool_dump.xml";
	in_prm.output_file = output_file;


	ret = parse_input_file();
	if (ret == 0) {
		cout <<"\t Unable to parse the input file."<<endl;
	}
	if (scripts == 0) {
		ret = create_output_xml_file();
		if (ret == 0) {
			cout <<"\t Unable to create output."<<endl;
		}
	} else {
		ret = create_helpful_scripts();
		if (ret == 0) {
			cout <<"\t Unable to create scripts."<<endl;
		}
	}

	cout<<endl<< "#########Done#######"<<endl;
	return 0;
}
