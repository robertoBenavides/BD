// GestorDeDB.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Tabla.h"
#include "Col.h"
#include "Fecha.h"
#include "StringAdapter.h"
#include "TableManager.h"

using namespace std;
StringAdapter SA;
TableManager TM;


string readComand() {
    string comando=" ";
    string line;
    do {
        getline(cin, line);
        comando += line;
    } while (comando[comando.size() - 1] != ';');
    return SA.trim(comando);
}


int main()
{
   
    bool running = true;
    while (running) {

        string comando = readComand();
        size_t pos = comando.find(" ");
        string ident = string(comando.begin(), comando.begin() + pos);
        string value = string(comando.begin() + pos + 1, comando.end());
        if (ident == "create") {
            pos = value.find(" ");
            size_t pospar = value.find("(");
            string a = (pospar < pos) ? value.substr(0, pospar) : value.substr(0, pos);
            pos = (pospar < pos) ? pospar : pos;
            if (value.substr(0, pos) == "table")
                TM.createTable(string(value.begin() + pos, value.end()));
        }
        if (ident == "insert") {
            pos = value.find(" ");
            if (string(value.begin(), value.begin() + pos) == "into") {
                value = string(value.begin() + pos + 1, value.end());
                pos = value.find(" ");
                TM.insertValue(value);

            }
        }
        if (ident == "update") {
            TM.updateValue(value);
            
        }
        if (ident == "delete") {
            TM.deleteValue(value);



        }
        if (ident == "select") {
            pos = value.find("from");
            string campos = string(value.begin(), value.begin() + pos);
            value.erase(value.begin(), value.begin() + pos + 4);
            TM.select(value, campos);
        }
    }
}