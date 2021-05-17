// GestorDeDB.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Tabla.h"
#include "Col.h"
using namespace std;

Tabla tableexist(string tbname);





vector<Tabla> tablas;
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
    if (tableexist(tablename).numcols) {
        string args = string(data.begin() + data.find("(") + 1, data.begin() + data.find(")"));
        vector<string> campos = split(trim(args), ",");
        const int c = campos.size();
        vector<Col> columnas;
        for (string a : campos) {
            vector<string> val = split(trim(a));
            columnas.push_back(Col(val[0], val[1]));
        }
        Tabla t(tablename, columnas.size(), columnas);
        writeTable(t);
        tablas.push_back(t);
    }
    else {
        cout << "tabla \"" + tablename + "\" ya existe"<<endl;
    }
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

Tabla tableexist(string tbname) {
    for (Tabla t : tablas) if (t.nombre == tbname) return t;
    return Tabla();
}
void insertValue(string val) {
    size_t pos = (val.find(" ")>val.find("("))?val.find("("): val.find(" ");
    
    string tablename = string(val.begin(), val.begin() + pos);
    val = string(val.begin()+val.find("(")+1,val.begin()+val.find(")"));
    Tabla tb = tableexist(tablename);
    if (tb.nombre!="") {
        vector<string> valores = split(val, ",");
        
        if (valores.size() != tb.numcols) {
            cout << "numero incorrecto de columnas"<<endl;
        }
        else {
            string finalvalues;
            for (string v : valores) finalvalues += trim(v) + ",";
            finalvalues = string(finalvalues.begin(), finalvalues.end() - 1);
            ofstream f(tablename + ".txt", ios::app);
            if (f.is_open()) {
                f << finalvalues;
                f << "\n";
                f.close();
            }
            else cout << "Error de apertura de archivo." << endl;
        }
    }
    else {
        cout << "Tabla no existe";
    }
}

void cargardata() {
    ifstream myfile("Tablas.txt");
    string line;
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            size_t pos = line.find("[");
            vector<string> tbd = split(string(line.begin(), line.begin() + pos-1),"/");
            vector<string> cd = split(string(line.begin() + pos + 1, line.end() - 1), ",");
            vector<Col> colums;
            for (string c : cd) {
                vector<string> ds = split(c, "/");
                colums.push_back(Col(ds[0], ds[1]));
            }
            tablas.push_back(Tabla(tbd[0],stoi(tbd[1]),colums));
        }
        myfile.close();
    }

    else cout << "Unable to open file";
}

vector<vector<string>> getall(string tablename) {
    fstream file;
    file.open(tablename + ".txt");
    string line;
    vector<vector<string>> data;
    while (std::getline(file, line)) {
        data.push_back(split(line, ","));
    }
    file.close();
    return data;
}
int getIndexColum(vector<Col> colums,string data) {
    for (int i = 0; i < colums.size(); i++) {
        if (colums[i].nombre == trim(data)) return i;
    }
    return -1;
}
int getbycol(vector<vector<string>>datos, int i,string val) {
    for (int j = 0; j < datos.size();j++) {
        if (datos[j][i] == val)  return j;
    }
    return -1;
}
int main()
{
    cargardata();
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
                createTable(string(value.begin() + pos, value.end()));
        }
        if (ident == "insert") {
            pos = value.find(" ");
            if (string(value.begin(), value.begin() + pos) == "into") {
                value = string(value.begin() + pos+1, value.end());
                pos = value.find(" ");
                insertValue(value);

            }
        }
        if (ident == "update") {
            pos = value.find(" ");
            string tablename = string(value.begin(),value.begin()+pos);
            value.erase(value.begin(), value.begin() + pos+1);
            Tabla tb = tableexist(tablename);
            if (tb.numcols) {
                pos = value.find(" ");
                if (string(value.begin(), value.begin() + pos) == "set") {
                    //primero valida que la condicion del where exista XD
                    value.erase(0, 4);
                    pos = value.find("where");
                    string updateds = trim(string(value.begin(), value.begin() + pos));
                    value.erase(0, pos);

                    vector<vector<string>> data= getall(tablename);
                    

                    string condicion = trim(string(value.begin() + value.find("where") + 6, value.end() - 1));
                    vector<string> cond = split(condicion, "=");
                    int colnumcond = getIndexColum(tb.colums, trim(cond[0]));
                    int indexdata = getbycol(data,colnumcond,trim(cond[1]));

                    vector<string> parametros = split(updateds, ",");
                    for (string p : parametros) {
                        vector<string> datos = split(p, "=");
                        int columnumber=getIndexColum(tb.colums,trim(datos[0]));
                        data[indexdata][columnumber] = trim(datos[1]);
                    }
                    vector<string>finald;
                    for (vector<string> d : data) {
                        string s;
                        for (int i = 0; i < d.size() - 1; i++) {
                            s += d[i] + ",";
                        }
                        s += d[d.size() - 1]+"\n";
                        finald.push_back(s);
                    }

                    ofstream f(tablename+".txt", ios::out);
                    
                    if (f.is_open()) {
                        for(string a:finald) f << a;
                        f.close();
                    }
                    else cout << "Error de apertura de archivo." << endl;

                }
            }
                
        }
        if (ident == "delete") {

        }
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
