

#include <../../libs/libs/mesh.h>

#include <cstdio>
#include <fstream>
#include <set>
#include <../../libs/libs/vd.h>



using namespace ofl;


int main(int argc, char** argv)
{
	printf("loading a ...\n");
	VertexData* a = VertexDataTools::readFromFile("a.obj");
	VertexDataTools::writeToFile(a,"ico2.off");
	printf("loading b ...\n");
	VertexData* b = VertexDataTools::readFromFile("b.obj");
	printf("loading c ...\n");
	VertexData* c = VertexDataTools::readFromFile("c.obj");

	printf("create meshes...\n");
	std::vector<Mesh*> ms= {new Mesh(a),new Mesh(b),new Mesh(c)};


	MeshTools::averageSurfaces(ms);



	return 0;

}

