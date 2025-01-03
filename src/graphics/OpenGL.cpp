#include "OpenGL.h"

#include <utils/Log.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

bool opengl_initialized = false;

const char* gl_source_to_string(const GLenum source)
{
    switch (source)
    {
    case GL_DEBUG_SOURCE_API: return "DEBUG_SOURCE_API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "DEBUG_SOURCE_WINDOW_SYSTEM";
    case GL_DEBUG_SOURCE_SHADER_COMPILER: return "GL_DEBUG_SOURCE_SHADER_COMPILER";
    case GL_DEBUG_SOURCE_THIRD_PARTY: return "DEBUG_SOURCE_THIRD_PARTY";
    case GL_DEBUG_SOURCE_APPLICATION: return "DEBUG_SOURCE_APPLICATION";
    case GL_DEBUG_SOURCE_OTHER: return "DEBUG_SOURCE_OTHER";

    default: return "UNKNOWN_DEBUG_SOURCE";
    }
}

const char* gl_type_to_string(const GLenum type)
{
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: return "DEBUG_TYPE_ERROR";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEBUG_TYPE_DEPRECATED_BEHAVIOR";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "DEBUG_TYPE_UNDEFINED_BEHAVIOR";
    case GL_DEBUG_TYPE_PORTABILITY: return "DEBUG_TYPE_PORTABILITY";
    case GL_DEBUG_TYPE_PERFORMANCE: return "DEBUG_TYPE_PERFORMANCE";
    case GL_DEBUG_TYPE_MARKER: return "DEBUG_TYPE_MARKER";
    case GL_DEBUG_TYPE_PUSH_GROUP: return "DEBUG_TYPE_PUSH_GROUP";
    case GL_DEBUG_TYPE_POP_GROUP: return "DEBUG_TYPE_POP_GROUP";
    case GL_DEBUG_TYPE_OTHER: return "DEBUG_TYPE_OTHER";

    default: return "UNKNOWN_DEBUG_TYPE";
    }
}

void viewport_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool OpenGL::initialize(GLFWwindow* pRawWindow)
{
	if (opengl_initialized)
		return true;

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LCRITICAL("Can't init GLAD!");
		return false;
	}

    glfwSetFramebufferSizeCallback(pRawWindow, viewport_callback);

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glDebugMessageCallback([](GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
        {
            switch (severity)
            {
            case GL_DEBUG_SEVERITY_HIGH:
                LERROR("OpenGL Error: [{0}:{1}]({2}): {3}",
                    gl_source_to_string(source),
                    gl_type_to_string(type),
                    id,
                    message);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                LWARN("OpenGL Warning: [{0}:{1}]({2}): {3}",
                    gl_source_to_string(source),
                    gl_type_to_string(type),
                    id,
                    message);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                LINFO("OpenGL Info: [{0}:{1}]({2}): {3}",
                    gl_source_to_string(source),
                    gl_type_to_string(type),
                    id,
                    message);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                LINFO("OpenGL Notificaton: [{0}:{1}]({2}): {3}",
                    gl_source_to_string(source),
                    gl_type_to_string(type),
                    id,
                    message);
                break;
            default:
                LERROR("OpenGL Error: [{0}:{1}] ({2}) : {3}",
                    gl_source_to_string(source),
                    gl_type_to_string(type),
                    id,
                    message);
            }
        }, nullptr);

    glEnable(GL_DEPTH_TEST);

    opengl_initialized = true;
	return true;
}

void OpenGL::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGL::clear_color(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void OpenGL::draw(const VAO& vao, Primitives primitive)
{
    vao.bind();
    glDrawElements(static_cast<unsigned int>(primitive), static_cast<GLsizei>(vao.get_indicesCount()), GL_UNSIGNED_INT, nullptr);
}

double OpenGL::get_time()
{
    return glfwGetTime();
}