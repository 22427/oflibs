
#include "mesh.h"
#include <set>
#include <algorithm>
namespace ofl
{
using namespace glm;
int addPos(std::map<vec3,int,Comperator<vec3>>& map ,const vec3& pos, std::vector<vec3>& poss)
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
	std::map<vec3,int,Comperator<vec3>> pmap;
	Triangle tri(0,0,0);
	const auto& pd = vd->get_all_attributes(ATTRIB_POSITION);
	const auto& id = vd->get_all_indices();

	for(uint i = 0 ; i < vd->index_count();i+=3)
	{
		for(uint j =0 ; j<3;j++)
			tri(j) = addPos(pmap,vec3(pd[id[i+j]]),m_positions);
		m_triangles.push_back(tri);
	}
	build_data_structure();
}

VertexData* Mesh::to_VertexData()
{
	VertexConfiguration cfg;
	cfg.add_attribute(Attribute(ATTRIB_POSITION,3,FLOAT,false));
	VertexData* vd = new VertexData(TRIANGLES,cfg,UNSIGNED_INT);
	vd->vertices_reserve(m_positions.size());
	Vertex v(cfg,nullptr);
	for(const vec3& p :m_positions)
	{
		v.set_value(ATTRIB_POSITION,p);
		vd->push_back(v);
	}
	vd->indices_reserve(m_triangles.size()*3);
	for(const Triangle& t : m_triangles)
	{
		for(int i  =0 ; i<3;i++)
			vd->push_back(static_cast<uint32_t>(t(i)));
	}
	return vd;
}

int corner_add(int c,int a)
{
	int  t = c/3;
	return t*3 + ((c%3)+a)%3;
};

std::vector<int> Mesh::adjacent_triangles(int vertex)
{
	return std::vector<int>(m_pos2tris[vertex].begin(), m_pos2tris[vertex].end());
}



std::vector<int> Mesh::adjacent_vertices(int vertex)
{

	return std::vector<int>(m_pos2pos[vertex].begin(), m_pos2pos[vertex].end());

}

template<typename T>
std::set<T> intersect(const std::set<T>& a,const std::set<T>&b )
{
	std::set<T> res;
	for(const auto& t : a)
	{
		if(b.find(t) != b.end())
			res.insert(t);
	}
	return res;
}


