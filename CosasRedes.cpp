/*
 /*
 * CosasRedes.cpp
 *
 *  Created on: 24 de feb. de 2016
 *      Author: Usuario
 */

#include "CosasRedes.h"

CosasRedes::CosasRedes() {
	carRec = 0;
	car = 0;
	cuantos = 0;
}

bool CosasRedes::ejecutar(HANDLE PuertoCOM) {
	PuertoCOM = AbrirPuerto("COM1", 9600, 8, 0, 0); //Abrimos el puerto COM1 (en la sala siempre abrimos el COM1)
	if (PuertoCOM == NULL) {
		printf("Error al abrir el puerto\n");
		getch();
		return false;
	}
	bool salir = false;
	while (salir == false) {
		recibirAlgo(PuertoCOM);
		if (kbhit())
			salir = enviarAlgo(PuertoCOM);
	}
	CerrarPuerto(PuertoCOM);
	return true;
}

bool CosasRedes::enviarAlgo(HANDLE PuertoCOM) {
	car = getch();
	int queTrama = 0;
	if (cuantos < 600 || car == '\0' || car == 8 || car == 27) { //Comprobacion de tamaño max
		switch (car) { //4
		case 27:
			return true;
			break;
		case '\0':
			car = getch();
			if (car == 59) {
				TramaDatos *datosEnv = new TramaDatos();
				for (int i = 0; i < cuantos; i++) {
					if (i % 254 == 0) //Comienzo nueva trama
							{
						delete datosEnv;
						datosEnv = new TramaDatos();
					}
					datosEnv->setDato(cadenaEnviar[i]);
					if (i % 254 == 253 || i == cuantos - 1)	//Fin trama o fin cadena
							{
						datosEnv->setDato('\0');
						datosEnv->enviarTrama(PuertoCOM);
					}
				}
				cuantos = 0;
				cout << endl;
			}
			if (car == 60) {

				cout << "Introduzca el tipo de trama de control a enviar:"
						<< endl;
				cout << "1. Trama ENQ" << endl;
				cout << "2. Trama EOT" << endl;
				cout << "3. Trama ACK" << endl;
				cout << "4. Trama NACK" << endl;

				TramaControl *tramaEnv;
				do {
					cin >> queTrama;

					switch (queTrama) {

					case 1:
						tramaEnv = new TramaControl(5);
						cout << "Se ha enviado una trama ENQ" << endl;
						break;
					case 2:
						tramaEnv = new TramaControl(4);
						cout << "Se ha enviado una trama EOT" << endl;
						break;
					case 3:
						tramaEnv = new TramaControl(6);
						cout << "Se ha enviado una trama ACK" << endl;
						break;
					case 4:
						tramaEnv = new TramaControl(21);
						cout << "Se ha enviado una trama NACK" << endl;
						break;
					default:
						cout
								<< "Valor no válido. Por favor, introduzca un valor entre 1 y 4."
								<< endl;
						break;
					}
				} while (queTrama > 4 || queTrama < 1);

				tramaEnv->enviarTrama(PuertoCOM);
			}
			if(car==61){
				EnviarFichero(PuertoCOM);
			}
			break;
		case 13:
			cout << endl;
			cadenaEnviar[cuantos] = '\n';
			cuantos++;
			break;
		case 8:
			cout << "\b \b";
			cuantos--;
			cadenaEnviar[cuantos] = 0;
			break;
		default:
			// COMPROBACION DE SOLO ALFANUMERICOS Y ESPACIOS
			if (car >= 32 && car <= 126) {	     //6
				cout << car;
				cadenaEnviar[cuantos] = car;
				cuantos++;
			}
			break;

		}
	}
	return false;
}
void CosasRedes::EnviarFichero(HANDLE PuertoCOM) {
	//TODO : enviar inicio de fichero
	ifstream lectura;
	lectura.open("frc.txt");
	char lect[1];
	int contador = 0;
	TramaDatos *tramaLec = new TramaDatos();
	if (lectura.is_open() == true) {
		while (lectura.eof() == false) {

			lectura.read(lect, 1);
			lectura.seekg(lectura.tellg() + 1);
			tramaLec->setDato(lect[0]);
			contador++;
			if (contador == 254 || lectura.eof()) {
				tramaLec->enviarTrama(PuertoCOM);
				delete tramaLec;
				tramaLec = new TramaDatos();
				contador = 0;
			}
		}
		//TODO ENVIAR FINAL DE FICHERO
		lectura.close();
	}

}
void CosasRedes::RecibirFichero(HANDLE PuertoCOM) {
	ofstream FichSal;
	char cadRec[254];
	unsigned char longitud;
	unsigned char BCE;
	unsigned char BCEr;
	FichSal.open("frcR.txt");
	if (FichSal.is_open()) {
		while (1) //FIN DE FICHERO, TODO
		{
			longitud = RecibirCaracter(PuertoCOM);
			BCE = RecibirCaracter(PuertoCOM);
			RecibirCadena(PuertoCOM, cadRec, longitud);
			BCEr = cadRec[0];
			for (int i = 1; i < longitud; i++) {
				BCEr = BCEr ^ cadRec[i];
			}
			if (BCE == BCEr) {
				FichSal.write(cadRec, 254);
			}
		}
	}

}

