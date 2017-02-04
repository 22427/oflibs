#pragma once
#include <glm/glm.hpp>
#include "vd.h"
#include "vmath.h"
#include <vector>
#include <map>
#include <set>
namespace ofl {

#ifndef OFL_INVALID_VERTEX
#define OFL_INVALID_VERTEX 0xFFFFFFFF
#endif

/**
 * @brief The MeshVertex class repesents a vertex in the mesh data structure.
 * MeshVertices are fixed size.
 */
class MeshVertex
{
public:
	vec3 pos;
	vec4 color;
	MeshVertex(const float x, const float y, const float z): pos(x,y,z),color(1.0f)
	{}
	MeshVertex(const vec3& p,const vec4& c=vec4(1.0f)): pos(p),color(c)
	{}
	operator vec3() {return pos;}
	operator const vec3() const {return pos;}
};

/**
 * @brief The MeshTriangle class represents a triangle inside the mesh, referencing
 * the vertices by their id.
 */
class MeshTriangle
{
	friend class MeshOps;
	uint v[3];
public:
	MeshTriangle(const int a=OFL_INVALID_VERTEX,
				 const int b=OFL_INVALID_VERTEX,
				 const int c=OFL_INVALID_VERTEX)
	{
		v[0] = a;
		v[1] = b;
		v[2] = c;
	}
	bool valid()
	{
		return v[0]<OFL_INVALID_VERTEX&&v[1]<OFL_INVALID_VERTEX&&v[2]<OFL_INVALID_VERTEX;
	}

	uint& operator()(const uint i){return v[i];}
	const uint& operator()(const uint i) const {return v[i];}

	uint& operator()(const int i){return v[i];}
	const uint& operator()(const int i) const {return v[i];}

	bool operator ==(const MeshTriangle& o) const
	{
		bool res = true;
		for(int i = 0 ; i<3; i++)
			res = res && v[i] == o.v[i];
		return res;
	}

	uint* begin(){return v;}
	uint* end(){return v+3;}

	const uint* begin()const {return v;}
	const uint* end()const{return v+3;}
};


/**
 * @brief The Mesh class represents a triangle mesh, containing vertices and triangles.
 * The used structures aren't very memory efficient, but the access times should
 * be well enough.
 */
class Mesh
{
protected:
	friend class MeshOps;

	std::vector<MeshVertex> m_verts;
	std::vector<MeshTriangle> m_triangles;


	std::vector<std::set<uint>> m_vert2tris;
	std::vector<std::set<uint>> m_vert2vert;

	std::map<vec3,uint,Comperator<vec3>> m_vertex_id_from_position;

	void recalc_v2v(uint vertex_id);
	void rebuild_data_structure();
	void update_triangle(int triangle_id);

public:


	Mesh(const VertexData* vd);
	Mesh(){}

	/**
	 * @brief adjacent_triangles gives access to all adjacesnt triangles to a
	 * given vertex id
	 * @param vertex_id
	 * @return std::set of triangle ids for the adjacent triangles.
	 */
	std::set<uint> adjacent_triangles(int vertex_id);
	/**
	 * @brief adjacent_vertices gives access to all adjacent vertices to a
	 * given vertex id
	 * @param vertex_id
	 * @return std::set of all adjacent vertices.
	 */
	std::set<uint> adjacent_vertices(int vertex_id);

	/*vec3& vertex_position(const uint i){return m_verts[i].pos;}
	const vec3& vertex_position(const uint i)const{return m_verts[i].pos;}

	vec4& vertex_color(const uint i){return m_verts[i].color;}
	const vec4& vertex_color(const uint i)const{return m_verts[i].color;}
*/
	MeshTriangle& triangle(const uint i){return m_triangles[i];}
	const MeshTriangle& triangle(const uint i)const{return m_triangles[i];}


	const MeshVertex& vertex(const uint id)const {return m_verts[id];}
	MeshVertex& vertex(const uint id){return m_verts[id];}


	const std::vector<MeshVertex>& vertices()const {return m_verts;}
	std::vector<MeshVertex>& vertices(){return m_verts;}

	const std::vector<MeshTriangle>& triangles()const{return  m_triangles;}
	std::vector<MeshTriangle>& triangles(){return  m_triangles;}

	/**
	 * @brief remove_rogue_elements removes unconnected vertices and invalid
	 * triangles. They emerge by deleting triangles or vertices. This
	 * contains a rebuild of the internal data structures and changes the indices!
	 */
	void remove_rogue_elements();

	/**
	 * @brief delete_triangle deletes a triangle by removing it from the
	 * vertex adjacent lists and marking it invalid, but it stays in the
	 * triangle array
	 * @param triangle_id of the triangle you wish to delete.
	 */
	void delete_triangle(int triangle_id);

	/**
	 * @brief delete_vertex deletes a vertex by removing it from de adjacent
	 * vertex lists of every adjacent vertex, and marking all adjacent triangles
	 * invalid.The vertex however stays in the vertex array until
	 * remove_rogue_elements() is called.
	 * @param vertex_id
	 */
	void delete_vertex(int vertex_id);



	uint add_triangle(const MeshVertex& a,const MeshVertex& b,const MeshVertex& c);
	uint add_triangle(const MeshTriangle& t);
	uint add_vertex(const MeshVertex& vtx);
};


class MeshOps
{
public:

	/**
	 * @brief getClosestVertex searches for the closest vertex to a given
	 * position. This is a search in O(number of vertices in the mesh)
	 * @param m Mesh
	 * @param p position
	 * @return The vertex id of the closes vertex or -1 if there are no vertices
	 */
	static int get_closest_vertex(Mesh* m, const vec3& p);
	static int get_closest_triangle(Mesh* m, const vec3& p);

	static vec3 get_closest_point(Mesh* m, const vec3& p);
	static vec3 get_closest_point(Mesh* m, const vec3& p,
								  const std::vector<mat_tri> &Ts,
								  const std::vector<mat_tri> &Tis);


	static void average_surfaces(std::vector<Mesh *>& ms);

	static VertexData* to_VertexData(const Mesh* m);

	static uint32_t insert_vertex(Mesh* m, const MeshVertex &v, const int t);
	static Mesh* combine(const Mesh* a, const Mesh*b);
	static Mesh* merge(Mesh* a, const Mesh*b);
};
}
