#include "CommonHead.h"
#include "Interpreter.h"
#include "API.h"
using namespace std;

Interpreter c;

int main() {
    API *api;
    string SQL, line;
    cout << "-------------miniSQL interface-------------" << endl;
    cout << "miniSQL>";
    while (true){
        getline(cin, line);
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        if(line == "exit" || line == "quit" || line == "exit;" || line == "quit;"){
            cout << "Bye!" << endl;
            break;
        }
        // 如果没有;号
        SQL += line;
        if(line.find(';') != std::string::npos){
            int condition = c.interpret(SQL);
            cout << "miniSQL>";
            SQL = "";
        }
    }
}
