
#include <cstdio>
#include <fstream>
#include <set>
#include <../../libs/libs/vd.h>
#include <../../libs/libs/mesh.h>


using namespace ofl;


int main(int argc, char** argv)
{
	VertexData* vd = VertexDataTools::readFromFile("test.off");
//	Vertex v;
//	v.setPosition(vec3(0,0,0));
//	vd.data().push_back(v);
//	v.setPosition(vec3(1,1,0));
//	vd.data().push_back(v);
//	v.setPosition(vec3(-1,1,0));
//	vd.data().push_back(v);
//	v.setPosition(vec3(-1,-1,0));
//	vd.data().push_back(v);
//	v.setPosition(vec3(1,-1,0));
//	vd.data().push_back(v);

//	vd.push_back(0);
//	vd.push_back(1);
//	vd.push_back(2);

//	vd.push_back(0);
//	vd.push_back(2);
//	vd.push_back(3);

//	vd.push_back(0);
//	vd.push_back(3);
//	vd.push_back(4);

//	vd.push_back(0);
//	vd.push_back(4);
//	vd.push_back(1);

	Mesh m(vd);

	VertexDataTools::writeOFF(vd,"test.off");

}

