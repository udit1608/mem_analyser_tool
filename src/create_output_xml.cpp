#include<common.h>
#include<bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#include<tinyxml.h>
#include<tinystr.h>

using namespace std;


extern struct input_params in_prm;
extern class general_data gd;
extern vector<info_reg_dump> vector_reg_info;
enum display_mode {
	display = 0,
	xml = 1
};
enum display_mode mode;	

int get_size_of_register(TiXmlElement* ptr, string *size)
{
	TiXmlAttribute* pAttrib = ptr->FirstAttribute();
	while (pAttrib)
	{
		if(strcmp(pAttrib->Name(), "size")==0) {
			*size =  pAttrib->Value();
			break;
			pAttrib=pAttrib->Next();
		}
		return 1;
	}
}

int bits_to_description_mapping(TiXmlElement* child, char *bits, string endianess, int size, int entry_num, char *o_file)
{
	TiXmlDocument doc( o_file );
	bool loadOkay = doc.LoadFile();
	if ( !loadOkay )
	{
		cout<< "\t Could not load output file "<<o_file<<endl;
		exit( 1 );
	}
	TiXmlNode* temp_node = 0;
	TiXmlElement* dump_element = 0;
	TiXmlElement* mod_element = 0;

	temp_node = doc.FirstChild("DUMP");
	dump_element = temp_node->ToElement();
	// Point to first child in register i.e. to first register field
	child = child->FirstChildElement();
	TiXmlNode* node;
	TiXmlElement *next_child;
	TiXmlAttribute* pAttrib;
	TiXmlText* text;
	int start, width;
	string desc, type;
	char *tmp;
	start = 0; width = 0;


	string mod_nm = vector_reg_info[entry_num].module_name;
	string reg_nm = vector_reg_info[entry_num].reg_name; 

	tmp = new char[mod_nm.size() + 1];
	std::copy(mod_nm.begin(), mod_nm.end(), tmp);
	tmp[mod_nm.size()] = '\0';

	TiXmlElement mod(tmp);

	string endness = vector_reg_info[entry_num].endianess;

	tmp = new char[endness.size() + 1];
	std::copy(endness.begin(), endness.end(), tmp);
	tmp[endness.size()] = '\0';

	mod.SetAttribute("endianess", tmp);

	tmp = new char[reg_nm.size() + 1];
	std::copy(reg_nm.begin(), reg_nm.end(), tmp);
	tmp[reg_nm.size()] = '\0';

	TiXmlElement reg(tmp);


	reg.SetAttribute("size", size);

	string off = vector_reg_info[entry_num].offset;
	tmp = new char[off.size() + 1];
	std::copy(off.begin(), off.end(), tmp);
	tmp[off.size()] = '\0';

	reg.SetAttribute("offset", tmp);

	if(mode == display) {
		cout<<"\t Module Name: "<<mod_nm<<endl;

	}
	while(child) {
		// Point to attributes of child
		pAttrib = child->FirstAttribute();
		// For attributes start, width, description, type fetch them
		while(pAttrib)
		{
			if(strcmp(pAttrib->Name(), "start")==0) {
				string val =  pAttrib->Value();
				char *temp = new char[val.size() + 1];
				std::copy(val.begin(), val.end(), temp);
				temp[val.size()] = '\0';

				start = strtoul(temp, NULL, 0);
			} else if(strcmp(pAttrib->Name(), "width")==0) {
				string val =  pAttrib->Value();
				char *temp = new char[val.size() + 1];
				std::copy(val.begin(), val.end(), temp);
				temp[val.size()] = '\0'; 

				width = strtoul(temp, NULL, 0);
			} else if(strcmp(pAttrib->Name(), "type")==0) {
				type =  pAttrib->Value();
			} else if(strcmp(pAttrib->Name(), "description")==0) {
				desc = pAttrib->Value();
			}
			pAttrib=pAttrib->Next();
		}

		string bit_value = "";
		// form string value from start of width size
		if(endianess == "LE") {
			for (int i = start; i < start + width; i++)
				bit_value += bits[i];
		} else if(endianess == "BE") {
			int end_offset = size - 1;
			for (int i = end_offset - start ; i > end_offset - (start + width); i--)
				bit_value += bits[i];

		}
		TiXmlElement field(child->Value());
		tmp = new char[desc.size() + 1];
		std::copy(desc.begin(), desc.end(), tmp);
		tmp[desc.size()] = '\0';
		field.SetAttribute("description", tmp);

		tmp = new char[bit_value.size() + 1];
		std::copy(bit_value.begin(), bit_value.end(), tmp);
		tmp[bit_value.size()] = '\0';
		field.SetAttribute("Value", tmp);
		if(mode == display) {
			cout<<"\t\t "<<reg_nm;
			cout<<"["<<child->Value()<<"] : "<<bit_value<<endl;
			cout<<"\t\t\t Description: "<<desc<<endl;
		}
		bit_value = 'V' + bit_value;
		if(child->FirstChildElement() != NULL){
			next_child = child->FirstChildElement();
			while(next_child) {

				if(next_child->Value() == bit_value) {
					text = next_child->FirstChild()->ToText();
					break;

				} else if (strcmp(next_child->Value(), "XX") == 0) {
					text = next_child->FirstChild()->ToText();
					break;
				}
				next_child = next_child->NextSiblingElement();
			}
			tmp = new char[bit_value.size() + 1];
			std::copy(bit_value.begin(), bit_value.end(), tmp);
			tmp[bit_value.size()] = '\0';

			TiXmlElement bits(tmp);
			TiXmlText txt(text->Value());
			bits.InsertEndChild( txt );
			field.InsertEndChild(bits);
			if(mode == display) {
				cout<<"\t\t\t Inference  : "<<text->Value()<<endl;
			}
		}
		if(mode == display) {
			cout<<endl;
		}
		reg.InsertEndChild(field);
		child = child->NextSiblingElement();

	}

	//mod.InsertEndChild(reg);
	mod.InsertEndChild(reg);
	dump_element->InsertEndChild(mod);
	doc.SaveFile(o_file);
}
int create_output_xml_file(void)
{

	char *file = in_prm.output_file;
	char *in_file = in_prm.input_file;
	int num = gd.num_of_reg_dumps;
	TiXmlNode* node = 0;
	TiXmlElement* child = 0;
	TiXmlElement* sibling = 0;
	TiXmlElement* ptr_node = 0;
	int flag = 0;

	ofstream op_file (file);

	if (op_file.is_open())
	{
		// Write tarting header for XML file
		op_file << "<?xml version=\"1.0\" ?>\n<DUMP>\n</DUMP>";
		op_file.close();
	} else {
		cout<<"Error in opening the file: %s => "<< file<<endl;
		return 0;
	}


	if(strcmp(in_prm.out_type, "xml") == 0) {
		mode = xml;	
	} else if (strcmp(in_prm.out_type, "display") == 0) {
		mode = display;
	} else {
		cout<<"\t Output type must be xml or display only."<<endl;
		return 0;
	}

	for (int i = 0; i < num; i++) {
		if(vector_reg_info[i].valid_bit == 0) {
			cout<<"\t Unable to create dump for "<<vector_reg_info[i].module_name<<" register "<<vector_reg_info[i].reg_name<<endl;
		} else {
			flag = 1;
			string s = vector_reg_info[i].module_name;
			string mod_nm = s;

			transform(s.begin(), s.end(), s.begin(), ::tolower);
			s = "../XML_DOCS/" + s + ".xml";
			// Make proper file name in char * format based on module name
			char *f = new char[s.size() + 1];
			std::copy(s.begin(), s.end(), f);
			f[s.size()] = '\0';

			// Convert string to char * type module name
			char *module = new char[mod_nm.size() + 1];
			std::copy(mod_nm.begin(), mod_nm.end(),module);
			module[mod_nm.size()] = '\0';

			TiXmlDocument doc( f );
			bool loadOkay = doc.LoadFile();
			node = doc.FirstChildElement( module );
			child = node->ToElement();
			child = child->FirstChildElement();

			int found = 0;
			while(child && (found == 0)) {
				if(vector_reg_info[i].reg_name == child->Value()) {
					// found = 1 if the reg_name is same for the child node
					found = 1;
					string temp_size; int reg_size;
					// get the size attribute value for child
					get_size_of_register(child, &temp_size);

					char *sz = new char[temp_size.size() + 1];
					std::copy(temp_size.begin(), temp_size.end(), sz);
					sz[temp_size.size()] = '\0';

					reg_size = strtoul(sz, NULL, 0);
					//Change from 32 bit to 64 bit
					int value = vector_reg_info[i].val;
					// Store the value in bit format	
					char bits[reg_size];
					for (int j = 0; j < reg_size; j++) {
						bits[j] = (char)(((value >> j) & 0x1) + 48);
					}

					bits_to_description_mapping(child, bits, vector_reg_info[i].endianess, reg_size, i, file);
					if(mode == display) {
						cout<<endl;
					}
				}
				child = child->NextSiblingElement();
			}

		}

	}
	if(mode == xml && flag != 0) {
		cout<<"\t Created output XML file: "<<file;
	}
	return 1;
}
