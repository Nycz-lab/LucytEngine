#include "opengl_shader.h"
#include <fstream>
#include "spdlog/spdlog.h"

OpenGL_Shader::OpenGL_Shader(const char* name){
    this->name = name;

    std::string vert_path = std::format(
            "{}{}_vert.glsl", SHADER_PATH, name);

    std::string frag_path = std::format(
            "{}{}_frag.glsl", SHADER_PATH, name);

    spdlog::debug("Vertex Shader Path: {} ; Fragment Shader Path {}", vert_path, frag_path);


    this->init(vert_path.c_str(), frag_path.c_str());
}

void OpenGL_Shader::init(const char* vertexPath, const char* fragmentPath){


    std::string vertexCode = read_shader_source(vertexPath);
    std::string fragCode = read_shader_source(fragmentPath);

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
        spdlog::error("Error reading shader files for shader {} at {}", this->name, path);
        return "";
    }

    return code;
}

GLuint OpenGL_Shader::compile(const char* shader_src, GLenum shader_type){

    // we need this since we cant print the enum name
    const char* shader_type_str;

    switch(shader_type){
        case GL_VERTEX_SHADER: shader_type_str = "Vertex Shader"; break;
        case GL_FRAGMENT_SHADER: shader_type_str = "Fragment Shader"; break;
        default: return 0; // TODO throw exception or sum
    }

    spdlog::info("compiling {} for {}", shader_type_str, this->name);
    // compile shader
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_src, NULL);
    glCompileShader(shader);

    // get infoLog buffer size per opengl implementation
    GLint infoLogSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogSize);

    GLint success;
    GLchar infoLog[infoLogSize];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        spdlog::error("Shader Compilation failed for shader: {} failed with error: {}", this->name, (char*) infoLog);
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
        spdlog::error("Shader Linking failed, with error: {}", (char*) infoLog);
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
