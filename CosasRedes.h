/*
 * CosasRedes.h
 *
 *  Created on: 24 de feb. de 2016
 *      Author: Usuario
 */

#ifndef COSASREDES_H_
#define COSASREDES_H_
#include "PuertoSerie.h"
#include "windows.h"
#include "conio.h"
#include <iostream>
#include "stdio.h"
#include "Trama.h"
#include "TramaDatos.h"
#include "TramaControl.h"
#include <fstream>

const int MAXCARACTERES = 602;

class CosasRedes {
private:
	char car;
	char carRec;
	char cadenaEnviar[MAXCARACTERES];
	int cuantos;
public:
	CosasRedes();
	//Ejecuta el programa, si devuelve false hay error
	bool ejecutar(HANDLE PuertoCOM);

	//Si es true, hay que salir
	bool enviarAlgo(HANDLE PuertoCOM);
	void EnviarFichero(HANDLE PuertoCOM);
	void RecibirFichero (HANDLE PuertoCOM);
	void recibirAlgo(HANDLE PuertoCOM);

	~CosasRedes();
};

#endif /* COSASREDES_H_ */