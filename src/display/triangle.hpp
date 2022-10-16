#pragma once

#include "../config/config.h"

namespace triangle {

inline mat4x4 mat4x4_ortho(Real l, Real r, Real b, Real t, Real n, Real f) {
  mat4x4 M{};
  M[0][0] = 2 / (r - l);
  M[1][1] = 2 / (t - b);
  M[2][2] = -2 / (f - n);
  M[3][0] = -(r + l) / (r - l);
  M[3][1] = -(t + b) / (t - b);
  M[3][2] = -(f + n) / (f - n);
  M[3][3] = 1;
  return M;
}

struct Vertex {
  vec2 pos;
  vec3 col;
};

inline std::array<Vertex, 3> rotate(std::array<Vertex, 3> triangle, Real angle, vec2 origin) {
    Real s = std::sin(angle);
    Real c = std::cos(angle);
    Real x0 = origin[0];
    Real y0 = origin[1];
    Real tempX, tempY;
    for (auto & i : triangle) {
        tempX = x0 + c*(i.pos[0]-x0) - s*(i.pos[1]-y0);
        tempY = y0 + s*(i.pos[0]-x0) + c*(i.pos[1]-y0);
        i.pos[0] = tempX;
        i.pos[1] = tempY;
    }
    return triangle;
}

inline vec2 center(std::array<Vertex, 3> triangle) {
    return {
            (triangle[0].pos[0] + triangle[1].pos[0] + triangle[2].pos[0])/3,
            (triangle[0].pos[1] + triangle[1].pos[1] + triangle[2].pos[1])/3,
    };
}

inline std::array<Vertex, 3> newTriangle(vec2 center, vec3 color, Real orientation, Real a) {
    Real b = a/2;
    Real h = a*std::sqrt(5)/3;
    Real l = 2*h/3;

    Real x0 = center[0];
    Real y0 = center[1];

    Real x1 = x0+l;
    Real y1 = y0;

    Real x2 = x0-l/2;
    Real y2 = y0+b/2;

    Real x3 = x0-l/2;
    Real y3 = y0-b/2;

    std::array<Vertex, 3> triangle = {{
        // (position 2d + color 3d pack)
        {{x1, y1}, color},  //
        {{x2, y2}, color},  //
        {{x3, y3}, color}   //
    }};

    triangle = rotate(triangle, orientation, center);

    return triangle;
}

inline std::vector<std::array<Vertex, 3>> newObstacle(vec2 center, vec3 color, Real h, Real w) {

    std::vector<std::array<Vertex, 3>> obs;

    Real x0 = center[0];
    Real y0 = center[1];

    Real x1 = x0-(w/2);
    Real y1 = y0+(h/2);

    Real x2 = x0-(w/2);
    Real y2 = y0-(h/2);

    Real x3 = x0+(w/2);
    Real y3 = y0-(h/2);

    obs.push_back({{
            // (position 2d + color 3d pack)
            {{x1, y1}, color},  //
            {{x2, y2}, color},  //
            {{x3, y3}, color}   //
        }});

    x2 = x0+(w/2);
    y2 = y0+(h/2);

    obs.push_back({{
                           // (position 2d + color 3d pack)
                           {{x1, y1}, color},  //
                           {{x2, y2}, color},  //
                           {{x3, y3}, color}   //
                   }});

    return obs;
}

static const char* const vertex_shader_text
    = "#version 330\n"
      "uniform mat4 MVP;\n"
      "in vec3 vCol;\n"
      "in vec2 vPos;\n"
      "out vec3 color;\n"
      "void main()\n"
      "{\n"
      "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
      "    color = vCol; // pass the color along to the fragment shader\n"
      "}\n";

static const char* const fragment_shader_text
    = "#version 330\n"
      "in vec3 color;\n"
      "out vec4 fragment;\n"
      "void main()\n"
      "{\n"
      "    // Set the fragment color to the color passed from the vertex shader\n"
      "    fragment = vec4(color, 1.0);\n"
      "}\n";

}  // namespace triangle