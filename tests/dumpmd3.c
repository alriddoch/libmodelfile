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

#include <libmd3/structure.h>
#include <libmd3/loader.h>
#include <libmd3/mesh.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void usage(const char * prgname)
{
    printf("usage: %s <md3file>\n", prgname);
}


static void dump_header(md3_header * header)
{
    printf("Got %s header version %d\n", header->version == 15 ? "standard" : "non-standard", header->version);

    if (strnlen(header->filename, 68) == 68) {
        fprintf(stderr, "Unterminated string for filename in file header.\n");
        header->filename[67] = '\0';
    }

    printf("Header: filename = \"%s\"\n", header->filename);
    printf("Header: frame count = %d\n", header->frame_count);
    printf("Header: tag count = %d\n", header->tag_count);
    printf("Header: mesh count = %d\n", header->mesh_count);
    printf("Header: skin count = %d\n", header->skin_count);
    printf("Header: frame start = %d\n", header->frame_start);
    printf("Header: tag start = %d\n", header->tag_start);
    printf("Header: mesh start = %d\n", header->mesh_start);
    printf("Header: file end = %d\n", header->file_end);
}

static void dump_frames(libmd3_file * file)
{
    int i, j;

    if (file->header->frame_count == 0) {
        printf("[No frames in file]\n");
        return;
    }
    for(i = 0; i < file->header->frame_count; ++i) {
        md3_frame * frame = file->frames + i;
        printf("Frame[%d]: mins = {%f, %f, %f}\n", i, frame->mins[0],
                                                      frame->mins[1],
                                                      frame->mins[2]);
        printf("Frame[%d]: maxs = {%f, %f, %f}\n", i, frame->maxs[0],
                                                      frame->maxs[1],
                                                      frame->maxs[2]);
        printf("Frame[%d]: position = {%f, %f, %f}\n", i, frame->position[0],
                                                          frame->position[1],
                                                          frame->position[2]);
        printf("Frame[%d]: scale = %f\n", i, frame->scale);
        for(j = 0; j < file->header->tag_count; ++j) {
            md3_tag * tag = file->tags + (i * file->header->tag_count + j);
            printf("Frame[%d]: Tag[%d]: name = %s\n", i, j, tag->name);
            printf("Frame[%d]: Tag[%d]: position = {%f, %f, %f}\n", i, j, 
                    tag->position[0], tag->position[1], tag->position[2]);
            printf("Frame[%d]: Tag[%d]: rotation = {%f, %f, %f, %f, %f, %f, %f, %f, %f}\n", i, j,
                   tag->rotation[0],
                   tag->rotation[1],
                   tag->rotation[2],
                   tag->rotation[3],
                   tag->rotation[4],
                   tag->rotation[5],
                   tag->rotation[6],
                   tag->rotation[7],
                   tag->rotation[8]);
        }
    }
}

static void dump_one_mesh(libmd3_mesh * mesh, int index)
{
    int i;
    md3_mesh * header = mesh->mesh_header;
    md3_skin * skinp;

    if (strnlen(header->name, 68) == 68) {
        fprintf(stderr, "Unterminated string for name in mesh header.\n");
        header->name[67] = '\0';
    }

    printf("Mesh[%d]: name = \"%s\"\n", index, header->name);
    printf("Mesh[%d]: frame_count = %d\n", index, header->frame_count);
    printf("Mesh[%d]: skin_count = %d\n", index, header->skin_count);
    printf("Mesh[%d]: vertex_count = %d\n", index, header->vertex_count);
    printf("Mesh[%d]: triangle_count = %d\n", index, header->triangle_count);
    printf("Mesh[%d]: triangle_start = %d\n", index, header->triangle_start);
    printf("Mesh[%d]: skin start %d\n", index, header->skin_start);
    printf("Mesh[%d]: texcoord_start = %d\n", index, header->texcoord_start);
    printf("Mesh[%d]: vertex_start = %d\n", index, header->vertex_start);
    printf("Mesh[%d]: mesh_len = %d\n", index, header->mesh_len);

    skinp = mesh->skins;
    for(i = 0; i < header->skin_count; ++i, ++skinp) {
        if (strnlen(skinp->Name, 68) == 68) {
            fprintf(stderr, "Unterminated string for mesh %d, skin %d.\n",
                            index, i);
            skinp->Name[67] = '\0';
        }
        printf("Mesh[%d]: Skin[%d] = \"%s\"\n", index, i, skinp->Name);
    }

    if (header->triangle_count == 0) {
        printf("No triangles in mesh %d\n", index);
        return;
    }
    printf("Mesh[%d]: Triangle indices = { %d", index, mesh->triangles[0]);
    for(i = 1; i < header->triangle_count * 3; ++i) {
        printf(", %d", mesh->triangles[i]);
    }
    printf(" }\n");

    if (header->vertex_count == 0) {
        printf("No vertices in mesh %d\n", index);
        return;
    }
    printf("Mesh[%d]: Texture Coords = { [%f, %f]", index, mesh->texcoords[0],
                                                           mesh->texcoords[1]);
    for(i = 1; i < header->vertex_count; ++i) {
        printf(" [%f, %f]", mesh->texcoords[2 * i], mesh->texcoords[2 * i + 1]);
    }
    printf(" }\n");

    printf("Mesh[%d]: Vertices = { [%hd, %hd, %hd]", index, mesh->vertices[0],
                                                            mesh->vertices[1],
                                                            mesh->vertices[2]);
    for(i = 1; i < header->vertex_count; ++i) {
        printf(" [%hd, %hd, %hd]", mesh->vertices[4 * i],
                                   mesh->vertices[4 * i + 1],
                                   mesh->vertices[4 * i + 2]);
    }
    printf(" }\n");
}

static void dump_meshes(libmd3_file * file)
{
    int i;
    libmd3_mesh * meshp;

    if (file->header->mesh_count == 0) {
        printf("[No meshes in file]\n");
        return;
    }

    meshp = file->meshes;
    for(i = 0; i < file->header->mesh_count; ++i, ++meshp) {
        dump_one_mesh(meshp, i);
    }
}

int main(int argc, char *argv[])
{
    libmd3_file * file = NULL;

    if (argc != 2) {
        usage(argv[0]);
        exit(1);
    }

    file = libmd3_file_load(argv[1]);
    if (file == NULL) {
        fprintf(stderr, "%s: Unable to load %s", argv[0], argv[1]);
        perror(argv[1]);
    }

    dump_header(file->header);
    dump_frames(file);
    dump_meshes(file);

    return 0;
}
