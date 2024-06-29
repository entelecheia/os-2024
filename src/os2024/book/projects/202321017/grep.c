#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

int search_in_file(const char *filename, const char *search_term) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("hgrep: cannot open file\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    int found = 0;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (strstr(line, search_term) != NULL) {
            printf("%s", line);
            found = 1;
        }
    }

    fclose(file);
    return found ? 0 : 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "hgrep: searchterm [file ...]\n");
        return 1;
    }

    if (argc < 3) {
        fprintf(stderr, "hgrep: searchterm [file ...]\n");
        return 1;
    }

    const char *search_term = argv[1];
    int overall_result = 0;

    for (int i = 2; i < argc; ++i) {
        int result = search_in_file(argv[i], search_term);
        if (result != 0) {
            overall_result = result;
        }
    }

    return overall_result;
}
