//
// Created by 陶辰宁 on 2021/5/2.
//

#include "Interpreter.h"

int Interpreter::interpret(string SQL) {
    regex Word("[a-z]+");
    smatch firstWord;
    if(regex_search(SQL, firstWord, Word)){
        // 用于判断是哪类操作
        string type = firstWord.str(0);
        if(type == "insert"){

        }else if(type == "drop"){
            smatch dropWord;
            regex dropTable("drop[\\s]table[\\s]");
            regex dropIndex("drop[\\s]index[\\s]");
            // drop table
            if(regex_search(SQL, dropWord, dropTable))
                return this->dropTable(SQL.substr(dropWord[0].length()));
        }else if(type == "create"){
            smatch dropWord;    // 用来将前面这一段去掉
            regex createTable("create[\\s]table[\\s]");
            regex createIndex("create[\\s]index[\\s]");
            if(regex_search(SQL, dropWord, createIndex))
                return this->createIndex(SQL.substr(dropWord[0].length()));
            else if(regex_search(SQL, dropWord, createTable))
                return this->createTable(SQL.substr(dropWord[0].length()));
            else
                cout << "Syntax error near create!" << endl;
        }else if(type == "delete"){

        }else if(type == "select"){

        }else
            cout << "Syntax error!" << endl;
    } else
        cout << "Syntax error!" << endl;
}

int Interpreter::createTable(string SQL) {
    regex Word("[\\w]+");
    smatch tableNameMatch;
    // 获得table的名字
    regex_search(SQL, tableNameMatch, Word);
    string tableName = tableNameMatch[0];
    SQL = SQL.substr(tableNameMatch[0].length());

    regex bracket(R"(\((.+?)\)[\s]?;[\s]?)");
    regex intRule(R"(^[\s]?[\s]?([\w]+)[\s]int[\s]?(unique)?,[\s]?)");
    regex charRule(R"(^[\s]?[\s]?([\w]+)[\s]char[\(]([0-9]+)[\)][\s]?(unique)?,[\s]?)");
    regex floatRule(R"(^[\s]?[\s]?([\w]+)[\s]float[\s]?(unique)?,[\s]?)");
    regex primaryRule(R"(^[\s]?[\s]?primary[\s]key[\s]?[\(]([\w]+)[\)][\s]?,[\s]?)");
    smatch attributesS;
    smatch intS;
    smatch charS;
    smatch floatS;
    smatch primaryS;

    regex_search(SQL, attributesS, bracket);
    string attributes = attributesS.str(1);
    attributes.push_back(',');
    Attribute newTable;
    while(!attributes.empty()){
        if(regex_search(attributes, charS, charRule)){
            newTable.setChar(charS[1], short(stoi(charS[2])), charS[3].length());
            attributes = attributes.substr(charS[0].length());
        }else if(regex_search(attributes, intS, intRule)){
            newTable.setInt(intS[1], intS[2].length());
            attributes = attributes.substr(intS[0].length());
        }else if(regex_search(attributes, floatS, floatRule)){
            newTable.setFloat(floatS[1], floatS[2].length());
            attributes = attributes.substr(floatS[0].length());
        }else if(regex_search(attributes, primaryS, primaryRule)){
            if(!newTable.setPrimary(primaryS[1])) return 0;
            attributes = attributes.substr(primaryS[0].length());
        }else{
            cout << "Error near ' " << attributes.substr(0, 10) << " '" <<endl;
            break;
        }
    }

    api.createTable(tableName, newTable);
    return 1;
}

int Interpreter::createIndex(string SQL){

}

bool Interpreter::dropTable(string SQL) {
    regex Word("([\\w]+)[\\s]?;");
    smatch tableNameMatch;
    // 获得table的名字
    if(regex_match(SQL, tableNameMatch, Word)) {
        string tableName = tableNameMatch[1];
        api.dropTable(tableName);
    }
    else{
        cout << "Syntax error near delete!" << endl;
        return false;
    }
}
