#include "Plane.h"

void Plane::create(float xsize, float zsize, int xdivs, int zdivs, float smax, float tmax)
{
    std::vector<float> p(3 * (xdivs + 1) * (zdivs + 1));
    std::vector<float> n(3 * (xdivs + 1) * (zdivs + 1));
    std::vector<float> tex(2 * (xdivs + 1) * (zdivs + 1));
    std::vector<unsigned int> el(6 * xdivs * zdivs);

    float x2 = xsize / 2.0f;
    float z2 = zsize / 2.0f;
    float iFactor = (float)zsize / zdivs;
    float jFactor = (float)xsize / xdivs;
    float texi = smax / zdivs;
    float texj = tmax / xdivs;
    float x, z;
    int vidx = 0, tidx = 0;
    for (int i = 0; i <= zdivs; i++)
    {
        z = iFactor * i - z2;
        for (int j = 0; j <= xdivs; j++)
        {
            x = jFactor * j - x2;
            p[vidx] = x;
            p[vidx + 1] = 0.0f;
            p[vidx + 2] = z;
            n[vidx] = 0.0f;
            n[vidx + 1] = 1.0f;
            n[vidx + 2] = 0.0f;

            tex[tidx] = j * texi;
            tex[tidx + 1] = i * texj;

            vidx += 3;
            tidx += 2;
        }
    }

    unsigned int rowStart, nextRowStart;
    int idx = 0;
    for (int i = 0; i < zdivs; i++)
    {
        rowStart = (unsigned int)(i * (xdivs + 1));
        nextRowStart = (unsigned int)((i + 1) * (xdivs + 1));
        for (int j = 0; j < xdivs; j++)
        {
            el[idx] = rowStart + j;
            el[idx + 1] = nextRowStart + j;
            el[idx + 2] = nextRowStart + j + 1;
            el[idx + 3] = rowStart + j;
            el[idx + 4] = nextRowStart + j + 1;
            el[idx + 5] = rowStart + j + 1;
            idx += 6;
        }
    }

    createBuffers(&el, &p, &n, &tex);
}