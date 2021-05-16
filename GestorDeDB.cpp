// GestorDeDB.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Tabla.h"
#include "Col.h"
using namespace std;
vector<string> split(string comando,string delimiter=" ") {
    vector<string> campos;
    size_t pos = comando.find(delimiter);
    while (pos != string::npos)
    {
        campos.push_back(string(comando.begin(), comando.begin() + pos));
        comando = string(comando.begin() + pos + 1, comando.end());
        pos = comando.find(delimiter);
    }
    campos.push_back(comando);
    return campos;

}
string trim(string texto) {
    string::iterator ini = texto.begin(),fin=texto.end()-1;
    while (*ini == ' ')ini++;
    while (*fin == ' ')fin--;
    return string(ini, fin+1);
}
void writeTable(Tabla tb) {
    ofstream f("Tablas.txt", ios::app);
    if (f.is_open()) {
        f << tb.toString();
        f << "\n";
        f.close();
    }
    else cout << "Error de apertura de archivo." << endl;
}

void createTable(string data) {
    string tablename = string(data.begin()+1, data.begin()+ data.find("("));
    string args = data.substr(data.find("(")+1, data.find(")")-1);
    vector<string> campos=split(trim(args),",");
    const int c = campos.size();
    vector<Col> columnas;
    for (string a : campos) {
        vector<string> val = split(trim(a));
        columnas.push_back(Col(val[0],val[1]));
    }
    Tabla t(tablename,columnas.size(),columnas);
    int a = 0;
    writeTable(t);
}
string readComand() {
    string comando;
    string line;
    do {
        getline(cin, line);
        comando += line;
    } while (line[line.size() - 1] != ';');
    return comando;
}
int main()
{
    string a = trim("hola");
    cout << a;
    string comando=readComand();

   
    size_t pos = comando.find(" ");
    
    if (comando.substr(0, pos) == "create") {
        string value = comando.substr(pos+1,comando.size()-1);
        pos = value.find(" ");
        size_t pospar = value.find("(");
        string a = (pospar < pos)? value.substr(0, pospar) : value.substr(0, pos);
        pos = (pospar < pos)?pospar:pos;
        if (value.substr(0, pos) == "table")
            createTable(string(value.begin()+pos,value.begin()+ value.size() - 2));
    }
    
    /*Profesor profesores[10] = { {"P1","RobertoAB"},{"P2","Sharon"},{"P3","Juan"} ,
                                    {"P4","Piero"} ,{"P5","Renzo"} ,{"P6","Lotus"},
                                    {"P7","Carlos"} ,{"P8","Carla"} ,{"P9","Kato"},{"O9","Sharon"} };

    ofstream f("profesores.bin", ios::binary);
    if (f.is_open()) {
        for (Profesor p : profesores) {
            f.write((char*)&p, sizeof(Profesor));
        }
    }
    else cout << "Error de apertura de archivo." << endl;
    f.close();
    Profesor p;

    ifstream fi("profesores.bin", ios::binary);

    if (fi.is_open()) {
        fi.read((char*)&p, sizeof(Profesor));
        while (!fi.eof()) {
            cout << "id: " << p.id << "nombre: " << p.nombre << endl;
            fi.read((char*)&p, sizeof(Profesor));
        }
    }
    else cout << "Error de apertura de archivo." << endl;

    fi.close();*/
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
