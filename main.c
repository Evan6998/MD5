#include "md5.h"

int main(int argc, char **argv) {
    char *msg;
    size_t len;
    uint8_t result[16];
 
    if (argc < 2) {
        printf("usage: %s 'string'\n", argv[0]);
        return 1;
    }
    msg = argv[1];
 
    len = strlen(msg);
 
    md5((uint8_t*)msg, len, result);
 
    // display result
    for (int i = 0; i < 16; i++)
        printf("%x", result[i]);
    puts("");
 
    return 0;
}