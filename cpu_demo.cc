#define _GNU_SOURCE     /* Obtain O_DIRECT definition from <fcntl.h> */
#include <fcntl.h>
#include <iostream>
#include <unistd.h>  

#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<stdio.h>
#include<sys/stat.h>
#include <malloc.h>
#include <memory>
 
// #define BUFFER_LENGTH 8192
#define BUFFER_LENGTH 8589934592

using namespace std;

void loop() {
    while (1) {
        /* code */
    }
} // 100% CPU

void sleep() {
    sleep(5); // 5s
} // 0% CPU

void cout_io() {
    // size_t count = 1000000;
    // for (size_t i = 0; i < count; i++)
    // {
    //     /* code */
    //     cout << "halo" << endl;
    // }

    while (1)
    {
        cout << "halo" << endl;
    }
    

    // size_t period = 20;
    // for (size_t i = 0; i < period; i++)
    // {
    //     /* code */
    //     sleep(1);
    //     for (size_t j = 0; j < 100000; j++)
    //     {
    //         cout << "halo " << i << "-" << j << endl;
    //     }
        
    // }
}


void CopyFile(std::string sourcePath,std::string destPath)
{
	int len = 0;
	// FILE *pIn = NULL;
	// FILE *pOut = NULL;
	int ret;
	// char* buff = (char*) malloc(sizeof(char) * BUFFER_LENGTH);
	unsigned char *buff;
    ret = posix_memalign((void **)&buff, 512, BUFFER_LENGTH);
	cout << "sizeof buf" << malloc_usable_size(buff) << endl;
	// char buff[BUFFER_LENGTH] = {0};

	int rfd = open(sourcePath.c_str(), O_RDONLY | O_DIRECT | O_LARGEFILE, 0755);
	
	if(rfd < 0)
	{
		printf("Open File %s Failed...\n", sourcePath.c_str());
		return;
	}

	int wfd = open(destPath.c_str(), O_WRONLY | O_DIRECT | O_CREAT, 0755);
	if(wfd < 0)
	{
		printf("Create Dest File Failed...\n");
		return;
	}
	
	// while((len = fread(buff,sizeof(char),sizeof(buff),pIn))>0)
	while(ret = read(rfd, buff, BUFFER_LENGTH))
	{
		printf("Start to write.\n");
		// fwrite(buff,sizeof(char),len,pOut);
		ret = write(wfd, buff, BUFFER_LENGTH);
	}

	close(rfd);
	close(wfd);

	free(buff);
}


int IsDir(std::string path)
{
	if(path.empty())
	{
		return 0;
	}
	struct stat st;
	if(0 != stat(path.c_str(),&st))
	{
		return 0;
	}
	if(S_ISDIR(st.st_mode))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// void CopyFolder(std::string sourcePath,std::string destPath)
// {
// 	struct dirent* filename = NULL;
// 	if(0 == opendir(destPath.c_str()))
// 	{
// 		if (mkdir(destPath.c_str(),0777))
// 		{
// 		    printf("Create Dir Failed...\n");
// 		}
// 		else
// 		{
// 			printf("Creaat Dir %s Successed...\n", destPath.c_str());
// 		}
// 	}
// 	std::string path = sourcePath;
// 	if(sourcePath.back() != '/')
// 	{
// 		sourcePath += "/";
// 	}
// 	if(destPath.back() != '/')
// 	{
// 		destPath += "/";
// 	}
	
// 	DIR* dp=opendir(path.c_str());
// 	while(filename=readdir(dp))
// 	{
// 		std::string fileSourceFath = sourcePath;
		
// 		std::string fileDestPath = destPath;
		
// 		fileSourceFath += filename->d_name;
// 		fileDestPath += filename->d_name;
// 		if(IsDir(fileSourceFath.c_str()))
// 		{
// 			if(strncmp(filename->d_name, ".", 1) && strncmp(filename->d_name, "..", 2))
// 			{
// 				CopyFolder(fileSourceFath, fileDestPath);
// 			}		
// 		}
// 		else
// 		{
// 			CopyFile(fileSourceFath,fileDestPath);
// 			printf("Copy From %s To %s Successed\n",fileSourceFath.c_str(),fileDestPath.c_str());
// 		}
// 	}	
// }

void file_io() {
	CopyFile("/root/test", "/root/test-cp");
}

int main() {
    file_io();
    return 0;
}