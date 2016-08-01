#pragma once


#include <vector>
#include <map>
#include <limits>
#include <numeric>
#include <cstdio>
#include <cstring>
#include <limits>
#include <vector>

#include "vd.h"

namespace ofl
{

class VertexDataManufacturer : public ofl::VertexDataTools
{

private:


	/**
	 * @brief m_normal_state Current normal state. With any call of .vertex(..)
	 * a vertex is created with this as a normal.
	 */
	vec3 m_normal_state;
	/**
	 * @brief m_color_state Current normal state. With any call of .vertex(..)
	 * a vertex is created with this as a color.
	 */
	vec4 m_color_state;
	/**
	 * @brief m_tex_coord_state Current normal state. With any call of
	 * .vertex(..) a vertex is created with this as a texture coordinate.
	 */
	vec3 tex_coord_state;

	/**
	 * @brief m_input_primitive The primitive mode chosen by begin(..).
	 */
	ofl::Primitive m_input_primitive;

private:
	/** Datastructures supporting the begin/end/finish operations.*/
	// A map to check if given vertex already exists and where it is.
	std::map<ofl::Vertex, unsigned int> vertex_ids;
	ofl::VertexData* current_mesh;

	// A primitive buffer to deal with quads.
	std::vector<unsigned int> primitive_buffer;

	void handlePrimitiveBuffer();

public:

	VertexDataManufacturer();
	~VertexDataManufacturer();

	/**
	 * @brief begin  Will set the GeomtryDataLoader in the corresponding
		OpenGL primitive mode.

		Supported modes are:
		TRIANGLES,
		POINTS,
		LINES,
		QUADS,

		LINE_STRIP,
		TRIANGLE_STRIP,
		QUAD_STRIP

		Note: Geometry created in this way is as dense packed as possible.
		For example...

		begin(TRIANGLES);
		vertex(0,0);
		vertex(1,0);
		vertex(0,1);

		vertex(0,1);
		vertex(1,0);
		vertex(1,1);
		finish();

		... will return the two triangles with four vertices.
	 * @param primitive
	 */
	void begin(ofl::Primitive primitive);



	/**
	 * @brief finish An alternative to end(). This will create a renderable
	 * geometry. After calling finish the manufacturer will then be empty and
	 * reusable. Creating a geometry once and rendering it again and again will
	 * be a lot faster then creating the geometry with begin(..) end() all over
	 * again. Note: You will have to free the geometry by yourself
	 * @return A Vertex data struct containing the vertex information.
	 */
	ofl::VertexData* finish();


	/**
	 * @brief color Sets the current color state
	 * @param color The color you wish to set.
	 */
	void color(const vec4& color);

	/**
	 * @brief color Sets the current color state
	 * @param color The color you wish to set.
	 */
	void color(const vec3& color);

	/**
	 * @brief color Sets the current color state.
	 * @param red The red part.
	 * @param green The green part.
	 * @param blue The blue part.
	 * @param alpha The alpha value.
	 */
	void color(
			const float& red,
			const float& green = 0.0f,
			const float& blue = 0.0f,
			const float& alpha = 1.0f);

	/**
	 * @brief normal Sets the current normal state
	 * @param normal The normal you wish to set.
	 */
	void normal(const vec3& normal);
	void normal(
			const float& x,
			const float& y = 0.0f,
			const float& z = 1.0f);

	/**
	 * @brief texCoord Sets the texture coordinate state
	 * @param tc The texture coordinate you want to set.
	 */
	void texCoord(const vec3& tc);
	void texCoord(const vec2& tc);
	void texCoord(const float& s,
				  const float& t = 0.0f,
				  const float& r = 0.0f);

	/**
	*/
	/**
	 * @brief vertex Creates a vertex using the current state values
	 * for color, normal and texture coordinate.
	 * @param vertex The position of the vertex
	 */
	void vertex(const vec4& vertex);
	void vertex(const vec2& vertex);
	void vertex(const vec3& vertex);
	void vertex(
			const float& x,
			const float& y = 0.0f,
			const float& z = 0.0f,
			const float& w = 1.0f);
	void vertex(const float * vertex);

	void vertex(const ofl::Vertex& vertex);


	/**
	 * @brief createBox Will create all vertices and faces needed to render a
	 * box. Texture coordinates and normals will be correct. The UV layout is:
	 *
	 *     0,1______  ______  0.7,0.9
	 *       |      ||      |
	 *       | BTM  ||	-z  |
	 *  0,0.7|______||______|______ 1,0.6
	 * 0.1,0.6|      |      |      |
	 *        |  -x  |  TOP |   +x |
	 * 0.1,0.3|______|______|______| 1.0.3
	 *               |      |
	 *               |  +z  |
	 *         0.4,0 |______| 0.7,0
	 *
	 *
	 * @param w width of the box
	 * @param h height of the box
	 * @param d depth of the box
	 * @return
	 */
	ofl::VertexData* createBox(
			float w = 1.0f,
			float h = 1.0f,
			float d = 1.0f);


	/** @brief createPlane Will create all vertices and faces needed to render a
	 * plane. Texture coordinates and normals will be correct. The UV layout is
	 *   0,1 ______1,1
	 *      |      |
	 *      |      |
	 *      |______|
	 *   0,0       1,0
	 * The primitive mode will be a TRIANGLE_STRIP.
	 * @param w width of the plane
	 * @param h height of the plane
	 * @param tess_w tesselation in width
	 * @param tess_h tesselation in height
	 * @return
	 */

