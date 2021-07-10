#ifndef SD_H
#define SD_H
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

class SdCard
{
public:
    void init();
    void listDir(const char *dirname, uint8_t levels);
    void createDir(const char *path);
    void removeDir(const char *path);
    void readFile(const char *path);
    void writeFile(const char *path, const char *message);
    void writeJson(const char *path, DynamicJsonDocument &doc);
    void readJson(const char *path,  DynamicJsonDocument &doc);
    void appendFile(const char *path, const char *message);
    void renameFile(const char *path1, const char *path2);
    void deleteFile(const char *path);
    void testFileIO(const char *path);

};

extern SdCard sd;

#endif