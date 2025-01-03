#include "CameraControl.h"

#include <graphics/OpenGL.h>
#include <windowing/Input.h>
#include <windowing/Window.h>

CameraControl::CameraControl(Camera* camera) : m_pCamera(camera), m_lastTime(OpenGL::get_time())
{
}

void handleMovement(double delta, double speed, Camera* camera)
{
    float movementSpeed = delta * speed;

    if (Input::pressed(KeyCode::KEY_W))
        camera->move_front(movementSpeed);
    if (Input::pressed(KeyCode::KEY_A))
        camera->move_right(-movementSpeed);
    if (Input::pressed(KeyCode::KEY_S))
        camera->move_front(-movementSpeed);
    if (Input::pressed(KeyCode::KEY_D))
        camera->move_right(movementSpeed);

    if (Input::pressed(KeyCode::KEY_LEFT_SHIFT))
        camera->move_up(-movementSpeed);
    if (Input::pressed(KeyCode::KEY_SPACE))
        camera->move_up(movementSpeed);
}

void CameraControl::update()
{
    double currentTime = OpenGL::get_time();
    this->m_delta = currentTime - this->m_lastTime;
    this->m_lastTime = currentTime;

    if (Window::is_cursorLocked())
    {
        double rotX = input.mousePosDeltaY / this->m_pCamera->get_height() * 2.0;
        double rotY = input.mousePosDeltaX / this->m_pCamera->get_width() * 2.0;

        this->m_pCamera->rotate(-rotX * this->m_sens * this->m_delta * 100.0,
            -rotY * this->m_sens * this->m_delta * 100.0,
            0.0f);
    }

    pInput->mousePosDeltaX = 0;
    pInput->mousePosDeltaY = 0;

    handleMovement(this->m_delta, this->m_speed, this->m_pCamera);
}


