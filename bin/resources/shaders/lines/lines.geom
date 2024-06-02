#version 450

layout (lines) in;
layout (line_strip, max_vertices=100) out;

uniform mat4 viewProjectionMatrix;
uniform mat4 inverseProjectionViewMatrix;

in vec4 vsPosition[];
in vec4 vsColor[];

out vec4 gsColor;

vec4 color = vec4(1,1,0,1);

void emitVertex(vec4 p)
{
    gsColor = color;
    gl_Position = viewProjectionMatrix * p;
    EmitVertex();
}

void emitSegment(vec4 p1, vec4 p2)
{
    gsColor = color;
    gl_Position = viewProjectionMatrix * p1;
    EmitVertex();

    gsColor = color;
    gl_Position = viewProjectionMatrix * p2;
    EmitVertex();

    EndPrimitive();
}

vec4 unscale(vec4 p)
{
    return vec4(p.xyz / p.w, 1.0);
}

void main()
{
    vec4 center = unscale(inverseProjectionViewMatrix * vec4(0,0,0,1));
    vec4 corner1 = unscale(inverseProjectionViewMatrix * vec4(-1,-1,1,1));
    vec4 corner2 = unscale(inverseProjectionViewMatrix * vec4(1,-1,1,1));
    vec4 corner3 = unscale(inverseProjectionViewMatrix * vec4(1,1,1,1));
    vec4 corner4 = unscale(inverseProjectionViewMatrix * vec4(-1,1,1,1));

    float radius = length(corner1 - center);

    vec4 p;
    for(float xv = -1; xv <= 1.1; xv += 0.2)
    {
        p = unscale(inverseProjectionViewMatrix * vec4(xv,0,1,1));
        p = center + (normalize(p - center) * radius);
        emitVertex(p);
    }
    EndPrimitive();

    for(float xv = -1; xv <= 1.1; xv += 0.2)
    {
        p = unscale(inverseProjectionViewMatrix * vec4(1,xv,1,1));
        p = center + (normalize(p - center) * radius);
        emitVertex(p);
    }
    EndPrimitive();

    for(float xv = -1; xv <= 1.1; xv += 0.2)
    {
        p = unscale(inverseProjectionViewMatrix * vec4(-1,xv,1,1));
        p = center + (normalize(p - center) * radius);
        emitVertex(p);
    }
    EndPrimitive();

    for(float xv = -1; xv <= 1.1; xv += 0.2)
    {
        p = unscale(inverseProjectionViewMatrix * vec4(xv,1,1,1));
        p = center + (normalize(p - center) * radius);
        emitVertex(p);
    }
    EndPrimitive();

    for(float xv = -1; xv <= 1.1; xv += 0.2)
    {
        p = unscale(inverseProjectionViewMatrix * vec4(xv,-1,1,1));
        p = center + (normalize(p - center) * radius);
        emitVertex(p);
    }
    EndPrimitive();

    for(float xv = -1; xv <= 1.1; xv += 0.2)
    {
        p = unscale(inverseProjectionViewMatrix * vec4(0,xv,1,1));
        p = center + (normalize(p - center) * radius);
        emitVertex(p);
    }
    EndPrimitive();


    emitSegment(center, corner1);
    emitSegment(center, corner2);
    emitSegment(center, corner3);
    emitSegment(center, corner4);

//    emitVertex(corner1);
//    emitVertex(corner2);
//    emitVertex(corner3);
//    emitVertex(corner4);
//    emitVertex(corner1);
//    EndPrimitive();

}