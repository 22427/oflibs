#pragma once
#include "vd.h"
#include "vmath.h"
#include <vector>
#include <map>
namespace ofl {


class Corner
{
public:
	int opp;

	Corner(int o = -1) : opp(o){}
	operator int()const {return opp;}
};

class Triangle
{
	friend class MeshTools;
	int v[3];
public:
	Triangle(const int a=-1, const int b=-1, const int c=-1)
	{
		v[0] = a;
		v[1] = b;
		v[2] = c;
	}
	int& operator()(const uint i){return v[i];}
	const int& operator()(const uint i) const {return v[i];}

	int& operator()(const int i){return v[i];}
	const int& operator()(const int i) const {return v[i];}

	int getEdgeStart(int a, int b) const
	{
		for(int i = 0 ; i< 3;i++)
		{
			if(v[i] == a && v[(i+1)%3] == b)
				return i;
		}
		return -1;
	}
	bool operator ==(const Triangle& o) const
	{
		bool res = true;
		for(int i = 0 ; i<3; i++)
			res = res && v[i] == o.v[i];
		return res;
	}


	/**
	 * @brief sharedEdgeWith
	 * @param o
	 * @param so
	 * @param st
	 * @return
	 */
	bool sharedEdgeWith(const Triangle o, int& so, int& st) const
	{
		for(int i = 0 ; i< 3;i++)
		{
			so=o.getEdgeStart(v[(i+1)%3],v[i]);
			if( so >= 0)
			{
				st = i;
				return true;
			}
		}
		return false;
	}
};
class Mesh
{
	// per vertex attributes
	std::vector<vec3> m_positions;
	std::vector<Triangle> m_triangles;

	std::vector<int> m_vertex2corner;
	std::vector<Corner> m_corners;

public:
	std::vector<int> adjacentTriangles(uint vertex);
	std::vector<int> adjacentVertices(uint vertex);

	vec3& vertex(const uint i){return m_positions[i];}
	const vec3& vertex(const uint i)const{return m_positions[i];}

	Triangle& triangle(const uint i){return m_triangles[i];}
	const Triangle& triangle(const uint i)const{return m_triangles[i];}

	void buildDataStructure();

	const std::vector<vec3>& positions()const {return m_positions;}
	std::vector<vec3>& positions(){return m_positions;}

	const std::vector<Corner>& corners()const {return m_corners;}
	std::vector<Corner>& corners(){return m_corners;}

	Mesh(const VertexData* vd);
	VertexData* toVertexData();
};


class MeshTools
{
	public:

	/**
	 * @brief getClosestVertex searches for the closest vertex to a given
	 * position. This is a search in O(number of vertices in the mesh)
	 * @param m Mesh
	 * @param p position
	 * @return The vertex id of the closes vertex or -1 if there are no vertices
	 */
	static int getClosestVertex(Mesh* m, const vec3& p);
};
}
