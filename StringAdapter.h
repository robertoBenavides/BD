#pragma once
#include<vector>
#include<string>
using namespace std;
struct StringAdapter
{
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

    string fixCadsize(string val, int tam) {       
        string finalstring;
        if (tam>val.size()) {
            int rellenar = tam - val.size();
            finalstring = val;
            string relleno = (rellenar < 1) ? "" : string(rellenar, ' ');
            finalstring += relleno;
        }
        else {
            finalstring = string(val.begin(), val.begin() + tam);
            
        }
        return finalstring;
    }
};

