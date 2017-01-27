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
		type = tkn.get_token();
		name = tkn.get_token();
		if(name && type)
		{
			res = new TypeNameValueBlock(type,name);
			// skip over till you find the first attribute/subblock
			tkn.skip_over_all(Tokenizer::whitespaces+"{=");
			state++;
		}
		else return  nullptr;
	}

	while((type = tkn.get_token()) != nullptr)
	{
		name = tkn.get_token(Tokenizer::whitespaces+"=");
		tkn.skip_white_spaces();
		if(strcmp(type,"int") == 0)
		{

			vtkn.set_base(tkn.get_token(';'));
			auto& arr = res->getArray<int>(name);
			int i;
			while(vtkn.get_token_as(i,","))
			{
				arr.push_back(i);
			}
		}
		else if(strcmp(type,"uint") == 0)
		{
			vtkn.set_base(tkn.get_token(';'));
			auto& arr = res->getArray<uint>(name);
			uint32_t i;
			while(vtkn.get_token_as(i,","))
			{
				arr.push_back(i);
			}
		}
		else if(strcmp(type,"float") == 0)
		{
			vtkn.set_base(tkn.get_token(';'));
			auto& arr = res->getArray<float>(name);
			float i;
			while(vtkn.get_token_as(i,","))
			{
				arr.push_back(i);
			}
		}
		else if(strcmp(type,"vec4") == 0)
		{
			vtkn.set_base(tkn.get_token(';'));
			auto& arr = res->getArray<vec4>(name);
			vec4 i;
			while(vtkn.get_token_as(i,","))
			{
				arr.push_back(i);
			}
		}
		else if(strcmp(type,"mat4") == 0)
		{
			vtkn.set_base(tkn.get_token(';'));
			auto& arr = res->getArray<mat4>(name);
			mat4 i;
			while(vtkn.get_token_as(i,","))
			{
				arr.push_back(i);
			}
		}

		else if(strcmp(type,"string") == 0)
		{
			vtkn.set_base(tkn.get_token(';'));
			auto& arr = res->getArray<std::string>(name);
			vtkn.skip_over_all(Tokenizer::whitespaces+",");
			std::string i;
			while(vtkn.get_rest())
			{
				if(vtkn.read_esc_string(i))
					arr.push_back(i);
				else
					break;
				vtkn.skip_over_all(Tokenizer::whitespaces+",;");
			}
		}
		else
		{
			value = tkn.get_token_till_closing('{','}');
			tkn.skip_over_all(Tokenizer::whitespaces+"};");
			auto& arr = res->getBlocks(type);
			arr[name] = load_TNVB_from_string(value,type,name);
		}
		vtkn.set_base(nullptr);



	}
	tkn.set_base(nullptr);
	return res;
}

}
