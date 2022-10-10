#define PI           3.14159265358979323846

namespace triangle {

inline mat4x4 mat4x4_identity() {
  mat4x4 M;
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      M[i][j] = i == j ? 1. : 0.;
  return M;
}

inline mat2x2 mat2x2_mul(mat2x2 const& a, mat2x2 const& b) {
    mat2x2 temp;
    for (int c = 0; c < 2; ++c)
        for (int r = 0; r < 2; ++r) {
            temp[c][r] = 0;
            for (int k = 0; k < 2; ++k)
                temp[c][r] += a[k][r] * b[c][k];
        }
    return temp;
}

inline mat4x4 mat4x4_mul(mat4x4 const& a, mat4x4 const& b) {
  mat4x4 temp;
  for (int c = 0; c < 4; ++c)
    for (int r = 0; r < 4; ++r) {
      temp[c][r] = 0;
      for (int k = 0; k < 4; ++k)
        temp[c][r] += a[k][r] * b[c][k];
    }
  return temp;
}

inline mat4x4 mat4x4_translate_X_Y(mat4x4 const& M, float X, float Y) {
    mat4x4 T = {{
        {{1, 0, 0, 0}},   //
        {{0, 1, 0, 0}},  //
        {{0, 0, 1, 0}},   //
        {{X, Y, 0, 1}}    //
    }};
    return mat4x4_mul(M, T);
    }

inline mat4x4 mat4x4_rotate_Z(mat4x4 const& M, float angle) {
  float s = std::sin(angle);
  float c = std::cos(angle);
  mat4x4 R = {{
      {{c, s, 0, 0}},   //
      {{-s, c, 0, 0}},  //
      {{0, 0, 1, 0}},   //
      {{0, 0, 0, 1}}    //
  }};
  return mat4x4_mul(M, R);
}

inline mat4x4 mat4x4_ortho(float l, float r, float b, float t, float n, float f) {
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

inline std::array<Vertex, 3> translate(std::array<Vertex, 3> triangle, float X, float Y) {
    for (size_t i(0); i<triangle.size(); i++) {
        triangle[i].pos[0] += X;
        triangle[i].pos[1] += Y;
    }
    return triangle;
}

inline std::array<Vertex, 3> rotate(std::array<Vertex, 3> triangle, float angle, vec2 origin) {
    float s = std::sin(angle);
    float c = std::cos(angle);
    float x0 = origin[0];
    float y0 = origin[1];
    for (size_t i(0); i<triangle.size(); i++) {
        triangle[i].pos[0] = x0 + c*(triangle[i].pos[0]-x0) + s*(triangle[i].pos[1]-y0);
        triangle[i].pos[1] = y0 - s*(triangle[i].pos[0]-x0) + c*(triangle[i].pos[1]-y0);
    }
    return triangle;
}

inline std::array<Vertex, 3> newTriangle(vec2 center, vec3 color) {
    float a = 0.05;
    float l = a*std::sqrt(3)/2;

    float x0 = center[0];
    float y0 = center[1];
    float x1 = x0;
    float y1 = y0+(2*l/3);

    float x2 = x0+a/2;
    float y2 = y0-l/3;

    float x3 = x0-a/2;
    float y3 = y0-l/3;

    std::array<Vertex, 3> triangle = {{
        // (position 2d + color 3d pack)
        {{x1, y1}, color},  //
        {{x2, y2}, color},  //
        {{x3, y3}, color}   //
    }};
    return triangle;
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