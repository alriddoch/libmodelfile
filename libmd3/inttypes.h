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

#ifndef LIBMD3_INTTYPES_H
#define LIBMD3_INTTYPES_H

#if defined(_MSC_VER)

  typedef signed int int32_t;
  typedef signed short int16_t;
  typedef signed char int8_t;

  typedef unsigned int uint32_t;
  typedef unsigned short uint16_t;
  typedef unsigned char uint8_t;

#else /* defined(_MSC_VER) */

  #include <stdint.h>

#endif /* defined(_MSC_VER) */

#endif /* LIBMD3_INTTYPES_H */
