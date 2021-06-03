#ifndef MINISQL_BPTREE_H
#define MINISQL_BPTREE_H
#include "../CommonHead.h"
#include "BufferManager.h"
using namespace std;

class BpTree{
public:
    int InsertKey(const string& name, const string& value, const RecordLocation& pointer);
    int DeleteKey(const conditionPair& CD, vector<RecordLocation>& recordList, int root);
    RecordLocation FindKey(const string &name, const string &value);

private:
    BufferManager *BM;
    short type;
    inline string getValue(char *&point){
        if(type == -1){
            int temp;
            charToOther(point, temp);
            return to_string(temp);
        }
        else if(type == 0){
            float temp;
            charToOther(point, temp);
            return to_string(temp);
        }
        else{
            string temp;
            charToOther(point, temp, type);
            return temp;
        }
    }
    inline bool isLeaf(char *&point) {
        bool temp;
        charToOther(point, temp);
        return temp;
    }
};


class BpNode{
public:
    set<string> key;
    short type;
};


#endif //MINISQL_BPTREE_H
