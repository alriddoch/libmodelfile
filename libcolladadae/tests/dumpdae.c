#include <libxml/parser.h>

int main(int argc, char ** argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        return 1;
    }
    return 0;
}
