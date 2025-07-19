//
// Created by USER on 04-May-25.
//
#include <iostream>
#include <iomanip>

#include "functions.h"



const int numFiles = 3;
const int textSize = 32;
int main() {
    std::cerr.setstate(std::ios_base::failbit);
    std::clog.setstate(std::ios_base::failbit);



    char** fileNames = new char* [numFiles];
     for(int i = 0; i < numFiles; i++) {
         fileNames[i] = new char[textSize];
         std::cout << "Enter file: [" << i+1 << "]:" << std::flush;
         std::cin.getline(fileNames[i], textSize-1);
         if(strlen(fileNames[i]) < textSize) {
             // std::cin.tie();
             // std::cin.sync();
             std::cin.clear();
             std::cout.flush();
         }
     }



    std::cout << std::boolalpha << encrypt("index.txt", "data.bin", "pass", const_cast<const char**>(fileNames), numFiles) << std::endl;
    std::cout << std::boolalpha << decrypt("index.txt", "data.bin", "pass", "file1.bin", "file1_decrypted.bin") << std::endl;
    std::cout << std::boolalpha << decrypt("index.txt", "data.bin", "pass", "file2.bin", "file2_decrypted.bin") << std::endl;
    std::cout << std::boolalpha << decrypt("index.txt", "data.bin", "pass", "file3.bin", "file3_decrypted.bin") << std::endl;
    std::cout << std::boolalpha << remove("index.txt", "data.bin", "pass", "file1.bin") << std::endl;






//  Proper memory management.
    for (int i = 0; i < numFiles; i++) {
        delete[] fileNames[i];
        fileNames[i] = nullptr;
    }
    delete[] fileNames;
    fileNames = nullptr;


    return 0;
}