void CosasRedes::recibirAlgo(HANDLE PuertoCOM) {
	unsigned char longitud;
	//TODO RECIBIR INICIO DE FICHERO
	carRec = RecibirCaracter(PuertoCOM);
	unsigned char BCE;
	unsigned char BCEt; //Para control
	if (carRec == 22) //ESTAMOS RECIBIENDO TRAMA
			{
		unsigned char tipo;
		//TODO Hacer cosas con estos caracteres, en esta pract solo imprimir, se puede almacenar
		//Con los while nos aseguramos de recibir los datos pertinentes, ya que las tramas se envían seguidas, el ultimo falla
		carRec = 0;
		while (carRec == 0) {
			carRec = RecibirCaracter(PuertoCOM); //Direccion
		}
		if (carRec != 'T')
			cout << "ERROR en Dir";
		carRec = 0;

		while (carRec == 0) {
			carRec = RecibirCaracter(PuertoCOM);
		}
		tipo = carRec; //Control
		carRec = 0;

		while (carRec == 0) {
			carRec = RecibirCaracter(PuertoCOM);
		}
		if (carRec != '0')
			cout << "Error en Num";
		carRec = 0;

		switch (tipo) {
		case 2:
			while (carRec == 0) {
				carRec = RecibirCaracter(PuertoCOM);
			}
			BCE = carRec;
			carRec = 0;
			while (carRec == 0) {
				carRec = RecibirCaracter(PuertoCOM);
			}
			longitud = carRec;
			char Datos[255];
			RecibirCadena(PuertoCOM, Datos, (int) longitud);
			BCEt = Datos[0];
			for (int i = 1; i < longitud; i++) {
				BCEt = BCEt ^ Datos[i];

			}

			if (BCE == BCEt) {
				cout << "Se ha recibido una trama de datos. Su contenido es:"
						<< endl;
				for (int i = 0; i < longitud; i++) {
					cout << Datos[i];
				}
			} else
				cout << "La trama no es correcta" << endl;
			break;
		case 4:
			cout << "Se ha recibido una trama EOT" << endl;
			break;
		case 5:
			cout << "Se ha recibido una trama ENQ" << endl;
			break;
		case 6:
			cout << "Se ha recibido una trama ACK" << endl;
			break;
		case 21:
			cout << "Se ha recibido una trama NACK" << endl;
			break;
		default:
			cout << "Error al recibir tipo de trama" << endl;
			break;
		}
	} else if (carRec != 0)
		cout << carRec;
}

CosasRedes::~CosasRedes() {

}
