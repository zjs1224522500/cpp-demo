#include "slice.h"
#include <memory>
#include <iostream>


using namespace leveldb;
using namespace std;

int main() {

    // std::shared_ptr<char> buffer(static_cast<char*>(aligned_alloc(512, 1024)), free);
    // char* str = buffer.get();

    // char* str = static_cast<char*>(aligned_alloc(512, 1024));
    char* str = new char[1024];
    snprintf(str, 16, "abcdefghijklmno");
    std::cout << "Buffer string with addrees " << &str << std::endl;
    // std::cout << "Buffer + 2 string with addrees " << (str + 2) << std::endl;


    Slice data(str, 3);
    // Slice data(str+2, 3); // cannot delete with data
    std::cout << "Data slice with address " << &data << " with value " << data.ToString() << std::endl;
    Slice sl(data.data(), 3);
    const char* haha(sl.data());
    std::cout << "Data slice with address " << &haha << " with value " << haha << std::endl;

    delete[] haha;
    std::cout << "delete success with offset 0" << std::endl;

    return 0;
}