#include "vdman.h"
#include <fstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;


namespace ofl {
using namespace ofl;

#define RESET_PRIMITIVE 0x0000FFFF


VertexDataManufacturer::VertexDataManufacturer()
{
	this->m_input_primitive = TRIANGLES;
	this->m_normal_state = vec3(0, 0, 1);
	this->tex_coord_state = vec3(0, 0, 0);
	this->m_color_state = vec4(1, 1, 1, 1);
	this->current_mesh = new VertexData();

}
VertexDataManufacturer::~VertexDataManufacturer()
{
	delete current_mesh;
}


void VertexDataManufacturer::begin(Primitive primitive)
{
	m_input_primitive = primitive;
}

void VertexDataManufacturer::handlePrimitiveBuffer()
{
	if (primitive_buffer.empty())
		return;

	if (m_input_primitive == QUADS)
	{
		if (primitive_buffer.size() == 4)
		{

			current_mesh->push_back(primitive_buffer[0]);
			current_mesh->push_back(primitive_buffer[1]);
			current_mesh->push_back(primitive_buffer[2]);
			current_mesh->push_back(primitive_buffer[0]);
			current_mesh->push_back(primitive_buffer[2]);
			current_mesh->push_back(primitive_buffer[3]);
			primitive_buffer.clear();
		}
	}
	else
	{
		current_mesh->push_back(primitive_buffer[0]);
		primitive_buffer.clear();
	}
	return;
}



#if 0
/**
This will end the current primitive run.
Note: If you combine different primitive types the whole patch will work
in TRIANGLES/LINES mode.
You can however combine TRIANGLE_STRIP and QUAD_STRIP since they
are both represented in TRIANGLE_STRIPs.
*/
void VertexDataManufacturer::end()
{


}

#endif

VertexData* VertexDataManufacturer::finish()
{
	Primitive res_prim = m_input_primitive;
	
	if(res_prim == QUADS)
		res_prim = TRIANGLES;
	if(res_prim == QUAD_STRIP)
		res_prim = TRIANGLE_STRIP;
	 
	current_mesh->setPrimitive(res_prim);
	auto res = this->current_mesh;
	this->current_mesh = new VertexData();
	this->vertex_ids.clear();
	return res;
}



void VertexDataManufacturer::color(const vec4& color)
{
	if (!vecIsNaN(color))
		m_color_state = color;
}
void VertexDataManufacturer::color(const vec3& color)
{
	if (!vecIsNaN(color))
		m_color_state = vec4(color,1.0f);
}

void VertexDataManufacturer::color(
		const float& red,
		const float& green,
		const float& blue,
		const float& alpha)
{
	m_color_state = vec4(red, green, blue, alpha);
}


void VertexDataManufacturer::normal(const vec3& normal)
{
	if (!vecIsNaN(normal))
		m_normal_state = normal;
}

void VertexDataManufacturer::normal(const float& x,
									const float& y ,
									const float& z )
{
	m_normal_state = vec3(x,y,z);
}

void VertexDataManufacturer::texCoord(const vec3& tc)
{
	if (!vecIsNaN(tc))
		this->tex_coord_state = tc;
}

void VertexDataManufacturer::texCoord(const vec2& tc)
{
	if (!vecIsNaN(tc))
		this->tex_coord_state = vec3(tc,0.0f);
}
void VertexDataManufacturer::texCoord(
		const float& s,
		const float& t ,
		const float& r )
{
	this->tex_coord_state = vec3(s,t,r);
}

/** creates a vertex using the setup values for color, normal and uv.
*/
void VertexDataManufacturer::vertex(const vec4& vertex)
{
	const vec3  vert = vec3(vertex.x,vertex.y,vertex.z);
	Vertex v(vert, m_normal_state, m_color_state, tex_coord_state);
	uint32_t id = 0;

	if (this->vertex_ids.find(v) != vertex_ids.end())
	{
		id = vertex_ids[v];
	}
	else
	{
		id = this->current_mesh->data().size();
		this->current_mesh->push_back(v);
		vertex_ids[v] = id;
	}

	this->primitive_buffer.push_back(id);
	this->handlePrimitiveBuffer();

}

/** Internal shortcut for normal and tangent caluclations.
*/
void VertexDataManufacturer::vertex(const Vertex& vertex)
{
	Vertex v = vertex;
	uint32_t id = 0;

	if (this->vertex_ids.find(v) != vertex_ids.end())
	{
		id = vertex_ids[v];
	}
	else
	{
		id = current_mesh->data().size();
		current_mesh->push_back(v);
		vertex_ids[v] = id;
	}

	this->primitive_buffer.push_back(id);
	this->handlePrimitiveBuffer();

}


void VertexDataManufacturer::vertex(const vec2& vertex)
{
	this->vertex(vec4(vertex, 0.0f, 1.0f));
}
void VertexDataManufacturer::vertex(const vec3& vertex)
{
	this->vertex(vec4(vertex, 1.0f));
}
void VertexDataManufacturer::vertex(
		const float& x,
		const float& y ,
		const float& z ,
		const float& w)
{
	this->vertex(vec4(x, y, z, w));
}



VertexData* VertexDataManufacturer::createBox(
		float x ,
		float y ,
		float z)
{

	this->begin(QUADS);
	float i = 0.5f;
	vec3 a(-i*x, -i*y, i*z);
	vec3 b(i*x, -i*y, i*z);
	vec3 c(-i*x, i*y, i*z);
	vec3 d(i*x, i*y, i*z);
	vec3 e(-i*x, -i*y, -i*z);
	vec3 f(i*x, -i*y, -i*z);
	vec3 g(-i*x, i*y, -i*z);
	vec3 h(i*x, i*y, -i*z);

	vec3 n(0, 0, 1);
	this->addQuad(a, b, d, c,
				  vec2(0.4, 0.0),vec2(0.7,0.0), vec2(0.7,0.3), vec2(0.4,0.3),
				  n,n,n,n);

	n = vec3(1, 0, 0);
	this->addQuad(b, f, h, d,
				  vec2(1.0, 0.3),vec2(1.0, 0.6), vec2(0.7, 0.6), vec2(0.7, 0.3),
				  n, n, n, n);

	n = vec3(0, 0, -1);
	this->addQuad(f, e, g, h,
				  vec2(0.7, 0.9),vec2(0.4, 0.9), vec2(0.4, 0.6), vec2(0.7, 0.6),
				  n, n, n, n);

	n = vec3(-1, 0, 0);
	this->addQuad(e, a, c, g,
				  vec2(0.1, 0.6),vec2(0.1, 0.3), vec2(0.4, 0.3), vec2(0.4, 0.6),
				  n, n, n, n);

	n = vec3(0, 1, 0);
	this->addQuad(c, d, h, g,
				  vec2(0.4, 0.3),vec2(0.7, 0.3), vec2(0.7, 0.6), vec2(0.4, 0.6),
				  n, n, n, n);

	n = vec3(0, -1, 0);
	this->addQuad(a, b, f, e,
				  vec2(0.0, 0.7),vec2(0.3, 0.7), vec2(0.3, 1.0), vec2(0.0, 1.0),
				  n, n, n, n);

	auto res = this->finish();
	this->calculateTangents(res);
	return res;

}

//******************************************************************************
VertexData* VertexDataManufacturer::createPlane(
		float w ,
		float h ,
		unsigned int tess_w ,
		unsigned int tess_h)
{
	float d_w = w / tess_w;
	float d_h = h / tess_h;

	vec3 offset = vec3(-w / 2, -h / 2, 0);
	this->begin(QUADS);
	normal(0.0f, 0.0f, 1.0f);

	for (unsigned int x = 0; x < tess_w; x++)
	{
		for (unsigned int y = 0; y < tess_h; y++)
		{
			this->addQuad(
						vec3((x + 0)*d_w, (y + 0)*d_h, 0) + offset,
						vec3((x + 1)*d_w, (y + 0)*d_h, 0) + offset,
						vec3((x + 1)*d_w, (y + 1)*d_h, 0) + offset,
						vec3((x + 0)*d_w, (y + 1)*d_h, 0) + offset,
						vec2((float)(x + 0) / tess_w, (float)(y + 0) / tess_h),
						vec2((float)(x + 1) / tess_w, (float)(y + 0) / tess_h),
						vec2((float)(x + 1) / tess_w, (float)(y + 1) / tess_h),
						vec2((float)(x + 0) / tess_w, (float)(y + 1) / tess_h));
		}
	}

	auto res = this->finish();
	this->calculateTangents(res);
	return res;
}

VertexData* VertexDataManufacturer::createCoordinateSystem()
{
	this->begin(LINES);
	this->color(vec3(0.9f, 0.0f, 0.0f));
	this->vertex(0.0f, 0.0f, 0.0f);
	this->vertex(1.0f, 0.0f, 0.0f);

	this->vertex(1.0f, 0.05f, 0.0f);
	this->vertex(1.0f, -0.05f, 0.0f);

	this->vertex(1.0f, 0.05f, 0.0f);
	this->vertex(1.05f, 0.0f, 0.0f);
	this->vertex(1.05f, 0.0f, 0.0f);
	this->vertex(1.0f, -0.05f, 0.0f);

	this->color(vec3(0.0f, 0.9f, 0.0f));
	this->vertex(0.0f, 0.0f, 0.0f);
	this->vertex(0.0f, 1.0f, 0.0f);

	this->vertex(0.05f, 1.0f, 0.0f);
	this->vertex(-0.05f, 1.0f, 0.0f);

	this->vertex(0.05f, 1.0f, 0.0f);
	this->vertex(0.00f, 1.05f, 0.0f);
	this->vertex(0.00f, 1.05f, 0.0f);
	this->vertex(-0.05f, 1.0f, 0.0f);

	this->color(vec3(0.0f, 0.0f, 0.9f));
	this->vertex(0.0f, 0.0f, 0.0f);
	this->vertex(0.0f, 0.0f, 1.0f);

	this->vertex(0.0f, 0.05f, 1.0f);
	this->vertex(0.0f, -0.05f, 1.0f);

	this->vertex(0.0f, 0.05f, 1.0f);
	this->vertex(0.0f, 0.0f, 1.05f);
	this->vertex(0.0f, 0.0f, 1.05f);
	this->vertex(0.0f, -0.05f, 1.0f);

	return this->finish();
}

//******************************************************************************

vec2 colorTexCoordFramPosition(const vec3& pos,const float& offset)
{
	const float pi = 3.14159265359f;
	vec2 uv = vec2(pos.x, pos.y);
	float factor = 1.0f - (sin(fabs(pos.z) * pi)*0.1f);
	uv *= factor;
	uv += vec2(1);
	uv *= vec2(0.5);
	uv += (offset);

	uv *= M_SQRT2 / (1.0 + M_SQRT2);
	return uv;
}

VertexData* VertexDataManufacturer::createUVSphere(
		float radius ,
		unsigned int slices ,
		unsigned int stacks )
{
	this->begin(TRIANGLES);
	const float pi = 3.14159265359f;
	const float st_step = pi / stacks;
	const float sl_step = pi*2.0f / slices;
	for (unsigned int st = 0; st < stacks; st++)
	{
		const float st_r1 = st_step*st;
		const float st_r2 = st_step*st + st_step;
		for (unsigned int sl = 0; sl < slices; sl++)
		{
			const float sl_r1 = sl_step*sl;
			const float sl_r2 = sl_step*sl + sl_step;

			const float sin_o1 = sin(st_r1);
			const float sin_o2 = sin(st_r2);

			const float cos_o1 = cos(st_r1);
			const float cos_o2 = cos(st_r2);

			const float sin_d1 = sin(sl_r1);
			const float sin_d2 = sin(sl_r2);

			const float cos_d1 = cos(sl_r1);
			const float cos_d2 = cos(sl_r2);

			float tex_offset = 0.0f;
			if (st >= stacks / 2u)
				tex_offset = M_SQRT2*0.5f;


			/*Compute positions and texCoords for the current quad:*/
			vec3 pos[4];
			vec2 uv[4];
			pos[0] = vec3(sin_o1*cos_d1, sin_o1*sin_d1, cos_o1);
			pos[1] = vec3(sin_o2*cos_d1, sin_o2*sin_d1, cos_o2);
			pos[2] = vec3(sin_o2*cos_d2, sin_o2*sin_d2, cos_o2);
			pos[3] = vec3(sin_o1*cos_d2, sin_o1*sin_d2, cos_o1);

			for (int i = 0; i < 4;i++)
				uv[i] = colorTexCoordFramPosition(pos[i], tex_offset);

			/* Draw a quad unless you are about do draw the last and the firrst
			   stack.
			*/
			if (st != 0 && st< stacks - 1)
			{
				texCoord(uv[0]);
				normal(pos[0]);
				vertex(pos[0]*radius);

				texCoord(uv[1]);
				normal(pos[1]);
				vertex(pos[1]*radius);

				texCoord(uv[2]);
				normal(pos[2]);
				vertex(pos[2]*radius);

				texCoord(uv[0]);
				normal(pos[0]);
				vertex(pos[0]*radius);

				texCoord(uv[2]);
				normal(pos[2]);
				vertex(pos[2]*radius);

				texCoord(uv[3]);
				normal(pos[3]);
				vertex(pos[3]*radius);
			}
			else
			{
				/* If it is the last stack:*/
				if (st != 0)
				{
					texCoord(uv[0]);
					normal(pos[0]);
					vertex(pos[0]*radius);

					texCoord(uv[2]);
					normal(pos[2]);
					vertex(pos[2]*radius);

					texCoord(uv[3]);
					normal(pos[3]);
					vertex(pos[3]*radius);
				}
				/* If it is the first stack:*/
				else
				{
					texCoord(uv[0]);
					normal(pos[0]);
					vertex(pos[0]*radius);

					texCoord(uv[1]);
					normal(pos[1]);
					vertex(pos[1]*radius);

					texCoord(uv[2]);
					normal(pos[2]);
					vertex(pos[2]*radius);
				}
			}
		}
	}

	auto res = this->finish();
	this->calculateTangents(res);
	return res;
}

//******************************************************************************

VertexData* VertexDataManufacturer::createCylinder(
		float radius,
		float height ,
		unsigned int slices ,
		unsigned int stacks )
{
	this->begin(QUADS);


	float nz = 0;

	/*Used to implement the UV mapping*/
	const float twoPi = 2.0f*3.14159265359f;
	float ar = height / (twoPi*radius);
	if (ar > 1)
		ar = 1;

	float circum_step = 2.0f*3.14159265359f / slices;
	float height_step = height / stacks;
	for (unsigned int st = 0; st < stacks ; st++) // stacks
	{
		float zlow = st *  height_step;
		float zhigh = (1+st) * height / stacks;
		for (unsigned int i = 0; i < slices; i++) // slices
		{

			/* Some precomputations we need for both normals and positions */
			float ax, bx, ay, by;
			ax = sin((i + 0)*circum_step);
			ay = cos((i + 0)*circum_step);
			bx = sin((i + 1)*circum_step);
			by = cos((i + 1)*circum_step);

			/* Create a quad.*/
			this->texCoord(((i + 0)*circum_step) / twoPi, zlow / height);
			normal(normalize(vec3(ax, ay, nz)));
			this->vertex(radius*ax, radius*ay, zlow);

			this->texCoord(((i + 1)*circum_step) / twoPi, zlow / height);
			normal(normalize(vec3(bx, by, nz)));
			this->vertex(radius*bx, radius*by, zlow);

			this->texCoord(((i + 1)*circum_step) / twoPi, zhigh / height);
			normal(normalize(vec3(bx, by, nz)));
			this->vertex(radius*bx, radius*by, zhigh);

			this->texCoord(((i + 0)*circum_step) / twoPi, zhigh / height);
			normal(normalize(vec3(ax, ay, nz)));
			this->vertex(radius*ax, radius*ay, zhigh);
		}
	}
	auto res = this->finish();
	this->calculateTangents(res);
	return res;
}

VertexData* VertexDataManufacturer::createCone(
		float baseRadius,
		float topRadius,
		float height,
		unsigned int slices,
		unsigned int stacks)

{
	this->begin(QUADS);
	float circum_step = 2.0f*3.141595654f / slices;
	float radius_step = (topRadius - baseRadius) / stacks;
	float height_step = height / stacks;
	const float inner_radius = 0.01f;
	float nz = tan((baseRadius - topRadius) / height);

	for (unsigned int st = 0; st < stacks; st++) // stacks
	{
		float zlow = st *  height_step;
		float zhigh = (1 + st) * height / stacks;
		float rlow = baseRadius + st*radius_step;
		float rhigh = baseRadius + (st + 1)*radius_step;

		float uv_r_low = ((1.0f) - ((float)st / stacks));
		uv_r_low = (0.5f - inner_radius) * uv_r_low + inner_radius;

		float uv_r_high = ((1.0f) - ((1.0f + (float)st) / stacks));
		uv_r_high = (0.5f - inner_radius) * uv_r_high + inner_radius;;

		for (unsigned int i = 0; i < slices; i++) // slices
		{
			/* Some precomputations we need for both normals and positions */
			float ax, bx, ay, by;
			ax = sin((i + 0)*circum_step);
			ay = cos((i + 0)*circum_step);
			bx = sin((i + 1)*circum_step);
			by = cos((i + 1)*circum_step);

			/* Create a quad.*/
			this->texCoord(uv_r_low * ax + 0.5f, uv_r_low*ay + 0.5f);
			normal(normalize(vec3(ax, ay, nz)));
			this->vertex(rlow*ax, rlow*ay, zlow);

			this->texCoord(uv_r_low * bx + 0.5f, uv_r_low*by + 0.5f);
			normal(normalize(vec3(bx, by, nz)));
			this->vertex(rlow*bx, rlow*by, zlow);

			this->texCoord(uv_r_high * bx + 0.5f, uv_r_high*by + 0.5f);
			normal(normalize(vec3(bx, by, nz)));
			this->vertex(rhigh*bx, rhigh*by, zhigh);

			this->texCoord(uv_r_high * ax + 0.5f, uv_r_high*ay + 0.5f);
			normal(normalize(vec3(ax, ay, nz)));
			this->vertex(rhigh*ax, rhigh*ay, zhigh);
		}
	}
	auto res = this->finish();
	this->calculateTangents(res);
	return res;
}


VertexData* VertexDataManufacturer::createDisk(
		float innerRadius ,
		float outerRadius ,
		unsigned int slices ,
		unsigned int loops )
{

	this->begin(TRIANGLES);
	float circum_step = 2.0f*3.141595654f / slices;
	float radius_step = (outerRadius - innerRadius) / loops;

	normal(vec3(0, 0, 1));
	for (unsigned int st = 0; st < loops; st++) // stacks
	{
		float rlow = innerRadius + st*radius_step;
		float rhigh = innerRadius + (st + 1)*radius_step;
		for (unsigned int i = 0; i < slices; i++) // slices
		{

			/* Some precomputations we need for both normals and positions */
			float ax, bx, ay, by;
			ax = sin((i + 0)*circum_step);
			ay = cos((i + 0)*circum_step);
			bx = sin((i + 1)*circum_step);
			by = cos((i + 1)*circum_step);

			/* Create a quad.*/

			vec3 pos[4];
			vec2 uv[4];
			pos[0] = vec3(rlow*ax, rlow*ay, 0);
			pos[1] = vec3(rlow*bx, rlow*by, 0);
			pos[2] = vec3(rhigh*bx, rhigh*by, 0);
			pos[3] = vec3(rhigh*ax, rhigh*ay, 0);

			for (int i = 0; i < 4; i++)
			{
				uv[i] = vec2(pos[i].x / (2 * outerRadius) + 0.5,
							 pos[i].y / (2 * outerRadius) + 0.5);
			}

			if (st != 0)
			{
				this->texCoord(uv[0]);
				this->vertex(pos[0]);
				this->texCoord(uv[1]);
				this->vertex(pos[1]);
				this->texCoord(uv[2]);
				this->vertex(pos[2]);
			}
			this->texCoord(uv[0]);
			this->vertex(pos[0]);
			this->texCoord(uv[2]);
			this->vertex(pos[2]);
			this->texCoord(uv[3]);
			this->vertex(pos[3]);
		}
	}
	auto res = this->finish();
	this->calculateTangents(res);
	return res;
}



void VertexDataManufacturer::addVertex(vec3 p, vec2 t, vec3 n, vec4 c)
{
	texCoord(t);
	normal(n);
	color(c);
	vertex(p);
}

void VertexDataManufacturer::addTriangle(
		vec3 p1, vec3 p2, vec3 p3,
		vec2 t1  , vec2 t2  , vec2 t3  ,
		vec3 n1  , vec3 n2  , vec3 n3 ,
		vec4 c1  , vec4 c2  , vec4 c3
		)
{
	vec3 n = normalize(cross(p2 - p1, p3 - p1));
	if (vecIsNaN(n1))
	{
		n1 = n;
	}
	if (vecIsNaN(n2))
	{
		n2 = n;
	}
	if (vecIsNaN(n3))
	{
		n3 = n;
	}

	if (m_input_primitive == QUADS ||
			m_input_primitive == QUAD_STRIP)
		return;
	if (m_input_primitive == LINES)
	{
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);

		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);

