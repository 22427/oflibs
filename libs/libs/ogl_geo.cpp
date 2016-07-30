#include "ogl_geo.h"

namespace ogl
{

Geometry::Geometry(vd::VertexData* vd, bool destroy_vd)
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

void Geometry::uploadData(vd::VertexData *vd)
{
	if (m_vao)
		glBindVertexArray(m_vao);

	const size_t vertex_size = sizeof(vd::Vertex);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vd->indices().size() *
				 sizeof(unsigned int), vd->indices().data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER,
				 vertex_size* vd->data().size(),
				 vd->data().data(),
				 GL_STATIC_DRAW);

	const vd::Vertex v = vd->data()[0];



	if (m_vao)
	{
#define addr_diff(a,b) ((void*)((char*)a-(char*) b))
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
	m_vertice_count = vd->indices().size();
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
