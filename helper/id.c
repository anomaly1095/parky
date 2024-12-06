#include <stdio.h>
#include <string.h>

#define FILE_NAME "id.dat"
#define BYTE_COUNT 40

int main() {
    // Open the file for binary writing
    FILE *file = fopen(FILE_NAME, "wb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    // Create an array of 40 bytes initialized to zero
    unsigned char buffer[BYTE_COUNT];
    memset(buffer, 0, BYTE_COUNT);

    // Write the 40 bytes to the file
    size_t written = fwrite(buffer, 1, BYTE_COUNT, file);
    if (written != BYTE_COUNT) {
        perror("Error writing to file");
        fclose(file);
        return 1;
    }

    // Close the file
    fclose(file);

    printf("Wrote %d zero bytes to '%s'\n", BYTE_COUNT, FILE_NAME);
    return 0;
}
