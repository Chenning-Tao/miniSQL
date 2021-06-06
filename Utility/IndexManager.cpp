
#include "IndexManager.h"

void IndexManager::insertKey(const string& tableName, int column, const string& key, int pageNum) {
    auto indexFind = index.find(tableName + to_string(column));
    if(indexFind != index.end()){
        indexFind->second.key.emplace(key, pageNum);
    }
}

int IndexManager::findKey(const string& tableName, int column, const string& key) {
    auto indexFind = index.find(tableName + to_string(column));
    if(indexFind == index.end()){
        throw runtime_error("Index doesn't exist!");
    } else {
        if(indexFind->second.key.find(key) == indexFind->second.key.end()) return -1;
        else return indexFind->second.key.find(key)->second;
    }
}

IndexManager::IndexManager(BufferManager *inBM, Table *inTB) {
    BM = inBM;
    TB = inTB;
    for(const auto& Table : TB->index){
        string tableName = Table.first;
        vector<bool> tableIndexColumn;
        TB->tableInfo[Table.second].getIndex(tableIndexColumn);
        pageInfo start;
        char *point;
        for(int i = 0; i < tableIndexColumn.size(); ++i){
            if(tableIndexColumn[i]){
                string fileName = tableName + to_string(i) + "_idx";
                short type;
                int totalNumber;
                start = BM->fetchPage(fileName, 0);
                point = start.content;
                charToOther(point, type);
                charToOther(point, totalNumber);
                createIndex(tableName, i, type);

                int recordPerPage;
                if(type <= 0) recordPerPage = PAGE_SIZE/8;
                else recordPerPage = PAGE_SIZE/(type+4);
                int count = 0;
                int curPage = 0;
                int num;

                for(int j = 0; j < totalNumber; ++j){
                    if(count == 0) {
                        count = recordPerPage;
                        BM->changeComplete(fileName, curPage);
                        ++curPage;
                        start = BM->fetchPage(fileName, curPage);
                        point = start.content;
                    }
                    if(type == -1) {
                        int temp;
                        charToOther(point, temp);
                        charToOther(point, num);
                        insertKey(tableName, i, to_string(temp), num);
                    }
                    else if(type == 0){
                        float temp;
                        charToOther(point, temp);
                        insertKey(tableName, i, to_string(temp), num);
                    }
                    else {
                        string temp;
                        charToOther(point, temp, type);
                        insertKey(tableName, i, temp, num);
                    }
                    --count;
                }
            }
        }
    }
}

void IndexManager::createIndex(const string& tableName, int column, short type) {
    auto indexFind = index.find(tableName + to_string(column));
    if(indexFind == index.end()) {
        BpNode newNode;
        newNode.type = type;
        index.emplace(tableName + to_string(column), newNode);
        BM->fetchPage(tableName + to_string(column) + "_idx", 0);
        BM->changeComplete(tableName + to_string(column) + "_idx", 0);
    }
}

void IndexManager::dropIndex(const string& tableName, int column) {
    auto indexFind = index.find(tableName + to_string(column));
    if(indexFind != index.end()){
        index.erase(indexFind);
        BM->deleteFile(tableName + to_string(column) + "_idx");
        BM->flush();
    }
}

IndexManager::~IndexManager() {
    for(const auto& indexFind : index){
        short type = indexFind.second.type;
        string fileName = indexFind.first + "_idx";
        pageInfo start = BM->fetchPage(fileName, 0);
        char *point = start.content;
        otherToChar(type, point);
        int size = indexFind.second.key.size();
        otherToChar(size, point);

        int recordPerPage;
        if(type <= 0) recordPerPage = PAGE_SIZE/8;
        else recordPerPage = PAGE_SIZE/(type+4);
        int count = 0;
        int curPage = 0;

        BM->changeComplete(fileName, 0);

        for(const auto& value : indexFind.second.key){
            if(count == 0) {
                count = recordPerPage;
                BM->changeComplete(fileName, curPage);
                ++curPage;
                start = BM->fetchPage(fileName, curPage);
                point = start.content;
            }
            if(type == -1) {
                otherToChar(stoi(value.first), point);
                otherToChar(value.second, point);
            }
            else if(type == 0){
                otherToChar(stof(value.first), point);
                otherToChar(value.second, point);
            }
            else {
                string temp = value.first;
                otherToChar(temp, point, type);
                otherToChar(value.second, point);
            }
            --count;
        }
        BM->changeComplete(fileName, curPage);
    }
}

void IndexManager::deleteKey(const string& tableName, int column, const string& key) {
    auto indexFind = index.find(tableName + to_string(column));
    if(indexFind != index.end()){
        indexFind->second.key.erase(key);
    }
}

void IndexManager::deleteAll(const string &tableName, int column) {
    auto indexFind = index.find(tableName + to_string(column));
    short type = indexFind->second.type;
    dropIndex(tableName, column);
    createIndex(tableName, column, type);
}
