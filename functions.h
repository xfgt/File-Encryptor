//
// Created by USER on 09-May-25.
//
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

void printCharIntArr(char*, int);
char* makePwdArr(const char*, int);
bool encryptCharArr(char*, const int, const char*);
bool decryptCharArr(char*, const int, const char*);
bool compareSectors(char*, char*, const int);
std::streampos getFileBytes(const char*);
bool indexBinaryBadRelationship(const char*, const char*);
char* readSector_charArr(const char*, int, int);
int getPosLineFromIndex(const char*, const char*);
bool readSector_fromBinaryIndex_toFile(const char*, const char*, const char*, const char*, const char*);
bool registerNewFile(const char*, const char*, const char*, const char*);
void deleteEmptyLines(const char*);
bool checkSingleEntry(const char*);
bool unregister(const char*, const char*, const char*);
bool encrypt(const char*, const char*, const char*, const char**, const int);
bool decrypt(const char*, const char*, const char*, const char*, const char*);
bool remove(const char*, const char*, const char*, const char*);
