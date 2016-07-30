#include "vd.h"
#include <cstdio>
#include <fstream>
#include <map>

#include "stru.h"
using namespace stru;
namespace vd {

bool Vertex::operator ==(const Vertex &o)
{
	bool to_ret = true;
	float* data = (float*)this;
	const float* odata = (const float*)&o;
	const float eps = std::numeric_limits<float>::epsilon();
	float d;
	for (uint i = 0; i < sizeof(Vertex)/sizeof(float); i++)
	{
		d = fabs(odata[i] - data[i]);
		to_ret = to_ret && d <= eps;
	}
	return to_ret;
}

bool Vertex::operator <(const Vertex &o) const
{
	float* data = (float*)this;
	const float* odata = (const float*)&o;
	for (uint i = 0; i < sizeof(Vertex)/sizeof(float); i++)
	{
		const float& a = data[i];
		const float& b = odata[i];
		if (a < b)
			return true;
		else if (a == b)
			continue;
		else
			return false;
	}
	return false;
}

VertexData::VertexData(Primitive primitive)
{
	m_render_primitive = primitive;
}


VertexData::~VertexData()
{

}



size_t VertexData::push_back(const Vertex &v)
{
	m_data.push_back(v);
	return m_data.size()-1;
}

const std::vector<Vertex>& VertexData::data() const
{
	return m_data;
}

const std::vector<uint32_t>& VertexData::indices() const
{
	return m_indices_data;
}

std::vector<Vertex> &VertexData::data()
{
	return m_data;
}

std::vector<uint32_t>& VertexData::indices()
{
	return m_indices_data;
}

Primitive VertexData::primitive() const
{
	return m_render_primitive;
}

void VertexData::setPrimitive(const Primitive &p)
{
	m_render_primitive = p;
}



VertexData *VertexDataTools::readVD(const std::string &path)
{
	VertexData* vd = new VertexData();

	FILE* f = fopen(path.c_str(),"rb");
	uint32_t first_line[5];
	uint32_t last_line[5];
	fread(first_line,5,4,f);

	for(unsigned int i = 0; i<first_line[3]+1;i++)
		fread(last_line,5,4,f);

	vd->data().resize(last_line[0]);
	vd->indices().resize(last_line[2]);

	fread(vd->data().data(),last_line[1],1,f);
	fread(vd->indices().data(),last_line[3],1,f);
	fclose(f);
	return vd;
}



void handle_v(VertexData* vd, std::map<Vertex,int>& v_loc, const Vertex& v)
{
	int v_id = 0;
	if(v_loc.find(v)!= v_loc.end())
	{
		v_id = v_loc[v];
	}
	else
	{
		v_id =  vd->push_back(v);
		v_loc[v] = v_id;
	}
	vd->push_back(v_id);
}
VertexData *VertexDataTools::readOBJ(const std::string &path)
{

	// mesh loader the 10000ths ^^

	std::ifstream fstream(path.c_str());
	if (!fstream.is_open())
		return nullptr;
	VertexData* vd = new VertexData();

	std::vector<vec3> positions;
	std::vector<vec3> normals;
	std::vector<vec2> tex_coords;

	std::map<Vertex,int> v_loc;

	std::string line;
	std::string type;
	char* arg[4];

	Tokenizer tkn("");
	Tokenizer attrib_tkn("");
	while (std::getline(fstream, line))
	{
		// remove comment
		line = line.substr(0, line.find_first_of('#'));
		trim(line);

		if (line.empty()) // the line was an empty line or a comment
			continue;

		attrib_tkn.reset(line);

		type = attrib_tkn.getToken(' ');

		for (int i = 0; i < 4; i++)
		{
			attrib_tkn.skipWhiteSpaces();
			arg[i] = attrib_tkn.getToken(' ');
		}


		trim(type);

		if (type == "v")
		{
			positions.push_back(vec3(atof(arg[0]),atof(arg[1]),
					atof(arg[2])));
		}
		else if (type == "vn")
		{
			normals.push_back(vec3(atof(arg[0]), atof(arg[1]),
					atof(arg[2])));
		}
		else if (type == "vt")
		{
			tex_coords.push_back(vec2(atof(arg[0]),
								 atof(arg[1])));
		}
		else if (type == "f")
		{
			Vertex v;

			if (arg[3])
			{

			}
			else
			{
				if(normals.empty() && tex_coords.empty())  // only positions
				{
					for (int i = 0; i < 3; i++)
					{

						int p_id  = atoi(arg[i]) - 1;

						v.pos() = positions[p_id];
						handle_v(vd,v_loc,v);
					}
				}
				else if (normals.empty())	// positions and texcoords
				{
					for (int i = 0; i < 3; i++)
					{
						tkn.reset(std::string(arg[i]));
						int  p_id = atoi(tkn.getToken('/')) - 1;
						int t_id = atoi(tkn.getRest()) - 1;
						v.pos() = positions[p_id];
						v.setTexcoord(tex_coords[t_id]);
						handle_v(vd,v_loc,v);
					}
				}
				else if (tex_coords.empty())	 // positions and normals
				{
					for (int i = 0; i < 3; i++)
					{
						tkn.reset(std::string(arg[i]));
						int  p_id = atoi(tkn.getToken('/')) - 1;
						tkn.getToken('/');
						int  n_id = atoi(tkn.getRest()) - 1;

						v.pos() = positions[p_id];
						v.nrm() = normals[n_id];

						handle_v(vd,v_loc,v);
					}
				}
				else		// all three
				{
					for (int i = 0; i < 3; i++)
					{
						tkn.reset(std::string(arg[i]));
						int  p_id = atoi(tkn.getToken('/')) - 1;
						int  t_id = atoi(tkn.getToken('/')) - 1;
						int  n_id = atoi(tkn.getRest()) - 1;

						v.pos() = positions[p_id];
						v.setTexcoord(tex_coords[t_id]);
						v.nrm() = normals[n_id];

						handle_v(vd,v_loc,v);

					}
				}
			}
		}
	}
	fstream.close();
	vd->setPrimitive(TRIANGLES);
	/*if (normals.empty())
		   calculateNormals();*/
	return vd;
}

enum PLY_DATA_TYPE
{
	plyFLOAT,
	plyDOUBLE,
	plyUINT,
	plyINT,
	plyUSHORT,
	plySHORT,
	plyUCHAR,
	plyCHAR,

};

VertexData *VertexDataTools::readPLY(const std::string &path)
{
	std::ifstream fstream(path.c_str());

	if (!fstream.is_open())
		return nullptr;
	VertexData* vd = new VertexData();

	std::string line;

	Tokenizer tkn("");
	//get the headers information
	unsigned int vertex_count;
	unsigned int face_count;

	std::vector<std::string> properties;
	std::vector<PLY_DATA_TYPE> prop_types;

	bool vertex_prop = false;

	while (std::getline(fstream, line))
	{
		tkn.reset(line);
		std::string op = tkn.getToken(' ');
		if (op == "element")
		{
			std::string s = tkn.getToken(' ');
			if (s == "vertex")
			{
				vertex_prop = true;
				vertex_count = atoi(tkn.getRest());
			}
			else if (s == "face")
			{
				face_count = atoi(tkn.getRest());
				vertex_prop = false;
			}
			else
			{
				printf("PLY loader ignored element %s\n", s.c_str());
			}
		}
		else if (op == "property" && vertex_prop)
		{
			std::string type = tkn.getToken(' ');
			std::string name = tkn.getToken(' ');

			PLY_DATA_TYPE dt = plyFLOAT;
			if (type == "float") dt = plyFLOAT;
			else if (type == "double") dt = plyDOUBLE;
			else if (type == "uint") dt = plyUINT;
			else if (type == "int") dt = plyINT;
			else if (type == "ushort") dt = plyUSHORT;
			else if (type == "short") dt = plySHORT;
			else if (type == "uchar") dt = plyUCHAR;
			else if (type == "char") dt = plyCHAR;
			properties.push_back(name);
			prop_types.push_back( dt);
		}
		else if (op == "end_header")
		{
			break;
		}

	}

	// now read the vertex data


	Vertex vtx;

	for (unsigned int i = 0; i < vertex_count; i++)
	{
		std::getline(fstream, line);
		tkn.reset(line);
		for (unsigned int j = 0; j < properties.size(); j++)
		{
			const auto& prp = properties[j];
			const auto& typ = prop_types[j];

			auto val = (float)atof(tkn.getToken(' '));

			if (typ == plyUCHAR)
				val /= 255.0f;

			if (prp[0] == 'x') vtx.pos()[0] = (val);
			else if (prp[0] == 'y') vtx.pos()[1] = (val);
			else if (prp[0] == 'z') vtx.pos()[2] = (val);
			else if (prp[0] == 's') vtx.tex()[0] = (val);
			else if (prp[0] == 't') vtx.tex()[1] = (val);
			else if (prp[0] == 'r') vtx.clr()[0] = (val);
			else if (prp[0] == 'g') vtx.clr()[1] = (val);
			else if (prp[0] == 'b') vtx.clr()[2] = (val);
			else if (prp[0] == 'n')
			{
				if( prp[1] == 'x') vtx.nrm()[0] = (val);
				else if (prp[1] == 'y') vtx.nrm()[1] = (val);
				else if (prp[1] == 'z') vtx.nrm()[2] = (val);
			}

		}
		vd->push_back(vtx);
	}
	// read the faces

	for (unsigned int i = 0; i < face_count; i++)
	{
		std::getline(fstream, line);
		tkn.reset(line);
		auto verts_in_this_face = atoi(tkn.getToken(' '));

		int vert0 = atoi(tkn.getToken(' '));
		int vert1 = atoi(tkn.getToken(' '));

		for (int i = 2; i < verts_in_this_face; i+=1)
		{
			int vert2 = atoi(tkn.getToken(' '));
			vd->push_back(vert0);
			vd->push_back(vert1);
			vd->push_back(vert2);
			vert1 = vert2;
		}
	}
	fstream.close();
	return vd;
}

bool VertexDataTools::writeVD(const VertexData *vd, const std::string &path)
{
	FILE* f = fopen(path.c_str(),"wb");
	if(!f)
		return 0;
	std::string prefix = "VDFF";
	uint32_t hline[5];

	int num_attrib = ::log2(ATTRIBUTE_LAST)+1;
	hline[0] = *((uint32_t*) prefix.c_str());
	hline[1] = (num_attrib+1)*5*sizeof(uint32_t); //headr size
	hline[2] = 1; // version
	hline[3] = num_attrib;
	hline[4] = vd->primitive();

	fwrite(hline,1,5*sizeof(uint32_t),f);
	const vd::Vertex v = vd->data()[0];

#define addr_diff(a,b) (((char*)a-(char*) b))
	hline[0] = POSITION;
	hline[1] = 3;
	hline[2] = FLOAT;
	hline[3] = 0;
	hline[4] = addr_diff(&(v.pos()),&v);
	fwrite(hline,1,5*sizeof(uint32_t),f);
	hline[0] = NORMAL;
	hline[1] = 3;
	hline[2] = FLOAT;
	hline[3] = 0;
	hline[4] = addr_diff(&(v.nrm()),&v);
	fwrite(hline,1,5*sizeof(uint32_t),f);
	hline[0] = TEXCOORD;
	hline[1] = 3;
	hline[2] = FLOAT;
	hline[3] = 0;
	hline[4] = addr_diff(&(v.tex()),&v);
	fwrite(hline,1,5*sizeof(uint32_t),f);
	hline[0] = TANGENT;
	hline[1] = 3;
	hline[2] = FLOAT;
	hline[3] = 0;
	hline[4] = addr_diff(&(v.tan()),&v);
	fwrite(hline,1,5*sizeof(uint32_t),f);
	hline[0] = COLOR;
	hline[1] = 4;
	hline[2] = FLOAT;
	hline[3] = 0;
	hline[4] = addr_diff(&(v.clr()),&v);
	fwrite(hline,1,5*sizeof(uint32_t),f);
#undef addr_diff

	hline[0] = vd->data().size();
	hline[1] = vd->data().size()*sizeof(Vertex);
	hline[2] = vd->indices().size();
	hline[3] =  vd->indices().size() * sizeof(uint32_t);
	hline[4] = UNSIGNED_INT;
	fwrite(hline,1,5*sizeof(uint32_t),f);

	fwrite(vd->data().data(),vd->data().size(),sizeof(Vertex),f);
	fwrite(vd->indices().data(),vd->indices().size(),sizeof(uint32_t),f);
	fclose(f);
	return true;
}

bool VertexDataTools::writeOBJ(const VertexData *vd, const std::string &path)
{
	FILE* f = fopen(path.c_str(),"w");
	if(!f)
		return false;

	struct obj_vert{int p;int t;int n;};
	typedef struct{obj_vert v[3];} obj_face;

	std::map<vec3,int,compare_vec_3> pos2id;
	int pid = 1;
	std::map<vec3,int,compare_vec_3> nrm2id;
	int nid = 1;
	std::map<vec3,int,compare_vec_3> tex2id;
	int tid = 1;
	//std::vector<obj_face> faces;
	const Primitive& prim = vd->primitive();
	int consumed = 0;
	if(prim == TRIANGLES)
		consumed = 3;
	else if(prim == TRIANGLE_STRIP)
		consumed = 1;
	else if(prim == QUADS)
		consumed = 4;
	else if(prim == QUAD_STRIP)
		consumed = 2;
	int vs = 4 -consumed%2;


	for(unsigned int i = 0 ; i< vd->indices().size();i+=consumed)
	{

		int v[4];
		if (consumed >= 3 || i%2 ==1)
		{
			for (int j = 0; j < vs; j++)
			{
				v[j] = vd->indices()[i+j];
			}
		}
		else
		{
			for (int j = 0; j < vs; j++)
			{
				const int idx = j==0? 1 : j==1? 0:j;
				v[j] = vd->indices()[i+idx];
			}
		}

		obj_face face;
		for (int j = 0; j < vs; j++)
		{
			const Vertex& vtx = vd->data()[v[j]];
			if(pos2id.find(vtx.pos()) == pos2id.end())
			{
				fprintf(f,"v %f %f %f\n",vtx.pos().x,vtx.pos().y,vtx.pos().z);
				pos2id[vtx.pos()] = pid;
				face.v[j].p = pid;
				pid ++;
			}
			else
			{
				face.v[j].p = pos2id[vtx.pos()];
			}

			if(nrm2id.find(vtx.nrm()) == nrm2id.end())
			{
				fprintf(f,"vn %f %f %f\n",vtx.nrm().x,vtx.nrm().y,vtx.nrm().z);
				nrm2id[vtx.nrm()] = nid;
				face.v[j].n = nid;
				nid ++;
			}
			else
			{
				face.v[j].n = nrm2id[vtx.nrm()];
			}

			if(tex2id.find(vtx.tex()) == tex2id.end())
			{
				fprintf(f,"vt %f %f\n",vtx.tex().x,vtx.tex().y);
				tex2id[vtx.tex()] = tid;
				face.v[j].t = tid;
				tid ++;
			}
			else
			{
				face.v[j].t = tex2id[vtx.tex()];
			}
		}
		fprintf(f,"f");
		for (int j = 0; j < vs; j++)
		{
			fprintf(f," %d/%d/%d",face.v[j].p,face.v[j].t,face.v[j].n);
		}
		fprintf(f,"\n");
	}

	fclose(f);
	return true;
}

bool VertexDataTools::writePLY(const VertexData */*vd*/, const std::string &/*path*/)
{
	return false;
}

bool VertexDataTools::writeToFile(
		const VertexData *vd,
		const std::string &p,
		VertexDataTools::Format f)
{


	std::string ending = paths::extension(p);
	if (f == FROM_PATH)
	{
		if(ending == "obj" || ending == "OBJ")
			f = OBJ;
		else if(ending == "ply" || ending == "PLY")
			f = PLY;
		else if(ending == "vd" || ending == "vd")
			f = VD;
	}

	switch (f)
	{
	case VD:
		return writeVD(vd,p);
	case OBJ:
		return writeOBJ(vd,p);
	case PLY:
		return writePLY(vd,p);
	default:
		return false;
	}
	return false;
}

VertexData* VertexDataTools::readFromFile(
		const std::string &path,
		VertexDataTools::Format f)
{
	std::string ending = paths::extension(path);
	if (f == FROM_PATH)
	{
		if(ending == "obj" || ending == "OBJ")
			f = OBJ;
		else if(ending == "ply" || ending == "PLY")
			f = PLY;
		else if(ending == "vd" || ending == "vd")
			f = VD;
	}
	switch (f) {
	case VD:
		return readVD(path);
	case OBJ:
		return readOBJ(path);
	case PLY:
		return readPLY(path);

	default:
		return nullptr;
	}
}

void VertexDataTools::calculateNormals(VertexData *vd)
{
	std::vector<Vertex>& verts =  vd->data();

	const std::vector<uint32_t>& indices = vd->indices();

	std::map<vec3, vec3, compare_vec_3> p2n;

	Primitive prim = vd->primitive();


	int consumed = 0;
	if(prim == TRIANGLES)
		consumed = 3;
	else if(prim == TRIANGLE_STRIP)
		consumed = 1;
	else if(prim == QUADS)
		consumed = 4;
	else if(prim == QUAD_STRIP)
		consumed = 2;
	int vs = 4 -consumed%2;
	if (prim != 0)
	{
		for (unsigned int i = 0; i < indices.size(); i += consumed)
		{
			const vec3* v[4];
			if (consumed >= 3 || i%2 ==1)
			{
				for (int j = 0; j < vs; j++)
				{
					v[j] = &(verts.at(indices[i+j]).pos());
				}
			}
			else
			{
				for (int j = 0; j < vs; j++)
				{
					const int idx = j==0? 1 : j==1? 0:j;
					v[j] = &(verts.at(indices[i+idx]).pos());
				}
			}

			int connected = -1;
			if(vs == 4) //QUADS
			{
				if(length2(*(v[0])-*(v[2])) < length2(*(v[1])-*(v[3])))
				{
					connected = 0;
				}
			}
			/* calculate normalized normal */
			vec3 n = normalize(cross(*(v[1])- *(v[0]), *(v[2]) - *(v[0])));
			float area = 0.5*length(cross(*(v[1])- *(v[0]), *(v[2]) - *(v[0])));

			/* add the normal to each corner of the triangle */
			for (int j = 0; j < vs; j++)
			{
				const vec3 A = *(v[(j+1)%vs])- *(v[j]);
				const vec3 B = *(v[(j-1)%vs])- *(v[j]);
				const vec3 C = *(v[(j+2)%vs])- *(v[j]);
				vec3& r = p2n[*(v[j])];
				if(vs == 4) // for a quad triangulate first
				{
					if(j % 2 != connected)
					{
						const vec3 cr = cross(A,B);
						area = 0.5f* length(cr);
						n = normalize(cr);
						const float w = area/((dot(A,A)*dot(B,B)));
						r += n*w ;
					}
					else
					{
						vec3 cr = cross(A,C);
						area = 0.5f* length(cr);
						n = normalize(cr);
						r += n*area/((dot(A,A)*dot(C,C)));

						cr = cross(C,B);
						area = 0.5f* length(cr);
						n = normalize(cr);
						r += n*area/((dot(C,C)*dot(B,B)));

					}
				}
				else //for a triangle use the one time normal.
				{
					const float w = area/((dot(A,A)*dot(B,B)));
					r += n*w ;
				}
			}
		}

		/*normalize all normals*/
		for (auto& pn : p2n)
			pn.second = normalize(pn.second);

		/* set all the normals */
		for (Vertex& v : verts)
		{
			v.nrm() = p2n[v.pos()];
		}
	}
}

void VertexDataTools::calculateTangents(VertexData *vd)
{

	for (auto& vert : vd->data())
	{
		vert.setTangent(vec3(0,0,0));
	}

	std::vector<Vertex>& vertex_data = vd->data();
	std::vector<uint32_t>& indices = vd->indices();

	for (unsigned int i = 0; i < indices.size(); i+=3)
	{
		const vec3  v0 = vertex_data[indices[i + 0]].pos();

		const vec3  v1 = vertex_data[indices[i + 1]].pos();
		const vec3  v2 = vertex_data[indices[i + 2]].pos();

		const vec3  uv0 = vertex_data[indices[i + 0]].tex();
		const vec3  uv1 = vertex_data[indices[i + 1]].tex();
		const vec3  uv2 = vertex_data[indices[i + 2]].tex();


		vec3 dPos1 = v1 - v0;
		vec3 dPos2 = v2 - v0;
		vec3 dUV1 = uv1 - uv0;
		vec3 dUV2 = uv2 - uv0;


		float r = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);

		vec3 t = (dPos1 * dUV2.y - dPos2 * dUV1.y)*r;
		//		vec3 b = (dPos2 * dUV1.x - dPos1 * dUV2.x)*r;

		vertex_data[indices[i + 0]].tan() += t;
		vertex_data[indices[i + 1]].tan() += t;
		vertex_data[indices[i + 2]].tan() += t;

	}

	for (auto& vert : vertex_data)
	{
		vec3 nt = normalize(vert.tan());
		vert.setTangent(vec3(nt));
	}


}




}
