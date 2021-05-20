#pragma once
#include <string>
#include <vector>
#include "col.h"
using namespace std;
class Tabla
{
public:
	string nombre;
	int numcols;
	vector<Col> colums;
	Tabla() {};

	Tabla(string n,int num,vector<Col> cs) {
		numcols = num;
		colums = cs;
		nombre = n;
	}

	string toString() {
		string sep = "/";
		string cadena = nombre + sep + to_string(numcols) + sep + "[";
		for (Col i :colums) cadena += i.toString()+",";
		cadena = string(cadena.begin(), cadena.end()-1);
		cadena+="]";
		return cadena;
	}
};

