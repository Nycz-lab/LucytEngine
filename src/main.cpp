
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <glad/gl.h>
#include "lucyt_version.h"

#define WINDOW_NAME "LucytEngine Window"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

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

    SDL_Event event;
    
    bool running = true;

    while(running){
        SDL_PollEvent(&event);

        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
        {
                running = false;
        }

        SDL_GL_SwapWindow(window);
    }

    return 0;
}

int main(int argc, char** argv){

    std::cout << LUCYT_NAME << " version: " << LUCYT_VERSION << " by " << LUCYT_AUTHOR;

    init_sdl();

    return 0;
}