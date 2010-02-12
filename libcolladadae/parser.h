/* Copyright (C) 2010 Alistair Riddoch <alriddoch@googlemail.com>
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

#ifndef LIBCOLLADADAE_PARSER_H
#define LIBCOLLADADAE_PARSER_H

typedef struct _libcolladadae_file {
    void * place_holder;
} libcolladadae_file;

libcolladadae_file * libcolladadae_file_load(const char * filename);
void libcolladadae_free(libcolladadae_file * file);

#endif /* LIBCOLLADADAE_PARSER_H */
