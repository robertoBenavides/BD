#pragma once
#include <string>
using namespace std;
struct Col
{
    string nombre;
    string tipo;
    Col() {}
    Col(string n, string t) {
        nombre = n;
        tipo = t;
    }
    string toString() {
        string del = "/";
        return nombre + del + tipo;
    }
};

