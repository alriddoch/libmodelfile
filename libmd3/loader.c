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

    if (file->header->tag_start != (sizeof(md3_header) +
                                    sizeof(md3_frame) * file->header->frame_count)) {
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

static int libmd3_skin_load(FILE * fptr, libmd3_file * file)
{
    return 0;
}

static int libmd3_mesh_load(FILE * fptr, libmd3_file * file)
{
    assert(fptr != NULL);
    assert(file != NULL);
    assert(file->header != NULL);


    return 0;
}

static md3_header * libmd3_header_load(FILE * fptr)
{
    size_t len;
    md3_header * header = calloc(0, sizeof(md3_header));
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

    file = calloc(0, sizeof(libmd3_file));
    if (file == NULL) {
        printf("5\n");
        free(header);
        return NULL;
    }

    file->header = header;

    libmd3_frame_load(fptr, file);
    libmd3_tag_load(fptr, file);
    libmd3_mesh_load(fptr, file);
    libmd3_skin_load(fptr, file);

    return file;
}
