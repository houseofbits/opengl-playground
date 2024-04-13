#pragma once
#include <GL/glew.h>
#include <iostream>

class RenderTarget
{
public:
    enum Target
    {
        TARGET_DEPTH,
        TARGET_COLOR,
    };

    Target target;
    unsigned int width;
    unsigned int height;
    unsigned int framebufferId;
    unsigned int targetTextureId;
    unsigned int renderbufferId;

    void create(Target target, unsigned int imageWidth, unsigned int imageHeight)
    {
        target = target;
        width = imageWidth;
        height = imageHeight;

        glGenFramebuffers(1, &framebufferId);
        glGenTextures(1, &targetTextureId);
        glBindTexture(GL_TEXTURE_2D, targetTextureId);
        if (target == TARGET_DEPTH)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

        if (target == TARGET_DEPTH)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, targetTextureId, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }
        else
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, targetTextureId, 0);
            // glDrawBuffer(GL_COLOR_ATTACHMENT0);
            // glReadBuffer(GL_COLOR_ATTACHMENT0);

            glGenRenderbuffers(1, &renderbufferId);
            glBindRenderbuffer(GL_RENDERBUFFER, renderbufferId);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbufferId);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glBindTexture(GL_TEXTURE_2D, 0);
    }

    void beginRender()
    {
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
        glBindTexture(GL_TEXTURE_2D, targetTextureId);

        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (target == TARGET_DEPTH)
        {
            // glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTextureId, 0);
            glClear(GL_DEPTH_BUFFER_BIT);
        }
        else
        {
            glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }

    void end()
    {
        // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
        // {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // }
    }
};