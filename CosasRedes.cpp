/*
 *
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
	modoFichero = false;
}

bool CosasRedes::ejecutar(HANDLE PuertoCOM) {
	//Se abre el puerto y se comprueba si se ha abierto correctamente.
	PuertoCOM = AbrirPuerto("COM1", 9600, 8, 0, 0);
	if (PuertoCOM == NULL) {
		printf("Error al abrir el puerto\n");
		getch();
		return false;
	}
	bool salir = false;

	//Se ejecuta hasta que se recibe la se�al de salida (pulsaci�n de la tecla ESC)
	while (salir == false) {
		RecibirAlgo(PuertoCOM);
		if (kbhit())
			salir = EnviarAlgo(PuertoCOM);
	}
	CerrarPuerto(PuertoCOM);
	return true;
}

bool CosasRedes::EnviarAlgo(HANDLE PuertoCOM) {
	car = getch();
	bool salir = false;
	int queTrama = 0;
	//Se impide escribir m�s si ya se han escrito 600 caracteres
	if (cuantos < 600 || car == '\0' || car == 8 || car == 27) { //Comprobacion de tamaño max

		switch (car) {
		case 27:
			salir = true;
			break;
			//Teclas de funci�n
		case '\0':
			//Env�o del mensaje escrito por pantalla
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
						RecibirAlgo(PuertoCOM);
					}
				}
				cuantos = 0;
				cout << endl;
			}
			//Env�o de una trama de control
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
								<< "Valor no v�lido. Por favor, introduzca un valor entre 1 y 4."
								<< endl;
						break;
					}
				} while (queTrama > 4 || queTrama < 1);

				tramaEnv->enviarTrama(PuertoCOM);
			}
			//Env�o de un fichero
			if (car == 61) {
				salir = EnviarFichero(PuertoCOM);
				if (salir == true)
					modoFichero = false;
			}
			break;
			//Salto de l�nea
		case 13:
			cout << endl;
			cadenaEnviar[cuantos] = '\n';
			cuantos++;
			break;
			//Retroceso
		case 8:
			cout << "\b \b";
			cuantos--;
			cadenaEnviar[cuantos] = 0;
			break;
			//Resto de caracteres v�lidos
		default:
			if (car >= 32 && car <= 126) {	     //6
				cout << car;
				cadenaEnviar[cuantos] = car;
				cuantos++;
			}
			break;

		}
	}
	return salir;
}
bool CosasRedes::EnviarFichero(HANDLE PuertoCOM) {
	ifstream lectura;
	lectura.open("frc.txt");
	char lect[255];
	int contador = 0;
	bool fin = false; //Para permitir salida del programa
	TramaDatos *tramaLec = new TramaDatos();
	//Al abrir el fichero enviamos el caracter suprimir para indicarlo
	EnviarCaracter(PuertoCOM, 127);
	if (lectura.is_open() == true) {
		while (lectura.eof() == false && fin == false) {
			//Si se pulsa ESC mientras se est� enviando se cancela el env�o
			if (kbhit() && getch() == 27) {
				cout << "Env�o cancelado";
				fin = true;
			}
			RecibirAlgo(PuertoCOM);
			//Si tenemos que salir dejamos de leer inmediatamente
			if (fin == false) {
				//Intentamos leer 254 caracteres
				lectura.read(lect, 254);
				contador = lectura.gcount();
				//Si no se ha le�do ninguno hemos terminado de leer
				if (contador != 0) {
					//Guardamos los caracteres en la trama y la enviamos
					for (int i = 0; i < contador; i++) {
						tramaLec->setDato(lect[0]);
					}
					lect[contador] = '\0';
					tramaLec->enviarTrama(PuertoCOM);
					delete tramaLec;
					tramaLec = new TramaDatos();
					contador = 0;
				} else
					delete tramaLec;
			} else
				delete tramaLec;
		}
	}
	//Cuando el fichero acaba enviamos el caracter retroceso
	if (fin == false)
		EnviarCaracter(PuertoCOM, 8);
	lectura.close();
	return fin;
}

void CosasRedes::RecibirAlgo(HANDLE PuertoCOM) {
	unsigned char longitud;
	carRec = RecibirCaracter(PuertoCOM);
	unsigned char BCE;
	unsigned char BCEt; //Para control
	if (carRec == 22) //ESTAMOS RECIBIENDO TRAMA
			{
		unsigned char tipo;
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
				if (modoFichero == false) {
					cout
							<< "Se ha recibido una trama de datos. Su contenido es:"
							<< endl;
					for (int i = 0; i < longitud; i++) {
						cout << Datos[i];
					}
				} else
					ficheroSal.write(Datos, longitud);
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
	} else if (carRec == 8) {
		cout << "Fichero recibido." << endl;
		modoFichero = false;
		ficheroSal.close();
	} else if (carRec == 127) {
		cout << "Recibiendo fichero..." << endl;
		modoFichero = true;
		ficheroSal.open("frcR.txt");
	} else
		cout << carRec;
}

CosasRedes::~CosasRedes() {

}
