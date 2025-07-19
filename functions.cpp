//
// Created by USER on 09-May-25.
//
#include "functions.h"
void printCharIntArr(char* arr, int sz) {
    for(int i = 0; i < sz; i++) {
        std::cout << int(arr[i]) << ' ';
    }
    std::cout << std::endl;
}

//passwordpasswordpasswordpassword...
char* makePwdArr(const char* password, int arraySize) {
    // store password chars in array as repeating characters

    char* arr = new char[arraySize+1]{};
    if(arr == nullptr) return nullptr;
    int pwSz = strlen(password);

    for (int i = 0; i < arraySize; i++) {
        arr[i] = password[i % pwSz]; // Repeat the string character-by-character
    }
    arr[arraySize] = '\0';
    return arr; // has to be dealocated outside
}

bool encryptCharArr(char* arr, const int size, const char* password) {
    if(arr == nullptr) false;

    char* pwdCharArr = makePwdArr(password, size);
    if(pwdCharArr == nullptr) false;

    for(int i = 0; i < size; i++) {
        char res = arr[i] ^ pwdCharArr[i];

        if(arr[i] == 0) { // get around of 0 XOR whatever
            // change to 1
            // then xor
            res = (2 ^ pwdCharArr[i]);

            //std::cout << i << ' ' << res << std::endl;
        }

        arr[i] = res;


    }




    delete[] pwdCharArr;
    pwdCharArr = nullptr;
    return true;
}

bool decryptCharArr(char* arr, const int size, const char* password) {
    if(arr == nullptr) false;

    char* pwdCharArr = makePwdArr(password, size);
    if(pwdCharArr == nullptr) false;

    for(int i = 0; i < size; i++) {
        char res = arr[i] ^ pwdCharArr[i];

        if(arr[i] == ((2 ^ pwdCharArr[i]))) { // 2 is custom
            // change to 0
            // then xor
            res = (res^2);

            //std::cout << i << ' ' << res << std::endl;
        }

        arr[i] = res;


    }


    delete[] pwdCharArr;
    pwdCharArr = nullptr;
    return true;
}


bool compareSectors(char* original, char* other, const int sz) {
    for(int i = 0; i < sz; i++) {
        if(original[i] != other[i])
            return false;
    }
    return true;
}




std::streampos getFileBytes(const char* fileName) {
    std::streampos numberOfBytes{};
    std::ifstream readFile;
    readFile.open(fileName, std::ios::in | std::ios::binary);
    if(!fileName) {
        std::cerr << "Error getting the size of file [" << fileName << "]" << std::endl;
        readFile.close();
        return readFile.tellg(); // -1
    }

    readFile.seekg(0, std::ios::end);
    numberOfBytes = readFile.tellg();
    readFile.close();
    return numberOfBytes;
}

// Check if correct binary
bool indexBinaryBadRelationship(const char* indexFile, const char* dataFile) {

    int binaryFileSize = getFileBytes(dataFile);

    int startPos{}, endPos{};
    std::string fileName("");

    // read the endpos from last line
    std::ifstream readFile(indexFile, std::ios::in);
    if(!readFile) return true;

    std::string lineString("");
    while(std::getline(readFile, lineString)) {
        std::stringstream ss;
        ss << lineString;
        ss >> fileName >> startPos >> endPos;
    }



    // if endpos is <= to the binaryFile size so we can read until there.
    // either way its larger and we get random values at some point of the reading procedure.
    if(endPos > binaryFileSize) {
        std::cerr << "Start position is out of bound relative to the binary file. [" << dataFile << "]\nBinary file size = " << binaryFileSize << std::endl;
        std::cerr << "Please provide the correct binary file associated with the index file. [" << indexFile << "]" << std::endl;
        return true; // positive for bad relationship
    }
    return false;
}



// reading a sector of a file with starting and ending positions
char* readSector_charArr(const char* fileName, int startPos, int endPos) {
    std::ifstream file;
    file.open(fileName, std::ios::in | std::ios::binary);
    if(!file) {
        std::cerr << "Error opening file [" << fileName << "]\n";
        return nullptr;
    }
    file.seekg(startPos, std::ios::beg);


    const int numberOfBytes = endPos - startPos; // read (endPos - startpos) amount of bytes (should be equal to the extracted endpos)
    char* temp = new char[numberOfBytes+1];
    file.read(reinterpret_cast<char*>(temp), numberOfBytes);
    temp[numberOfBytes] = '\0';
    std::clog << "Data from file [" << fileName << "]:\n" << temp << std::endl;

    file.close();
    return temp;
}



