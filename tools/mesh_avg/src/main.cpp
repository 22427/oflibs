

#include <../../libs/libs/mesh.h>

#include <cstdio>
#include <fstream>
#include <set>
#include <../../libs/libs/vd.h>



using namespace ofl;


int main(int argc, char** argv)
{
	printf("loading a ...\n");
	VertexData* a = VertexDataTools::readFromFile("a.off");
	VertexDataTools::writeToFile(a,"ico2.off");
	printf("loading b ...\n");
	VertexData* b = VertexDataTools::readFromFile("b.off");
	printf("loading c ...\n");
	VertexData* c = VertexDataTools::readFromFile("c.off");

	printf("create meshes...\n");
	Mesh ms[3] = {Mesh(a),Mesh(b),Mesh(c)};
	delete a;
	delete b;
	delete c;
	printf("calculate positions ... \n");

	for(uint j = 0 ; j< 1;j++)
	{
		uint t = ms[j].positions().size()/79;
		for(uint i = 0 ; i< ms[j].positions().size(); i++)
		{
			vec3& p  = ms[j].positions()[i];
			vec3 cp;
			if(i%t==0)
			{
				printf("|");
				fflush(stdout);
			}
			cp += MeshTools::getClosestPoint(&ms[(j+1)%3],p)*0.5f;
			cp += MeshTools::getClosestPoint(&ms[(j+2)%3],p)*0.5f;
			p=p+ (cp-p)*0.5f;
		}
		printf("\n");
		VertexData* vd = ms[j].toVertexData();
		VertexDataTools::writeToFile(vd,std::to_string(j)+".off");
		delete vd;
	}



	return 0;

}

