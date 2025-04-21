
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <glad/gl.h>
#include "SDL3/SDL_events.h"
#include "lucyt_version.h"
#include "opengl_shader.h"
#include "spdlog/spdlog.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "spdlog/fmt/bin_to_hex.h"

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

    spdlog::info("initializing sdl...");

    if ( !SDL_Init(SDL_INIT_VIDEO) ){
        /* Failed initializing */
        spdlog::critical("failed initializing sdl video, exiting");
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
        spdlog::critical("Failed to initialize GLAD, exiting");
        return -1;
    }

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    spdlog::info("Renderer: {}", (char*) renderer);
    spdlog::info("OpenGL Version: {}", (char*) version);
    
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    return 0;
}

void playground(){

    float vertices[] = {
        // positions         // colors
         0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,  // bottom right
        -0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom left
         0.0f,  -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f    // top
    };

    float texCoords[] = {
          // lower-left corner  
          // lower-right corner
           // top-center corner
    };

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLsizei width, height;
    GLint nrChannels;
    unsigned char* data = stbi_load("assets/wall.jpg", &width, &height, &nrChannels, 0);
    spdlog::debug((int)nrChannels);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
    }else{
        spdlog::critical("error loading texture");
        return;
    }



    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    OpenGL_Shader bruh = OpenGL_Shader("bruh");

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

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

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        off+=0.00001;

        SDL_GL_SwapWindow(window);
    }
}

int main(int argc, char** argv){
    spdlog::info("Initializing {}, version {} by {}", LUCYT_NAME, LUCYT_VERSION, LUCYT_AUTHOR);
    spdlog::set_level(spdlog::level::debug);
    
    if(init_sdl() != 0){
        return -1;
    }

    playground();
    

    return 0;
}
