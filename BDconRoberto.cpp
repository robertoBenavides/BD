// GestorDeDB.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Tabla.h"
#include "Col.h"
using namespace std;

vector<Tabla> tablas;
bool tableexist(string tbname, Tabla*& a);

vector<string> split(string comando, string delimiter = " ") {
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
    string::iterator ini = texto.begin(), fin = texto.end() - 1;
    while (*ini == ' ')ini++;
    while (*fin == ' ')fin--;
    return string(ini, fin + 1);
}
void writeTable(Tabla tb) {
    ofstream f("Tablas.txt", ios::app);
    if (f.is_open()) {
        f << tb.toString();
        f << "\n";
        f.close();
    }
    else  cout << "Error de apertura de archivo." << endl;

}
void rewriteInfo(vector<string>finald, string tablename) {
    ofstream f(tablename + ".txt", ios::out);

    if (f.is_open()) {
        for (string a : finald) f << a;
        f.close();
    }
    else cout << "Error de apertura de archivo." << endl;
}
void createTable(string data) {
    string tablename = string(data.begin() + 1, data.begin() + data.find("("));
    Tabla* tb;
    if (!tableexist(tablename, tb)) {
        string args = string(data.begin() + data.find("(") + 1, data.begin() + data.find(")"));
        vector<string> campos = split(trim(args), ",");
        const int c = campos.size();
        vector<Col> columnas;

        for (string a : campos) {
            vector<string> val = split(trim(a));
            size_t pos = val[1].find("char");
            if (pos != string::npos) {
                vector<string> tam = split(trim(val[1]), "[");
                size_t postam = tam[1].find("]");
                string tamanio = string(tam[1].begin(), tam[1].begin() + postam);
                columnas.push_back(Col(val[0], tam[0], tamanio));
            }
            else {
                columnas.push_back(Col(val[0], val[1]));
            }
        }
        Tabla t(tablename, columnas.size(), columnas);
        writeTable(t);
        tablas.push_back(t);
    }
    else {
        cout << "tabla \"" + tablename + "\" ya existe" << endl;
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
bool tableexist(string tbname, Tabla*& a) {
    for (Tabla& t : tablas)
        if (t.nombre == tbname) {
            a = &t;
            return true;
        }
    return false;
}
bool confirmMessage(string conf) {
    cout << conf << endl;
    cout << "0:no" << "\t" << "1:si" << endl;
    int c;
    cin >> c;
    return c;
}
string fixCadsize(string val, int tam) {
    int rellenar = tam - val.size();
    string finalstring = (rellenar <= 0) ? string(val.begin(), val.begin() + tam) : val;
    string relleno = (rellenar > 0) ? string(rellenar, ' ') : "";
    finalstring += relleno;
    return finalstring;
}
void insertValue(string val) {
    size_t pos = (val.find(" ") > val.find("(")) ? val.find("(") : val.find(" ");

    string tablename = string(val.begin(), val.begin() + pos);
    val = string(val.begin() + val.find("(") + 1, val.begin() + val.find(")"));
    Tabla* t;
    if (tableexist(tablename, t)) {
        Tabla tb = *t;
        vector<string> valores = split(val, ",");

        if (valores.size() != tb.numcols) {
            cout << "numero incorrecto de columnas" << endl;
        }
        else {
            string finalvalues;
            for (int i = 0; i < valores.size(); i++) {

                if (tb.colums[i].tamanio != "0") {
                    finalvalues += fixCadsize(trim(valores[i]), stoi(tb.colums[i].tamanio));
                }
                else {
                    finalvalues += trim(valores[i]);
                }
                finalvalues += ",";
            }
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
            vector<string> tbd = split(string(line.begin(), line.begin() + pos - 1), "/");
            vector<string> cd = split(string(line.begin() + pos + 1, line.end() - 1), ",");
            vector<Col> colums;
            for (string c : cd) {
                vector<string> ds = split(c, "/");
                if (ds.size() == 3) {
                    colums.push_back(Col(ds[0], ds[1], ds[2]));
                }
                else {
                    colums.push_back(Col(ds[0], ds[1]));
                }
            }
            tablas.push_back(Tabla(tbd[0], stoi(tbd[1]), colums));
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
vector<string> toFlatString(vector<vector<string>> data, string del = ",") {
    vector<string> finald;
    for (vector<string> d : data) {
        string s;
        for (int i = 0; i < d.size() - 1; i++) {
            s += d[i] + del;
        }
        s += d[d.size() - 1] + "\n";
        finald.push_back(s);
    }
    return finald;
}
int getIndexColum(vector<Col> colums, string data) {
    for (int i = 0; i < colums.size(); i++) {
        if (colums[i].nombre == trim(data)) return i;
    }
    return -1;
}
vector<int> getbycol(Tabla tb, vector<vector<string>>datos, int i, string val,string condicional) {
    vector<int> coinciden;
    for (int j = 0; j < datos.size(); j++) {
        if (condicional == "=") {
            if (trim(datos[j][i]) == val)  coinciden.push_back(j);
        }
        else if (condicional == "<") {
            string tipo=tb.colums[i].tipo;
            if (tipo == "int") {
                int cond1=stoi(datos[j][i]);
                int cond2 = stoi(val);
                if (cond1 < cond2)  coinciden.push_back(j);
            }
            else {
                if (trim(datos[j][i]) < val)  coinciden.push_back(j);
            }
        }
        else if (condicional == ">") {
            string tipo = tb.colums[i].tipo;
            if (tipo == "int") {
                int cond1 = stoi(datos[j][i]);
                int cond2 = stoi(val);
                if (cond1 > cond2)  coinciden.push_back(j);
            }
            else {
                if (trim(datos[j][i]) > val)  coinciden.push_back(j);
            }
        }
    }
    return coinciden;
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
                value = string(value.begin() + pos + 1, value.end());
                pos = value.find(" ");
                insertValue(value);

            }
        }
        if (ident == "update") {
            pos = value.find(" ");
            string tablename = string(value.begin(), value.begin() + pos);
            value.erase(value.begin(), value.begin() + pos + 1);
            Tabla* t;
            if (tableexist(tablename, t)) {
                Tabla tb = *t;
                pos = value.find(" ");
                if (string(value.begin(), value.begin() + pos) == "set") {
                    value.erase(0, 4);
                    pos = value.find("where");
                    string updateds = trim(string(value.begin(), value.begin() + pos));
                    value.erase(0, pos);

                    vector<vector<string>> data = getall(tablename);


                    string condicion = trim(string(value.begin() + value.find("where") + 6, value.end() - 1));
                    string condicional;
                    if (condicion.find("=") != string::npos) {
                        condicional = "=";
                    }
                    if (condicion.find("<") != string::npos) {
                        condicional = "<";
                    }
                    if (condicion.find(">") != string::npos) {
                        condicional = ">";
                    }
                    vector<string> cond = split(condicion, condicional);

                    int colnumcond = getIndexColum(tb.colums, trim(cond[0]));
                    vector<int> indexdata = getbycol(tb, data, colnumcond, trim(cond[1]), condicional);

                    vector<string> parametros = split(updateds, ",");
                    for (string p : parametros) {
                        vector<string> datos = split(p, "=");
                        int columnumber = getIndexColum(tb.colums, trim(datos[0]));
                        for (int i = 0; i < indexdata.size(); i++) {
                            if (tb.colums[columnumber].tamanio != "0") {
                                string charval = fixCadsize(datos[1], stoi(tb.colums[columnumber].tamanio));
                                data[indexdata[i]][columnumber] = charval;
                            }
                            else {
                                data[indexdata[i]][columnumber] = trim(datos[1]);
                            }
                        }
                        
                    }
                    vector<string>finald = toFlatString(data);


                    rewriteInfo(finald, tablename);

                }
            }

        }
        if (ident == "delete") {
            pos = value.find(" ");
            if (string(value.begin(), value.begin() + pos) == "from") {
                value.erase(value.begin(), value.begin() + pos + 1);
                pos = value.find(" ");
                string tablename = string(value.begin(), value.begin() + pos);
                pos = value.find("where");
                value.erase(value.begin(), value.begin() + pos + 5);
                string condicion = trim(string(value.begin(), value.end() - 1));


                string condicional;
                if (condicion.find("=") != string::npos) {
                    condicional = "=";
                }
                if (condicion.find("<") != string::npos) {
                    condicional = "<";
                }
                if (condicion.find(">") != string::npos) {
                    condicional = ">";
                }

                vector<string> cond = split(trim(string(value.begin(), value.end() - 1)), condicional);

                Tabla* t;
                if (tableexist(tablename, t)) {
                    Tabla tb = *t;
                    vector<vector<string>> data = getall(tablename);
                    int colnumcond = getIndexColum(tb.colums, trim(cond[0]));
                    vector<int> indexdata = getbycol(tb, data, colnumcond, trim(cond[1]),condicional);
                    
                    for (int i = indexdata.size()-1; i >= 0;i--) {
                        data.erase(data.begin() + indexdata[i]);
                    }

                    vector<string>finald = toFlatString(data);
                    rewriteInfo(finald, tablename);
                }
                else {
                    cout << "la tabla \"" + tablename + "\" no existe";
                }


            }



        }
        if (ident == "select") {
            pos = value.find("from");
            string campos = string(value.begin(), value.begin() + pos);
            value.erase(value.begin(), value.begin() + pos + 4);
            string tablename = trim(string(value.begin(), value.end() - 1));
            pos = value.find("where");
            string condicional;
            vector<string> cond;
            if (pos != string::npos) {
                tablename = trim(string(value.begin(), value.begin() + pos));
                value.erase(value.begin(), value.begin() + pos + 5);
                string condicion = trim(string(value.begin(), value.end() - 1));

                
                if (condicion.find("=") != string::npos) {
                    condicional = "=";
                }
                if (condicion.find("<") != string::npos) {
                    condicional = "<";
                }
                if (condicion.find(">") != string::npos) {
                    condicional = ">";
                }

                cond = split(trim(string(value.begin(), value.end() - 1)), condicional);
            }

            
            Tabla* t;
            if (tableexist(tablename, t)) {
                Tabla tb = *t;
                vector<vector<string>>data = getall(tablename);

                if (pos != string::npos) {
                    int colnumcond = getIndexColum(tb.colums, trim(cond[0]));
                    vector<int> indexdata = getbycol(tb, data, colnumcond, trim(cond[1]), condicional);
                    vector<vector<string>> newData;
                    for (int i : indexdata) {
                        newData.push_back(data[i]);
                    }
                    data = newData;
                }

                if (trim(campos) == "*") {
                    for (Col c : tb.colums) {
                        if (c.tamanio != "0") {
                            string charval = fixCadsize(c.nombre, stoi(c.tamanio));
                            cout << charval << "\t";
                        }
                        else {
                            cout << c.nombre << "\t";
                        }
                    }
                    cout << endl;
                    for (vector<string>camp : data) {
                        for (string c : camp) cout << c << "\t";
                        cout << endl;
                    }

                }
                else {
                    vector<string> camps = split(campos, ",");
                    vector<int> indices;
                    for (string c : camps) {
                        indices.push_back(getIndexColum(tb.colums, trim(c)));
                    }

                    for (int c : indices) {
                        if (tb.colums[c].tamanio != "0") {
                            string charval = fixCadsize(tb.colums[c].nombre, stoi(tb.colums[c].tamanio));
                            cout << charval << "\t";
                        }
                        else {
                            cout << tb.colums[c].nombre << "\t";
                        }
                    }
                    
                    cout << endl;

                    for (vector<string>camp : data) {
                        for (int c : indices) cout << camp[c] << "\t";
                        cout << endl;
                    }
                }


            }
        }
    }
}