void Mesh::build_data_structure()
{
	int ct = 0;
	for(auto& t : m_triangles)
	{
		for(uint i =0 ; i<3;i++)
		{
			m_pos2tris[t(i)].insert(ct);
			m_pos2pos[t(i)].insert(t((i+1)%3));
			m_pos2pos[t(i)].insert(t((i+2)%3));
		}
		ct++;

	}

	//	for(auto& t : m_triangles)
	//	{
	//		std::set<Triangle*> res = intersect(m_pos2tris[t(0)],m_pos2tris[t(1)]);

	//	}


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

void Mesh::insert_vertex(const vec3 &v, const int t)
{
	Triangle tt = m_triangles[t];
	m_positions.push_back(v);
	int newpos = m_positions.size()-1;
	Triangle newts[3];

	for(int i = 0 ; i<3;i++)
	{
		newts[i](0) = tt(i);
		newts[i](1) = tt((i+1)%3);
		newts[i](2) = newpos;
		m_pos2pos[i].insert(newpos);
	}

	m_triangles[t] = newts[0];
	m_triangles.push_back(newts[1]);
	m_triangles.push_back(newts[2]);

	m_pos2tris[tt(0)].insert(m_triangles.size()-1);
	m_pos2tris[tt(1)].insert(m_triangles.size()-2);

	m_pos2tris[tt(2)].insert(m_triangles.size()-1);
	m_pos2tris[tt(2)].insert(m_triangles.size()-2);
	m_pos2tris[tt(2)].erase(m_pos2tris[tt(2)].find(t));


}

int MeshTools::get_closest_vertex(Mesh *m, const vec3 &p)
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
		const vec3& p,
		const mat4& T,
		const mat4& Ti)
{
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

mat4 calcT(const vec3& t1,
		   const vec3& t2,
		   const vec3& t3)
{
	vec3 a = t2-t1;
	vec3 b = t3-t1;
	vec3 n = cross(a,b);
	n = normalize(n);

	return  mat4(vec4(a,0),vec4(b,0),vec4(n,0),vec4(t1,1));
}
vec3 triangle_cp(
		const vec3& t1,
		const vec3& t2,
		const vec3& t3,
		const vec3& p)
{
	mat4 T = calcT(t1,t2,t3);
	mat4 Ti = glm::inverse(T);

	return triangle_cp(p,T,Ti);

}

int MeshTools::get_closest_triangle(Mesh *m, const vec3 &p)
{
	const std::set<int> tris = m->m_pos2tris[get_closest_vertex(m,p)];

	float min = std::numeric_limits<float>::max();
	int res = *(tris.begin());
	for(const int tt : tris)
	{
		const Triangle& t = m->m_triangles[static_cast<uint>(tt)];
		float d = distance2(triangle_cp(m->vertex(static_cast<uint>(t(0))),
										m->vertex(static_cast<uint>(t(1))),
										m->vertex(static_cast<uint>(t(2))),p),p);
		if(d < min)
		{
			min = d;
			res = tt;
		}
	}

	return res;

}


vec3 MeshTools::get_closest_point(Mesh *m, const vec3 &p)
{
	int cv = get_closest_vertex(m,p);
	auto tris = m->adjacent_triangles(cv);
	float min = distance2(m->vertex(cv),p);
	vec3 res = m->vertex(cv);

	for(const auto tt : tris)
	{
		const Triangle &t = m->triangle(static_cast<uint>(tt));
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

vec3 MeshTools::get_closest_point(Mesh *m, const vec3 &p, const std::vector<mat4> &Ts, const std::vector<mat4> &Tis)
{
	int cv = get_closest_vertex(m,p);
	auto tris = m->adjacent_triangles(static_cast<uint>(cv));
	float min = distance2(m->vertex(cv),p);
	vec3 res = m->vertex(cv);

	for(const auto tt : tris)
	{
		auto r = triangle_cp(p,Ts[tt],Tis[tt]);
		float d = distance2(p,r);
		if(d < min)
		{
			min = d;
			res = r;
		}
	}
	return res;
}

Mesh *MeshTools::merge(const Mesh *a, const Mesh *b)
{
	Mesh* res = new Mesh(*a);
	for(const glm::vec3 p : b->m_positions)
	{
		res->insert_vertex(p,get_closest_triangle(res,p));
	}
	return res;
}

Mesh *MeshTools::average_surfaces(const std::vector<Mesh *> ms)
{
	std::vector<Mesh *> rs;
	std::vector<std::vector<mat4>> Ts(ms.size());
	std::vector<std::vector<mat4>> Tis(ms.size());


	for(uint im = 0 ; im< ms.size();im++)
	{
		const Mesh* m = ms[im];
		rs.push_back(new Mesh(*(m)));

		std::vector<mat4>& tTs = Tis[im];
		std::vector<mat4>& tTis = Tis[im];
		tTs.resize(m->m_triangles.size());
		tTis.resize(m->m_triangles.size());

#pragma omp parallel for
		for(uint it=0 ; it <m->m_triangles.size();it++)
		{
			const Triangle& t = m->m_triangles[it];
			tTs[it] = (calcT(m->vertex(t(0)),m->vertex(t(1)),m->vertex(t(2))));
			tTis[it] = (inverse(tTs.back()));
		}

	}



	for(uint j = 0 ; j< ms.size();j++)
	{
#pragma omp parallel for
		for(uint i = 0 ; i< ms[j]->positions().size(); i++)
		{
			const vec3& p  = ms[j]->positions()[i];
			vec3 cp = p;
			for( uint k = 0 ; k<ms.size();k++)
			{
				if(k!= j)
					cp+=MeshTools::get_closest_point(ms[k],p);//,Ts[k],Tis[k]);
			}
			rs[j]->positions()[i] = cp/static_cast<float>(ms.size());
		}

		VertexData* vd = rs[j]->to_VertexData();
		VertexDataOperations::write_to_file(vd,std::to_string(j)+".obj");
		delete vd;
		delete rs[j];
	}


	return nullptr;


}






}
