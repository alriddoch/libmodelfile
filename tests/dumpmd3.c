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

#include <stdlib.h>
#include <stdio.h>

static void usage(const char * prgname)
{
    printf("usage: %s <md3file>\n", prgname);
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

    printf("Got %s header version %d\n", file->header->version == 15 ? "standard" : "non-standard", file->header->version);

    if (strnlen(file->header->filename, 68) == 68) {
        fprintf(stderr, "Unterminated string for name in header.\n");
        file->header->filename[67] = '\0';
    }

    printf("Header: filename = \"%s\"\n", file->header->filename);
    printf("Header: frame count = %d\n", file->header->frame_count);
    printf("Header: tag count = %d\n", file->header->tag_count);
    printf("Header: mesh count = %d\n", file->header->mesh_count);
    printf("Header: skin count = %d\n", file->header->skin_count);
    printf("Header: header len = %d\n", file->header->header_len);
    printf("Header: tag start = %d\n", file->header->tag_start);
    printf("Header: tag end = %d\n", file->header->tag_end);
    printf("Header: size = %d\n", file->header->size);
}
