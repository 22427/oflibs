#include "vdman.h"
#include <fstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>


namespace ofl {

void VertexDataManufactory::handle_primitive_buffer()
{
	if (primitive_buffer.empty())
		return;

	if (m_input_primitive == QUADS)
	{
		if (primitive_buffer.size() == 4)
		{

			m_current_mesh->push_back(primitive_buffer[0]);
			m_current_mesh->push_back(primitive_buffer[1]);
			m_current_mesh->push_back(primitive_buffer[2]);
			m_current_mesh->push_back(primitive_buffer[0]);
			m_current_mesh->push_back(primitive_buffer[2]);
			m_current_mesh->push_back(primitive_buffer[3]);
			primitive_buffer.clear();
		}
	}
	else
	{
		m_current_mesh->push_back(primitive_buffer[0]);
		primitive_buffer.clear();
	}
	return;
}

VertexDataManufactory::~VertexDataManufactory()
{
	if(m_current_mesh)
		delete  m_current_mesh;
}

void VertexDataManufactory::begin(Primitive primitive)
{
	if(!m_current_mesh)
		m_current_mesh = new VertexData(primitive,m_cfg,m_index_type);
	m_input_primitive = primitive;
}

VertexData *VertexDataManufactory::finish()
{
	Primitive res_prim = m_input_primitive;

	if(res_prim == QUADS)
		res_prim = TRIANGLES;
	if(res_prim == QUAD_STRIP)
		res_prim = TRIANGLE_STRIP;

	m_current_mesh->set_primitive(res_prim);

	auto res = m_current_mesh;
	m_current_mesh = nullptr;
	m_vertex_ids.clear();
	return res;
}

void VertexDataManufactory::color(const vec4 &color)
{
	m_attribute_state[ATTRIB_COLOR] = color;
}

void VertexDataManufactory::color(const vec3 &color)
{
	m_attribute_state[ATTRIB_COLOR] = vec4(color,1.0f);
}

void VertexDataManufactory::color(const float &red, const float &green, const float &blue, const float &alpha)
{
	m_attribute_state[ATTRIB_COLOR] = vec4(red,green,blue,alpha);
}

void VertexDataManufactory::normal(const vec3 &normal)
{
	m_attribute_state[ATTRIB_NORMAL] = vec4(normal,0.0f);
}

void VertexDataManufactory::normal(const float &x, const float &y, const float &z)
{
	m_attribute_state[ATTRIB_NORMAL] = vec4(x,y,z,0.0f);
}

void VertexDataManufactory::tex_coord(const vec3 &tc)
{
	m_attribute_state[ATTRIB_TEXCOORD] = vec4(tc,0.0f);
}

void VertexDataManufactory::tex_coord(const vec2 &tc)
{
	m_attribute_state[ATTRIB_TEXCOORD] = vec4(tc,0.0f,0.0f);
}

void VertexDataManufactory::tex_coord(const float &s, const float &t, const float &r)
{
	m_attribute_state[ATTRIB_TEXCOORD] = vec4(s,t,r,0.0f);
}

void VertexDataManufactory::vertex(const vec4 &v)
{
	uint id = 0;
	m_v.set_value(ATTRIB_POSITION,v);
	for(int i = 1 ; i  < ATTRIB_COUNT ;i++)
	{
		m_v.set_value(static_cast<AttributeID>(i),m_attribute_state[i]);
	}


	if (this->m_vertex_ids.find(m_v) != m_vertex_ids.end())
	{
		id = m_vertex_ids[m_v];
	}
	else
	{
		id = static_cast<uint>(this->m_current_mesh->vertex_count());
		this->m_current_mesh->push_back(m_v);
		m_vertex_ids[m_v] = id;
	}

	this->primitive_buffer.push_back(id);
	this->handle_primitive_buffer();
}

void VertexDataManufactory::vertex(const vec2 &v)
{
	vertex(vec4(v,0.0f,1.0f));
}

void VertexDataManufactory::vertex(const vec3 &v)
{
	vertex(vec4(v,1.0f));
}

void VertexDataManufactory::vertex(const float &x, const float &y, const float &z, const float &w)
{
	vertex(vec4(x,y,z,w));
}

void VertexDataManufactory::vertex(const float *v)
{
	vertex(vec4(v[0],v[1],v[2],v[3]));
}

void VertexDataManufactory::vertex(const Vertex &vertex)
{
	Vertex v = vertex;
	uint32_t id = 0;

	if (this->m_vertex_ids.find(v) != m_vertex_ids.end())
	{
		id = m_vertex_ids[v];
	}
	else
	{
		id = static_cast<uint>(m_current_mesh->vertex_count());
		m_current_mesh->push_back(v);
		m_vertex_ids[v] = id;
	}
	this->primitive_buffer.push_back(id);
	this->handle_primitive_buffer();
}

VertexData *VertexDataManufactory::create_box(float x, float y, float z)
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
				  vec2(0.4f, 0.0f),vec2(0.7f,0.0f), vec2(0.7f,0.3f), vec2(0.4f,0.3f),
				  n, n, n, n);

	n = vec3(1, 0, 0);
	this->addQuad(b, f, h, d,
				  vec2(1.0f, 0.3f),vec2(1.0f, 0.6f), vec2(0.7f, 0.6f), vec2(0.7f, 0.3f),
				  n, n, n, n);

	n = vec3(0, 0, -1);
	this->addQuad(f, e, g, h,
				  vec2(0.7f, 0.9f),vec2(0.4f, 0.9f), vec2(0.4f, 0.6f), vec2(0.7f, 0.6f),
				  n, n, n, n);

	n = vec3(-1, 0, 0);
	this->addQuad(e, a, c, g,
				  vec2(0.1f, 0.6f),vec2(0.1f, 0.3f), vec2(0.4f, 0.3f), vec2(0.4f, 0.6f),
				  n, n, n, n);

	n = vec3(0, 1, 0);
	this->addQuad(c, d, h, g,
				  vec2(0.4f, 0.3f),vec2(0.7f, 0.3f), vec2(0.7f, 0.6f), vec2(0.4f, 0.6f),
				  n, n, n, n);

	n = vec3(0, -1, 0);
	this->addQuad(a, b, f, e,
				  vec2(0.0f, 0.7f),vec2(0.3f, 0.7f), vec2(0.3f, 1.0f), vec2(0.0f, 1.0f),
				  n, n, n, n);

	return this->finish();
}