// seperate procedure for searching the file and outputing the line on which is found ( offset position from beg)
int getPosLineFromIndex(const char* indexFile, const char* searchedFile) {

    std::ifstream readFile;
    readFile.open(indexFile, std::ios::in);
    if(!readFile) {
        std::cerr << "Error opening file [" << indexFile << "]\n";
        return -1;
    }
    readFile.seekg(0, std::ios::beg);

    int foundLineStart{};
    int lineEnd = readFile.tellg();

    bool isFound = false;
    std::string lineString("");
    while(std::getline(readFile,lineString)) {
        std::string path("");
        std::stringstream ss;

        ss << lineString;
        ss.seekg(0, std::ios::end); // getting the end (which is the beginning of the new line)
        lineEnd += (int)ss.tellg()+2;
        ss.seekg(0);


        ss >> path;
        if(path == searchedFile) {
            isFound = true;
            std::clog << "MATCH! [" << indexFile << "] -> [" << searchedFile << "]" << std::endl;
            // the start position of the current line

            // the end of the line - the size of the string on the line = the beginning of the found string and whole line with data.
            foundLineStart = lineEnd-lineString.size()-2;
            break;
        }


    }
    readFile.close();

    if(!isFound)
        std::clog << "Could not find a matching for \"" << searchedFile  <<"\" in the indexes file! [" << indexFile << "]" << std::endl;
    else
        return foundLineStart;


    return -1; // searched file is missing from the index file contents.
}



// reading a sector of data based on the index and binary files respectively
bool readSector_fromBinaryIndex_toFile(const char* searchedFile, const char* indexFile, const char* dataFile, const char* newFile, const char* password) {


    //  Check if correct binary
    if(indexBinaryBadRelationship(indexFile, dataFile)) return false;

    std::ifstream readFile;
    readFile.open(indexFile, std::ios::in);
    if(!readFile) {
        std::cerr << "Error opening file [" << indexFile << "]\n";
        return false;
    }

    int linePos = getPosLineFromIndex(indexFile, searchedFile);
    if(linePos == -1) {
        std::cerr << "A file with the name \"" << searchedFile << "\" was not found in the provided index file [" << indexFile << "]" << std::endl;
        readFile.close();
        return false;
    }


    std::string stringLine("");
    readFile.seekg(linePos, std::ios::beg);
    std::getline(readFile, stringLine);

    std::stringstream ss;
    ss << stringLine;

    std::string fileName{};
    int startPos{}, endPos{};
    ss >> fileName >> startPos >> endPos;
    std::clog << "starting position: " << startPos << '\n' << "ending position: " << endPos << std::endl;
    readFile.close();



    char* sector = readSector_charArr(dataFile, startPos, endPos);
    if(sector == nullptr){ return false; }


    decryptCharArr(sector, endPos-startPos, password);







//  Writing to the new file
    std::ofstream outNewFile(newFile, std::ios::out | std::ios::trunc | std::ios::binary);
    if(!outNewFile) {
        delete[] sector;
        sector = nullptr;
        return false;
    }

    outNewFile.write(reinterpret_cast<char*>(sector), endPos - startPos);
    outNewFile.close();

    delete[] sector;
    sector = nullptr;



    return true;
}



