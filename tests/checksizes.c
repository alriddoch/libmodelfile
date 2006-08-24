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

#include <libmd3/structure.h>
#include <libmd3/loader.h>
#include <libmd3/mesh.h>
#include <libmd3/convert.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
    int exit_flag = 0;

    if (sizeof(int32_t) != 4) {
        fprintf(stderr, "int32_t has incorrect size %d\n", sizeof(int32_t));
        exit_flag = 1;
    }

    if (sizeof(int16_t) != 2) {
        fprintf(stderr, "int16_t has incorrect size %d\n", sizeof(int16_t));
        exit_flag = 1;
    }

    if (sizeof(int8_t) != 1) {
        fprintf(stderr, "int8_t has incorrect size %d\n", sizeof(int8_t));
        exit_flag = 1;
    }

    if (sizeof(uint32_t) != 4) {
        fprintf(stderr, "uint32_t has incorrect size %d\n", sizeof(uint32_t));
        exit_flag = 1;
    }

    if (sizeof(uint16_t) != 2) {
        fprintf(stderr, "uint16_t has incorrect size %d\n", sizeof(uint16_t));
        exit_flag = 1;
    }

    if (sizeof(uint8_t) != 1) {
        fprintf(stderr, "uint8_t has incorrect size %d\n", sizeof(uint8_t));
        exit_flag = 1;
    }

    return exit_flag;
}
