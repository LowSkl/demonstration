#pragma once

#include "Camera.h"
#include "glm/glm.hpp"

#include <memory>

class CameraControl
{
	Camera* m_pCamera = nullptr;

	double m_lastTime = 0.f;
	double m_delta    = 0.f;
	double m_speed    = 1.f;
	double m_sens     = 1.5f;

public:
	CameraControl(Camera* camera);

	const glm::mat4& get_matricies() const { return this->m_pCamera->get_projection_matrix() * this->m_pCamera->get_view_matrix(); }
	void update();

	Camera* const get_camera() { return this->m_pCamera; }

	double const speed()             { return this->m_speed;         }
	        void speed(double speed) {        this->m_speed = speed; }

	double const sens()            { return this->m_sens;        }
	        void sens(double sens) {        this->m_sens = sens; }
};