	ofl::VertexData* createPlane(
			float w = 1.0f,
			float h = 1.0f,
			unsigned int tess_w = 1,
			unsigned int tess_h = 1);


	/**
	 * @brief createCoordinateSystem Will create a colorfull coordinate system
	 * @return The coordinate system
	 */
	ofl::VertexData* createCoordinateSystem();


	/**
	 * @brief createUVSphere Will create all vertices and faces needed to
	 * render a UV-Shpere.  Normals will be set correct!
	 *
	 *     0,1 _____________1,1
	 *        |       /    \|		The top and bottom hemisphere are seperated.
	 *        | TOP> |      |
	 *        |       \____/|
	 *        |/    \       |
	 *        |      |< BOT	|
	 *        |\____/_______|
	 *		 0,0            1,0
	 *
	 * @param radius of the sphere.
	 * @param slices the number of slices between the poles, parallel to the
	 *        equator. This number should allways be odd! otherwise the default
	 *        uv-coordinates wont work.
	 * @param stacks
	 * @return
	 */
	ofl::VertexData* createUVSphere(
			float radius = 1,
			unsigned int slices = 32,
			unsigned int stacks = 16);



	/**
	 * @brief createCylinder Will create all vertices and faces needed to render
	 * a cylinder. Normals and tangents will be set correct. The UV layout is
	 *
	 *         circumference
	 *     0,1 ____________1,1
	 *        |             | h
	 *        |             | e
	 *        |             | i
	 *        |             | g
	 *        |             | h
	 *        |_____________| t
	 *		 0,0       q+.01,0
	 *
	 * @param radius of the cylinder.
	 * @param height of the cylinder.
	 * @param slices segments around the circumference of the cylinder
	 * @param stacks segments along the length of the cylinder
	 * @return
	 */
	ofl::VertexData* createCylinder(
			float radius = 1,
			float height = 1,
			unsigned int slices = 32,
			unsigned int stacks = 1);


	/**
	 * @brief createCone Will create a cone.
	 * Normals and tangents will be set correctly.
	 * The UV layout is
	 *
	 *     0,1 ____________1,1
	 *        | /         \ |    T is  the  top border  with a raduis of 0.01.
	 *        |/     _T    \|    The  outer circle   b  has  a  radius  of 0.5.
	 *        |     / \     |    Note: this  mapping is  far from good, but it
	 *        |     \_/     |    is ok to use it, especially for pointed cones.
	 *        |\          b/|
	 *        |_\_________/_|
	 *		 0,0       q+.01,0
	 *
	 * @param baseRadius the base radius on the XY-plane
	 * @param topRadius the top radius, parallel to the XY-plane
	 * @param height hight of the cone
	 * @param slices segments around the circumference of the cone
	 * @param stacks segments along the length of the cone
	 * @return
	 */
	ofl::VertexData* createCone(
			float baseRadius = 1,
			float topRadius = 0,
			float height = 1,
			unsigned int slices = 32,
			unsigned int stacks = 1);


	/**
	 * @brief createDisk Will create a disk.
	 *  Normals and tangents will be set correctly.
	 * The UV layout is
	 *     0,1 ____________1,1
	 *        | /         \ |    T has the  inner radius/outer radius.
	 *        |/     _T    \|    The  outer circle   b  has  a  radius of 2 Pi.
	 *        |     / \     |
	 *        |     \_/     |
	 *        |\          b/|
	 *        |_\_________/_|
	 *		 0,0       q+.01,0
	 * @param innerRadius the inner radius of the disk.
	 * @param outerRadius the outer radius of the disk.
	 * @param slices segments around the circumference of the disk(Pizza slices)
	 * @param loops loops between inner and outer radius
	 * @return
	 */
	ofl::VertexData* createDisk(
			float innerRadius = 0,
			float outerRadius = 1,
			unsigned int slices = 32,
			unsigned int loops = 1);



	/** Shortcut function to add a vertex with parameters.
		The same operation like:
		.texCoord(t);
		.normal(n);
		.color(c);
		.vertex(p);
	*/
	void addVertex(vec3 p, vec2 t, vec3 n, vec4 c);

	/** Shortcut function to add a whole triangle with parameters.
	The same result like:
	addVertex(p1,t1,n1,c1);
	addVertex(p2,t2,n2,c2);
	addVertex(p3,t3,n3,c3);
	*/
	void addTriangle(
			vec3 p1          , vec3 p2          , vec3 p3,
			vec2 t1= vec2NaN, vec2 t2= vec2NaN, vec2 t3= vec2NaN,
			vec3 n1= vec3NaN, vec3 n2= vec3NaN, vec3 n3= vec3NaN,
			vec4 c1= vec4NaN, vec4 c2= vec4NaN, vec4 c3= vec4NaN
			);


	void addQuad(
			vec3 p1          , vec3 p2          ,
			vec3 p3          , vec3 p4          ,
			vec2 t1 = vec2NaN, vec2 t2 = vec2NaN,
			vec2 t3 = vec2NaN, vec2 t4 = vec2NaN,
			vec3 n1 = vec3NaN, vec3 n2 = vec3NaN,
			vec3 n3 = vec3NaN, vec3 n4 = vec3NaN,
			vec4 c1 = vec4NaN, vec4 c2 = vec4NaN,
			vec4 c3 = vec4NaN, vec4 c4 = vec4NaN
			);

};

}
