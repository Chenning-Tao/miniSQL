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
    bool isDirty() const;
    bool isFree() const;
    void makeDirty();
    void pinPage();
    void unPinPage();
    pageInfo getPageInfo();
    pageInfo initialize(std::string newName, int newBlockID);
    void contentInit(pageInfo in);
private:
    bool free;
    bool dirty;
    char *content;
    int blockID;
    int usedSize;
    bool pin;
    std::string name;
};

class BufferManager {
public:
    BufferManager();
//    ~BufferManager();
    pageInfo fetchPage(std::string fileName, int blockID);
    int freePage();
    static int blockNum(const std::string& fileName);
private:
    Page *bufferPool;
    std::vector<Page*> dirtyPage;
    std::unordered_map<std::string, int> index;
};

template<typename T> inline void otherToChar(T raw, char *&cur){
    int size = sizeof(raw);
    memcpy(cur, &raw, size);
    cur += size;
}

#endif //MINISQL_BUFFERMANAGER_H
