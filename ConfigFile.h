#pragma once

#include <string>
#include <set>
#include <fstream>
#include <windows.h>
#include <ShlObj.h>

using namespace std;

class ConfigFile {
public:
	string editKey;
	wstring path;
	string type;

	ConfigFile(wstring path, string editKey, string type);
	ConfigFile(REFKNOWNFOLDERID folder, LPCWSTR file, string editKey, string type);

	set<string> getHosts();
	void edit();

private:
	const char* whitespace = "\t ";
	const char* separator = "\t =";
};
