#pragma once

#include "Console.h"
#include <GL/glew.h>
#include <iostream>
#include <string>

const bool DISPLAY_NOTIFICATIONS = false;

std::string getSourceString(GLenum source) {
    switch (source) {
        case GL_DEBUG_SOURCE_API:
            return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION:
            return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:
        default:
            return "UNKNOWN";
    }
}

std::string getTypeString(GLenum type) {
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "DEPRECATED BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "UNDEFINED BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY:
            return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE:
            return "PERFORMANCE";
        case GL_DEBUG_TYPE_OTHER:
            return "OTHER";
        case GL_DEBUG_TYPE_MARKER:
            return "MARKER";
        default:
            return "UNKNOWN";
    }
}

std::string getSeverityString(GLenum severity) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            return "HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM:
            return "MEDIUM";
        case GL_DEBUG_SEVERITY_LOW:
            return "LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "NOTIFICATION";
        default:
            return "UNKNOWN";
    }
}

bool isHighPriority(GLenum type, GLenum severity) {
    if (type == GL_DEBUG_TYPE_ERROR) {
        return true;
    }

    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        return true;
    }

    return false;
}

bool isLowPriority(GLenum severity) {
    if (severity == GL_DEBUG_SEVERITY_LOW || severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
        return true;
    }

    return false;
}

void GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *msg, const void *data) {

    return;

    if (isLowPriority(severity) && !DISPLAY_NOTIFICATIONS) {
        return;
    }

    std::cout << FOREWHT
              << id
              << ": ";

    if (isHighPriority(type, severity))
        std::cout << FORERED;
    else if (isLowPriority(severity))
        std::cout << FOREGRN;
    else
        std::cout << FOREYEL;

    std::cout
            << getTypeString(type) << " - "
            << getSeverityString(severity)
            << ": "
            << msg;

    std::cout
            << RESETTEXT
            << std::endl;
}
