#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices) :
	m_localSpace(vertices),
	m_vbo(vertices->data(), vertices->size() * sizeof(Vertex), this->m_dataLayout),
	m_ebo(indices->data(), indices->size() * sizeof(unsigned int))
{
	this->m_vao.add_vertexBuffer(this->m_vbo);
	this->m_vao.set_indexBuffer(this->m_ebo);

	this->m_vao.unbind();
	this->m_vbo.unbind();
	this->m_ebo.unbind();
}

void Mesh::draw(Shader* shader, CameraControl* camera, Primitives primitiv)
{
	shader->bind();
	this->m_vao.bind();

	unsigned int numDiffuseTex = 0;
	unsigned int numSpecularTex = 0;

	shader->set_matrix4("camMatrix", camera->get_matricies());
	shader->set_vec3("camPos", camera->get_camera()->get_position());

	OpenGL::draw(this->m_vao, primitiv);
}