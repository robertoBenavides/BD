#pragma once

#include "StringAdapter.h"
using namespace std;
struct Fecha
{
	int anio=-1;
	int mes=-1;
	int dia=-1;

	Fecha(vector<string> fecha) {
		dia = stoi(fecha[0]);
		mes = stoi(fecha[1]);
		anio = stoi(fecha[2]);
	}

	bool operator <(Fecha a) {
		if (this->anio < a.anio) {
			return true;
		}
		if (this->anio == a.anio) {
			if (this->mes < a.mes ) {
				return true;
			}
			if (this->mes == a.mes) {
				return (this->dia < a.dia);
			}
			
		}

		return false;

	}
	bool operator >(Fecha a) {
		if (this->anio > a.anio) {
			return true;
		}
		if (this->anio == a.anio) {
			if (this->mes > a.mes) {
				return true;
			}
			if (this->mes == a.mes) {
				return (this->dia > a.dia);
			}

		}

		return false;

	}
	bool operator ==(Fecha a) {
		return (this->anio == a.anio && this->mes == a.mes && this->dia == a.dia);
	}
private:
	int dias[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
};

