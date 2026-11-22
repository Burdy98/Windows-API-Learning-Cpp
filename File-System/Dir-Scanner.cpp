#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

// Define Directory Search func
void searchDir(const std::string& path) {
	
	// Declare Required Vars
	std::vector<std::string> dirs = {};
	WIN32_FIND_DATAA dir;
	std::string new_path = path + "\\*";

	// Create handle from path arg
	HANDLE hFile = FindFirstFileA(
		new_path.c_str(),
		&dir
	);
	// Check handle succeeded
	if (hFile == INVALID_HANDLE_VALUE) {
		DWORD err = GetLastError();
		std::cerr << "Error: " << err << std::endl;
		return;
	}
	
	std::cout << "\n---- Searching Dir: " << new_path << " ---\n";
	
	// Enumerate directory entries until FindNextFileA fails
	do {
		if (dir.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (strcmp(dir.cFileName, "..") != 0 && strcmp(dir.cFileName, ".") != 0) {
				std::cout << "[+] <DIR>: " << dir.cFileName << "\n";
				dirs.push_back(dir.cFileName);
			}
		}
		
		if (!(dir.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			if (strcmp(dir.cFileName, "..") != 0 && strcmp(dir.cFileName, ".") != 0) {
				std::cout << "[+] <FILE>: " << dir.cFileName << "\n";
			}
		};

	} while (FindNextFileA(hFile, &dir));

	// Close Handle
	FindClose(hFile);

	// Recursion
	for (const std::string& dir : dirs) {
		new_path = path + "\\" + dir;
		searchDir(new_path.c_str());
	}
}

int main(int argc, char* argv[])
{
	// Check Valid Args
	if (argc < 2) {
		std::cerr << "[Usage]: " << argv[0] << " Dir..." << std::endl;
		return 1;
	}
	// Create path Var and Call func.
	std::string path = argv[1];
	searchDir(path);
}
