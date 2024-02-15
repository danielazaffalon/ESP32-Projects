#ifndef SDCARD_h
#define SDCARD_h

#include <FS.h>
#include <SD.h>
#include "../debug.h"

/*
Estas funciones están basadas en un ejemplo que no corresponde a la libreria incluida en el esp32
por lo cual se agregaron operaciones nuevas a partir de las siguientes definiciones de overwite y append
@see ESP32-SD
*/

#define FILE_OVERWRITE 0x52
#define FILE_APPEND 0x17

void listDir(SDFileSystemClass &fs, const char * dirname, uint8_t levels);
void createDir(SDFileSystemClass &fs, const char * path);
void removeDir(SDFileSystemClass &fs, const char * path);
bool fileExist(SDFileSystemClass &fs, const char * path);
void readFile(SDFileSystemClass &fs, const char * path);
bool writeFile(SDFileSystemClass &fs, const char * path, const char * message);
bool appendFile(SDFileSystemClass &fs, const char * path, const char * message);
void deleteFile(SDFileSystemClass &fs, const char * path);
void testFileIO(SDFileSystemClass &fs, const char * path);
bool startSD();

#endif