#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

void reverse_lines(FILE *input, FILE *output) {
    char **lines = NULL;
    size_t lines_count = 0;
    size_t lines_capacity = 0;
    char buffer[MAX_LINE_LENGTH];

    // Read all lines from input
    while (fgets(buffer, sizeof(buffer), input)) {
        if (lines_count >= lines_capacity) {
            lines_capacity = lines_capacity ? lines_capacity * 2 : 1;
            lines = realloc(lines, lines_capacity * sizeof(char*));
            if (!lines) {
                perror("Unable to allocate memory");
                exit(EXIT_FAILURE);
            }
        }
        lines[lines_count] = strdup(buffer);
        if (!lines[lines_count]) {
            perror("Unable to allocate memory");
            exit(EXIT_FAILURE);
        }
        lines_count++;
    }

    // Print lines in reverse order
    for (int i = lines_count - 1; i >= 0; i--) {
        fputs(lines[i], output);
        free(lines[i]);
    }
    free(lines);
}

int main(int argc, char *argv[]) {
    FILE *input = stdin;
    FILE *output = stdout;

    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (!input) {
            perror("Error opening input file");
            return EXIT_FAILURE;
        }
    }

    if (argc > 2) {
        output = fopen(argv[2], "w");
        if (!output) {
            perror("Error opening output file");
            if (input != stdin) fclose(input);
            return EXIT_FAILURE;
        }
    }

    reverse_lines(input, output);

    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return EXIT_SUCCESS;
}
