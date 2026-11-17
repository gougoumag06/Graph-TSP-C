/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "tools.h"

void Memcpy(void* const dst, size_t dstSize, const void* src, size_t srcSize)
{
#ifdef _MSC_VER
    memcpy_s(dst, dstSize, src, srcSize);
#else
    memcpy(dst, src, (srcSize < dstSize) ? srcSize : dstSize);
#endif
}

char* Strdup(const char* src)
{
    size_t size = strlen(src);
    char* dst = (char*)calloc(size + 1, sizeof(char));
    AssertNew(dst);
    Memcpy(dst, size, src, size);
    return dst;
}


char* Challenge_getIPath(char* folderPath, int instanceID)
{
    char fileName[128] = { 0 };
    sprintf(fileName, "/input%d.txt", instanceID);
    char* path = (char*)calloc(
        strlen(folderPath) + strlen(fileName) + 1,
        sizeof(char)
    );
    AssertNew(path);

    path = strcpy(path, folderPath);
    path = strcat(path, fileName);

    return path;
}

char* Challenge_getOPath(char* folderPath, int instanceID)
{
    char fileName[128] = { 0 };
    sprintf(fileName, "/output%d.txt", instanceID);
    char* path = (char*)calloc(
        strlen(folderPath) + strlen(fileName) + 1,
        sizeof(char)
    );
    AssertNew(path);

    path = strcpy(path, folderPath);
    path = strcat(path, fileName);

    return path;
}

void Challenge_printSolution(char* folderPath, int instanceID)
{
    char* path = Challenge_getOPath(folderPath, instanceID);

    FILE* file = fopen(path, "rb");
    AssertNew(file);

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    char* buffer = (char*)calloc(size + 1, 1);
    AssertNew(buffer);

    rewind(file);
    fread(buffer, sizeof(char), size, file);
    printf("Solution du challenge %d [%s]\n", instanceID, folderPath);
    printf("%s\n", buffer);

    free(buffer);
    free(path);
    fclose(file);
}

char* readFile(char* path)
{
    long length;
    char* buf = NULL;
    FILE* fp = fopen(path, "r");
    AssertNew(fp);

    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    rewind(fp);
    buf = calloc(length + 1, 1);
    fread(buf, length, 1, fp);
    fclose(fp);

    return buf;
}
