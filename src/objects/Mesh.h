#pragma once

#include <graphics/buffers/VAO.h>
#include <graphics/SneakyThings.h>

#include <graphics/Shader.h>
#include <misc/camera/CameraControl.h>

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <graphics/OpenGL.h>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
};

class Mesh
{
	std::vector<Vertex>*  m_localSpace;

	BufferLayout m_dataLayout{
		ShaderDataType::Float3,
		ShaderDataType::Float3,
		ShaderDataType::Float3,
	};

	VAO m_vao;
	VBO m_vbo;
	EBO m_ebo;

public:

	Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices);

	Mesh(const Mesh&      ) = delete;
	Mesh(      Mesh&& mesh) = delete;

	Mesh& operator=(const Mesh&      ) = delete;
	Mesh& operator=(      Mesh&& mesh) = delete;

	void draw(Shader* shader, CameraControl* camera, Primitives primitiv);
};

