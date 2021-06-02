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
    FILE *f = fopen(filePath.c_str(), "rb");
    if(f == nullptr){
        fclose(f);
        return 0;
    }
    else{
        fseek(f, 0, SEEK_END);
        long length = ftell(f);
        fclose(f);
        return int(length/PAGE_SIZE);
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
        FILE *f = fopen(filePath.c_str(), "rb+");
        // 如果没有，创建新的page
        // 获得一个空闲的块
        int freePageID = freePage();
        newPage = bufferPool[freePageID].initialize(fileName, blockID);
        bufferPool[freePageID].pinPage();
        // 读入page
        if(f != nullptr){
            fseek(f, blockID * PAGE_SIZE, SEEK_SET);
            fread(newPage.content, PAGE_SIZE, 1, f);
            newPage.blockID = blockID;
            bufferPool[freePageID].contentInit(newPage);
        }
        // 创建索引
        index.emplace(indexFind, freePageID);
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
        if(bufferPool[i].isFree()) return i;
    }
    return -1;
}

void BufferManager::changeComplete(const std::string& fileName, int blockID) {
    std::string indexFind = fileName + std::to_string(blockID);
    auto bufferID = index.find(indexFind);
    bufferPool[bufferID->second].makeDirty();
    bufferPool[bufferID->second].unPinPage();
    bool already = false;
    for(int cur : dirtyPage){
        if (bufferID->second == cur) {
            already = true;
            break;
        }
    }
    if(!already) dirtyPage.push_back(bufferID->second);
}

BufferManager::~BufferManager() {
    for(int cur : dirtyPage){
        bufferPool[cur].pageWrite();
    }
    for(std::string cur : deleteList){
        remove(cur.data());
    }
}

void BufferManager::deletePage(const std::string& fileName, int blockID) {
    pageInfo deletePage = fetchPage(fileName, blockID);
    memset(deletePage.content, '\0', PAGE_SIZE);
    changeComplete(fileName, blockID);
}

void BufferManager::deleteFile(const std::string& fileName) {
    deleteList.push_back(fileName);
}

void BufferManager::flush() {
    for(int cur : dirtyPage){
        bufferPool[cur].pageWrite();
    }
    std::vector<int>().swap(dirtyPage);
    for(std::string cur : deleteList){
        int i = 0;
        std::string temp = cur + std::to_string(i);
        auto getID = index.find(temp);

        while(getID != index.end()) {
            if (getID != index.end()) {
                bufferPool[getID->second].makeFree();
                index.erase(getID);
            }
            ++i;
            temp = cur + std::to_string(i);
            getID = index.find(temp);
        }
        std::string fileName = DatabasePath + cur;
        remove(fileName.data());
    }
    std::vector<std::string>().swap(deleteList);
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
        FILE* f = fopen(filePath.c_str(), "rb+");
        // 如果无法写入
        if(f == nullptr){
            f = fopen(filePath.c_str(), "wb+");
            std::cout << "Create " << name << " in " << filePath << std::endl;
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
    memset(this->content, '\0', PAGE_SIZE);
    newPage.content = content;
    newPage.blockID = newBlockID;
    // 没啥用
    return newPage;
}

void Page::makeDirty() {
    dirty = true;
}

pageInfo Page::getPageInfo() {
    pageInfo temp;
    temp.content = content;
    temp.blockID = blockID;
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
    blockID = in.blockID;
}

void Page::makeFree() {
    free = true;
}

Page::~Page() = default;


