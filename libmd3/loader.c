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

#include <libmd3/loader.h>
#include <libmd3/mesh.h>
#include <libmd3/structure.h>

#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

static int libmd3_frame_load(FILE * fptr, libmd3_file * file)
{
    assert(fptr != NULL);
    assert(file != NULL);
    assert(file->header != NULL);

    if (file->header->frame_count == 0) {
        return 0;
    }

    md3_frame * frames = calloc(file->header->frame_count, sizeof(md3_frame));
    if (frames == NULL) {
        perror("calloc");
        return 1;
    }
    int cnt = fread(frames, sizeof(md3_frame), file->header->frame_count, fptr);
    if (cnt != file->header->frame_count) {
        fprintf(stderr, "Unexpected end of file.\n");
        free(frames);
        return 1;
    }
    file->frames = frames;
    return 0;
}

static int libmd3_tag_load(FILE * fptr, libmd3_file * file)
{
    assert(fptr != NULL);
    assert(file != NULL);
    assert(file->header != NULL);

    if (file->header->tag_start != (sizeof(md3_header) + sizeof(md3_frame) *
                                    file->header->frame_count)) {
        fprintf(stderr, "Unexpected tag start pos in header.\n");
        return 1;
    }

    int num_tags = file->header->frame_count * file->header->tag_count;
    if (num_tags == 0) {
        return 0;
    }

    size_t len_tags = num_tags * sizeof(md3_tag);
    printf("Actual len %d, stated len %d\n", len_tags, file->header->tag_end - file->header->tag_start);
    if (len_tags != (file->header->tag_end - file->header->tag_start)) {
        fprintf(stderr, "Unexpected tag size in header.\n");
        return 1;
    }

    md3_tag * tags = calloc(num_tags, sizeof(md3_tag));
    if (tags == NULL) {
        perror("calloc");
        return 1;
    }
    int cnt = fread(tags, sizeof(md3_tag), num_tags, fptr);
    if (cnt != num_tags) {
        fprintf(stderr, "Unexpected end of file.\n");
        free(tags);
        return 1;
    }
    file->tags = tags;
    return 0;
}

static int libmd3_mesh_load(FILE * fptr, libmd3_mesh * mesh)
{
    int cnt;
    int num_indices;
    int num_texcoords;
    int num_vertices;
    size_t vertex_size;

    /* We need a way to handle multiple mesh strcuture */

    mesh->mesh_header = calloc(1, sizeof(md3_mesh));
    if (mesh->mesh_header == NULL) {
        perror("calloc");
        free(mesh);
        return 1;
    }

    if (fread(mesh->mesh_header, sizeof(md3_mesh), 1, fptr) != 1) {
        fprintf(stderr, "Unexpected end of file.\n");
        free(mesh->mesh_header);
        free(mesh);
        return 1;
    }

    if ((mesh->mesh_header->ident[0] != 'I') ||
        (mesh->mesh_header->ident[1] != 'D') ||
        (mesh->mesh_header->ident[2] != 'P') ||
        (mesh->mesh_header->ident[3] != '3')) {
        fprintf(stderr, "Invalid ident \"%c%c%c%c\" in mesh header.\n",
                        mesh->mesh_header->ident[0],
                        mesh->mesh_header->ident[1],
                        mesh->mesh_header->ident[2],
                        mesh->mesh_header->ident[3]);
        free(mesh->mesh_header);
        free(mesh);
        return 1;
    }

#if 0
    if (mesh->mesh_header->header_len > sizeof(md3_mesh)) {
        /* FIXME Should we save the extra header */
        printf("Skipping extra mesh header\n");
        cnt = fseek(fptr, mesh->mesh_header->header_len - sizeof(md3_mesh),
                    SEEK_CUR);
        if (cnt != 0) {
            fprintf(stderr, "Unexpected end of file.\n");
            free(mesh->mesh_header);
            free(mesh);
            return 1;
        }
    }
#endif

    mesh->skins = calloc(mesh->mesh_header->skin_count, sizeof(md3_skin));
    if (mesh->skins == NULL) {
        perror("calloc");
        free(mesh->mesh_header);
        free(mesh);
        return 1;
    }

    cnt = fread(mesh->skins, sizeof(md3_skin),
                    mesh->mesh_header->skin_count, fptr);
    if (cnt != mesh->mesh_header->skin_count) {
        fprintf(stderr, "Unexpected end of file.\n");
        free(mesh->mesh_header);
        free(mesh->skins);
        free(mesh);
        return 1;
    }

    num_indices = mesh->mesh_header->triangle_count * 3;
    mesh->triangles = calloc(num_indices, sizeof(int32_t));
    if (mesh->triangles == NULL) {
        perror("calloc");
        free(mesh->mesh_header);
        free(mesh->skins);
        free(mesh);
        return 1;
    }

    cnt = fread(mesh->triangles, sizeof(int32_t), num_indices, fptr);
    if (cnt != num_indices) {
        fprintf(stderr, "Unexpected end of file.\n");
        free(mesh->mesh_header);
        free(mesh->skins);
        free(mesh->triangles);
        free(mesh);
        return 1;
    }

    num_texcoords = mesh->mesh_header->vertex_count * 2;
    mesh->texcoords = calloc(num_texcoords, sizeof(float));
    if (mesh->texcoords == NULL) {
        perror("calloc");
        free(mesh->mesh_header);
        free(mesh->skins);
        free(mesh->triangles);
        free(mesh);
        return 1;
    }

    cnt = fread(mesh->texcoords, sizeof(float), num_texcoords, fptr);
    if (cnt != num_texcoords) {
        fprintf(stderr, "Unexpected end of file.\n");
        free(mesh->mesh_header);
        free(mesh->skins);
        free(mesh->triangles);
        free(mesh->texcoords);
        free(mesh);
        return 1;
    }

    vertex_size = sizeof(int16_t) * 3 + sizeof(int8_t) * 2;
    num_vertices = mesh->mesh_header->vertex_count * 
                   mesh->mesh_header->frame_count;
    mesh->vertices = calloc(num_vertices, vertex_size);
    if (mesh->vertices == NULL) {
        perror("calloc");
        free(mesh->mesh_header);
        free(mesh->skins);
        free(mesh->triangles);
        free(mesh->texcoords);
        free(mesh);
        return 1;
    }

    cnt = fread(mesh->vertices, vertex_size, num_vertices, fptr);
    if (cnt != num_vertices) {
        fprintf(stderr, "Unexpected end of file.\n");
        free(mesh->mesh_header);
        free(mesh->skins);
        free(mesh->triangles);
        free(mesh->texcoords);
        free(mesh->vertices);
        free(mesh);
        return 1;
    }

    return 0;
}

