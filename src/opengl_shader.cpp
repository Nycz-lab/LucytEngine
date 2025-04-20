#include "opengl_shader.h"
#include <fstream>

OpenGL_Shader::OpenGL_Shader(const char* name){
    this->name = name;

    std::string vert_path = std::format(
            "{}{}_vert.glsl", SHADER_PATH, name);

    std::string frag_path = std::format(
            "{}{}_frag.glsl", SHADER_PATH, name);


    this->init(vert_path.c_str(), frag_path.c_str());
}

void OpenGL_Shader::init(const char* vertexPath, const char* fragmentPath){


    std::string vertexCode = read_shader_source(vertexPath);
    std::string fragCode = read_shader_source(fragmentPath);

    //std::cout << vertexCode << std::endl << fragCode;

    this->vert_shader_id = this->compile(vertexCode.c_str(), GL_VERTEX_SHADER);
    this->frag_shader_id = this->compile(fragCode.c_str(), GL_FRAGMENT_SHADER);

    this->ID = this->link();
}

std::string OpenGL_Shader::read_shader_source(const char* path){

    // TODO check if file exists

    std::string code;

    std::ifstream ShaderFile;
    // ensure ifstream objects can throw exceptions:
    ShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {
        ShaderFile = std::ifstream(path);

        std::stringstream ShaderStream;
        ShaderStream << ShaderFile.rdbuf();

        ShaderFile.close();

        code = ShaderStream.str();
    }
    catch(std::ifstream::failure e){
        std::cout << "Error reading shader files for shader " << this->name
            << " at " << path;
        return "";
    }

    return code;
}

GLuint OpenGL_Shader::compile(const char* shader_src, GLenum shader_type){
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_src, NULL);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Shader Compilation failed for shader: " << this->name
        << " failed with error: " << infoLog << std::endl;
    }

    return shader;
}

GLuint OpenGL_Shader::link(){
    GLuint shader_program = glCreateProgram();
    GLint success;
    GLchar infoLog[512];

    glAttachShader(shader_program, this->vert_shader_id);
    glAttachShader(shader_program, this->frag_shader_id);
    glLinkProgram(shader_program);

    glGetShaderiv(shader_program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "Shader Linking failed: \n" << infoLog << std::endl;
    }

    glDeleteShader(this->vert_shader_id);
    glDeleteShader(this->frag_shader_id);

    return shader_program;
}

void OpenGL_Shader::render(){
    glUseProgram(this->ID);
}

void OpenGL_Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void OpenGL_Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void OpenGL_Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
