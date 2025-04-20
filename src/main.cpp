
#include <GLES2/gl2.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <glad/gl.h>
#include "SDL3/SDL_events.h"
#include "lucyt_version.h"

#define WINDOW_NAME "LucytEngine Window"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void process_input(SDL_Event* event, bool* running){
    if (event->type == SDL_EVENT_QUIT || event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
    {
        *running = false;
    }
}

int init_sdl(){

    if ( !SDL_Init(SDL_INIT_VIDEO) ){
        /* Failed initializing */
        std::cout << "failed initializing sdl video, exiting";
        return -1;
    }


    SDL_WindowFlags flags = SDL_WINDOW_OPENGL;

    SDL_Window* window = SDL_CreateWindow(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, flags);
    SDL_GLContext ctx = SDL_GL_CreateContext( window );


    if (!gladLoadGL(SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const GLchar* vertex_shader_src =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}";

    const GLchar* fragment_shader_src =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
            "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}";

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertex_shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "Compilation failed: \n" << infoLog << std::endl;
    }

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fragment_shader_src, NULL);
    glCompileShader(frag_shader);

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(frag_shader, 512, NULL, infoLog);
        std::cout << "Compilation failed: \n" << infoLog << std::endl;
    }

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, frag_shader);
    glLinkProgram(shader_program);

    glGetShaderiv(shader_program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "Compilation failed: \n" << infoLog << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);
    glUseProgram(shader_program);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    SDL_Event event;

    bool running = true;

    while(running){
        SDL_PollEvent(&event);

        process_input(&event, &running);

        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);



        SDL_GL_SwapWindow(window);
    }

    return 0;
}

int main(int argc, char** argv){

    std::cout << LUCYT_NAME << " version: " << LUCYT_VERSION << " by " << LUCYT_AUTHOR << std::endl;

    init_sdl();

    return 0;
}
