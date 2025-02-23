#include <error.h>
#ifdef WIN32
#include <windows.h>
#include <iostream>
void errorWindow(const char* message) {
  std::cerr << "[Error] " << message << "\n";
  MessageBox(NULL, message, "Error", MB_ICONERROR | MB_OK);
}
#else
#ifdef __linux__
#include <iostream>
void errorWindow(const char* message) {
  std::cerr << "[Error] " << message << "\n";
}
#endif

#endif // ERROR_H
