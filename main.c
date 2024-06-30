#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#include "split_HTML.h"

int main(int argc, char* argv[]) {
    int max_len;
    char maxLenStr[4];

    if (strncmp(argv[1], "--max-len=", 10) == 0) {
        max_len = strtol(strncat(maxLenStr, argv[1] + sizeof("--max-len=") - 1, 4), 0, 10);
    }
    if (max_len < 0 || max_len > 4096) {
        puts("incorrect size");
        return -1;
    }

    char* htmlName = argv[2];
    printf("%s\n", htmlName);
    FILE* html = fopen(htmlName, "r");

    split_HTML(html, max_len);

    fclose(html);
    return 0;
}
