/**
 * gcc cached_io_read_file.c -o cached_io_read_file -D_GNU_SOURCE
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <string.h>
#include <malloc.h>
#include <memory>

#define BUF_SIZE 1024

using namespace std;


void mem_align(char* buffer) {
    std::cout << "Before Memalign buffer " << &buffer << " with length " << malloc_usable_size(buffer) << std::endl;
    posix_memalign((void **)&buffer, 512, BUF_SIZE);
    std::cout << "After Memalign buffer " << &buffer << " with length " << malloc_usable_size(buffer) << std::endl;

}

void test_ptr() {
    char* backup;
    std::cout << "IsNull " << (backup == NULL) << std::endl;
    std::cout << "Before func buffer " << &backup << " with length " << malloc_usable_size(backup) << std::endl;
    mem_align(backup);
    std::cout << "After memalign IsNull " << (backup == NULL) << std::endl;
    std::cout << "After func buffer " << &backup << " with length " << malloc_usable_size(backup) << std::endl;

}




// void test_uniq_ptr(char* ptr) {
//     char* xxx;
//     for (size_t i = 0; i < 1000; i++)
//     {
        std::shared_ptr<char> buffer(static_cast<char*>(aligned_alloc(512, BUF_SIZE)), free);
        // std::shared_ptr<char> buffer = std::make_shared(static_cast<char*>(aligned_alloc(512, BUF_SIZE)));
//         char* buf = buffer.get();
//         if (i == 1) {
//             xxx = buf;
//         }
//         snprintf(buf, 19, "aaaa %lu", i);
//         // std::cout << "aligned_alloc buffer " << &buf << " with length " << malloc_usable_size(buf) << " with val " << buf << std::endl;
//         // std::cout << "Move val " << buf + 3 << std::endl;
//     }
//     std:cout << "xxx " << &xxx << " with value " << xxx << std::endl;
    
// }


void align(char* buf) {
    std::cout << "Memalign buffer2 " << &buf << " with length " << malloc_usable_size(buf) << std::endl;
    int ret = posix_memalign((void **)&buf, 512, BUF_SIZE);
    if (ret) {
        perror("posix_memalign buffer2 failed");
        exit(1);
    }
    std::cout << "Memalign buffer2 " << &buf << " with length " << malloc_usable_size(buf) << std::endl;
    free(buf);
}

int main()
{
    // int fd;
    int ret;
    char *buf;
    char buffer2[10];
    char buf3[100];

    test_ptr();

    std::cout << "&buffer2[0] " << (void*) &buffer2[0] << std::endl; // cannot output with &buffer2[0] directly
    std::cout << "&buffer2 " << &buffer2 << " with length " << sizeof(buffer2) / sizeof(char) << std::endl;

    align(buffer2);

    std::cout << "&buf " << &buf << std::endl;
    ret = posix_memalign((void **)&buf, 512, BUF_SIZE);
    if (ret) {
        perror("posix_memalign failed");
        exit(1);
    }
    std::cout << "&buf " << &buf << " with length " << malloc_usable_size(buf)<< std::endl;

 
    // fd = open("./direct_io.data", O_RDONLY, 0755);
    // if (fd < 0){
    //     perror("open ./direct_io.data failed");
    //     free(buf);
    //     exit(1);
    // }
 
    // do {
    //     ret = read(fd, buf, BUF_SIZE);
    //     if (ret < 0) {
    //         perror("read ./direct_io.data failed");
    //     }
    // } while (ret > 0);

    // test_uniq_ptr(buf3);
     
    free(buf);
    // close(fd);
}
 