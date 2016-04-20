//============================================================================
// ----------- PRÁCTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES -----------
// ---------------------------- CURSO 2015/16 --------------------------------
// ----------------------------- SESION1.CPP ---------------------------------
//============================================================================

#include "stdio.h"
#include "conio.h"
#include "windows.h"
#include <iostream>
#include "PuertoSerie.h"
#include "CosasRedes.h"

using namespace std;

HANDLE PuertoCOM;

int main() {

// Parámetros necesarios al llamar a AbrirPuerto:
// - Nombre del puerto a abrir ("COM1", "COM2", "COM3", ...).
// - Velocidad (1200, 1400, 4800, 9600, 19200, 38400, 57600, 115200).
// - Número de bits en cada byte enviado o recibido (4, 5, 6, 7, 8).
// - Paridad (0=sin paridad, 1=impar, 2=par, 3=marca, 4=espacio).
// - Bits de stop (0=1 bit, 1=1.5 bits, 2=2 bits)

	CosasRedes *principal = new CosasRedes();
	if (principal->ejecutar(PuertoCOM) == false)
		return 1;

	return 0;
}
