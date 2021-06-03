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
                    throw string("Cannot find key!");
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
}

int BpTree::InsertKey(const string &name, const string &value, const RecordLocation &pointer) {
    return 0;
}

int BpTree::DeleteKey(const conditionPair &CD, vector<RecordLocation> &recordList, int root) {
    return 0;
}
