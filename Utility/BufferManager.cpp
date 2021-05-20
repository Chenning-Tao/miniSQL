//
// Created by 陶辰宁 on 2021/5/19.
//

#include "BufferManager.h"

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
    // 初始化 如果返回是content为nullptr表示失败
    pageInfo newPage;
    newPage.usedSize = -1;
    newPage.content = nullptr;

    std::string filePath = DatabasePath + fileName;
    FILE *f = fopen(filePath.c_str(), "r");
    if(f == nullptr)
        return newPage;
    // 读入page
    fseek(f, blockID * PAGE_SIZE, SEEK_SET);
    fread(newPage.content, PAGE_SIZE, 1, f);

    FILE *current = f;
    fseek(f, 0, SEEK_END);
    int disToEnd = int(f - current);
    if(disToEnd > PAGE_SIZE)
        newPage.usedSize = PAGE_SIZE;
    else
        newPage.usedSize = disToEnd;
    return newPage;
}

Page::Page() {
    valid = false;
    dirty = false;
    blockID = -1;
    memset(content, '\0', PAGE_SIZE);
    usedSize = 0;
}

bool Page::pageWrite() {
    if(dirty && valid){
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

bool Page::isDirty() {
    return dirty;
}

bool Page::isValid() {
    return valid;
}