VertexData *VertexDataManufactory::create_plane(float w, float h, unsigned int tess_w, unsigned int tess_h)
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
						vec3(d_w*(x + 0),d_h* (y + 0), 0.0f) + offset,
						vec3(d_w*(x + 1),d_h* (y + 0), 0.0f) + offset,
						vec3(d_w*(x + 1),d_h* (y + 1), 0.0f) + offset,
						vec3(d_w*(x + 0),d_h* (y + 1), 0.0f) + offset,
						vec2(static_cast<float>(x + 0) / tess_w,
							 static_cast<float>(y + 0) / tess_h),
						vec2(static_cast<float>(x + 1) / tess_w,
							 static_cast<float>(y + 0) / tess_h),
						vec2(static_cast<float>(x + 1) / tess_w,
							 static_cast<float>(y + 1) / tess_h),
						vec2(static_cast<float>(x + 0) / tess_w,
							 static_cast<float>(y + 1) / tess_h));
		}
	}

	return this->finish();
}

VertexData *VertexDataManufactory::createCoordinateSystem()
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

vec2 colorTexCoordFromPosition(const vec3 &pos, const float &offset)
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

VertexData *VertexDataManufactory::create_uv_sphere(float radius, unsigned int slices, unsigned int stacks)
{
	this->begin(TRIANGLES);
	const float st_step = M_PI / stacks;
	const float sl_step = M_PI*2.0f / slices;
	for (unsigned int st = 0; st < stacks; st++)
	{
		const float st_r1 = st_step*st;
		const float st_r2 = st_step*st + st_step;

		const float sin_o1 = sin(st_r1);
		const float sin_o2 = sin(st_r2);

		const float cos_o1 = cos(st_r1);
		const float cos_o2 = cos(st_r2);

		for (unsigned int sl = 0; sl < slices; sl++)
		{
			const float sl_r1 = sl_step*sl;
			const float sl_r2 = sl_step*sl + sl_step;



			const float sin_d1 = sin(sl_r1);
			const float sin_d2 = sin(sl_r2);

			const float cos_d1 = cos(sl_r1);
			const float cos_d2 = cos(sl_r2);

			float tex_offset = 0.0f;
			if (st >= stacks / 2u)
				tex_offset = static_cast<float>(M_SQRT2)*0.5f;


			/*Compute positions and tex_coords for the current quad:*/
			vec3 pos[4];
			vec2 uv[4];
			pos[0] = vec3(sin_o1*cos_d1, sin_o1*sin_d1, cos_o1);
			pos[1] = vec3(sin_o2*cos_d1, sin_o2*sin_d1, cos_o2);
			pos[2] = vec3(sin_o2*cos_d2, sin_o2*sin_d2, cos_o2);
			pos[3] = vec3(sin_o1*cos_d2, sin_o1*sin_d2, cos_o1);

			for (int i = 0; i < 4;i++)
			{
				pos[i] = glm::normalize(pos[i]);
				uv[i] = colorTexCoordFromPosition(pos[i], tex_offset);
			}

			/* Draw a quad unless you are about do draw the last and the first
				   stack.
				*/
			if (st != 0 && st< stacks - 1)
			{
				tex_coord(uv[0]);
				normal(pos[0]);
				vertex(pos[0]*radius);

				tex_coord(uv[1]);
				normal(pos[1]);
				vertex(pos[1]*radius);

				tex_coord(uv[2]);
				normal(pos[2]);
				vertex(pos[2]*radius);

				tex_coord(uv[0]);
				normal(pos[0]);
				vertex(pos[0]*radius);

				tex_coord(uv[2]);
				normal(pos[2]);
				vertex(pos[2]*radius);

				tex_coord(uv[3]);
				normal(pos[3]);
				vertex(pos[3]*radius);
			}
			else
			{
				/* If it is the last stack:*/
				if (st != 0)
				{
					tex_coord(uv[0]);
					normal(pos[0]);
					vertex(pos[0]*radius);

					tex_coord(uv[2]);
					normal(pos[2]);
					vertex(pos[2]*radius);

					tex_coord(uv[3]);
					normal(pos[3]);
					vertex(pos[3]*radius);
				}
				/* If it is the first stack:*/
				else
				{
					tex_coord(uv[0]);
					normal(pos[0]);
					vertex(pos[0]*radius);

					tex_coord(uv[1]);
					normal(pos[1]);
					vertex(pos[1]*radius);

					tex_coord(uv[2]);
					normal(pos[2]);
					vertex(pos[2]*radius);
				}
			}
		}
	}

	return this->finish();
}

