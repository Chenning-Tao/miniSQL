#ifndef MINISQL_BPTREE_H
#define MINISQL_BPTREE_H
#include "../CommonHead.h"
#include "BufferManager.h"
using namespace std;

class BpTree{
public:
    int InsertKey(const string& name, string& value, const RecordLocation& pointer);
    int DeleteKey(const string& name, const conditionPair& CD, vector<RecordLocation>& recordList);
    RecordLocation FindKey(const string &name, const string &value);

private:
    BufferManager *BM;
    short type;
    int totalPage;
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
    inline int Num(bool leaf) const{
        if(!leaf) {
            if(type <= 0){
                return ((PAGE_SIZE - 1 - 8)/8);
            }
            else {
                return ((PAGE_SIZE - 1 - 8)/(type+4));
            }
        }else {
            if(type <= 0){
                return ((PAGE_SIZE - 1 - 8)/12);
            }
            else {
                return ((PAGE_SIZE - 1 - 8)/(type+8));
            }
        }
    }
    inline int offset(bool leaf){
        if(!leaf) {
            if(type <= 0){
                return 8;
            }
            else {
                return (type+4);
            }
        }else {
            if(type <= 0){
                return 12;
            }
            else {
                return (type+8);
            }
        }
    }
    inline void outputValue(char *&point, string& value){
        if(type == -1){
            otherToChar(stoi(value), point);
        }else if(type == 0){
            otherToChar(stof(value), point);
        }
        else{
            otherToChar(value, point, type);
        }
    }
    inline bool condition(const conditionPair& CD, const string& value){
        if(CD.type > 0){
            switch (CD.OP) {
                case Greater: return value > CD.condition;
                case Less: return value < CD.condition;
                case Equal: return value == CD.condition;
                case NotEqual: return value != CD.condition;
                case GreaterEqual: return value >= CD.condition;
                case LessEqual: return value <= CD.condition;
                default: return false;
            }
        }else if(CD.type == -1){
            int a = stoi(value);
            int b = stoi(CD.condition);
            switch (CD.OP) {
                case Greater: return a > b;
                case Less: return a < b;
                case Equal: return a == b;
                case NotEqual: return a != b;
                case GreaterEqual: return a >= b;
                case LessEqual: return a <= b;
                default: return false;
            }
        }
        else if(CD.type == 0){
            float a = stof(value);
            float b = stof(CD.condition);
            switch (CD.OP) {
                case Greater: return a > b;
                case Less: return a < b;
                case Equal: return a == b;
                case NotEqual: return a != b;
                case GreaterEqual: return a >= b;
                case LessEqual: return a <= b;
                default: return false;
            }
        }
    };
};


class BpNode{
public:
    map<string, int> stringKey;
    map<float, int> floatKey;
    map<int, int> intKey;
    short type;
};


#endif //MINISQL_BPTREE_H
