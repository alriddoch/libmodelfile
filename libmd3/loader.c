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

    if (fseek(fptr, file->header->frame_start, SEEK_SET) != 0) {
        fprintf(stderr, "Unexpected error finding start of frames.\n");
        return 1;
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
        fprintf(stderr, "Unusual tag start pos in header.\n");
    }

    int num_tags = file->header->frame_count * file->header->tag_count;
    if (num_tags == 0) {
        return 0;
    }

    if (fseek(fptr, file->header->tag_start, SEEK_SET) != 0) {
        fprintf(stderr, "Unexpected error finding start of tags.\n");
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
    long file_pos;

    file_pos = ftell(fptr);
    if (file_pos == -1) {
        fprintf(stderr, "Unexpected error reading start pos of mesh.\n");
        free(mesh);
        return 1;
    }

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

    if (fseek(fptr, file_pos + mesh->mesh_header->skin_start, SEEK_SET) != 0) {
        fprintf(stderr, "Unexpected error finding start of skins.\n");
        free(mesh->mesh_header);
        free(mesh);
        return 1;
    }

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

    if (fseek(fptr, file_pos + mesh->mesh_header->triangle_start, SEEK_SET) != 0) {
        fprintf(stderr, "Unexpected error finding start of tringles.\n");
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

    if (fseek(fptr, file_pos + mesh->mesh_header->texcoord_start, SEEK_SET) != 0) {
        fprintf(stderr, "Unexpected error finding start of texcoords.\n");
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

    if (fseek(fptr, file_pos + mesh->mesh_header->vertex_start, SEEK_SET) != 0) {
        fprintf(stderr, "Unexpected error finding start of texcoords.\n");
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
    long file_pos;

    assert(fptr != NULL);
    assert(file != NULL);
    assert(file->header != NULL);

    if (file->header->mesh_count == 0) {
        return 0;
    }

    file_pos = file->header->mesh_start;
    if (fseek(fptr, file_pos, SEEK_SET) != 0) {
        fprintf(stderr, "Unexpected error seeking to meshes\n");
        return 1;
    }

    meshes = calloc(file->header->mesh_count, sizeof(libmd3_mesh));
    if (meshes == NULL) {
        perror("calloc");
        return 1;
    }

    libmd3_mesh * meshp = meshes;
    for(i = 0; i < file->header->mesh_count; ++i, ++meshp) {
        if (libmd3_mesh_load(fptr, meshp)) {
            fprintf(stderr, "Unexpected error reading mesh %d\n", i);
            return 1;
        }
        file_pos += meshp->mesh_header->mesh_len;
        if (fseek(fptr, file_pos, SEEK_SET) != 0) {
            fprintf(stderr, "Unexpected error seeking to mesh %d\n", i + 1 + 1);
            return 1;
        }
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
