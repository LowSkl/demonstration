#pragma once

#include "Mesh.h"
#include <cmath>
#include <corecrt_math_defines.h>

class SphereMesh : public Mesh {
public:
    SphereMesh(float radius, unsigned int precision, glm::vec3 color = { 1.0f, 1.0f, 1.0f })
        : Mesh(createSphereMesh(radius, precision, color), createSphereIndices(precision)) {
    }

private:
    static std::vector<Vertex>* createSphereMesh(float radius, unsigned int precision, glm::vec3 color) {
        std::vector<Vertex>* vertices = new std::vector<Vertex>();
        double phiStep = M_PI / precision; // ”гол по вертикали
        double thetaStep = 2.0f * M_PI / precision; // ”гол по горизонтали

        // —оздаем вершины
        for (unsigned int i = 0; i <= precision; ++i) {
            for (unsigned int j = 0; j <= precision; ++j) {
                float phi = i * phiStep;
                float theta = j * thetaStep;

                Vertex vertex;
                vertex.position = glm::vec3(
                    radius * sin(phi) * cos(theta),   // x
                    radius * cos(phi),               // y
                    radius * sin(phi) * sin(theta)  // z
                );

                vertex.normal = glm::normalize(vertex.position);
                vertex.color = color;
                vertices->push_back(vertex);
            }
        }

        return vertices;
    }

    static std::vector<unsigned int>* createSphereIndices(unsigned int precision) {
        std::vector<unsigned int>* indices = new std::vector<unsigned int>();

        for (unsigned int i = 0; i < precision; ++i) {
            for (unsigned int j = 0; j < precision; ++j) {
                unsigned int first = (i * (precision + 1)) + j;
                unsigned int second = first + precision + 1;

                indices->push_back(first);
                indices->push_back(second);
                indices->push_back(first + 1);

                indices->push_back(second);
                indices->push_back(second + 1);
                indices->push_back(first + 1);
            }
        }

        return indices;
    }
};
