#include<common.h>
using namespace std;


extern struct input_params in_prm;
extern class general_data gd;

//class info_reg_dump *ptr_reginfo;
extern vector<info_reg_dump> vector_reg_info;

// converts string to number
int stoi(string s, int *gd)
{
	int temp, tmp ;

	temp = 0;
	tmp = 0;
	for(int i = 0; s[i]!= '\0'; i++) {
		tmp = s[i]-'0';
		if (temp == 0) {
			temp = tmp;
		} else {
			temp = (temp*10)+tmp;
		}
	}
	*gd = temp;

	return temp;
}

string NumberToString (int Number )
{
	stringstream ss;
	ss << Number;
	return ss.str();
}

char* string_to_char(string s)
{
	char data[1024];
	for (int i = 0; s[i]!= '\0'; i++)
		data[i] = s[i];

	return data;
}
// fills the integer type variable with field name from given file

int fill_int_data(int *gd, char *field_name, char *file) {

	ifstream infile(file);
	string line, temp = "";
	int found, ret;

	found  = -1;
	while (getline(infile, line)) {
		if(line[0] != '#') {
			found = line.find(field_name);
			if (found != -1) {
				break;
			} else {
				line = "";
			}
		}
	}
	if (found != -1) {
		int start = strlen(field_name);
		for(int i = start; line[i]!= '\0'; i++)
			if(line[i] != 32) {
				temp = temp + line[i];
			}
	} else {
		cout <<"\t Bad format input file. Please use as shown in sample_input_file"<<endl;
		return 0;
	}

	ret = stoi(temp, gd);
	return ret;
}

int fill_string_data(string *gd, char *field_name) {

	string line, temp = "";
	int ret;

	for(int i = 0; field_name[i]!= '\0'; i++) {
			if(field_name[i] != 32) {
				temp = temp + field_name[i];
		}
	}
	*gd = temp;
	return 1;
}
