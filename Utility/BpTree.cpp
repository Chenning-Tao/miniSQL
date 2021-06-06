#include "BpTree.h"

RecordLocation BpTree::FindKey(const string &name, const string &value) {
    // 第0块是根
    pageInfo start = BM->fetchPage(name, 0);
    while(true) {
        char *point = start.content;
        bool leaf = isLeaf(point);

        int count, parent;
        charToOther(point, count);
        charToOther(point, parent);
        if(leaf){
            string prev, cur;
            RecordLocation prevLoc, curLoc;
            charToOther(point, prevLoc.page);
            charToOther(point, prevLoc.offset);
            prev = getValue(point);
            for(int i = 1; i < count; ++i){
                charToOther(point, curLoc.page);
                charToOther(point, curLoc.offset);
                cur = getValue(point);
                if(cur == value){
                    return curLoc;
                }
                else if(prev < value && cur > value){
                    throw runtime_error("Cannot find key!");
                }
            }
            break;
        }
        else {
            charToOther(point, count);
            int prevChild, curChild;
            charToOther(point, parent);

            // 寻找到哪一块
            charToOther(point, prevChild);
            string prev;
            string cur;
            prev = getValue(point);
            for (int i = 1; i < count; ++i) {
                charToOther(point, curChild);
                cur = getValue(point);
                if (prev < value && value < cur) {
                    BM->fetchPage(name, prevChild);
                }
            }
        }
    }
    return RecordLocation ();
}

int BpTree::InsertKey(const string &name, string &value, const RecordLocation &pointer) {
    // 读入第0块
    pageInfo start = BM->fetchPage(name, 0);
    int blockNum, parent;
    char *point = start.content;
    bool leaf = isLeaf(point);
    charToOther(point, blockNum);
    charToOther(point, parent);
    // 判断根有没有满
    int maxNum = Num(leaf);
    if(leaf && blockNum < maxNum){
        point += (blockNum*offset(leaf));
        outputValue(point, value);
    }else if(leaf && blockNum == maxNum){
        ++totalPage;
        pageInfo newPage = BM->fetchPage(name, totalPage);
    }
    return 0;
}

int BpTree::DeleteKey(const string& name, const conditionPair& CD, vector<RecordLocation>& recordList) {
    // 读入第0块
    pageInfo start = BM->fetchPage(name, 0);
    int blockNum, parent;
    char *point = start.content;
    bool leaf = isLeaf(point);
    charToOther(point, blockNum);
    charToOther(point, parent);
    // 判断根有没有满
    int maxNum = Num(leaf);
    int off = offset(leaf);
    int count = 0;
    if(leaf && blockNum < maxNum){
        while(count < blockNum){

        }
    }
    while(leaf && blockNum < maxNum){
        RecordLocation tempLoc;
        charToOther(point, tempLoc.page);
        charToOther(point, tempLoc.offset);
        string temp = getValue(point);
        if(condition(CD, temp)){
            point -= off;
            memset(point, '\0', off);
        }
    }
    return 0;
}