VertexData *VertexDataManufactory::create_cylinder(float radius, float height, unsigned int slices, unsigned int stacks)
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
			const float ax = sin(circum_step*(i + 0));
			const float ay = cos(circum_step*(i + 0));
			const float bx = sin(circum_step*(i + 1));
			const float by = cos(circum_step*(i + 1));

			/* Create a quad.*/
			this->tex_coord(((i + 0)*circum_step) / twoPi, zlow / height);
			normal(glm::normalize(vec3(ax, ay, nz)));
			this->vertex(radius*ax, radius*ay, zlow);

			this->tex_coord(((i + 1)*circum_step) / twoPi, zlow / height);
			normal(glm::normalize(vec3(bx, by, nz)));
			this->vertex(radius*bx, radius*by, zlow);

			this->tex_coord(((i + 1)*circum_step) / twoPi, zhigh / height);
			normal(glm::normalize(vec3(bx, by, nz)));
			this->vertex(radius*bx, radius*by, zhigh);

			this->tex_coord(((i + 0)*circum_step) / twoPi, zhigh / height);
			normal(glm::normalize(vec3(ax, ay, nz)));
			this->vertex(radius*ax, radius*ay, zhigh);
		}
	}
	return this->finish();

}

VertexData *VertexDataManufactory::create_cone(float baseRadius, float topRadius, float height, unsigned int slices, unsigned int stacks)
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

		float uv_r_low = ((1.0f) - (static_cast<float>(st) / stacks));
		uv_r_low = (0.5f - inner_radius) * uv_r_low + inner_radius;

		float uv_r_high = ((1.0f) - ((1.0f + static_cast<float>(st)) / stacks));
		uv_r_high = (0.5f - inner_radius) * uv_r_high + inner_radius;;

		for (unsigned int i = 0; i < slices; i++) // slices
		{
			/* Some precomputations we need for both normals and positions */
			const float ax = sin(circum_step*(i + 0));
			const float ay = cos(circum_step*(i + 0));
			const float bx = sin(circum_step*(i + 1));
			const float by = cos(circum_step*(i + 1));

			/* Create a quad.*/
			this->tex_coord(uv_r_low * ax + 0.5f, uv_r_low*ay + 0.5f);
			normal(glm::normalize(vec3(ax, ay, nz)));
			this->vertex(rlow*ax, rlow*ay, zlow);

			this->tex_coord(uv_r_low * bx + 0.5f, uv_r_low*by + 0.5f);
			normal(glm::normalize(vec3(bx, by, nz)));
			this->vertex(rlow*bx, rlow*by, zlow);

			this->tex_coord(uv_r_high * bx + 0.5f, uv_r_high*by + 0.5f);
			normal(glm::normalize(vec3(bx, by, nz)));
			this->vertex(rhigh*bx, rhigh*by, zhigh);

			this->tex_coord(uv_r_high * ax + 0.5f, uv_r_high*ay + 0.5f);
			normal(glm::normalize(vec3(ax, ay, nz)));
			this->vertex(rhigh*ax, rhigh*ay, zhigh);
		}
	}
	return this->finish();
}

