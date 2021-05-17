#pragma once
#include <string>
using namespace std;
struct Col
{
    string nombre;
    string tipo;
    string tamanio;
    Col() {}
    Col(string n, string t,string tam="0") {
        nombre = n;
        tipo = t;
        tamanio=tam;
    }
    string toString() {
        string del = "/";
        if(tamanio=="0"){
          return nombre + del + tipo;
        }
        return nombre + del + tipo + del + tamanio;
    }
};
