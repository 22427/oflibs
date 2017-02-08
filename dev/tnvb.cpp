#include "tnvb.h"

namespace ofl
{


TNVB::TNVB(const std::string &type, const std::string &name)
	:m_type(type), m_name(name)
{
}

std::map<std::string, TNVB *> &TNVB::get_blocks(const std::string &type)
{
	return m_blocks[type];
}

TNVB *&TNVB::get_block(const std::string &type, const std::string &name)
{
	if(m_blocks[type].find(name) == m_blocks[type].end())
		m_blocks[type][name] = new TNVB(type,name);
	return m_blocks[type][name];
}

const std::map<std::string, std::vector<int32_t> > &TNVB::get_ints() const {return m_ints;}

const std::map<std::string, std::vector<uint32_t> > &TNVB::get_uints() const {return m_uints;}

const std::map<std::string, std::vector<float> > &TNVB::get_floats() const {return m_floats;}

const std::map<std::string, std::vector<double> > &TNVB::get_doubles() const {return m_doubles;}

const std::map<std::string, std::vector<vec2> > &TNVB::get_vec2s() const {return m_vec2s;}

const std::map<std::string, std::vector<vec3> > &TNVB::get_vec3s() const {return m_vec3s;}

const std::map<std::string, std::vector<vec4> > &TNVB::get_vec4s() const {return m_vec4s;}

const std::map<std::string, std::vector<mat4> > &TNVB::get_mat4s() const {return m_mat4s;}

const std::map<std::string, std::vector<mat3> > &TNVB::get_mat3s() const {return m_mat3s;}

const std::map<std::string, std::vector<std::string> > &TNVB::get_strings() const {return m_strings;}

const std::map<std::string, std::map<std::string, TNVB *> > &TNVB::get_blocks() const {return m_blocks;}


const std::string& TNVB::type() const
{
	return m_type;
}

const std::string& TNVB::name() const
{
	return m_name;
}


std::string& TNVB::type()
{
	return m_type;
}

std::string& TNVB::name()
{
	return m_name;
}

bool TNVB::has_block(const std::string &type, const std::string &name)
{
	return (m_blocks.find(type) != m_blocks.end()) &&  (m_blocks[type].find(name) != m_blocks[type].end());
}

bool TNVB::has_blocks_of_type(const std::string &type)
{
	return (m_blocks.find(type) != m_blocks.end());
}


std::string read_type(const char** code)
{
	std::string type;
	const char*r = skip_ws(*code);
	while(*r && *r!='#' && !isspace(*r))
	{
		type += *r;
		r++;
	}
	*code = r;
	return type;
}

std::string read_name(const char** code)
{
	std::string name;
	const char*r = skip_ws(*code);
	while(*r && *r!='#' && !isspace(*r) && *r!='=')
	{
		name += *r;
		r++;
	}
	// skip over '=' or ' ='
	r=skip_ws(r);
	r++;

	*code = r;
	return name;
}


std::string read_values(const char** code)
{
	std::string value;
	const char*r = skip_ws(*code);
	while(*r)
	{
		if(*r==';')
		{
			r++;
			break;
		}
		if(*r == '\\')
			r++;
		value += *r;
		r++;
	}
	*code = r;
	return value;
}
std::string read_block(const char** code)
{
	std::string block;

	const char*r = skip_ws(*code);
	if(*r!='{')
		return "";
	r++;
	int count = 1;
	bool in_string = false;
	while(*r && count>0)
	{
		if(*r == '\\')
		{
			block += *r;
			r++;
		}
		else if(*r == '\"')
			in_string = !in_string;
		else if(*r == '{' && !in_string)
			count++;
		else if(*r == '}' && !in_string)
			count--;
		if(count==0)
		{
			r++; //skip the } that made the count go to zero
			r=skip_ws(r); // skip anything between } and ;
			r++; // skip ;
			break;
		}

		block+=*r;
		r++;


	}
	*code = r;
	return block;
}

std::string read_value(const char** code)
{
	const char*r = skip_ws(*code);
	if(*r=='{')
		return read_block(code);
	else
		return read_values(code);
}


TNVB * TNVBOperations::read_from_string(const char *code, const char **end, const std::string &type, const std::string &name)
{
	TNVB* res;

	if(type.empty() || name.empty())
	{
		res = new TNVB(read_type(&code),read_name(&code));
		code = skip_ws(code);
		if(*code !='{')
			return nullptr;
		code++;
	}
	else
		res = new TNVB(type,name);

	std::string t;
	std::string n;
	std::string v;

	while(*code != '}')
	{
		t = read_type(&code);
		n = read_name(&code);
		v = read_value(&code);

		if(t == "int")
		{
			res->get_array<int>(n) = read_vector_from_string<int>(v.c_str());
		}
		else if(t == "uint")
		{
			res->get_array<uint>(n) = read_vector_from_string<uint>(v.c_str());
		}
		else if(t == "float")
		{
			res->get_array<float>(n) = read_vector_from_string<float>(v.c_str());
		}
		else if(t == "double")
		{
			res->get_array<double>(n) = read_vector_from_string<double>(v.c_str());
		}
		else if(t == "vec2")
		{
			res->get_array<vec2>(n) = read_vector_from_string<vec2>(v.c_str());
		}
		else if(t == "vec3")
		{
			res->get_array<vec3>(n) = read_vector_from_string<vec3>(v.c_str());
		}
		else if(t == "vec4")
		{
			res->get_array<vec4>(n) = read_vector_from_string<vec4>(v.c_str());
		}
		else if(t == "mat3")
		{
			res->get_array<mat3>(n) = read_vector_from_string<mat3>(v.c_str());
		}
		else if(t == "mat4")
		{
			res->get_array<mat4>(n) = read_vector_from_string<mat4>(v.c_str());
		}
		else if(t == "string")
		{
			res->get_array<std::string>(n) = read_vector_from_string<std::string>(v.c_str());
		}
		else
		{
			res->get_block(t,n) = read_from_string(v.c_str(),nullptr,t,n);
		}
		code = skip_ws(code);
	}
	code++; // skip }
	code = skip_ws(code); // skip anything between } and ;
	code++; // skip ;
	if(end)
		*end =  code;
	return res;
}

std::string TNVBOperations::write_to_string(const TNVB &v)
{
	std::string res = v.type()+" "+v.name()+" =\n{\n";
	for(const auto& i : v.get_ints())
		res+="int "+i.first+" = "+ofl::write_to_string(i.second)+";\n";

	for(const auto& i : v.get_uints())
		res+="uint "+i.first+" = "+ofl::write_to_string(i.second)+";\n";

	for(const auto& i : v.get_floats())
		res+="float "+i.first+" = "+ofl::write_to_string(i.second)+";\n";

	for(const auto& i : v.get_doubles())
		res+="double "+i.first+" = "+ofl::write_to_string(i.second)+";\n";

	for(const auto& i : v.get_vec2s())
		res+="vec2 "+i.first+" = "+ofl::write_to_string(i.second)+";\n";

	for(const auto& i : v.get_vec3s())
		res+="vec3 "+i.first+" = "+ofl::write_to_string(i.second)+";\n";

	for(const auto& i : v.get_vec4s())
		res+="vec4 "+i.first+" = "+ofl::write_to_string(i.second)+";\n";

	for(const auto& i : v.get_mat3s())
		res+="mat3 "+i.first+" = "+ofl::write_to_string(i.second)+";\n";

	for(const auto& i : v.get_mat4s())
		res+="mat4 "+i.first+" = "+ofl::write_to_string(i.second)+";\n";

	for(const auto& i : v.get_strings())
		res+="string "+i.first+" = "+ofl::write_to_string(i.second)+";\n";

	for(const auto& i : v.get_blocks())
		for(const auto& b : i.second)
		{
			res+= write_to_string(*(b.second))+"\n";
		}

	res+="};";
	return res;
}

TNVB *TNVBOperations::read_from_file(const std::string &path)
{
	char * buffer = nullptr;
	long length;

	FILE * f = fopen (path.c_str(), "r");
	if(!f)
		return nullptr;

	fseek (f, 0, SEEK_END);
	length = ftell (f);
	fseek (f, 0, SEEK_SET);
	buffer = static_cast<char*>(malloc (length));

	if (buffer)
	{
		fread (buffer, 1, length, f);
	}
	fclose (f);

	if (!buffer)
	{
		return nullptr;
	}

	const char* end;

	auto res = read_from_string(buffer,&end);
	free(buffer);
	return res;
}

bool TNVBOperations::write_to_file(const TNVB *tnvb, const std::string &path)
{
	FILE * f = fopen (path.c_str(), "w");
	if(!f)
		return false;
	auto str = write_to_string(*tnvb);
	fwrite(str.c_str(),1,str.size(),f);
	fclose(f);
	return true;
}

}