VertexData *VertexDataManufactory::create_disk(float innerRadius, float outerRadius, unsigned int slices, unsigned int loops)
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

			/* Some precomputations we need for both normals and positions*/
			const float ax = sin(circum_step*(i + 0));
			const float ay = cos(circum_step*(i + 0));
			const float bx = sin(circum_step*(i + 1));
			const float by = cos(circum_step*(i + 1));

			/* Create a quad.*/

			vec3 pos[4];
			vec2 uv[4];
			pos[0] = vec3(rlow*ax, rlow*ay, 0);
			pos[1] = vec3(rlow*bx, rlow*by, 0);
			pos[2] = vec3(rhigh*bx, rhigh*by, 0);
			pos[3] = vec3(rhigh*ax, rhigh*ay, 0);

			for (int i = 0; i < 4; i++)
			{
				uv[i] = vec2(pos[i].x / (2.0f * outerRadius) + 0.5f,
							 pos[i].y / (2.0f * outerRadius) + 0.5f);
			}

			if (st != 0)
			{
				this->tex_coord(uv[0]);
				this->vertex(pos[0]);
				this->tex_coord(uv[1]);
				this->vertex(pos[1]);
				this->tex_coord(uv[2]);
				this->vertex(pos[2]);
			}
			this->tex_coord(uv[0]);
			this->vertex(pos[0]);
			this->tex_coord(uv[2]);
			this->vertex(pos[2]);
			this->tex_coord(uv[3]);
			this->vertex(pos[3]);
		}
	}
	return this->finish();
}

