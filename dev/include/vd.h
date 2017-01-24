#pragma once
#include "vmath.h"

/** @include vmath.md */

namespace ofl
{
enum Primitive
{
	POINTS = 0x0000,			// == GL_POINTS
	LINES = 0x0001,				// == GL_LINES
	//LINE_LOOP = 0x0002,		// == GL_LINE_LOOP
	LINE_STRIP = 0x0003,		// == GL_LINE_STRIP
	TRIANGLES = 0x0004,			// == GL_TRIANGLES
	TRIANGLE_STRIP = 0x0005,	// == GL_TRIANGLE_STRIP
	//TRIANGLE_FAN = 0x0006,	// == GL_TRIANGLE_FAN
	QUADS = 0x0007,				// == GL_QUADS
	QUAD_STRIP = 0x0008,		// == GL_QUAD_STRIP
	//POLYGON = 0x0009,			// == GL_POLYGON
};

enum Attribute
{
	POSITION =1,
	NORMAL = 2,
	TEXCOORD = 4,
	TANGENT = 8,
	COLOR = 16,
	ATTRIBUTE_LAST = COLOR
};

enum Types
{
	BYTE =0x1400,				// == GL_BYTE
	UNSIGNED_BYTE =0x1401,		// == GL_UNSIGNED_BYTE
	SHORT =0x1402,				// == GL_SHORT
	UNSIGNED_SHORT= 0x1403,		// == GL_UNSIGNED_SHORT
	INT= 0x1404,				// == GL_INT
	UNSIGNED_INT= 0x1405,		// == GL_UNSIGNED_INT
	FLOAT= 0x1406,				// == GL_FLOAT
	DOUBLE =0x140A,				// == GL_DOUBLE
};
class Vertex
{
	vec3 m_pos;
	vec3 m_normal;
	vec3 m_texcoord;
	vec3 m_tangent;
	vec4 m_color;
public:
	Vertex(const vec3& pos = vec3(0, 0, 0),
		   const vec3& nrm = vec3(0, 0, 0),
		   const vec4& clr = vec4(1, 1, 1, 1),
		   const vec3& tex = vec3(0, 0, 0),
		   const vec3& tan = vec3(0, 0, 0))
	{
		m_pos = pos;
		m_normal = nrm;
		m_color = clr;
		m_texcoord = tex;
		m_tangent = tan;
	}

	const vec3& pos() const { return m_pos; }
	const vec3& nrm() const { return m_normal; }
	const vec4& clr() const { return m_color;}
	const vec3& tex() const { return m_texcoord; }
	const vec3& tan() const { return m_tangent; }

	vec3& pos()  { return m_pos; }
	vec3& nrm()  { return m_normal; }
	vec4& clr()  { return m_color;}
	vec3& tex()  { return m_texcoord; }
	vec3& tan()  { return m_tangent; }

	void setNormal(const vec3& n) { m_normal = n; }
	void setPosition(const vec3& p) { m_pos = p; }
	void setTexcoord(const vec2& u)
	{
		m_texcoord.x = u.x;
		m_texcoord.y = u.y;
		m_texcoord.z = 0;
	}
	void setTexcood(const vec3& u)	{ m_texcoord = u; }
	void setColor(const vec4& c) {	m_color = c; }
	void setTangent(const vec3& t)	{m_tangent = t; }

	bool operator == (const Vertex& o);
	bool operator < (const Vertex& o)	const;
};


/**
 * @brief The VertexData class represents vertex data in a renderable form.
 */
class VertexData
{
private:
	std::vector<Vertex> m_data;
	std::vector<uint32_t> m_indices_data;
	Primitive m_render_primitive;

public:
	VertexData(Primitive primitive = TRIANGLES);
	virtual ~VertexData();
	
	/**
	 * @brief push_back adds an index to the index list
	 * @param i the new index
	 */
	void push_back(const uint32_t& i)
	{
		this->m_indices_data.push_back(i);
	}
	
	/**
	 * @brief push_back adds a Vertex to the vertex list.
	 * @param v The new Vertex
	 * @return the index of the newly added vertex.
	 */
	size_t push_back(const Vertex& v);

	/**
	 * @brief data gives read access to the Vertex list
	 * @return The vertex list.
	 */
	const std::vector<Vertex>& data() const;
	
	/**
	 * @brief indices gives read access to the index list
	 * @return The index list
	 */
	const std::vector<uint32_t>& indices() const;
	
	/**
	 * @brief data gives write access to the Vertex list
	 * @return The vertex list.
	 */
	std::vector<Vertex>& data();
	
	/**
	 * @brief indices gives write access to the index list
	 * @return The index list
	 */
	std::vector<uint32_t>& indices();
	
	/**
	 * @brief primitive 
	 * @return The vertex datas primitive
	 */
	virtual Primitive primitive() const;
	/**
	 * @brief setPrimitive sets the primitive mode this vertex data is 
	 * constructed in.
	 * @param p new primitive 
	 */
	void setPrimitive(const Primitive& p);

	auto begin() -> decltype(m_data.begin())
	{return m_data.begin();}
	auto end() -> decltype(m_data.end())
	{return m_data.end();}
};


class VertexDataTools
{
protected:
	public:
	static VertexData* readVD(const std::string& path);
	static VertexData* readOBJ(const std::string& path);
	static VertexData* readPLY(const std::string& path);
	static VertexData* readOFF(const std::string& path);

	static bool writeVD(const VertexData* vd, const std::string& path);
	static bool writeOBJ(const VertexData* vd, const std::string& path);
	static bool writePLY(const VertexData* vd, const std::string& path);
	static bool writeOFF(const VertexData* vd, const std::string& path);


	enum Format
	{
		OBJ,
		PLY,
		VD,
		OFF,
		FROM_PATH
	};
	/**
	 * @brief writeToFile writes the given vertexdaa into a file.
	 * @param vd Vertex data to write from
	 * @param path Path to write to
	 * @param f Format of the outpu file. if FROM_PATH is used, the format will
	 * be determined from the file ending
	 * @return true if everything went well, false if there was a problem.
	 */
	static bool writeToFile(const VertexData* vd,const std::string& path, Format f=FROM_PATH);
	
	/**
	 * @brief readFromFile reads VertexData from a file
	 * @param path Path to the source file.
	 * @param f The format of the source file
	 * @return the VertexData read, or a nullptr, if something went wrong.
	 */
	static VertexData* readFromFile(const std::string& path, Format f = FROM_PATH);

	static void calculateNormals(VertexData* vd);
	static void calculateTangents(VertexData* vd);


};
}
