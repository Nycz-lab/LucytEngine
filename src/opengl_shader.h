#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <glad/gl.h>
#include <string>
#include <format>
#include <iostream>
#include <fstream>
#include <sstream>

#define SHADER_PATH "src/"

class OpenGL_Shader{
    public:
        GLuint ID;
        std::string name;

        OpenGL_Shader(const char* name);
        OpenGL_Shader(const char* vertexPath, const char* fragmentPath);
        void render();
        void init(const char* vertexPath, const char* fragmentPath);


    private:
        GLuint vert_shader_id;
        GLuint frag_shader_id;

        GLuint compile(const char* shader_src, GLenum shader_type);
        GLuint link();
        std::string read_shader_source(const char* path);
};

#endif
