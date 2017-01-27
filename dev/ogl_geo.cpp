#include "ogl_geo.h"
#include <climits>
namespace ofl
{

Geometry::Geometry(VertexData* vd, bool destroy_vd)
{

	m_vbo = m_vao = m_ibo = 0;
	m_vcfg = vd->vertex_configuration();
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	m_index_type = vd->index_type();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 static_cast<GLsizeiptr>(vd->index_type().size()*vd->index_count()),
				 vd->indices(), GL_STATIC_DRAW);



	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER,
				 static_cast<GLsizeiptr>(m_vcfg.vertex_size()*vd->vertex_count()),
				 vd->vertex_data(),
				 GL_STATIC_DRAW);


	m_primitive = vd->primitive();
	m_vertice_count =  static_cast<GLsizei>(vd->index_count());


	if (m_vao)
	{
		for ( const auto& a : m_vcfg)
		{
			if(!a.use_constant)
			{
			glVertexAttribPointer(a.attribute_id, a.elements, a.type,a.normalized,
								  static_cast<int>(m_vcfg.vertex_size()),reinterpret_cast<const void*>(a.offset));
			glEnableVertexAttribArray(a.attribute_id);
			}
			else
			{
				// TODO: Do this for all types and all number of attributes.
				vec4 attrib;
				a.read(nullptr,attrib);
				glVertexAttrib4fv(a.attribute_id,&(attrib.x));
			}
		}
	}
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
