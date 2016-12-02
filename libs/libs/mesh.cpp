
#include "mesh.h"
#include <set>
namespace ofl
{

int addPos(std::map<vec3,int,compare_vec_3>& map ,const vec3& pos, std::vector<vec3>& poss)
{
	if(map.find(pos)== map.end())
	{
		map[pos] = static_cast<int>(poss.size());
		poss.push_back(pos);
	}
	return  map[pos];
}



Mesh::Mesh(const VertexData *vd)
{
	if(vd->primitive() != TRIANGLES)
		fprintf(stderr,"Mesh can only be created from TRIANGLE based vertex data!\n");
	std::map<vec3,int,compare_vec_3> pmap;
	Triangle tri(0,0,0);
	const auto& pd = vd->data();
	const auto& id = vd->indices();
	for(uint i = 0 ; i < vd->indices().size();i+=3)
	{
		for(uint j =0 ; j<3;j++)
			tri(j) = addPos(pmap,pd[id[i+j]].pos(),m_positions);
		m_triangles.push_back(tri);
	}
	buildDataStructure();
}

VertexData* Mesh::toVertexData()
{
	VertexData* vd = new VertexData;
	vd->data().reserve(m_positions.size());
	Vertex v;
	for(const vec3& p :m_positions)
	{
		v.setPosition(p);
		vd->data().push_back(v);
	}

	vd->indices().reserve(m_triangles.size()*3);
	for(const Triangle& t : m_triangles)
	{
		for(int i  =0 ; i<3;i++)
			vd->push_back(static_cast<uint>(t(i)));
	}
	return vd;
}

int corner_add(int c,int a)
{
	int  t = c/3;
	return t*3 + ((c%3)+a)%3;
};

std::vector<Triangle*> Mesh::adjacentTriangles(int vertex)
{
	return std::vector<Triangle*>(m_pos2tris[vertex].begin(), m_pos2tris[vertex].end());
}



std::vector<int> Mesh::adjacentVertices(int vertex)
{

	return std::vector<int>(m_pos2pos[vertex].begin(), m_pos2pos[vertex].end());

}

void Mesh::buildDataStructure()
{
	for(auto& t : m_triangles)
	{

		for(uint i =0 ; i<3;i++)
		{
			m_pos2tris[t(i)].insert(&t);
			m_pos2pos[t(i)].insert(t((i+1)%3));
			m_pos2pos[t(i)].insert(t((i+2)%3));
		}

	}
	/*m_corners.resize(m_triangles.size()*3);
	m_vertex2corner.resize(m_positions.size(),-1);

	int corners = 0;
	for(const Triangle& t : m_triangles)
	{
		for(int i = 0 ; i<3;i++)
			if(m_vertex2corner[t(i)]<0) m_vertex2corner[t(i)] = corners+i;


		int ot = 0;
		for(const Triangle& tt : m_triangles)
		{
			if(tt==t)
			{
				ot+=3;
				continue;
			}
			int ts=0, tts=0;
			if(t.sharedEdgeWith(tt,tts,ts))
			{
				int	opp =  (ot)+(tts+3-1)%3;
				m_corners[corners+(ts+3-1)%3].opp = opp;
				m_corners[opp].opp = corners+(ts+3-1)%3;

			}
			ot+=3;
		}
		corners+=3;
	}*/

}

int MeshTools::getClosestVertex(Mesh *m, const vec3 &p)
{
	if (m->positions().empty())
		return -1;
	float min = distance2(p,m->positions()[0]);
	int r = 0 ;
	int i = 0;
	for(const auto v : m->positions())
	{
		const float d = distance2(p,v) ;
		if(d<min)
		{
			r = i ;
			min = d;
		}
		i++;
	}
	return r;
}

vec3 triangle_cp(
		const vec3& t1,
		const vec3& t2,
		const vec3& t3,
		const vec3& p)
{
	vec3 a = t2-t1;
	vec3 b = t3-t1;
	vec3 n = cross(a,b);
	n = normalize(n);

	mat4 T(vec4(a,0),vec4(b,0),vec4(n,0),vec4(t1,1));
	mat4 Ti = glm::inverse(T);

	vec3 pt = vec3(Ti * vec4(p,1));


	pt.z = 1.0f-pt.x-pt.y;

	if(pt.z < 0)
	{
		pt.z = 0;
		vec3 diag(0.5f*sqrt(2.0f),-0.5f*sqrt(2.0f),0);
		pt = vec3(0,1,0) + diag* dot(diag,pt-vec3(0,1,0));
	}

	pt.x = pt.x<0.0f?0.0f : (pt.x > 1? 1.0f: pt.x);
	pt.y = pt.y<0.0f?0.0f : (pt.y > 1? 1.0f: pt.y);





	pt.z = 0;
	return vec3(T * vec4(pt,1));

}
vec3 MeshTools::getClosestPoint(Mesh *m, const vec3 &p)
{
	int cv = getClosestVertex(m,p);
	auto tris = m->adjacentTriangles(static_cast<uint>(cv));
	float min = distance2(m->vertex(cv),p);
	vec3 res = m->vertex(cv);

	for(const auto tt : tris)
	{
		const Triangle &t = *tt;//m->triangle(static_cast<uint>(tt));
		auto r = triangle_cp(
					m->vertex(static_cast<uint>(t(0))),
					m->vertex(static_cast<uint>(t(1))),
					m->vertex(static_cast<uint>(t(2))),p);

		float d = distance2(p,r);
		if(d < min)
		{
			min = d;
			res = r;
		}
	}
	return res;
}




}
