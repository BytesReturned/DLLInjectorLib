#include "pch.h"
#include "framework.h"
#include <string>
#include <Windows.h>
using namespace std;

bool InjectDLLByProcessHandle(char DLLPath[256], HANDLE process) {
	LPVOID pDllPath = VirtualAllocEx(process, 0, strlen(DLLPath) + 1, MEM_COMMIT, PAGE_READWRITE);  // Alloco memoria nel processo
	if (!pDllPath)
	{
		Sleep(5000);
		return false;
	}
	if (!WriteProcessMemory(process, pDllPath, (LPVOID)DLLPath, strlen(DLLPath) + 1, 0))  // scrivo la DLL nella memoria del processo
	{
		Sleep(5000);
		return false;
	}
	HANDLE thread = CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), pDllPath, NULL, NULL); // Creo il thread nel processo remoto
	if (thread == 0) {
		Sleep(5000);
		return false;
	}
	WaitForSingleObject(thread, INFINITE);  // Attendo l'apertura del thread
	if (VirtualFreeEx(process, pDllPath, 0, MEM_RELEASE) == 0)
	{
		Sleep(5000);
		return false;
	}
	CloseHandle(thread);  // Chiudo gli handle
	CloseHandle(process);  // Chiudo gli handle
	return true;
}
bool InjectDLLByProcessID(char DLLPath[256], DWORD procID)
{
	HANDLE process = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, 0, procID); // Accesso al processo.
	if (procID == 0) {
		Sleep(3000);
		return false;
	}
	return InjectDLLByProcessHandle(DLLPath, process);
}
bool InjectDLLByWindowName(char DLLPath[256], string name) {
	LPCSTR finestra = name.c_str();
	HWND hwnd = FindWindowA(NULL, finestra);
	if (hwnd == NULL) {
		return false;
	}
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	bool status = InjectDLLByProcessID(DLLPath, pid);
	return status;
}
DWORD GetProcessIDByWindowName(string name) {
	LPCSTR finestra = name.c_str();
	HWND hwnd = FindWindowA(NULL, finestra);
	if (hwnd == NULL) {
		return false;
	}
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	return pid;
}
