
#include <cstdio>
#include <fstream>
#include <set>
#include <vector>
#include <map>
#include "../../libs/libs/stru.h"

class AFile
{

public:
	AFile(std::string file_name,
		  std::string path):file_name(file_name),path(path){printed=false;}

	std::set<AFile*> used_by;
	std::set<AFile*> uses;
	std::string file_name;
	std::string path;
	bool printed;

	void done(AFile* f)
	{
		uses.erase(f);
	}

	void print(const std::string& line_prefix="")
	{
		if(printed)
			return;

		std::string name = paths::filename(file_name);
		std::string extension = paths::extension(file_name);
		std::transform(name.begin(), name.end(),name.begin(),::toupper);
		std::transform(extension.begin(), extension.end(),extension.begin(),::toupper);

		printf("#ifndef USING_OFL_%s_%s\n#define USING_OFL_%s_%s\n",name.c_str(),extension.c_str(),name.c_str(),extension.c_str());

		for(AFile* f : uses)
			f->print();
		std::ifstream f(path);
		std::string l;
		std::string working;

		while(std::getline(f,l))
		{
			working = l;
			ofl::ltrim(working);
			if(working.find("#pragma once") != working.npos)
				continue;
			if( working.find("#include") == working.npos)
			{
				printf("%s%s\n",line_prefix.c_str(),l.c_str());
				continue;
			}


			auto first = working.find_first_of('"');
			auto last = working.find_last_of('"');
			if(first == last)
			{
				printf("%s\n",l.c_str());
			}
		}
		for( AFile* f : used_by)
		{
			f->done(this);
		}


		printf("\n#endif //USING_OFL_%s_%s\n",name.c_str(),extension.c_str());


		printed = true;

	}

	bool exists ()
	{
		std::ifstream f(path.c_str());
		return f.good();
	}
	bool analyze(std::map<std::string,AFile*>& files)
	{

		if(!exists())
			return false;
		std::string dir = path.substr(0,this->path.find_last_of('/')+1);

		if(files.find(file_name) != files.end())
			return false;

		files[file_name] = this;

		std::ifstream f(path);
		std::string l;
		std::string working;

		while(std::getline(f,l))
		{
			working = l;
			ofl::ltrim(working);
			if(working[0]!='#')
			{
				continue;
			}
			auto first = working.find_first_of('"');
			auto last = working.find_last_of('"');
			if(first != last)
			{
				std::string incl = working.substr(first+1,last-first-1);
				std::string mod = paths::without_extension(incl);
				std::string name = paths::filename(incl);
				AFile* h = new AFile(name+".h",dir+mod+".h");
				AFile* cpp = new AFile(name+".cpp",dir+mod+".cpp");
				AFile* txt = new AFile(name+".md",dir+mod+".md");
				if(!h->analyze(files))
				{
					if(h->exists())
					{
						AFile* real = files[h->file_name];
						real->used_by.insert(this);
						this->uses.insert(files[h->file_name]);
					}
					delete h;
				}
				else
				{
					h->used_by.insert(this);
					this->uses.insert(h);
				}
				if(!cpp->analyze(files)) delete cpp;
				if(!txt->analyze(files)) delete txt;
			}
		}
		return true;
	}



};



int main(int argc, char** argv)
{
	std::string header_name;
	std::string mod;


	std::map<std::string,AFile*> files;
	for(int i =1 ; i< argc;i++)
	{
		header_name = argv[i];
		mod = paths::without_extension(header_name);
		std::string name = paths::filename(header_name);
		AFile* f = new AFile(name+".h",mod+".h");
		if(!f->analyze(files))
			delete f;
		f = new AFile(name+".cpp",mod+".cpp");
		if(!f->analyze(files))
			delete f;
		f = new AFile(name+".md",mod+".md");
		if(!f->analyze(files))
			delete f;

	}

	std::vector<AFile*> headers;
	std::vector<AFile*> src;
	std::vector<AFile*> docs;

	for(const std::pair<std::string,AFile*>& f: files)
	{
		if(paths::extension(f.first) == "h")
			headers.push_back(f.second);
		if(paths::extension(f.first) == "cpp")
			src.push_back(f.second);
		if(paths::extension(f.first) == "md")
			docs.push_back(f.second);

	}
	
	AFile license("LICENSE.md","../LICENSE.md");
	AFile readme("README.md","../README.md");
	
	
	printf("#if 0\n");
	printf("<begin_doc>\n");
	license.print("//");
	printf("\n--------------------------------------------------------------------------------\n\n");
	readme.print("//");
	printf("\n--------------------------------------------------------------------------------\n\n");
	
	for( AFile *f : docs)
	{
		f->print("//");
		printf("\n--------------------------------------------------------------------------------\n\n");
	}
	
	printf("<end_doc>\n#endif \n");

	for( AFile *f : headers)
	{
		std::string name = paths::filename(f->file_name);
		std::transform(name.begin(), name.end(),name.begin(),::toupper);
	//	printf("#ifndef USING_OFL_%s_H\n #define USING_OFL_%s_H\n ",name.c_str(),name.c_str());
		f->print();
	//	printf("\n#endif //USING_OFL_%s_H\n",name.c_str());
	}


	printf("#ifdef OFL_IMPLEMENTATION\n");
	for(AFile*f :src)
	{
		f->print();
	}
	printf("#endif\n");

	for(auto& f : files)
		delete f.second;
	return 0;
}
