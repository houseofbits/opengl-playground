#pragma once

#include "BaseRenderTest.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

class TestTangentSpaceCalculation : public BaseRenderTest {
public:
    void setup(Window &window) {
        message("Test Tangent space vectors calculation");
    }

    void render(Camera &camera) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glLoadMatrixf(glm::value_ptr(camera.getProjectionMatrix()));
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glLoadMatrixf(glm::value_ptr(camera.getViewMatrix()));

        glm::vec3 p[3] = {
                {0, 0, 0},
                {100, 0, 0},
                {50, 100, 0}};
        glm::vec2 uv[3] = {
                {1, 1},
                {0, 1},
                {0, 0}};

        glm::vec3 center = glm::vec3(
                (p[0].x + p[1].x + p[2].x) / 3.0,
                (p[0].y + p[1].y + p[2].y) / 3.0,
                (p[0].z + p[1].z + p[2].z) / 3.0);

        glm::vec3 edge1 = p[1] - p[0];
        glm::vec3 edge2 = p[2] - p[1];
        glm::vec2 uv1 = uv[1] - uv[0];
        glm::vec2 uv2 = uv[2] - uv[1];

        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        //        std::cout<<normal.x<<","<<normal.y<<","<<normal.z<<std::endl;

        float f = 1.0f / (uv1.x * uv2.y - uv2.x * uv1.y);

        glm::vec3 T;
        glm::vec3 B;

        T.x = f * (uv2.y * edge1.x - uv1.y * edge2.x);
        T.y = f * (uv2.y * edge1.y - uv1.y * edge2.y);
        T.z = f * (uv2.y * edge1.z - uv1.y * edge2.z);

        B.x = f * (-uv2.x * edge1.x + uv1.x * edge2.x);
        B.y = f * (-uv2.x * edge1.y + uv1.x * edge2.y);
        B.z = f * (-uv2.x * edge1.z + uv1.x * edge2.z);

        T = glm::normalize(T);
        B = glm::normalize(B);

        glm::mat3 TBN = glm::inverse(glm::transpose(glm::mat3(T, B, normal)));

        glm::vec3 mapNormal = glm::normalize(glm::vec3(0,0,1));
//        mapNormal = glm::normalize((mapNormal * 2.0f) - 1.0f);
//        mapNormal = TBN * mapNormal;
        mapNormal = glm::normalize(glm::vec3(mapNormal.x * T + mapNormal.y * B + mapNormal.z * normal));

        float scale = 20;
        glColor3f(0, 0, 0);
        glPointSize(10.0);
        glBegin(GL_POINTS);
        glVertex3f(center.x, center.y, center.z);
        glEnd();

        glm::vec3 np2;
        glLineWidth(2.0);

        //N
        glColor3f(0, 0, 1);
        glBegin(GL_LINES);
        glVertex3f(center.x, center.y, center.z);
        np2 = center + normal * scale;
        glVertex3f(np2.x, np2.y, np2.z);
        glEnd();

        //T
        glColor3f(1, 0, 0);
        glBegin(GL_LINES);
        glVertex3f(center.x, center.y, center.z);
        np2 = center + T * scale;
        glVertex3f(np2.x, np2.y, np2.z);
        glEnd();

        //B
        glColor3f(0, 1, 0);
        glBegin(GL_LINES);
        glVertex3f(center.x, center.y, center.z);
        np2 = center + B * scale;
        glVertex3f(np2.x, np2.y, np2.z);
        glEnd();

        //Mapped normal
        glLineWidth(1.0);
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);
        glVertex3f(center.x, center.y, center.z);
        np2 = center + mapNormal * scale * 2.0f;
        glVertex3f(np2.x, np2.y, np2.z);
        glEnd();

        //Triangle
        glBegin(GL_TRIANGLES);
        glColor3f(uv[0].x, uv[0].y, 0);
        glVertex3f(p[0].x, p[0].y, p[0].z);

        glColor3f(uv[1].x, uv[1].y, 0);
        glVertex3f(p[1].x, p[1].y, p[1].z);

        glColor3f(uv[2].x, uv[2].y, 0);
        glVertex3f(p[2].x, p[2].y, p[2].z);
        glEnd();
    }

    void destroy() {
    }
};