#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <windows.h>
#include <knownfolders.h>
#include "HostNames.h"
#include "ConfigFile.h"
#include "Result.h"

using namespace std;

vector<ConfigFile*> files;
HostNames* hostNames;

static string getChoice(optional<string> error) {
	system("cls");

	cout << "SSHStart" << endl << endl;

	for (ConfigFile* file : files)
		cout << file->editKey << ") " << "Edit " << file->type << " config" << endl;

	cout << endl;

	int i = 1;

	for (const string& host : hostNames->hosts) {
		cout << i << ") " << host << endl;
		i++;
	}

	cout << endl;

	if (error.has_value())
		cout << "Error: " << error.value() << endl << endl;

	cout << "Choose: ";

	string line;
	getline(cin, line);

	return line;
}

static Result processChoice(string choice) {
	if (choice.empty()) {
		return {};
	}

	for (ConfigFile* file : files) {
		if (choice.size() > 1 || choice[0] != file->editKey) continue;

		file->edit();
		hostNames->reload();

		return {};
	}

	int index;

	try {
		index = stoi(choice) - 1;
	} catch (...) {
		return { .error = "Invalid choice" };
	}

	if (index < 0 || index >= hostNames->hosts.size()) {
		return { .error = "Choice out of range" };
	}

	return { *next(hostNames->hosts.begin(), index) };
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	setlocale(LC_ALL, ".UTF8");

	files.push_back(new ConfigFile(FOLDERID_Profile, L".ssh", L"config", "user", 'e'));
	files.push_back(new ConfigFile(FOLDERID_ProgramData, L"ssh", L"ssh_config", "global", 'g'));

	hostNames = new HostNames(&files);

	Result result;

	while (!result.choice.has_value()) {
		string choice = getChoice(result.error);
		result = processChoice(choice);
	}

	system("cls");
	system(("ssh " + result.choice.value()).c_str());

	delete hostNames;

	for (ConfigFile* file : files)
		delete file;
}