		addVertex(p3, t3, n3, c3);
		addVertex(p1, t1, n1, c1);
	}

	else if (m_input_primitive == LINE_STRIP)
	{
		this->current_mesh->push_back(RESET_PRIMITIVE);
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);
		addVertex(p1, t1, n1, c1);
	}
	else
	{
		if (m_input_primitive == TRIANGLE_STRIP)
			this->current_mesh->push_back(RESET_PRIMITIVE);
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);
	}
}




void VertexDataManufacturer::addQuad(
		vec3 p1, vec3 p2,
		vec3 p3, vec3 p4,
		vec2 t1, vec2 t2,
		vec2 t3, vec2 t4,
		vec3 n1, vec3 n2,
		vec3 n3, vec3 n4,
		vec4 c1, vec4 c2,
		vec4 c3, vec4 c4
		)
{
	if (vecIsNaN(n1))
	{
		n1 = m_normal_state;
	}
	if (vecIsNaN(n2))
	{
		n2 = m_normal_state;
	}
	if (vecIsNaN(n3))
	{
		n3 = m_normal_state;
	}
	if (vecIsNaN(n4))
	{
		n4 = m_normal_state;
	}

	if (m_input_primitive == TRIANGLES)
	{
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);

		addVertex(p1, t1, n1, c1);
		addVertex(p3, t3, n3, c3);
		addVertex(p4, t4, n4, c4);
	}
	if (m_input_primitive == QUADS)
	{
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);
		addVertex(p4, t4, n4, c4);
	}
	else if (m_input_primitive == LINES)
	{
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);

		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);

		addVertex(p3, t3, n3, c3);
		addVertex(p4, t4, n4, c4);

		addVertex(p4, t4, n4, c4);
		addVertex(p1, t1, n1, c1);
	}

	else if (m_input_primitive == LINE_STRIP)
	{
		this->current_mesh->push_back(RESET_PRIMITIVE);
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);
		addVertex(p4, t4, n4, c4);
		addVertex(p1, t1, n1, c1);
	}
	else
	{
		if (m_input_primitive == TRIANGLE_STRIP ||
				m_input_primitive == QUAD_STRIP)
			this->current_mesh->push_back(RESET_PRIMITIVE);
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);
		addVertex(p4, t4, n4, c4);
	}

}

}
