//
// Created by 陶辰宁 on 2021/5/19.
//

#include "BufferManager.h"
#include <utility>

BufferManager::BufferManager() {
    this->bufferPool = new Page[POOL_SIZE];
}

int BufferManager::blockNum(const std::string& fileName) {
    std::string filePath = DatabasePath + fileName;
    FILE *f = fopen(filePath.c_str(), "r");
    if(f == nullptr) return 0;
    else{
        FILE *start = f;
        fseek(f, 0, SEEK_END);
        return (f - start);
    }
}

pageInfo BufferManager::fetchPage(std::string fileName, int blockID) {
    pageInfo newPage;

    // 首先查看bufferPool中是否存在
    std::string indexFind = fileName + std::to_string(blockID);
    auto getID = index.find(indexFind);
    // 如果没有找到
    if(getID == index.end()){
        std::string filePath = DatabasePath + fileName;
        FILE *f = fopen(filePath.c_str(), "r");
        // 如果没有，创建新的page
        // 获得一个空闲的块
        int freePageID = freePage();
        newPage = bufferPool[freePageID].initialize(fileName, blockID);
        bufferPool[freePageID].pinPage();
        // 读入page
        if(f != nullptr){
            fseek(f, blockID * PAGE_SIZE, SEEK_SET);
            fread(newPage.content, PAGE_SIZE, 1, f);
            FILE *current = f;
            fseek(f, 0, SEEK_END);
            int disToEnd = int(f - current);
            if (disToEnd > PAGE_SIZE)
                newPage.usedSize = PAGE_SIZE;
            else
                newPage.usedSize = disToEnd;
            bufferPool[freePageID].contentInit(newPage);
        }
    // 如果找到了
    } else{
       newPage = bufferPool[getID->second].getPageInfo();
       bufferPool[getID->second].pinPage();
    }
    return newPage;
}

int BufferManager::freePage() {
    // 如果有free page，返回bufferPoolID，没有返回-1
    for(int i = 0; i < POOL_SIZE; ++i){
        if(bufferPool[i].isFree())
            return i;
    }
    return -1;
}


Page::Page() {
    free = true;
    dirty = false;
    blockID = -1;
    content = new char[PAGE_SIZE];
    memset(content, '\0', PAGE_SIZE);
    usedSize = 0;
    pin = false;
}

bool Page::pageWrite() {
    if(dirty && !free){
        std::string filePath = DatabasePath + name;
        FILE* f = fopen(filePath.c_str(), "w+");
        // 如果无法写入
        if(f == nullptr){
            std::cout << "Cannot open " << name << "! Failed to write back!" << std::endl;
            return false;
        }
        fseek(f, blockID * PAGE_SIZE, SEEK_SET);
        fwrite(content, PAGE_SIZE, 1, f);
        fclose(f);
    }
    else return true;
}

bool Page::isDirty() const {
    return dirty;
}

bool Page::isFree() const {
    return free;
}

pageInfo Page::initialize(std::string newName, int newBlockID) {
    name = std::move(newName);
    blockID = newBlockID;
    free = false;
    pageInfo newPage;
    newPage.content = content;
    newPage.usedSize = 0;
    // 没啥用
    return newPage;
}

void Page::makeDirty() {
    dirty = true;
}

pageInfo Page::getPageInfo() {
    pageInfo temp;
    temp.content = content;
    temp.usedSize = usedSize;
    return temp;
}

void Page::pinPage() {
    pin = true;
}

void Page::unPinPage() {
    pin = false;
}

void Page::contentInit(pageInfo in) {
    content = in.content;
    usedSize = in.usedSize;
}


