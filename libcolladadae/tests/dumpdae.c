#include <libcolladadae/parser.h>
#include <libcolladadae/structure.h>

#include <stdio.h>

int main(int argc, char ** argv)
{
    libcolladadae_file * ret;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        return 1;
    }
    ret = libcolladadae_file_load(argv[1]);
    if (ret == 0) {
        fprintf(stderr, "Parsing %s failed\n", argv[1]);
        return 1;
    }
    libcolladadae_free(ret);
    return 0;
}
