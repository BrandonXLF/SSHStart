#pragma once

#include <string>
#include <set>
#include <fstream>
#include <windows.h>
#include <ShlObj.h>
#include "ConfigFile.h"

using namespace std;

ConfigFile::ConfigFile(wstring cPath, string cEditKey, string cType)
	: path{ cPath }, editKey{ cEditKey }, type{ cType } { }

ConfigFile::ConfigFile(REFKNOWNFOLDERID folder, LPCWSTR file, string cEditKey, string cType)
	: editKey{ cEditKey }, type{ cType } {
	PWSTR folderPath;

	SHGetKnownFolderPath(folder, NULL, NULL, &folderPath);

	path = wstring(folderPath);
	path.append(file);

	CoTaskMemFree(folderPath);
}

set<string> ConfigFile::getHosts() {
	set<string> hosts;

	ifstream file(this->path.c_str());

	if (!file.good())
		return hosts;

	string line;

	while (getline(file, line)) {
		// Remove whitespace
		line.erase(0, line.find_first_not_of(whitespace));
		line.erase(line.find_last_not_of(whitespace) + 1);

		// Check for "Host"
		if (line.substr(0, 4) != "Host")
			continue;

		// Make sure "Host" is followed by whitespace
		size_t nextNonSpace = line.find_first_not_of(whitespace, 4);

		if (nextNonSpace == 4 || nextNonSpace == string::npos)
			continue;

		// Remainder of the line is the host name
		line.erase(0, nextNonSpace);
		hosts.insert(line);
	}

	return hosts;
}

void ConfigFile::edit() {
	SHELLEXECUTEINFO execInfo{};
	execInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	execInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	execInfo.lpVerb = L"open";
	execInfo.lpFile = L"notepad";
	execInfo.lpParameters = this->path.c_str();
	execInfo.nShow = SW_SHOW;

	ShellExecuteEx(&execInfo);
	WaitForSingleObject(execInfo.hProcess, INFINITE);
	CloseHandle(execInfo.hProcess);
}