/* Copyright (C) 2004 Alistair Riddoch <alriddoch@zepler.org>
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifdef WIN32
#include <Windows.h>
#endif

#include <SDL/SDL.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <math.h>

#define M_PI 3.14159265f

static int done = 0;

static const int step_time = 1000;

int initScreen()
{
    const int width = 400;
    const int height = 400;

    /* Initialise SDL */
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "Failed to initialise video\n");
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Surface * screen;

    /* Create the window */
    if ((screen = SDL_SetVideoMode(width, height, 0, SDL_OPENGL)) == NULL) {
        fprintf(stderr, "Failed to set video mode\n");
        SDL_Quit();
        return 1;
    }

    /* Setup the viewport transform */
    glViewport(0,0,width,height);

    /* Enable the depth test */
    glEnable(GL_DEPTH_TEST);

    glEnableClientState(GL_VERTEX_ARRAY);

    /* Set the colour the screen will be when cleared - black */
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(0.3, 0.3, 0.3);

    /* Set the projection transform */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, width/height, 1.f, 100.f);

    return 0;
}

void draw_one_block()
{
    static const float front_vertices[] = {
        0.f, 0.f, 1.f,
        1.f, 0.f, 1.f,
        1.f, 1.f, 1.f,
        0.f, 1.f, 1.f,
    };
    glVertexPointer(3, GL_FLOAT, 0, front_vertices);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float left_vertices[] = {
        0.f, 0.f, 0.f,
        0.f, 0.f, 1.f,
        0.f, 1.f, 1.f,
        0.f, 1.f, 0.f,
    };
    glVertexPointer(3, GL_FLOAT, 0, left_vertices);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float right_vertices[] = {
        1.f, 0.f, 1.f,
        1.f, 0.f, 0.f,
        1.f, 1.f, 0.f,
        1.f, 1.f, 1.f,
    };
    glVertexPointer(3, GL_FLOAT, 0, right_vertices);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float top_vertices[] = {
        0.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 0.f,
        0.f, 1.f, 0.f,
    };
    glVertexPointer(3, GL_FLOAT, 0, top_vertices);
    glDrawArrays(GL_QUADS, 0, 4);

    static const float bottom_vertices[] = {
        0.f, 0.f, 0.f,
        1.f, 0.f, 0.f,
        1.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
    };
    glVertexPointer(3, GL_FLOAT, 0, bottom_vertices);
    glDrawArrays(GL_QUADS, 0, 4);
}

static float rot = 0.0f;

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Set up the modelview - camera 20 units from the objects */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -20.0f);

    /* Add a little camera movement */
    glRotatef(10, sin(rot), cos(rot), 0.0f);

    /* Draw the scene */
    draw_one_block();

    SDL_GL_SwapBuffers();
}

void loop()
{
    SDL_Event event;
    int elapsed_time = SDL_GetTicks();
    int last_step = elapsed_time;
    int fps = 0;

    /* This is the main program loop. It will run until something sets
     * the flag to indicate we are done. */
    while (!done) {
        /* Check for events */
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    /* The user closed the window */
                    done = 1;
                    break;
                case SDL_KEYDOWN:
                    /* We have a keypress */
                    if ( event.key.keysym.sym == SDLK_ESCAPE ) {
                        /* quit */
                        done = 1;
                    }
                    break;
                default:
                    break;
            }
        }
        /* Get the time and check if enough time has elapsed for
         * the moving block to move */
        int ticks = SDL_GetTicks();
        ++fps;
        if ((ticks - last_step) > step_time) {
            last_step = ticks;
            fps = 0;
            /* step(); */
        }
        float delta = (ticks - elapsed_time) / 1000.0f;
        elapsed_time = ticks;

        /* Update the rotation on the camera */
        rot += delta;
        if (rot > (2 * M_PI)) {
            rot -= (2 * M_PI);
        }

        /* Render the screen */
        render();
    }
}

int main(int argc, char ** argv)
{
    if (initScreen()) {
        return 1;
    }

    loop();
    return 0;
}

#ifdef WIN32

int WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
    main();
}

#endif