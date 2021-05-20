//
// Created by 陶辰宁 on 2021/5/19.
//

#ifndef MINISQL_BUFFERMANAGER_H
#define MINISQL_BUFFERMANAGER_H
#include "../CommonHead.h"

#define POOL_SIZE 1024*1024
#define PAGE_SIZE 4096

class Page{
public:
    Page();
    bool pageWrite();
    bool isDirty();
    bool isValid();
private:
    bool valid;
    bool dirty;
    char content[PAGE_SIZE];
    int blockID;
    int usedSize;
    std::string name;
};

class BufferManager {
public:
    BufferManager();
//    ~BufferManager();
    pageInfo fetchPage(std::string fileName, int blockID);
    static int blockNum(const std::string& fileName);
private:
    Page *bufferPool;
};


#endif //MINISQL_BUFFERMANAGER_H
