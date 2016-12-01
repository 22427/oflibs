#include "mesh.h"
#include <set>
namespace ofl
{

int addPos(std::map<vec3,int>& map ,const vec3& pos, std::vector<vec3>& poss)
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
	std::map<vec3,int> pmap;
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

std::vector<int> Mesh::adjacentTriangles(uint vertex)
{
	std::vector<int> res;
	int c = m_vertex2corner[vertex];
	int first_t = c/3;
	int start_c = first_t*3 + ((c%3)+1)%3;
	res.push_back(first_t);

	int t = first_t+1;
	c = start_c;
	bool other_way = false;
	while (t != first_t)
	{
		c = m_corners[c].opp;
		if(c<0)
		{
			other_way = true;
			break;
		}
		t = c/3;

		if(t == first_t)
			break;
		res.push_back(t);
		c = corner_add(c,2);
	}

	if(other_way)
	{
		int c = m_vertex2corner[vertex];
		c = corner_add(c,2);
		while (c >= 0)
		{
			c = m_corners[c].opp;
			if(c<0)
			{
				break;
			}
			t = c/3;
			res.push_back(t);
			c = corner_add(c,1);
		}
	}

	return res;
}



std::vector<int> Mesh::adjacentVertices(uint vertex)
{
	std::vector<int> tris = adjacentTriangles(vertex);

	std::vector<int> res;
	std::set<int> r;
	for(const int i : tris)
	{
		const Triangle& t = triangle(i);
		for(int j =0 ; j< 3;j++)
		{
			r.insert(t(j));

		}

	}

	for(int i: r)
	{
		res.push_back(i);
	}

	return res;

}

void Mesh::buildDataStructure()
{
	m_corners.resize(m_triangles.size()*3);
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
	}

}

int MeshTools::getClosestVertex(Mesh *m, const vec3 &p)
{
	if (m->positions().empty())
		return -1;
	float min = distance2(p,m->positions()[0]);
	int r = -1 ;
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


}
