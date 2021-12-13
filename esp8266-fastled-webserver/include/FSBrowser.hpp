#pragma once
#if !defined(FS_BROWSER_HPP)
#define FS_BROWSER_HPP


//holds the current upload
extern File fsUploadFile;

//format bytes
String formatBytes(size_t bytes);
String getContentType(String filename);
bool handleFileRead(String path);
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();

#endif