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

#include <libmd3/convert.h>
#include <libmd3/mesh.h>
#include <libmd3/structure.h>

#include <string.h>

#include <assert.h>

void libmd3_strip_env_texcoords(libmd3_mesh * mesh)
{
    int i;

    if (mesh->mesh_header->vertex_count < 2) {
        return;
    }

    for(i = 1; i < mesh->mesh_header->vertex_count; ++i) {
        memmove(&mesh->vertices[i * 3],
                &mesh->vertices[i * 4],
                3 * sizeof(int16_t));
    }
}
