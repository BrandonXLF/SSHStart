#pragma once

#include <string>
#include <optional>

using namespace std;

struct Result {
	optional<string> choice;
	optional<string> error;
};