// fills the binary and indexes file from a new given source file;
// returns the beginning cursor position of the last written file;
bool registerNewFile(const char* newFile, const char* indexFile, const char* dataFile, const char* password) {

//  Search if the source file already exists in the index file!
    int line = getPosLineFromIndex(indexFile, newFile);
    if(line != -1) {
        std::cerr << "File with the name \"" << newFile << "\" already exists in the given file with indexes [" << indexFile << "]" << std::endl;
        return false;
    }


//  reading source file
    std::fstream source;
    source.open(newFile, std::ios::in | std::ios::binary);
    if(!source) {
        std::cerr << "Error opening file [" << newFile << "]\n";
        return false;
    }


    const int numberOfBytes = getFileBytes(newFile);
    char* temp = new char[numberOfBytes+1]; //number of bytes to read. (+1 to delete ['\0'])
    source.read(reinterpret_cast<char*>(temp), numberOfBytes); // txt, bin (no&)
    temp[numberOfBytes] = '\0';


    // ENCRYPTION
    if(strlen(password) > 0) {
        encryptCharArr(temp, numberOfBytes, password);
        source.close();


        //  Encrypting the file itself

        source.open(newFile, std::ios::out | std::ios::trunc | std::ios::binary);
        source.seekp(0, std::ios::beg);
        source.write(reinterpret_cast<char*>(temp), numberOfBytes);
    }

    source.close();




//  writing to destination binary file
    std::fstream dest;
    dest.open(dataFile, std::ios::in | std::ios::out | std::ios::app | std::ios::binary);
    if(!dest) {
        std::cerr << "Error opening file [" << dataFile << "]\n";
        return false;
    }
    dest.seekp(0, std::ios::end);
    int startPos = dest.tellp(); // end of the previous file's contents (appending)
    //std::clog << "[" << sourceFilePath << "] Starting from: " << startPos << std::endl;
    dest.seekp(0, std::ios::beg);


    dest.write(reinterpret_cast<const char*>(temp), numberOfBytes); // no&
    int endPos = startPos; // number of bytes in the file with the overall size before writing
    /// DELETE (int)dest.tellp() + startPos FOR MSVC IDK WHY ITS 2 AM
    //std::clog << "[" << sourceFilePath << "] Ending at: " << endPos << std::endl;
    dest.close();

    delete[] temp;
    temp = nullptr;



//  Writing to the index file
    std::fstream dest_txtFile;
    dest_txtFile.open(indexFile, std::ios::in | std::ios::out | std::ios::app);
    if(!dest_txtFile) {
        std::cerr << "Error opening file [" << indexFile << "]\n";
        return false;
    }
    dest_txtFile << newFile << ' ' << startPos << ' ' << endPos << std::endl;
    dest.close();


    return true; // the beginning cursor position of the last written file
}






void deleteEmptyLines(const char* filePath)
{
    std::ifstream in(filePath);
    std::string line, text;
    while (std::getline(in, line))
        if (!(line.empty() || line.find_first_not_of(' ') == std::string::npos))
            text += line + "\n";
    in.close();
    std::ofstream out(filePath);
    out << text;
}

bool checkSingleEntry(const char* indexFileName) {


    // the last position of the last line should be equal to the size of the file.
    // either way is false
    int fileSize = getFileBytes(indexFileName);

    std::ifstream ifile(indexFileName, std::ios::in);
    if(!ifile) return false;

    std::string lineString{};
    std::getline(ifile, lineString);
    ifile.close();

    auto sz= lineString.size() + 2;
    if(fileSize == sz)
        return true;

    return false;
}

