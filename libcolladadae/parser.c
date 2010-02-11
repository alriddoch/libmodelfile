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

#include <libcolladadae/parser.h>

#include <libxml/parser.h>

xmlSAXHandler my_handler = {
    0, /* internalSubsetSAXFunc, */
    0, /* isStandaloneSAXFunc, */
    0, /* hasInternalSubsetSAXFunc, */
    0, /* hasExternalSubsetSAXFunc, */
    0, /* resolveEntitySAXFunc, */
    0, /* getEntitySAXFunc, */
    0, /* entityDeclSAXFunc, */
    0, /* notationDeclSAXFunc, */
    0, /* attributeDeclSAXFunc, */
    0, /* elementDeclSAXFunc, */
    0, /* unparsedEntityDeclSAXFunc, */
    0, /* setDocumentLocatorSAXFunc, */
    0, /* startDocumentSAXFunc, */
    0, /* endDocumentSAXFunc, */
    0, /* startElementSAXFunc, */
    0, /* endElementSAXFunc, */
    0, /* referenceSAXFunc, */
    0, /* charactersSAXFunc, */
    0, /* ignorableWhitespaceSAXFunc, */
    0, /* processingInstructionSAXFunc, */
    0, /* commentSAXFunc, */
    0, /* warningSAXFunc, */
    0, /* errorSAXFunc, */
    0, /* fatalErrorSAXFunc, */
};

libcolladadae_file * libcolladadae_file_load(const char * filename)
{
}

void libcolladadae_free(libcolladadae_file * file)
{
}
