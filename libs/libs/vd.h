#pragma once
#include "vmath.h"

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



class VertexData
{
private:
	std::vector<Vertex> m_data;
	std::vector<uint32_t> m_indices_data;
	Primitive m_render_primitive;

public:
	VertexData(Primitive primitive = TRIANGLES);
	virtual ~VertexData();


	void push_back(const uint32_t& i)
	{
		this->m_indices_data.push_back(i);
	}
	size_t push_back(const Vertex& v);

	const std::vector<Vertex>& data() const;
	const std::vector<uint32_t>& indices() const;

	std::vector<Vertex>& data();
	std::vector<uint32_t>& indices();

	virtual Primitive primitive() const;
	void setPrimitive(const Primitive& p);

	auto begin() -> decltype(m_data.begin())
	{return m_data.begin();}
	auto end() -> decltype(m_data.end())
	{return m_data.end();}
};


class VertexDataTools
{
protected:
	VertexData* readVD(const std::string& path);
	VertexData* readOBJ(const std::string& path);
	VertexData* readPLY(const std::string& path);

	bool writeVD(const VertexData* ofl, const std::string& path);
	bool writeOBJ(const VertexData* ofl, const std::string& path);
	bool writePLY(const VertexData* ofl, const std::string& path);
public:
	enum Format
	{
		OBJ,
		PLY,
		VD,
		FROM_PATH
	};
	bool writeToFile(const VertexData* ofl,const std::string& path, Format f=FROM_PATH);
	VertexData* readFromFile(const std::string& path, Format f = FROM_PATH);

	void calculateNormals(VertexData* ofl);
	void calculateTangents(VertexData* ofl);
};
}