static int libmd3_meshes_load(FILE * fptr, libmd3_file * file)
{
    int i;
    libmd3_mesh * meshes;

    assert(fptr != NULL);
    assert(file != NULL);
    assert(file->header != NULL);

    if (file->header->mesh_count == 0) {
        return 0;
    }

    meshes = calloc(file->header->mesh_count, sizeof(libmd3_mesh));
    if (meshes == NULL) {
        perror("calloc");
        return 1;
    }

    libmd3_mesh * meshp = meshes;
    for(i = 0; i < file->header->mesh_count; ++i, ++meshp) {
        libmd3_mesh_load(fptr, meshp);
    }

    file->meshes = meshes;
    return 0;
}

static md3_header * libmd3_header_load(FILE * fptr)
{
    size_t len;
    md3_header * header = calloc(1, sizeof(md3_header));
    if (header == NULL) {
        printf("2\n");
        return NULL;
    }

    if ((len = fread(header, sizeof(md3_header), 1, fptr)) != 1) {
        printf("3 %d %d\n", len, sizeof(md3_header));
        free(header);
        return NULL;
    }

    if ((header->ident[0] != 'I') ||
        (header->ident[1] != 'D') ||
        (header->ident[2] != 'P') ||
        (header->ident[3] != '3')) {
        printf("4\n");
        free(header);
        return NULL;
    }

    if (header->header_len > sizeof(md3_header)) {
        /* FIXME Should we save the extra header */
        printf("Skipping extra header\n");
        len = fseek(fptr, header->header_len - sizeof(md3_header), SEEK_CUR);
        if (len != 0) {
            fprintf(stderr, "Unexpected end of file.\n");
            free(header);
            return NULL;
        }
    }

    return header;
}

libmd3_file * libmd3_file_load(const char * filename)
{
    FILE * fptr = 0;
    md3_header * header = 0;
    libmd3_file * file = 0;

    fptr = fopen(filename, "rb");
    if (fptr == NULL) {
        printf("1\n");
        return NULL;
    }

    header = libmd3_header_load(fptr);

    file = calloc(1, sizeof(libmd3_file));
    if (file == NULL) {
        printf("5\n");
        free(header);
        return NULL;
    }

    file->header = header;

    libmd3_frame_load(fptr, file);
    libmd3_tag_load(fptr, file);
    libmd3_meshes_load(fptr, file);

    return file;
}
