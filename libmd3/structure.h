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

#ifndef LIBMD3_STRUCTURE_H
#define LIBMD3_STRUCTURE_H

#include <stdint.h>

/* This file contains definitions for the structures in a .md3 file on disk
 * They are typically used as targets for reading blocks of data from
 * disk.
 */

typedef struct _md3_header {
    uint8_t         ident[4];       /* Always "IDP3" */
    int32_t         version;        /* Always 15 */
    uint8_t         filename[68];   /* Name of file */
    int32_t         frame_count;    /* Number of frames */
    int32_t         tag_count;      /* Number of tags */
    int32_t         mesh_count;     /* Number of meshes */
    int32_t         skin_count;     /* Number of skins */
    int32_t         header_len;     /* Header length */
    int32_t         tag_start;      /* Start of tags */
    int32_t         tag_end;        /* End of tags */
    int32_t         size;           /* File size */
} md3_header;

typedef struct _md3_frame {
    float           mins[3];
    float           maxs[3];
    float           position[3];
    float           scale;
    int8_t          string[16];
} md3_frame;

typedef struct _md3_tag {
    int8_t          name[64];       /* Name */
    float           postition[3];   /* Position */
    float           rotation[9];    /* Rotation */
} md3_tag;

typedef struct _md3_mesh {
    uint8_t         ident[4];       /* Always "IDP3" */ 
    uint8_t         name[68];       /* Name */ 
    int32_t         frame_count;    /* Number of mesh frames */
    int32_t         skin_count;     /* Number of skins */
    int32_t         vertex_count;   /* Number of vertices */
    int32_t         triangle_count; /* Number of triangles */
    int32_t         triangle_start; /* Relative start of triangles */
    int32_t         header_len;     /* Header length */
    int32_t         texcoord_start; /* Relative start of texcoords */
    int32_t         vertex_start;   /* Relative start of vertices */
    int32_t         mesh_len;       /* Length of this mesh */
} md3_mesh;

typedef struct _md3_skin {
    uint8_t         Name[68];       /* Name */
} md3_skin;

typedef struct _md3_triangle {
    int32_t         vertices[3];    /* Indexes of vertices */
} md3_triangle;

typedef struct _md3_texcoord {
    float           texcoord[2];    /* Indexes of vertices */
} md3_texcoord;

#endif // LIBMD3_STRUCTURE_H
