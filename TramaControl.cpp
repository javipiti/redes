/*
 * TramaControl.cpp
 *
 *  Created on: 24 de feb. de 2016
 *      Author: Usuario
 */

#include "TramaControl.h"

TramaControl::TramaControl() :
		Trama() {
	Cont = 0;
}

TramaControl::TramaControl(int Control) :
		Trama() {
	Cont = Control;
}

TramaControl::~TramaControl() {
}

void TramaControl::enviarTrama(HANDLE PuertoCOM) {
	EnviarCaracter(PuertoCOM, Sinc);
	EnviarCaracter(PuertoCOM, Dir);
	EnviarCaracter(PuertoCOM, Cont);
	EnviarCaracter(PuertoCOM, Num);

}

