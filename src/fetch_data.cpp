#include <common.h>
#include<bits/stdc++.h>
#include<tinyxml.h>
#include<tinystr.h>

using namespace std;

#define TIXML_USE_STL

extern struct input_params in_prm;
extern class general_data gd;
extern vector<info_reg_dump> vector_reg_info;

// print all attributes of pElement.
// returns the number of attributes printed
int dump_attribs_to_stdout(TiXmlElement* pElement)
{
	if ( !pElement ) return 0;

	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	while (pAttrib)
	{
		cout<<"Name:"<< pAttrib->Name()<<" Value:"<<pAttrib->Value();
		cout<<endl;
		pAttrib=pAttrib->Next();
	}
	return 1;
}


// to check the module name in XML and input file
int check_for_correct_module_name(TiXmlElement* child, char *module)
{
	const char *name = child->Value();
	if(strcmp(name,module)!=0)
		return 0;
	return 1;

}

int check_for_endianess(TiXmlElement *child, string temp)
{
	char *module_endianess = new char[temp.size() + 1];
	std::copy(temp.begin(), temp.end(), module_endianess);
	module_endianess[temp.size()] = '\0';

	TiXmlAttribute* pAttrib = child->FirstAttribute();
	while (pAttrib)
	{
		if(strcmp( pAttrib->Name(),"endianess") == 0){
			const char *end = pAttrib->Value();
			if(strcmp(end, module_endianess)!=0)
				cout<<"\t Warning!! Endianess in input file differs from default endianess for module "<<child->Value()<<endl;
			break;
		}
		pAttrib=pAttrib->Next();
	}
	return 1;
}

void parse_node_and_fill_data(TiXmlElement *child, class info_reg_dump *reg)
{
	reg->reg_name = child->Value();
	TiXmlAttribute* pAttrib = child->FirstAttribute();
	while (pAttrib)
	{
		if(strcmp( pAttrib->Name(),"size") == 0){
			const char *size = pAttrib->Value();
			int sz; 
			stoi(size, &sz);
			reg->size = sz;
		}
		if(strcmp( pAttrib->Name(),"offset") == 0){
			reg->offset = pAttrib->Value();
		}
		pAttrib=pAttrib->Next();
	}

}
// Check the endianess of the module with its endianess attribute
// Also return the pointer to child with the asked register info

TiXmlElement* get_node_with_reg_name(TiXmlElement *child, char *reg_name, char *endianess)
{

	TiXmlAttribute* pAttrib = child->FirstAttribute();
	while (pAttrib)
	{
		if(strcmp( pAttrib->Name(),"endianess") == 0){
			const char *end = pAttrib->Value();
			if(strcmp(end, endianess)!=0)
				cout<<"\tWarning!! Endianess in input file differs from default endianess"<<endl;
			break;
		}
		pAttrib=pAttrib->Next();
	}
	child = child->FirstChildElement();

	while(child) {
		if(strcmp(child->Value(),reg_name) == 0) {
			break;
		}
		child = child->NextSiblingElement();
	}
	return child;
}

