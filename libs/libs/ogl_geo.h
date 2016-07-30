#pragma once

#ifndef OFL_GL_HEADER
#define OFL_GL_HEADER <glad/glad.h>
#endif

#include OFL_GL_HEADER



#include "vd.h"
namespace ogl
{

#ifndef ALOC_POSITION
#define ALOC_POSITION 0
#define ALOC_NORMAL 1
#define ALOC_COLOR 2
#define ALOC_TEXCOORD 3
#define ALOC_TANGENT 4
#endif

/**
 * @brief The Geometry class represents a VAO, with an associated IBO and VBO.
 * It is due to ATIs special interpretation to the reference counting on buffer
 * objects in the case of an IBO, that we have to store the IBO, so I store the
 * VBO as well. It also stores the primitive type and the vertex count.
 */
class Geometry
{
protected:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ibo;
	GLuint m_primitive;
	GLsizei m_vertice_count;

public:
	/**
	 * @brief Geometry creates a geometry from VertexData.
	 * Will create a VBO and a VAO uploading the data from vp.
	 * The attribute locations will be set to:
	 * 	position              0  (= ALOC_POSITION )
	 * 	normal                1  (= ALOC_NORMAL   )
	 * 	color                 2	 (= ALOC_TEXCOORD )
	 * 	texture coordinates   3  (= ALOC_COLOR    )
	 * 	tangent               4  (= ALOC_TANGENT  )
	 * @param vd The vertex data.
	 * @param destroy_vd If set true vd will be freed.
	 */
	Geometry(vd::VertexData* vd, bool destroy_vd = false);
	Geometry();

	/**
	 * @brief uploadData updates all the data storen with data from vp
	 * @param vp Source of the new data.
	 */
	void uploadData(vd::VertexData* vp);

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
							   GL_UNSIGNED_INT, nullptr);
			}
			else
			{
				glDrawElements(m_primitive, m_vertice_count,
							   GL_UNSIGNED_INT, nullptr);
			}
		}
		else
		{

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

			const size_t vertex_size = sizeof(vd::Vertex);
			const vd::Vertex v;


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
			glDrawElements(
						m_primitive,
						m_vertice_count,
						GL_UNSIGNED_INT,
						nullptr);
		}
	}

};

}
