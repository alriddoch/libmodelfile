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

libmd3_file * libmd3_file_load(const char * filename)
{
    FILE * file;
    md3_header * header;
    libmd3_file * lf;
    size_t len;

    file = fopen(filename, "rb");
    if (file == NULL) {
        printf("1\n");
        return NULL;
    }

    header = calloc(0, sizeof(md3_header));
    if (header == NULL) {
        printf("2\n");
        return NULL;
    }

    if ((len = fread(header, sizeof(md3_header), 1, file)) != 1) {
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

    lf = calloc(0, sizeof(libmd3_file));
    if (lf == NULL) {
        printf("5\n");
        free(header);
        return NULL;
    }

    lf->header = header;
    return lf;
}