bool unregister(const char* fileNameToRemove, const char* indexFileName, const char* dataFileName) {
    bool isSingle = false;
//  Check if index relates with binary
    if(indexBinaryBadRelationship(indexFileName, dataFileName))
        return false;

//  check if we have only one file
    if(checkSingleEntry(indexFileName)) {
        // check for file name match
        std::ifstream f(indexFileName, std::ios::in);
        if(!f) return false;
        std::string lineString{}, fileName{};
        std::getline(f, lineString);
        std::stringstream ss;
        ss << lineString;
        ss >> fileName;


        if(fileNameToRemove == fileName) {
            std::ofstream index(indexFileName, std::ios::trunc);
            std::ofstream bin(dataFileName, std::ios::trunc);
            index.close();
            bin.close();
        }

        return false;
    }


    // todo: zamesti vector
    std::string outputStrings[100];
    int n{};


//  Read the index file and find the file to remove
    std::fstream readFile(indexFileName, std::ios::in | std::ios::out);
    if(!readFile) return false;


    std::string lineString("");
    bool begin = false;
    std::fstream binaryFile;
    int sectorBytesSize{};



    while(std::getline(readFile, lineString)) {

//      Extract each line
        std::stringstream ssp;
        std::stringstream ss;
        ss << lineString;
        if(lineString.empty()) continue; // skip empty lines

//      Getting coordinates for the binary
        std::string fileName(""); int startPos{}, endPos{};
        ss >> fileName >> startPos >> endPos;


/*
 * Check for name match;
 * Set flag to true; ( gives a way to edit all the entries after the matching line)
 * Get sector size;
 *
 * Reposition put cursor;
 *
 * Skip including the line entry in the array of entries (for the new indexFile contents)
 */
        if(fileName == fileNameToRemove) {
            begin = true;
            sectorBytesSize = endPos - startPos;
            binaryFile.open(dataFileName, std::ios::in | std::ios::out | std::ios::binary);
            if(!binaryFile) break;

            binaryFile.seekp(startPos, std::ios::beg);

            continue;
        }


/*
 * Get last position;
 * Move the reading position to the end of the deleting sector (the start of the next);
 *
 * Keep the next sector in a buffer;
 *
 * Jump back to the beginning of the sector for deletion;
 * Write the contents into the binary file (truncate contents);
 *
 * Edit index file lines;
 *
 * Default: Put the entry into the string vector.
 */

        if(begin) {
            // edit binary
            int before = binaryFile.tellp();
            binaryFile.seekg(startPos, std::ios::beg);

            const int fileSizeBytes = endPos-startPos;
            char* holdData = new char[fileSizeBytes+1];
            binaryFile.read(reinterpret_cast<char*>(holdData), fileSizeBytes);
            holdData[fileSizeBytes] = '\0';

            binaryFile.seekp(-(fileSizeBytes+sectorBytesSize), std::ios::cur);
            binaryFile.write(reinterpret_cast<char*>(holdData), fileSizeBytes);

            std::clog << binaryFile.tellp() << std::endl;




            delete[] holdData;
            holdData = nullptr;

            //write to indexFile
            startPos -= sectorBytesSize;
            endPos -= sectorBytesSize;
            ssp << fileName << " " << startPos << " " << endPos << std::endl;
            lineString = ssp.str();
        }

        outputStrings[n++] = lineString;

    }





    if(!begin) { // if name was not found we don't need to change anything
        binaryFile.close();
        readFile.close();
        return false;
    }
    readFile.close();

/* If begin was true - we've got a match:
 *
 * Get the last entry from the string vector.
 * Get the last position from it - this is the number of bytes for the new file after modifying it.
 *
 * Copy those number of bytes in a buffer.
 *
 * Open the binary file as truncate and write the contents of the buffer inside.
 * (Everything else is deleted entirely, because the file has been opened as std::ios::trunc.)
 *
 * Put the new entries in the index file. (again as a truncate)
 */

    std::string x{};
    int numberOfBytes{};
    std::stringstream ss;
    ss << outputStrings[n-1];
    ss >> x >> numberOfBytes >> numberOfBytes;
    binaryFile.close();


    binaryFile.open(dataFileName, std::ios::in  | std::ios::binary);
    if(!binaryFile) return false;

    char* copyHold = new char[numberOfBytes+1];
    binaryFile.read(reinterpret_cast<char*>(copyHold), numberOfBytes);
    copyHold[numberOfBytes] = '\0';

    binaryFile.close();



    std::ofstream copyDataFile(dataFileName, std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);
    if(!copyDataFile) return false;

    copyDataFile.write(reinterpret_cast<const char*>(copyHold), numberOfBytes);
    std::clog << copyDataFile.tellp() << std::endl;

    delete[] copyHold;
    copyHold = nullptr;

    copyDataFile.close();









//  Filling the index file with new contents
    readFile.open(indexFileName, std::ios::out | std::ios::trunc);
    for(auto path : outputStrings)
        readFile << path << std::endl;

//  trim the index file
    deleteEmptyLines(indexFileName);

    readFile.close();



    return true;

}

























bool encrypt(const char* indexName, const char* dataName,
                const char* password,
            const char** filenames, const int numFiles) {

    for(int i = 0; i < numFiles; i++) {
       if(!registerNewFile(const_cast<char*>(filenames[i]), indexName, dataName, password)) {
           return false;
       }
    }
    return true;
}



bool decrypt(const char* indexName, const char* dataName,
                const char* password,
            const char* originalFilename, const char* newFilename) {

    if(indexBinaryBadRelationship(indexName, dataName)) return false;

    // check if the new file already exists
    std::ifstream nf(newFilename, std::ios::in | std::ios::binary);
    if(nf.is_open()) {
        std::cerr << "File \"" << newFilename << "\" aleady exists!" << std::endl;
        nf.close();
        return false;
    }

    return readSector_fromBinaryIndex_toFile(originalFilename, indexName, dataName, newFilename, password);
}


bool remove(const char* indexName, const char* dataName,
                const char* password,
                    const char* originalFilename) {

    return unregister(originalFilename, indexName, dataName);
}