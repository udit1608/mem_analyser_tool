#include<common.h>
#include<stdlib.h>

using namespace std;

extern class general_data gd;
extern vector<info_reg_dump> vector_reg_info;


void get_addr(string *addr, string *val, string line)
{
	int start, end, len;
	start = 0;
	//	start = line.find("0x");
	for(end = start; line[end] != 'X',line[end] != 'x'; end++);
	start = end-1;

	// from 0x to space
	for (end = start; line[end] != ' '; end++);
	end--;
	len = end - start + 1;
	// get addr
	*addr = line.substr(start,len);

	// for value emitting spaces
	start = end + 1;
	for (end = start; line[end] == ' '; end++);

	start = end;
	for (end = start; line[end] != '\0'; end++);
	end--;

	len = end - start + 1;
	*val = line.substr(start, len);

}
int read_and_get_regs_value(char * file)
{
	FILE *fp;
	int num = gd.num_of_reg_dumps;
	int ret = 0;

	fp = fopen(file, "r");

	if (fp == NULL) {
		cout<<"\t Error in opening the file: %s => "<< file<<endl;
		return 0;
	} else {

		string line ="";
		ifstream infile(file);
		while(getline(infile,line)) {	
			string addr; string val = "";
			get_addr(&addr, &val, line);				
			transform(addr.begin(), addr.end(), addr.begin(), ::toupper);
			for (int i = 0; i < num; i++){
				if((vector_reg_info[i].valid_bit == 0) && (vector_reg_info[i].addr == addr)) {
					char *value = new char[val.size() + 1];
					std::copy(val.begin(), val.end(), value);
					value[val.size()] = '\0';
					int temp = strtoul(value, NULL, 16);

					vector_reg_info[i].val = temp;
					vector_reg_info[i].valid_bit = 1;
					break;
				}
			}

		}
	}
	return 1;
}
