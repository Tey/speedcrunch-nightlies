#include <string>
#include <iostream>

using namespace std;

void DisplayErrorOnMismatch(const char* file, int line, const char* msg, const std::string& result, const char* expected, int& failed, int& newFailed, int issue = 0)
{
    if (result != expected) {
        ++failed;
        cerr << file << "[" << line << "]\t" << msg;
        if (issue)
            cerr << "\t[ISSUE " << issue << "]";
        else {
            cerr << "\t[NEW]";
            ++newFailed;
        }
        cerr << endl;
        cerr << "\tResult   : " << result << endl
             << "\tExpected : " << expected << endl;
    }
}