int get_reg_size(TiXmlElement *ptr, char *name)
{
	TiXmlAttribute* pAttrib = ptr->FirstAttribute();
	while (pAttrib)
	{
		cout<<"Name:"<< pAttrib->Name()<<" Value:"<<pAttrib->Value();
		cout<<endl;
		pAttrib=pAttrib->Next();
	}

}
int fetch_module_data(string s, string endianess, string addr) {

	TiXmlNode* node = 0;
	TiXmlElement* child = 0;
	TiXmlElement* sibling = 0;
	TiXmlElement* ptr_node = 0;
	int size;

	string mod_nm = s;

	transform(s.begin(), s.end(), s.begin(), ::tolower);
	s = "../XML_DOCS/" + s + ".xml";

	// Make proper file name in char * format based on module name
	char *file = new char[s.size() + 1];
	std::copy(s.begin(), s.end(), file);
	file[s.size()] = '\0';

	// Convert string to char * type module name
	char *module = new char[mod_nm.size() + 1];
	std::copy(mod_nm.begin(), mod_nm.end(),module);
	module[mod_nm.size()] = '\0';

	// Load the XML file in tiny XML object
	TiXmlDocument doc( file );
	bool loadOkay = doc.LoadFile();
	int ret;

	// To Module 
	node = doc.FirstChildElement( module );
	if(! node ) {
		cout<<"\t No XML file in database for module "<<module<<endl;
		return 0;
	}
	child = node->ToElement();

	ret = check_for_correct_module_name(child, module);
	if(ret == 0 ) {
		cout<<"Module name in input file and XML donot match."<<endl;
		return 0;
	}
	ret = check_for_endianess(child, endianess);

	child = child->FirstChildElement();
	int count = 0;
	while(child) {
		class info_reg_dump reg_dump;
		reg_dump.module_name = mod_nm;
		reg_dump.endianess = endianess;
		// Parse XMl and fill reg_name and size field
		parse_node_and_fill_data(child, &reg_dump);
		child = child->NextSiblingElement();

		if (count == 0) {
			reg_dump.addr = addr;
		} else {
			int e = addr.length();
			string offset = reg_dump.offset;
			int offset_end = offset.length() - 1 ;// mius 1 done to remove h from end of offset
			while(offset_end) {
				addr[e-1] = reg_dump.offset[offset_end-1];
				e--; offset_end--;
			}
			reg_dump.addr = addr;
		}
		count++;
#ifdef DEBUG
		cout<<reg_dump.module_name<<" "<<reg_dump.reg_name<<" "<<reg_dump.endianess<<" "<<reg_dump.size<<" "<<reg_dump.addr<<" "<<reg_dump.offset;
		cout<<endl;
#endif
		vector_reg_info.push_back(reg_dump);		
		gd.num_of_reg_dumps++;
	}
#ifdef DEBUG
	for (vector<info_reg_dump>::iterator it = vector_reg_info.begin() ; it != vector_reg_info.end(); ++it){
		class info_reg_dump p = *it;
		cout<<p.reg_name<<" "<<p.endianess<<" "<<p.size<<" "<<p.addr<<" "<<p.offset;
	}
#endif
	return 1;
}

int fetch_register_data(string s, string endianess, string addr, string reg_name) {
	TiXmlNode* node = 0;
	TiXmlElement* child = 0;
	TiXmlElement* sibling = 0;
	TiXmlElement* ptr_node = 0;
	int size;

	string mod_nm = s;

	transform(s.begin(), s.end(), s.begin(), ::tolower);
	s = "../XML_DOCS/" + s + ".xml";

	// Make proper file name in char * format based on module name
	char *file = new char[s.size() + 1];
	std::copy(s.begin(), s.end(), file);
	file[s.size()] = '\0';

	// Convert string to char * type module name
	char *module = new char[mod_nm.size() + 1];
	std::copy(mod_nm.begin(), mod_nm.end(),module);
	module[mod_nm.size()] = '\0';

	// Load the XML file in tiny XML object
	TiXmlDocument doc( file );
	bool loadOkay = doc.LoadFile();
	int ret;

	// To Module 
	node = doc.FirstChildElement( module );
	if(! node ) {
		cout<<"\t No XML file in database for module "<<module<<endl;
		return 0;
	}
	child = node->ToElement();

	ret = check_for_correct_module_name(child, module);
	if(ret == 0 ) {
		cout<<"Module name in input file and XML donot match."<<endl;
		return 0;
	}
	ret = check_for_endianess(child, endianess);

	child = child->FirstChildElement();
	int count = 0;
	class info_reg_dump reg_dump;
	while(child && (count != 1)) {
		reg_dump.module_name = mod_nm;
		reg_dump.endianess = endianess;

		if (child->Value() == reg_name) {
			// Parse XMl and fill reg_name and size field
			parse_node_and_fill_data(child, &reg_dump);
			reg_dump.addr = addr;
			vector_reg_info.push_back(reg_dump);
			gd.num_of_reg_dumps++;
			count = 1;
		}
		child = child->NextSiblingElement();
	}

	if(count == 0) {
		cout<<"\t Register with name "<<reg_name<<" not found"<<endl;
	}
#ifdef DEBUG
		for (vector<info_reg_dump>::iterator it = vector_reg_info.begin() ; it != vector_reg_info.end(); ++it){
			class info_reg_dump p = *it;
			cout<<p.reg_name<<" "<<p.endianess<<" "<<p.size<<" "<<p.addr<<" "<<p.offset;
		}
#endif
		return 1;
}
