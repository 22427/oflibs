#include "tnvb.h"

namespace ofl
{
std::string TypeNameValueBlock::get_type() const
{
	return m_type;
}

std::string TypeNameValueBlock::get_name() const
{
	return m_name;
}

bool ofl::TypeNameValueBlock::has_block(const std::string &type, const std::string &name)
{
	return (m_blocks.find(type) != m_blocks.end()) &&  (m_blocks[type].find(name) != m_blocks[type].end());
}

bool ofl::TypeNameValueBlock::has_blocks_of_type(const std::string &type)
{
	return (m_blocks.find(type) != m_blocks.end());
}

TypeNameValueBlock *load_TNVB_from_string(char *s, const std::string &btype, const std::string &bname)
{
	TypeNameValueBlock* res = nullptr;
	int state = 0;
	if(!btype.empty())
	{
		res = new TypeNameValueBlock(btype,bname);
		state = 1;
	}

	char* type;
	char* name;
	char* value;

	Tokenizer tkn(s);
	Tokenizer vtkn(nullptr);
	if(state == 0)
	{
		type = tkn.getToken();
		name = tkn.getToken();
		if(name && type)
		{
			res = new TypeNameValueBlock(type,name);
			// skip over till you find the first attribute/subblock
			tkn.skipOverAll(Tokenizer::whitespaces+"{=");
			state++;
		}
		else return  nullptr;
	}

	while((type = tkn.getToken()) != nullptr)
	{
		name = tkn.getToken(Tokenizer::whitespaces+"=");
		tkn.skipWhiteSpaces();
		if(strcmp(type,"int") == 0)
		{

			vtkn.setBase(tkn.getToken(';'));
			auto& arr = res->getArray<int>(name);
			int i;
			while(vtkn.getTokenAs(i,","))
			{
				arr.push_back(i);
			}
		}
		else if(strcmp(type,"uint") == 0)
		{
			vtkn.setBase(tkn.getToken(';'));
			auto& arr = res->getArray<uint>(name);
			uint32_t i;
			while(vtkn.getTokenAs(i,","))
			{
				arr.push_back(i);
			}
		}
		else if(strcmp(type,"float") == 0)
		{
			vtkn.setBase(tkn.getToken(';'));
			auto& arr = res->getArray<float>(name);
			float i;
			while(vtkn.getTokenAs(i,","))
			{
				arr.push_back(i);
			}
		}
		else if(strcmp(type,"vec4") == 0)
		{
			vtkn.setBase(tkn.getToken(';'));
			auto& arr = res->getArray<vec4>(name);
			vec4 i;
			while(vtkn.getTokenAs(i,","))
			{
				arr.push_back(i);
			}
		}
		else if(strcmp(type,"mat4") == 0)
		{
			vtkn.setBase(tkn.getToken(';'));
			auto& arr = res->getArray<mat4>(name);
			mat4 i;
			while(vtkn.getTokenAs(i,","))
			{
				arr.push_back(i);
			}
		}

		else if(strcmp(type,"string") == 0)
		{
			vtkn.setBase(tkn.getToken(';'));
			auto& arr = res->getArray<std::string>(name);
			vtkn.skipOverAll(Tokenizer::whitespaces+",");
			std::string i;
			while(vtkn.getRest())
			{
				if(vtkn.readEscString(i))
					arr.push_back(i);
				else
					break;
				vtkn.skipOverAll(Tokenizer::whitespaces+",;");
			}
		}
		else
		{
			value = tkn.getTokenTillClosing('{','}');
			tkn.skipOverAll(Tokenizer::whitespaces+"};");
			auto& arr = res->getBlocks(type);
			arr[name] = load_TNVB_from_string(value,type,name);
		}
		vtkn.setBase(nullptr);



	}
	tkn.setBase(nullptr);
	return res;
}

}
