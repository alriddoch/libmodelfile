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

#include <libmd3/structure.h>
#include <libmd3/loader.h>
#include <libmd3/convert.h>
#include <libmd3/mesh.h>

#include <SDL/SDL.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <string.h>

#include <math.h>

#define M_PI 3.14159265f

static int done = 0;

static const int step_time = 1000;

static libmd3_file * modelFile = 0;

static SDL_Surface *LoadBMP(const char *filename)
{
    Uint8 *rowhi, *rowlo;
    Uint8 *tmpbuf, tmpch;
    SDL_Surface *image;
    int i, j;

    image = SDL_LoadBMP(filename);
    if ( image == NULL ) {
        fprintf(stderr, "Unable to load %s: %s\n", filename, SDL_GetError());
        return(NULL);
    }

    /* GL surfaces are upsidedown and RGB, not BGR :-) */
    tmpbuf = (Uint8 *)malloc(image->pitch);
    if ( tmpbuf == NULL ) {
        fprintf(stderr, "Out of memory\n");
        return(NULL);
    }
    rowhi = (Uint8 *)image->pixels;
    rowlo = rowhi + (image->h * image->pitch) - image->pitch;
    for ( i=0; i<image->h/2; ++i ) {
        for ( j=0; j<image->w; ++j ) {
            tmpch = rowhi[j*3];
            rowhi[j*3] = rowhi[j*3+2];
            rowhi[j*3+2] = tmpch;
            tmpch = rowlo[j*3];
            rowlo[j*3] = rowlo[j*3+2];
            rowlo[j*3+2] = tmpch;
        }
        memcpy(tmpbuf, rowhi, image->pitch);
        memcpy(rowhi, rowlo, image->pitch);
        memcpy(rowlo, tmpbuf, image->pitch);
        rowhi += image->pitch;
        rowlo -= image->pitch;
    }
    free(tmpbuf);
    return(image);
}

/* Load Bitmaps And Convert To Textures */
static GLuint LoadGLTexture(const char * filename)
{       
    /* Load Texture */
    SDL_Surface *image1;
    GLuint texture;
    
    image1 = LoadBMP(filename);
    if (!image1) {
        return 0;
    }

    /* Create Texture */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); /* 2d texture (x and y size) */

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->w, image1->h, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image1->pixels);
    return texture;
}

static int initScreen()
{
    const int width = 800;
    const int height = 600;
    SDL_Surface * screen;

    static const GLfloat AmbientColor[] = {0.6f, 0.6f, 0.7f, 1.f};
    static const GLfloat DiffuseColor[] = {1.f, 1.f, 1.00, 1.f};
    static const GLfloat lmodel_ambient[] = {0.f, 0.f, 0.f, 1.f};

    /* Initialise SDL */
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_NOPARACHUTE) != 0) {
        fprintf(stderr, "Failed to initialise video\n");
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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

    glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientColor);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseColor);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    /* Enable lighting */
    glEnable(GL_LIGHTING);

    /* Enable normalisation of normals */
    glEnable(GL_NORMALIZE);

    glEnableClientState(GL_VERTEX_ARRAY);

    /* Set the colour the screen will be when cleared - black */
    glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
    glColor3f(1.0f, 1.0f, 1.0f);

    /* Set the projection transform */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, width/height, 1.f, 1000.f);

    return 0;
}

static int setup(const char * file)
{
    int i;

    modelFile = libmd3_file_load(file);
    if (modelFile == NULL) {
        return 1;
    }

    for(i = 0; i < modelFile->header->mesh_count; ++i) {
        libmd3_unpack_normals(&modelFile->meshes[i]);
    }
    return 0;
}

static void fixPath(char * filename)
{
    int i;
    for(i = 0; i < strlen(filename); ++i) {
        if (filename[i] == '\\') {
            filename[i] = '/';
        }
    }
}

static void draw_one_mesh(libmd3_mesh * mesh)
{
    if (mesh->mesh_header->skin_count != 0) {
        if (mesh->user.u == 0) {
            fixPath(mesh->skins[0].name);
            mesh->user.u = LoadGLTexture(mesh->skins[0].name);
        }
    }

    if (mesh->user.u != 0) {
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindTexture(GL_TEXTURE_2D, mesh->user.u);
    }

    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_SHORT, 0, mesh->vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, mesh->texcoords);
    glNormalPointer(GL_FLOAT, 0, mesh->normals);
    glDrawElements(GL_TRIANGLES, mesh->mesh_header->triangle_count * 3,
                   GL_UNSIGNED_INT, mesh->triangles);

    glDisableClientState(GL_NORMAL_ARRAY);

    if (mesh->user.u != 0) {
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}

static void draw_md3_file()
{
    int i;
    libmd3_mesh * meshp;

    if (modelFile->header->mesh_count == 0) {
        printf("[No meshes in file]\n");
        return;
    }

    meshp = modelFile->meshes;
    for(i = 0; i < modelFile->header->mesh_count; ++i, ++meshp) {
        draw_one_mesh(meshp);
    }

}

static float rot = 0.0f;

static void render()
{
    static const GLfloat lightPos[] = {1.f, 0.f, 0.f, 0.f};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Set up the modelview - camera 20 units from the objects */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -60.0f);

    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);

    /* Add a little camera movement */
    glRotatef((rot * 180) / M_PI, 1, 2, 0.0f);
    /* glRotatef(10, sin(rot), cos(rot), 0.0f); */

    /* Draw the scene */
    glScalef(0.015625, 0.015625, 0.015625);
    draw_md3_file();

    SDL_GL_SwapBuffers();
}

static void loop()
{
    SDL_Event event;
    int elapsed_time = SDL_GetTicks();
    int last_step = elapsed_time;
    int fps = 0;

    /* This is the main program loop. It will run until something sets
     * the flag to indicate we are done. */
    while (!done) {
        int ticks;
        float delta;

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
        ticks = SDL_GetTicks();
        ++fps;
        if ((ticks - last_step) > step_time) {
            last_step = ticks;
            printf("fps = %d\n", fps);
            fps = 0;
            /* step(); */
        }
        delta = (ticks - elapsed_time) / 1000.0f;
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
    if (argc != 2) {
        fprintf(stderr, "No filename given\n");
        return 1;
    }

    if (initScreen()) {
        return 1;
    }

    if (setup(argv[1])) {
        fprintf(stderr, "%s: Failed to load \"%s\".\n", argv[0], argv[1]);
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
