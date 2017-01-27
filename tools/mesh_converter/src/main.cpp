
#include <cstdio>
#include <fstream>
#include <set>

#include <ofl_all.h>

using namespace ofl;

class Converter
{
	bool calc_normals;
	bool calc_tangents;
	VertexDataOperations::FileFormat target_format;
	std::vector<std::string> input_files;
	std::vector<std::string> output_files;

	void print_help()
	{
		printf("Help text:\n");
		printf("-f <file>\t Adds another input file.\n");
		printf("-o <file>\t Adds another output file.\n");
		printf("\t\t The input files will be assigned to the output files in order\n");
		printf("-cn \t\t Tell the converter to calculate normals.\n");
		printf("-ct \t\t Tell the converter to calculate tangents.\n");
		printf("-of {OBJ,PLY,VD} Specify the output format. If there is no."
			   " output file \n\t\t the input file's name with a new extension will be used.\n");
	}



public:
	Converter()
	{
		calc_normals = calc_tangents = false;
		target_format = VertexDataOperations::FROM_PATH;
	}



	int go(std::vector<std::string> argv)
	{
		if(argv.empty())
		{
			print_help();
			return 0;
		}
		for(uint i = 0 ; i< argv.size(); i++)
		{
			const std::string& str = argv[i];
			if(str == "-h" || str == "--h")
			{
				print_help();
				return 0;
			}
			else if(str == "-f")
			{
				input_files.push_back(argv[++i]);
			}
			else if(str == "-o")
			{
				output_files.push_back(argv[++i]);
			}
			else if(str == "-cn")
				calc_normals = true;
			else if(str == "-ct")
				calc_tangents = true;
			else if(str == "-of")
			{
				std::string of = argv[++i];
				if(of == "obj" || of == "OBJ")
					target_format = VertexDataOperations::OBJ;
				else if(of == "PLY" || of =="ply")
					target_format = VertexDataOperations::PLY;
				else if(of == "VD" || of == "vd")
					target_format = VertexDataOperations::VD;
				else if(of == "OFF" || of == "off")
					target_format = VertexDataOperations::OFF;
				else
				{
					fprintf(stderr,"Unknown output format '%s' \n",of.c_str());
					print_help();
					return 0;
				}

			}

		}

		if(target_format==VertexDataOperations::FROM_PATH && output_files.empty())
		{
			fprintf(stderr,"No output format choose one via -of or by setting an"
						   "output file name.\n");
			print_help();
			return 0;
		}

		for(uint i = 0 ; i< input_files.size();i++)
		{
			ofl::VertexData* vd =VertexDataOperations::read_from_file(input_files[i]);

			if(calc_normals)
				VertexDataOperations::recalculate_normals(vd,ATTRIB_NORMAL);
			if(calc_tangents)
				VertexDataOperations::recalculate_tangents(vd,ATTRIB_TANGENT);
			std::string out_path;
			if(target_format != VertexDataOperations::FROM_PATH)
			{
				std::string ext = "obj";
				if(target_format == VertexDataOperations::PLY)
					ext = "ply";
				else if (target_format == VertexDataOperations::VD)
					ext = "vd";
				else if (target_format == VertexDataOperations::OFF)
					ext = "off";
				out_path  = paths::without_extension(input_files[i])+"."+ext;

			}
			else
			{
				out_path= output_files[i];
			}
			VertexDataOperations::write_to_file(vd,out_path);
			delete vd;
		}
		return 0;
	}
};

int main(int argc, char** argv)
{
	Converter c;
	std::vector<std::string> args;
	for(int i = 0 ; i< argc;i++)
	{
		args.push_back(argv[i]);
	}
	return c.go(args);
}

#define OFL_IMPLEMENTATION
#include <ofl_all.h>
