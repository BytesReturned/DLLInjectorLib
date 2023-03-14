#pragma once
#include <Windows.h>
#include <string>
using namespace std;
bool InjectDLLByProcessHandle(char DLLPath[256], HANDLE process);
bool InjectDLLByProcessID(char DLLPath[256], DWORD procID);
bool InjectDLLByWindowName(char DLLPath[256], string name);
DWORD GetProcessIDByWindowName(string name)