void VertexDataManufactory::add_vertex(vec3 p, vec2 t, vec3 n, vec4 c)
{
	tex_coord(t);
	normal(n);
	color(c);
	vertex(p);
}


#define vecIsNaN(v) (!(v.x==v.x))

void VertexDataManufactory::add_triangle(vec3 p1, vec3 p2, vec3 p3, vec2 t1, vec2 t2, vec2 t3, vec3 n1, vec3 n2, vec3 n3, vec4 c1, vec4 c2, vec4 c3)
{
	vec3 n = glm::normalize(glm::cross(p2 - p1, p3 - p1));
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
		add_vertex(p1, t1, n1, c1);
		add_vertex(p2, t2, n2, c2);

		add_vertex(p2, t2, n2, c2);
		add_vertex(p3, t3, n3, c3);

		add_vertex(p3, t3, n3, c3);
		add_vertex(p1, t1, n1, c1);
	}

	else if (m_input_primitive == LINE_STRIP)
	{
		this->m_current_mesh->push_back(RESET_PRIMITIVE);
		add_vertex(p1, t1, n1, c1);
		add_vertex(p2, t2, n2, c2);
		add_vertex(p3, t3, n3, c3);
		add_vertex(p1, t1, n1, c1);
	}
	else
	{
		if (m_input_primitive == TRIANGLE_STRIP)
			this->m_current_mesh->push_back(RESET_PRIMITIVE);
		add_vertex(p1, t1, n1, c1);
		add_vertex(p2, t2, n2, c2);
		add_vertex(p3, t3, n3, c3);
	}
}

void VertexDataManufactory::addQuad(vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec2 t1, vec2 t2, vec2 t3, vec2 t4, vec3 n1, vec3 n2, vec3 n3, vec3 n4, vec4 c1, vec4 c2, vec4 c3, vec4 c4)
{
	vec3 n = glm::normalize(glm::cross(p2 - p1, p3 - p1));
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
	if (vecIsNaN(n4))
	{
		n4 = n;
	}

	if (m_input_primitive == TRIANGLES)
	{
		add_vertex(p1, t1, n1, c1);
		add_vertex(p2, t2, n2, c2);
		add_vertex(p3, t3, n3, c3);

		add_vertex(p1, t1, n1, c1);
		add_vertex(p3, t3, n3, c3);
		add_vertex(p4, t4, n4, c4);
	}
	if (m_input_primitive == QUADS)
	{
		add_vertex(p1, t1, n1, c1);
		add_vertex(p2, t2, n2, c2);
		add_vertex(p3, t3, n3, c3);
		add_vertex(p4, t4, n4, c4);
	}
	else if (m_input_primitive == LINES)
	{
		add_vertex(p1, t1, n1, c1);
		add_vertex(p2, t2, n2, c2);

		add_vertex(p2, t2, n2, c2);
		add_vertex(p3, t3, n3, c3);

		add_vertex(p3, t3, n3, c3);
		add_vertex(p4, t4, n4, c4);

		add_vertex(p4, t4, n4, c4);
		add_vertex(p1, t1, n1, c1);
	}

	else if (m_input_primitive == LINE_STRIP)
	{
		this->m_current_mesh->push_back(RESET_PRIMITIVE);
		add_vertex(p1, t1, n1, c1);
		add_vertex(p2, t2, n2, c2);
		add_vertex(p3, t3, n3, c3);
		add_vertex(p4, t4, n4, c4);
		add_vertex(p1, t1, n1, c1);
	}
	else
	{
		if (m_input_primitive == TRIANGLE_STRIP ||
				m_input_primitive == QUAD_STRIP)
			this->m_current_mesh->push_back(RESET_PRIMITIVE);
		add_vertex(p1, t1, n1, c1);
		add_vertex(p2, t2, n2, c2);
		add_vertex(p3, t3, n3, c3);
		add_vertex(p4, t4, n4, c4);
	}

}
#undef vecIsNaN



}
