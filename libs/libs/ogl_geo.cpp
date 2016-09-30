#include "ogl_geo.h"
#include <climits>
namespace ofl
{

Geometry::Geometry(VertexData* vd, bool destroy_vd)
{

	m_vbo = m_vao = m_ibo = 0;
	if (glGenVertexArrays)
		glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_ibo);
	glGenBuffers(1, &m_vbo);


	uploadData(vd);
	if(destroy_vd)
		delete vd;
}

Geometry::Geometry()
{
	m_vbo = m_vao = m_ibo = 0;
	if (glGenVertexArrays)
		glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_ibo);
	glGenBuffers(1, &m_vbo);
}

void Geometry::uploadData(VertexData *vd)
{
	if (m_vao)
		glBindVertexArray(m_vao);

	const size_t vertex_size = sizeof(Vertex);




	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	if(vd->indices().size() < USHRT_MAX)
	{
		m_index_type = GL_UNSIGNED_SHORT;

		std::vector<GLushort> sid;
		sid.reserve(vd->indices().size());
		for(const uint32_t& i: vd->indices())
		{
			sid.push_back(static_cast<GLushort>(i));
		}
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(sizeof(GLushort)*sid.size()),
			sid.data(), GL_STATIC_DRAW);
	}
	else
	{
		m_index_type = GL_UNSIGNED_INT;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(sizeof(GLuint)*vd->indices().size()),
			vd->indices().data(), GL_STATIC_DRAW);
	}




	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER,
				 static_cast<GLsizeiptr>(vertex_size* vd->data().size()),
				 vd->data().data(),
				 GL_STATIC_DRAW);

	const Vertex v = vd->data()[0];



	if (m_vao)
	{
#define addr_diff(a,b) (reinterpret_cast<void*>(reinterpret_cast<const char*>(a)-reinterpret_cast<const char*>(b)))
		glVertexAttribPointer(ALOC_POSITION, 3, GL_FLOAT, GL_FALSE,
							  vertex_size, addr_diff(&(v.pos()),&v));
		glEnableVertexAttribArray(ALOC_POSITION);

		glVertexAttribPointer(ALOC_NORMAL, 3, GL_FLOAT, GL_FALSE,
							  vertex_size, addr_diff(&(v.nrm()),&v));
		glEnableVertexAttribArray(ALOC_NORMAL);

		glVertexAttribPointer(ALOC_COLOR, 4, GL_FLOAT, GL_FALSE,
							  vertex_size, addr_diff(&(v.clr()),&v));
		glEnableVertexAttribArray(ALOC_COLOR);

		glVertexAttribPointer(ALOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE,
							  vertex_size, addr_diff(&(v.tex()),&v));
		glEnableVertexAttribArray(ALOC_TEXCOORD);

		glVertexAttribPointer(ALOC_TANGENT, 3, GL_FLOAT, GL_FALSE,
							  vertex_size, addr_diff(&(v.tan()),&v));
		glEnableVertexAttribArray(ALOC_TANGENT);
#undef addr_diff
	}

	m_primitive = vd->primitive();
	m_vertice_count =  static_cast<GLsizei>(vd->indices().size());
}

Geometry::~Geometry()
{
	if(m_vbo)
		glDeleteBuffers(1, &m_vbo);
	if(m_ibo)
		glDeleteBuffers(1, &m_ibo);
	if(m_vao)
		glDeleteVertexArrays(1, &m_vao);
}

}
