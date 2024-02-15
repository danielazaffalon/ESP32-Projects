#include "sdCard.h"

void listDir(SDFileSystemClass &fs, const char* dirname, uint8_t levels){
  DEBUG_PRINT Serial.printf("Listing directory: %s\n", dirname);

  SDFile root = fs.open(dirname);
  if(!root){
    DEBUG_PRINT Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    DEBUG_PRINT Serial.println("Not a directory");
    return;
  }

  SDFile file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      DEBUG_PRINT Serial.print("  DIR : ");
      DEBUG_PRINT Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      DEBUG_PRINT Serial.print("  FILE: ");
      DEBUG_PRINT Serial.print(file.name());
      DEBUG_PRINT Serial.print("  SIZE: ");
      DEBUG_PRINT Serial.println(file.size());
    }
    file = root.openNextFile();
  }
  return;
}

void createDir(SDFileSystemClass &fs, const char * path){
  DEBUG_PRINT Serial.printf("Creating Dir: %s\n", path);
  if(fs.mkdir(path)){
    DEBUG_PRINT Serial.println("Dir created");
  } else {
    DEBUG_PRINT Serial.println("mkdir failed");
  }
}

void removeDir(SDFileSystemClass &fs, const char * path){
  DEBUG_PRINT Serial.printf("Removing Dir: %s\n", path);
  if(fs.rmdir(path)){
    DEBUG_PRINT Serial.println("Dir removed");
  } else {
    DEBUG_PRINT Serial.println("rmdir failed");
  }
}

bool fileExist(SDFileSystemClass &fs, const char * path){
  SDFile file = fs.open(path);
  if(!file){
    DEBUG_PRINT Serial.println("Failed to verify file existing");
    return false;
  }
  else {
    DEBUG_PRINT Serial.printf("The File %s Exists\n", path);
    file.close();
    return true;
  }
}

void readFile(SDFileSystemClass &fs, const char * path){
  DEBUG_PRINT Serial.printf("Reading file: %s\n", path);

  SDFile file = fs.open(path);
  if(!file){
    DEBUG_PRINT Serial.println("Failed to open file for reading");
    return;
  }

  DEBUG_PRINT Serial.print("Read from file: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

bool writeFile(SDFileSystemClass &fs, const char * path, const char * message){
  DEBUG_PRINT Serial.printf("Writing file: %s\n", path);

  SDFile file = fs.open(path, FILE_OVERWRITE);
  if(!file){
    DEBUG_PRINT Serial.println("Failed to open file for writing"); //ACA ESTA EL ERROR
    return false;
  }
  if(file.write(message)){
    DEBUG_PRINT Serial.println("File written");
    file.close();
    return true;
  } 
  else {
    DEBUG_PRINT Serial.println("Write failed");
    return false;
  }
}

bool appendFile(SDFileSystemClass &fs, const char * path, const char * message){
  DEBUG_PRINT Serial.printf("Appending to file: %s\r\n", path);
  SDFile file = fs.open(path, FILE_APPEND);
  if(!file){
    DEBUG_PRINT Serial.println("Failed to open file for appending"); 
    // file.close(); //BORRAR
    return false;
  }
  if(file.write(message)){
    DEBUG_PRINT Serial.println("Message appended");
    file.close();
    return true;
  } 
  else {
    DEBUG_PRINT Serial.println("Append failed");
    // file.close(); //BORRAR
    return false;
  }
}

void deleteFile(SDFileSystemClass &fs, const char * path){
  DEBUG_PRINT Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)){
    DEBUG_PRINT Serial.println("File deleted");
  } else {
    DEBUG_PRINT Serial.println("Delete failed");
  }
}

void testFileIO(SDFileSystemClass &fs, const char * path){
  SDFile file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if(file){
    len = file.size();
    size_t flen = len;
    start = millis();
    while(len){
      size_t toRead = len;
      if(toRead > 512){
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    DEBUG_PRINT Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    DEBUG_PRINT Serial.println("Failed to open file for reading");
  }

  file = fs.open(path, FILE_WRITE);
  if(!file){
    DEBUG_PRINT Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for(i=0; i<2048; i++){
    file.write(buf, 512);
  }
  end = millis() - start;
  DEBUG_PRINT Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}

bool startSD(){
  Serial.begin(115200);
  if(!SD.begin()){
    DEBUG_PRINT Serial.println("Card Mount Failed");
    return false;
  }
  else return true;
}
    