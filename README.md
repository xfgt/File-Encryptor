# File-Encryptor

This project implements a basic file encryption, decryption, registration, and removal for binary data of file using C++. It simulates secure storage by splitting metadata and actual binary data between two files: `index.txt` and `data.bin`. It uses a simple XOR cipher for encryption and decryption.
## Features

- Register and encrypt new files into a binary storage.
- Decrypt encrypted files from binary storage.
- Remove files from the binary storage and update the index accordingly.
- Handles multiple files with offset tracking via an index file.
- Deletes empty lines from index for file cleanliness.

## How It Works

- `index.txt`: Stores file entries and their byte positions within `data.bin`.
- `data.bin`: Stores raw (encrypted) file data.
- Encryption uses XOR with a repeated password pattern.
- A password ensures obfuscation, and a custom XOR logic handles null characters.


## File Structure

- `main()`: Handles file input and calls `encrypt()`, `decrypt()`, and `remove()` functions.
- `functions.h/cpp`: Contains helper and core logic:
  - `encryptCharArr`, `decryptCharArr`: XOR-based encryption/decryption.
  - `registerNewFile`: Adds encrypted file data to binary.
  - `readSector_fromBinaryIndex_toFile`: Decrypts and writes file content.
  - `unregister`: Removes a file and reorganizes binary/index data.


## Compilation

Use a C++11 (or later) compliant compiler:

```bash
g++ -std=c++11 -o file_encryptor main.cpp functions.cpp fuctions.h
```

Make sure you have placed your files in the same working directory.
