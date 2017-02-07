#pragma once
// OFL_IB GL
#ifndef OFL_GL_HEADER
#define OFL_GL_HEADER <glad/glad.h>
#endif
#include OFL_GL_HEADER
// OFL_IB


#include "dll.h"
#include "vd.h"
namespace ofl
{
/**
 * @brief The Geometry class represents a VAO, with an associated IBO and VBO.
 * It is due to ATIs special interpretation to the reference counting on buffer
 * objects in the case of an IBO, that we have to store the IBO, so I store the
 * VBO as well. It also stores the primitive type and the vertex count.
 */
class OFL_DLL_PUBLIC Geometry
{
protected:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ibo;
	GLuint m_primitive;
	GLsizei m_vertice_count;
	GLenum m_index_type;
	VertexConfiguration m_vcfg;

public:
	/**
	 * @brief Geometry creates a geometry from VertexData.
	 * Will create a VBO and a VAO uploading the data from vp.
	 * The attribute locations will be set to:
	 * TODO!!!
	 * @param vd The vertex data.
	 * @param destroy_vd If set true vd will be freed.
	 */
	Geometry(ofl::VertexData* ofl, bool destroy_vd = false);
	Geometry();

	/**
	 * @brief uploadData updates all the data storen with data from vp
	 * @param vp Source of the new data.
	 */
	void uploadData(ofl::VertexData* vp);

	/** Destroys the structure and releases all memory allocated on the GPU-
	 * and CPUside.
	 */
	~Geometry();

	/* this geometry draws itself.*/
	/**
	 * @brief draw This geometry draws itself.
	 * If patchsize is set, it will draw with GL_PATCH_VERTICES as primitive,
	 * and set the patchsize before drawing.
	 * @param patchsize The patchsize you choose or 0 for the stored primitive
	 * mode.
	 */
	void draw(int patchsize = 0)
	{
		if (m_vao)
		{
			glBindVertexArray(m_vao);
			if (patchsize > 0)
			{
				glPatchParameteri(GL_PATCH_VERTICES, patchsize);
				glDrawElements(GL_PATCHES, m_vertice_count,
							   m_index_type, nullptr);
			}
			else
			{
				glDrawElements(m_primitive, m_vertice_count,
							   m_index_type, nullptr);
			}
		}
		else
		{

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

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
					// TODO: Do this for all types and element counts of attributes.
					vec4 attrib;
					a.read(nullptr,attrib);
					glVertexAttrib4fv(a.attribute_id,&(attrib.x));
				}
			}
			glDrawElements(
						m_primitive,
						m_vertice_count,
						m_index_type,
						nullptr);
		}
	}

};

}
