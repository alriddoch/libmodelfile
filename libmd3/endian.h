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
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef LIBMD3_ENDIAN_H
#define LIBMD3_ENDIAN_H

/* PDP_ENDIAN is ignored, as its unlikely that this code will be used
 * on such a systems */

#ifdef __linux__

 #include <endian.h>

 #define LIBMD3_LITTLE_ENDIAN   __LITTLE_ENDIAN
 #define LIBMD3_BIG_ENDIAN      __BIG_ENDIAN

 #define LIBMD3_BYTE_ORDER      __BYTE_ORDER

#else /* __linux__ */

 #define LIBMD3_LITTLE_ENDIAN   1234
 #define LIBMD3_BIG_ENDIAN      4321

 /* Check for any OSs or architectures which are known to be little endian */
 #if defined(__LITTLE_ENDIAN__) || defined(WIN32) || defined(__SYMBIAN32__) || \
     defined(__i386__) || defined(__ia64__) || defined(__alpha__) || \
     defined(__alpha) || defined(__arm__) || defined(__x86_64__) || \
     (defined(__mips__) && defined(__MIPSEL__))

  #define LIBMD3_BYTE_ORDER     LIBMD3_LITTLE_ENDIAN

 #else

  #define LIBMD3_BYTE_ORDER     LIBMD3_BIG_ENDIAN

 #endif

#endif /* __linux __ */

#include <libmd3/inttypes.h>

struct _md3_header;
struct _md3_frame;
struct _md3_mesh;
struct _md3_tag;

void libmd3_endian_header(struct _md3_header *);
void libmd3_endian_frames(struct _md3_frame * frames, int count);
void libmd3_endian_mesh(struct _md3_mesh *);
void libmd3_endian_tags(struct _md3_tag * tags, int count);

void libmd3_endian_int32s(int32_t*, int count);
void libmd3_endian_floats(float*, int count);

void libmd3_endian_vertices(int16_t*, int num_vertices);

#endif /* LIBMD3_ENDIAN_H */
