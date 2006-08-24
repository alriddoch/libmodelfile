/* Copyright (C) 2005 Alistair Riddoch <alriddoch@zepler.org>
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
 * 2. Altered source versions must be plainly marked as such.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <libmd3/endian.h>
#include <libmd3/structure.h>

void swap_32(uint8_t * ptr)
{
    uint8_t tmp = ptr[0];
    ptr[0] = ptr[3];
    ptr[3] = tmp;
    tmp = ptr[1];
    ptr[1] = ptr[2];
    ptr[2] = tmp;
}

void swap_16(uint8_t * ptr)
{
    uint8_t tmp = ptr[0];
    ptr[0] = ptr[1];
    ptr[1] = tmp;
}

#if (LIBMD3_BYTE_ORDER != LIBMD3_LITTLE_ENDIAN)
static void swap_int32(int32_t * num)
{
    swap_32((uint8_t*)num);
}

static void swap_uint32(uint32_t * num)
{
    swap_32((uint8_t*)num);
}

static void swap_float(float * num)
{
    swap_32((uint8_t*)num);
}

static void swap_int16(int16_t * num)
{
    swap_16((uint8_t*)num);
}
#endif

void libmd3_endian_header(md3_header * header)
{
#if (LIBMD3_BYTE_ORDER != LIBMD3_LITTLE_ENDIAN)
    swap_int32(&header->version);
    swap_int32(&header->frame_count);
    swap_int32(&header->tag_count);
    swap_int32(&header->mesh_count);
    swap_int32(&header->skin_count);
    swap_int32(&header->frame_start);
    swap_int32(&header->tag_start);
    swap_int32(&header->mesh_start);
    swap_int32(&header->file_end);
#endif
}

static void libmd3_endian_frame(md3_frame * frame)
{
#if (LIBMD3_BYTE_ORDER != LIBMD3_LITTLE_ENDIAN)
    swap_float(&frame->mins[0]);
    swap_float(&frame->mins[1]);
    swap_float(&frame->mins[2]);
    swap_float(&frame->maxs[0]);
    swap_float(&frame->maxs[1]);
    swap_float(&frame->maxs[2]);
    swap_float(&frame->position[0]);
    swap_float(&frame->position[1]);
    swap_float(&frame->position[2]);
    swap_float(&frame->scale);
#endif
}

void libmd3_endian_frames(md3_frame * frames, int count)
{
#if (LIBMD3_BYTE_ORDER != LIBMD3_LITTLE_ENDIAN)
    int i;
    md3_frame * frame;

    for (frame = frames, i = 0; i < count; ++frames, ++i) {
        libmd3_endian_frame(frame);
    }
#endif
}

void libmd3_endian_mesh(md3_mesh * mesh)
{
#if (LIBMD3_BYTE_ORDER != LIBMD3_LITTLE_ENDIAN)
    swap_int32(&mesh->frame_count);
    swap_int32(&mesh->skin_count);
    swap_int32(&mesh->vertex_count);
    swap_int32(&mesh->triangle_count);
    swap_int32(&mesh->triangle_start);
    swap_int32(&mesh->skin_start);
    swap_int32(&mesh->texcoord_start);
    swap_int32(&mesh->vertex_start);
    swap_int32(&mesh->mesh_len);
#endif
}

void libmd3_endian_tag(md3_tag * tag)
{
#if (LIBMD3_BYTE_ORDER != LIBMD3_LITTLE_ENDIAN)
    float * r = tag->rotation;
    unsigned int i;
    
    swap_float(&tag->position[0]);
    swap_float(&tag->position[1]);
    swap_float(&tag->position[2]);
    
    for (i=0; i<9; ++i, ++r)
        swap_float(r);
#endif
}

void libmd3_endian_tags(md3_tag * tags, int count)
{
#if (LIBMD3_BYTE_ORDER != LIBMD3_LITTLE_ENDIAN)
    int i;
    md3_tag * tag;

    for (tag = tags, i = 0; i < count; ++tag, ++i)
        libmd3_endian_tag(tag);
    
#endif    
}

void libmd3_endian_int32s(int32_t *d, int count)
{
#if (LIBMD3_BYTE_ORDER != LIBMD3_LITTLE_ENDIAN)
    int i;

    for (i = 0; i < count; ++d, ++i)
        swap_int32(d);
#endif 
}

void libmd3_endian_floats(float * d, int count)
{
#if (LIBMD3_BYTE_ORDER != LIBMD3_LITTLE_ENDIAN)
    int i;

    for (i = 0; i < count; ++d, ++i)
        swap_float(d);
#endif
}

void libmd3_endian_vertices(int16_t* d, int num_vertices)
{
#if (LIBMD3_BYTE_ORDER != LIBMD3_LITTLE_ENDIAN)
    int i;

    for (i = 0; i < num_vertices; ++i, ++d)
    {
        swap_int16(d++);
        swap_int16(d++);
        swap_int16(d++);
        /* extra int16 encodes normal, skipped by ++d in loop update */
    }
#endif
}
