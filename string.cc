#include <iostream>
#include <cstring>

using namespace std;

int main() {
    cout << "Hello String " << endl;
    char _db_path[128] = "/mnt/pmem1-shun/db_bench";
    const char* delim = "/";

    // method 1
    char *p = strtok(_db_path, delim);
    while (p)
    {
        cout << p << endl;
        p = strtok(NULL, delim);
    }

    // method 2
    string str = _db_path;
    size_t found = str.find_last_of(delim);
    cout << str.substr(found);
    return 0;
}