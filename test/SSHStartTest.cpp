#include <set>
#include <string>
#include "CppUnitTest.h"
#include "../src/ConfigFile.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace SSHStartTest {
	TEST_CLASS(TestConfigFile) {
	public:
		TEST_METHOD(getHosts) {
			string expectedHosts = u8"a..b..c..d..ee..f..h..i..j..k..l l..m..验🍍..";

			ConfigFile configFile(wstring(L"../../test"), wstring(L"../../test/test_config"), "test", 't');
			string hostList;

			for (const string& host : configFile.getHosts())
				hostList += host + "..";

			Assert::AreEqual(expectedHosts, hostList);
		}
	};
}
