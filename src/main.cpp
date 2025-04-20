
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <glad/gl.h>
#include "SDL3/SDL_events.h"
#include "lucyt_version.h"
#include "opengl_shader.h"

#define WINDOW_NAME "LucytEngine Window"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

SDL_Window* window;
SDL_GLContext ctx;

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

    // Request OpenGL 3.3 Core Profile (Required on macOS)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    
    window = SDL_CreateWindow(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, flags);
    ctx = SDL_GL_CreateContext( window );
    
    
    if (!gladLoadGL(SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL Version: " << version << std::endl;
    
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    return 0;
}

void playground(){

    float vertices[] = {
        // positions         // colors
         0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  -0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    OpenGL_Shader bruh = OpenGL_Shader("bruh");

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    SDL_Event event;

    bool running = true;
    float off = 0.0f;
    while(running){
        SDL_PollEvent(&event);

        process_input(&event, &running);

        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        bruh.render();
        bruh.setFloat("offset", off);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        off+=0.001;

        SDL_GL_SwapWindow(window);
    }
}

int main(int argc, char** argv){

    std::cout << LUCYT_NAME << " version: " << LUCYT_VERSION << " by " << LUCYT_AUTHOR << std::endl;

    if(init_sdl() != 0){
        return -1;
    }

    playground();
    

    return 0;
}
