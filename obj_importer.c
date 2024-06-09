
#include <stdio.h>
#include <stdlib.h>

char* readObjFileToString(const char* filePath) {
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filePath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* fileContent = (char*)malloc(fileSize + 1);
    if (fileContent == NULL) {
        fprintf(stderr, "Failed to allocate memory for file content\n");
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(fileContent, 1, fileSize, file);
    if (bytesRead != fileSize) {
        fprintf(stderr, "Failed to read file: %s\n", filePath);
        free(fileContent);
        fclose(file);
        return NULL;
    }

    fileContent[fileSize] = '\0';

    fclose(file);
    printf("File content: %s\n", fileContent);
    return fileContent;
}
