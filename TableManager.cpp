#include "TableManager.h"

TableManager::TableManager()
{
    {
        ifstream myfile("Tablas.txt");
        string line;
        if (myfile.is_open())
        {
            while (getline(myfile, line))
            {
                size_t pos = line.find("[");
                vector<string> tbd = SA.split(string(line.begin(), line.begin() + pos - 1), "/");
                vector<string> cd = SA.split(string(line.begin() + pos + 1, line.end() - 1), ",");
                vector<Col> colums;
                for (string c : cd) {
                    vector<string> ds = SA.split(c, "/");
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
}

bool TableManager::tableexist(string tbname, Tabla*& a)
{
    for (Tabla& t : tablas)
        if (t.nombre == tbname) {
            a = &t;
            return true;
        }
    return false;
}

void TableManager::writeTable(Tabla tb)
{
    {
        ofstream f("Tablas.txt", ios::app);
        if (f.is_open()) {
            f << tb.toString();
            f << "\n";
            f.close();
        }
        else  cout << "Error de apertura de archivo." << endl;

    }
}

void TableManager::rewriteInfo(vector<string> finald, string tablename)
{
    {
        ofstream f(tablename + ".txt", ios::out);

        if (f.is_open()) {
            for (string a : finald) f << a;
            f.close();
        }
        else cout << "Error de apertura de archivo." << endl;
    }
}

void TableManager::createTable(string data)
{
    {
        string tablename = string(data.begin() + 1, data.begin() + data.find("("));
        Tabla* tb;
        if (!tableexist(tablename, tb)) {
            string args = string(data.begin() + data.find("(") + 1, data.begin() + data.find(")"));
            vector<string> campos = SA.split(SA.trim(args), ",");
            const int c = campos.size();
            vector<Col> columnas;

            for (string a : campos) {
                vector<string> val = SA.split(SA.trim(a));
                size_t pos = val[1].find("char");
                if (pos != string::npos) {
                    vector<string> tam = SA.split(SA.trim(val[1]), "[");
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
            print("tabla \"" + tablename + "\" creada",verde);
        }
        else {
            print("tabla \"" + tablename + "\" ya existe", rojo);
        }
    }
}

void TableManager::insertValue(string val)
{
    {
        size_t pos = (val.find(" ") > val.find("(")) ? val.find("(") : val.find(" ");

        string tablename =SA.trim( string(val.begin(), val.begin() + pos));
        Tabla* t;
        if (tableexist(tablename, t)) {
            val = string(val.begin() + val.find("(") + 1, val.begin() + val.find(")"));
            Tabla tb = *t;
            vector<string> valores = SA.split(val, ",");

            if (valores.size() != tb.numcols) {
                cout << "numero incorrecto de columnas" << endl;
            }
            else {
                string finalvalues;
                for (int i = 0; i < valores.size(); i++) {

                    if (tb.colums[i].tamanio != "0") {
                        finalvalues += SA.fixCadsize(SA.trim(valores[i]), stoi(tb.colums[i].tamanio));
                    }
                    else {
                        finalvalues += SA.trim(valores[i]);
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
                print("se inserto correctamente", verde);
               
            }
        }
        else {
            print("tabla \"" + tablename + "\" no existe", rojo);
        }
    }
}

void TableManager::deleteValue(string value)
{
    int pos = value.find(" ");
    if (string(value.begin(), value.begin() + pos) == "from") {
        value.erase(value.begin(), value.begin() + pos + 1);
        pos = value.find(" ");
        string tablename = string(value.begin(), value.begin() + pos);
        pos = value.find("where");
        value.erase(value.begin(), value.begin() + pos + 5);
        string condicion = SA.trim(string(value.begin(), value.end() - 1));


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

        vector<string> cond = SA.split(SA.trim(string(value.begin(), value.end() - 1)), condicional);

        Tabla* t;
        if (tableexist(tablename, t)) {
            Tabla tb = *t;
            vector<vector<string>> data = getall(tablename);
            int colnumcond = getIndexColum(tb.colums, SA.trim(cond[0]));
            vector<int> indexdata = getbycol(tb, data, colnumcond, SA.trim(cond[1]), condicional);

            if (indexdata.size()) {
                for (int i = indexdata.size() - 1; i >= 0; i--) {
                    data.erase(data.begin() + indexdata[i]);
                }

                vector<string>finald = SA.toFlatString(data);
                rewriteInfo(finald, tablename);
                print("se elimino correctamente",verde);
            }
            else {
                print("dato(s) no encontrado(s)",rojo);
            }
           
        }
        else {
            print( "la tabla \"" + tablename + "\" no existe",rojo);
        }


    }
}

void TableManager::select(string value, string campos)
{
    string tablename = SA.trim(string(value.begin(), value.end() - 1));
    int pos = value.find("where");
    string condicional;
    vector<string> cond;
    if (pos != string::npos) {
        tablename = SA.trim(string(value.begin(), value.begin() + pos));
        value.erase(value.begin(), value.begin() + pos + 5);
        string condicion = SA.trim(string(value.begin(), value.end() - 1));


        if (condicion.find("=") != string::npos) {
            condicional = "=";
        }
        if (condicion.find("<") != string::npos) {
            condicional = "<";
        }
        if (condicion.find(">") != string::npos) {
            condicional = ">";
        }

        cond = SA.split(SA.trim(string(value.begin(), value.end() - 1)), condicional);
    }


    Tabla* t;
    if (tableexist(tablename, t)) {
        Tabla tb = *t;
        vector<vector<string>>data = getall(tablename);

        if (pos != string::npos) {
            int colnumcond = getIndexColum(tb.colums, SA.trim(cond[0]));
            vector<int> indexdata = getbycol(tb, data, colnumcond, SA.trim(cond[1]), condicional);
            vector<vector<string>> newData;
            for (int i : indexdata) {
                newData.push_back(data[i]);
            }
            data = newData;
        }
        string charval;
        if (SA.trim(campos) == "*") {
            for (Col c : tb.colums) {
                if (c.tamanio != "0") {
                    charval += SA.fixCadsize(c.nombre, stoi(c.tamanio))+"\t";
                    
                }
                else {
                    charval+=c.nombre + "\t";
                }
            }
            print(charval, plomo);
            for (vector<string>camp : data) {
                for (string c : camp) cout << c << "\t";
                cout << endl;
            }

        }
        else {
            vector<string> camps = SA.split(campos, ",");
            vector<int> indices;
            for (string c : camps) {
                indices.push_back(getIndexColum(tb.colums, SA.trim(c)));
            }

            for (int c : indices) {
                if (tb.colums[c].tamanio != "0") {
                    charval += SA.fixCadsize(tb.colums[c].nombre, stoi(tb.colums[c].tamanio))+"\t";
                    
                }
                else {
                    charval+=tb.colums[c].nombre + "\t";
                }
            }
            print(charval + "\t", plomo);
            for (vector<string>camp : data) {
                for (int c : indices) cout << camp[c] << "\t";
                cout << endl;
            }
        }
    }
}

void TableManager::updateValue(string value)
{
    int pos = value.find(" ");
    string tablename = string(value.begin(), value.begin() + pos);
    value.erase(value.begin(), value.begin() + pos + 1);
    Tabla* t;
    if (tableexist(tablename, t)) {
        Tabla tb = *t;
        pos = value.find(" ");
        if (string(value.begin(), value.begin() + pos) == "set") {
            value.erase(0, 4);
            pos = value.find("where");
            string updateds = SA.trim(string(value.begin(), value.begin() + pos));
            value.erase(0, pos);

            vector<vector<string>> data = getall(tablename);


            string condicion = SA.trim(string(value.begin() + value.find("where") + 6, value.end() - 1));
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
            vector<string> cond = SA.split(condicion, condicional);

            int colnumcond = getIndexColum(tb.colums, SA.trim(cond[0]));
            vector<int> indexdata = getbycol(tb, data, colnumcond, SA.trim(cond[1]), condicional);

            vector<string> parametros = SA.split(updateds, ",");
            for (string p : parametros) {
                vector<string> datos = SA.split(p, "=");
                int columnumber = getIndexColum(tb.colums, SA.trim(datos[0]));
                for (int i = 0; i < indexdata.size(); i++) {
                    if (tb.colums[columnumber].tamanio != "0") {
                        string charval = SA.fixCadsize(datos[1], stoi(tb.colums[columnumber].tamanio));
                        data[indexdata[i]][columnumber] = charval;
                    }
                    else {
                        data[indexdata[i]][columnumber] = SA.trim(datos[1]);
                    }
                }

            }
            vector<string>finald = SA.toFlatString(data);


            rewriteInfo(finald, tablename);

            print("tabla \"" + tablename + "\" actualizada", verde);
        }
        else {
            print("la tabla \"" + tablename + "\"no se actualizo", rojo);
        }
    }
    else {
        print("tabla \"" + tablename + "\" no existe", rojo);
    }
}

vector<vector<string>> TableManager::getall(string tablename)
{
    fstream file;
    file.open(tablename + ".txt");
    string line;
    vector<vector<string>> data;
    while (std::getline(file, line)) {
        data.push_back(SA.split(line, ","));
    }
    file.close();
    return data;
}

int TableManager::getIndexColum(vector<Col> colums, string data)
{
    for (int i = 0; i < colums.size(); i++) {
        if (colums[i].nombre == SA.trim(data)) return i;
    }
    return -1;
}

vector<int> TableManager::getbycol(Tabla tb, vector<vector<string>> datos, int i, string val, string condicional)
{
    vector<int> coinciden;
    for (int j = 0; j < datos.size(); j++) {
        if (condicional == "=") {
            if (SA.trim(datos[j][i]) == val)  coinciden.push_back(j);
        }
        else if (condicional == "<") {
            string tipo = tb.colums[i].tipo;
            if (tipo == "int") {
                int cond1 = stoi(datos[j][i]);
                int cond2 = stoi(val);
                if (cond1 < cond2)  coinciden.push_back(j);
            }
            else {
                if (SA.trim(datos[j][i]) < val)  coinciden.push_back(j);
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
                if (SA.trim(datos[j][i]) > val)  coinciden.push_back(j);
            }
        }
    }
    return coinciden;
}

void TableManager::print(string msg, int color)
{
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    cout << msg << endl;
    SetConsoleTextAttribute(hConsole, 15);
}
