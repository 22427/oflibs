
#include "mesh.h"
#include <set>
#include <algorithm>
namespace ofl
{
using namespace glm;

int addVtx(std::map<vec3,int,Comperator<vec3>>& map ,const vec3& pos, std::vector<MeshVertex>& vtxs)
{
	if(map.find(pos)== map.end())
	{
		map[pos] = static_cast<int>(vtxs.size());
		vtxs.push_back(MeshVertex(pos));
	}
	return  map[pos];
}



Mesh::Mesh(const VertexData *vd)
{
	if(vd->primitive() != TRIANGLES)
		fprintf(stderr,"Mesh can only be created from TRIANGLE based vertex data!\n");
	std::map<vec3,int,Comperator<vec3>> pmap;
	MeshTriangle tri(0,0,0);

	std::vector<vec4> colors = vd->get_all_attributes(ATTRIB_COLOR);

	const auto pd = vd->get_all_attributes(ATTRIB_POSITION);
	const auto id = vd->get_all_indices();

	for(uint i = 0 ; i < vd->index_count();i+=3)
	{
		for(uint j =0 ; j<3;j++)
		{
			tri(j) = addVtx(pmap,vec3(pd[id[i+j]]),m_verts);

		}
		m_triangles.push_back(tri);
	}
	rebuild_data_structure();
}



void Mesh::remove_rogue_elements()
{
	std::vector<uint> missings;
	uint i = 0;
	for (const auto& ptt : m_vert2tris)
	{
		if(ptt.empty())
			missings.push_back(i);
		i++;
	}

	for(auto i : missings)
	{
		m_verts.erase(m_verts.begin()+i);
	}

	std::vector<uint> missing_tris;
	i = 0;
	for(auto& t:  m_triangles)
	{
		if(!t.valid())
		{
			missing_tris.push_back(i);
			continue;
		}
		for(auto& v : t)
		{
			int to_rem = 0;
			for(auto j : missings)
			{
				if(v> j)
					to_rem--;
			}
			v+=to_rem;
		}
	}

	for(auto i : missing_tris)
	{
		m_triangles.erase(m_triangles.begin()+i);
	}

	rebuild_data_structure();
}

void Mesh::delete_triangle(int triangle_id)
{
	if(triangle_id <0 )
		return;

	auto& t = m_triangles[triangle_id];
	for(const auto v : t)
	{
		m_vert2tris[v].erase(triangle_id);
		recalc_v2v(v);
	}
	m_triangles[triangle_id] = MeshTriangle(); // make all vertices invalid
}

void Mesh::delete_vertex(int vertex_id)
{
	if(vertex_id<0)
		return;
	// delete every adjacent triangle
	for(const auto ti : m_vert2tris[vertex_id])
	{
		delete_triangle(ti);
	}
}

uint Mesh::add_triangle(const MeshVertex &a, const MeshVertex &b, const MeshVertex &c)
{
	MeshTriangle nt(add_vertex(a),add_vertex(b),add_vertex(c));
	return add_triangle(nt);
}

uint Mesh::add_triangle(const MeshTriangle &nt)
{
	const uint r = m_triangles.size();
	m_triangles.push_back(nt);

	for(int i = 0 ; i< 3;i++)
	{
		m_vert2vert[nt(i)].insert(nt((i+1)%3));
		m_vert2vert[nt(i)].insert(nt((i+2)%3));

		m_vert2tris[nt(i)].insert(r);
	}
	return r;
}

uint Mesh::add_vertex(const MeshVertex &vtx)
{
	auto f = m_vertex_id_from_position.find(vtx.pos);
	if(f != m_vertex_id_from_position.end())
	{
		m_verts[(*f).second] = vtx;
		return (*f).second;
	}
	const uint r = m_verts.size();
	m_verts.push_back(vtx);
	m_vert2tris.push_back(std::set<uint>());
	m_vert2vert.push_back(std::set<uint>());
	m_vertex_id_from_position[vtx.pos] = r;

	return  r;
}


std::set<uint> Mesh::adjacent_triangles(int vertex_id)
{
	return m_vert2tris[vertex_id];
}

std::set<uint> Mesh::adjacent_vertices(int vertex_id)
{
	return m_vert2vert[vertex_id];
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


void Mesh::recalc_v2v(uint vertex_id)
{
	if(vertex_id==OFL_INVALID_VERTEX)
		return;
	auto& ptp = m_vert2vert[vertex_id];
	ptp.clear();
	for(const auto ti : m_vert2tris[vertex_id])
	{
		for(const auto v : m_triangles[ti])
		{
			if(v!=vertex_id)
				ptp.insert(v);
		}
	}
}

void Mesh::rebuild_data_structure()
{
	int ct = 0;
	m_vert2vert.resize(m_verts.size());
	m_vert2tris.resize(m_verts.size());

	m_vertex_id_from_position.clear();
	for(uint i = 0 ; i< m_verts.size();i++)
	{
		m_vertex_id_from_position[m_verts[i].pos] = i;
	}

	for(auto& t : m_triangles)
	{
		for(uint i =0 ; i<3;i++)
		{
			m_vert2tris[t(i)].insert(ct);
			m_vert2vert[t(i)].insert(t((i+1)%3));
			m_vert2vert[t(i)].insert(t((i+2)%3));
		}
		ct++;
	}
}

void Mesh::update_triangle(int triangle_id)
{
	const auto nt = m_triangles[triangle_id];


	for(int i = 0 ; i< 3;i++)
	{
		m_vert2tris[nt(i)].erase(triangle_id);
		recalc_v2v(nt(i));

		m_vert2vert[nt(i)].insert(nt((i+1)%3));
		m_vert2vert[nt(i)].insert(nt((i+2)%3));
		m_vert2tris[nt(i)].insert(triangle_id);
	}
}



int MeshOps::get_closest_vertex(Mesh *m, const vec3 &p)
{
	float min = distance2(p,m->vertices()[0].pos);
	int r = 0 ;
	int i = 0;
	for(const auto v : m->vertices())
	{
		const float d = distance2(p,v.pos) ;
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

int MeshOps::get_closest_triangle(Mesh *m, const vec3 &p)
{
	const std::set<uint> tris = m->m_vert2tris[get_closest_vertex(m,p)];

	float min = std::numeric_limits<float>::max();
	int res = *(tris.begin());
	for(const int tt : tris)
	{
		const MeshTriangle& t = m->m_triangles[static_cast<uint>(tt)];
		float d = distance2(triangle_cp(m->vertex_position(static_cast<uint>(t(0))),
										m->vertex_position(static_cast<uint>(t(1))),
										m->vertex_position(static_cast<uint>(t(2))),p),p);
		if(d < min)
		{
			min = d;
			res = tt;
		}
	}

	return res;

}


vec3 MeshOps::get_closest_point(Mesh *m, const vec3 &p)
{
	int cv = get_closest_vertex(m,p);
	auto tris = m->adjacent_triangles(cv);
	float min = distance2(m->vertex_position(cv),p);
	vec3 res = m->vertex_position(cv);

	for(const auto tt : tris)
	{
		const MeshTriangle &t = m->triangle(static_cast<uint>(tt));
		auto r = triangle_cp(
					m->vertex_position(static_cast<uint>(t(0))),
					m->vertex_position(static_cast<uint>(t(1))),
					m->vertex_position(static_cast<uint>(t(2))),p);

		float d = distance2(p,r);
		if(d < min)
		{
			min = d;
			res = r;
		}
	}
	return res;
}

vec3 MeshOps::get_closest_point(Mesh *m, const vec3 &p, const std::vector<mat4> &Ts, const std::vector<mat4> &Tis)
{
	int cv = get_closest_vertex(m,p);
	auto tris = m->adjacent_triangles(static_cast<uint>(cv));
	float min = distance2(m->vertex_position(cv),p);
	vec3 res = m->vertex_position(cv);

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

Mesh *MeshOps::combine(const Mesh *a, const Mesh *b)
{
	Mesh* res = new Mesh(*a);
	for(const auto& v : b->m_verts)
	{
		MeshOps::insert_vertex(res,v,get_closest_triangle(res,v.pos));
	}
	return res;
}

Mesh *MeshOps::merge(Mesh *a, const Mesh *b)
{

	for(const auto& v : b->m_verts)
	{
		MeshOps::insert_vertex(a,v,get_closest_triangle(a,v.pos));
	}
	return a;
}

void MeshOps::average_surfaces(std::vector<Mesh *> ms)
{
	//std::vector<Mesh *> rs;

	// Precalculating the triangle matrices !?
//	std::vector<std::vector<mat4>> Ts(ms.size());
//	std::vector<std::vector<mat4>> Tis(ms.size());
//	for(uint im = 0 ; im< ms.size();im++)
//	{
//		const Mesh* m = ms[im];
//		rs.push_back(new Mesh(*(m)));

//		std::vector<mat4>& tTs = Tis[im];
//		std::vector<mat4>& tTis = Tis[im];
//		tTs.resize(m->m_triangles.size());
//		tTis.resize(m->m_triangles.size());

//#pragma omp parallel for
//		for(uint it=0 ; it <m->m_triangles.size();it++)
//		{
//			const MeshTriangle& t = m->m_triangles[it];
//			tTs[it] = (calcT(m->vertex_position(t(0)),m->vertex_position(t(1)),m->vertex_position(t(2))));
//			tTis[it] = (inverse(tTs.back()));
//		}

//	}


	std::vector<Mesh *> rs;

	for(uint im = 0 ; im< ms.size();im++)
	{
		rs.push_back(new Mesh(*(ms[im])));
	}

	for(uint j = 0 ; j< ms.size();j++)
	{
#pragma omp parallel for
		for(uint i = 0 ; i< ms[j]->vertices().size(); i++)
		{
			const vec3& p  = ms[j]->vertices()[i].pos;
			vec3 cp = p;
			for( uint k = 0 ; k<ms.size();k++)
			{
				if(k!= j)
					cp+=MeshOps::get_closest_point(ms[k],p);//,Ts[k],Tis[k]);
			}
			rs[j]->vertices()[i].pos = cp/static_cast<float>(ms.size());
		}
	}

	for(uint im = 0 ; im< ms.size();im++)
	{
		delete ms[im];
		ms[im] =rs[im];
	}
}

VertexData *MeshOps::to_VertexData(const Mesh *m)
{
	VertexConfiguration cfg;
	cfg.add_attribute(Attribute(ATTRIB_POSITION,3,FLOAT,false));
	cfg.add_attribute(Attribute(ATTRIB_COLOR,4,FLOAT,false));

	VertexData* vd = new VertexData(TRIANGLES,cfg,UNSIGNED_INT);
	vd->vertices_reserve(m->m_verts.size());

	Vertex v(cfg,nullptr);
	for(const auto& mv :m->m_verts)
	{
		v.set_value(ATTRIB_POSITION,mv.pos);
		v.set_value(ATTRIB_COLOR,mv.color);
		vd->push_back(v);
	}

	vd->indices_reserve(m->m_triangles.size()*3);
	for(const MeshTriangle& t : m->m_triangles)
	{
		for(int i  =0 ; i<3;i++)
			vd->push_back(static_cast<uint32_t>(t(i)));
	}
	return vd;
}

uint32_t MeshOps::insert_vertex(Mesh *m, const MeshVertex &v, const int t)
{
	auto vid = m->add_vertex(v);
	MeshTriangle tt = m->m_triangles[t];

	MeshTriangle newts[3];

	newts[0] = MeshTriangle(tt(0),tt(1),vid);
	newts[1] = MeshTriangle(tt(1),tt(2),vid);
	newts[2] = MeshTriangle(tt(2),tt(0),vid);

	// instead of deleting to old triangle, reuse it as the first one!
	m->m_triangles[t] = tt;
	m->update_triangle(t);

	// add the two new ones.
	m->add_triangle(newts[1]);
	m->add_triangle(newts[2]);
	return vid;

}




}
