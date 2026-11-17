/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"

void Memcpy(void* const dst, size_t dstSize, const void* src, size_t srcSize);
char* Strdup(const char* src);
char* readFile(char* path);

INLINE int Int_min(int a, int b) { return a < b ? a : b; }
INLINE int Int_max(int a, int b) { return a > b ? a : b; }

char* Challenge_getIPath(char* folderPath, int instanceID);
char* Challenge_getOPath(char* folderPath, int instanceID);
void Challenge_printSolution(char* folderPath, int instanceID);
