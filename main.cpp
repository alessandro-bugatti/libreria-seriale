/*
  Copyright (C) 2010 Alessandro Bugatti (alessandro.bugatti@istruzione.it)

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

/*! \file
 *  \brief Esempio di utilizzo della porta seriale per la lettura/scrittura
 *  Riferimenti: Serial port programming in Windows and linux, Maxwell Walter, 2003
 *  \author Alessandro Bugatti
 *
 *  \version 0.1
 *  \date  Creazione  26/05/2010
 *  \date  Ultima modifica 28/05/2018
 *
 */

#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;
#ifdef __WIN32__
    #include <windows.h>
    const string nome_seriale = "COM3";
#endif // __WIN32__

#ifdef __linux__
    const string nome_seriale = "/dev/ttyACM0";
#endif // __linux__
#include "seriale.h"

#define SIZE 1024

char dataIn[SIZE];
char dataOut[SIZE];

int main()
{
    int choice;
    int seriale;
    cout << "Questo è un programma di test per l'utilizzo della seriale con Arduino." << endl
        << "Premi 1 e invio per avviare la parte che fa solo lettura dei dati da Arduino" << endl
        << "Arduino ---> PC" << endl
        << "Premi 2 e invio per avviare la parte che fa lettura/scrittura dei dati da Arduino" << endl
        << "Arduino <--> PC" << endl;
    cin >> choice;
    if (choice == 1)
    {
        //Apertura della porta seriale, in questo esempio la COM3
        seriale = serial_open(nome_seriale, SERIAL_READ, 9600, 'n', 8, 1);
        //Controllo dell'apertura
        if (seriale == INVALID_VALUE)
        {
            printf("Apertura della seriale fallita\n");
            return 1;
        }
        int read, i=0;
        //10 letture, solo per verificare il funzionamento
        while(i < 10)
        {
            read = serial_read(seriale,dataIn,100);
            //La lettura è andata a buon fine
            if (read != -1)
            {
                dataIn[read]='\0';
                printf("%d byte letti\n%s \n",read,dataIn);
                i++;
            }
            //Non c'era niente da leggere
            else
                printf("Timeout\n");
        }
    }
    else if (choice == 2)
    {
        //Apertura della porta seriale, in lettura/scrittura
        seriale = serial_open(nome_seriale, SERIAL_READ | SERIAL_WRITE, 9600, 'n', 8, 1);
        //Controllo dell'apertura
        if (seriale == INVALID_VALUE)
        {
            printf("Apertura della seriale fallita\n");
            return 1;
        }
        int read, write, i=0;
        dataOut[0] = 'A';
        //10 letture, solo per verificare il funzionamento
        while(i < 10)
        {
            //Scrive un carattere
            write = serial_write(seriale, dataOut,1);
            //Il programma su Arduino invia dei dati quando riceve un carattere
            read = serial_read(seriale,dataIn,100);
            //La lettura è andata a buon fine
            if (read != -1)
            {
                dataIn[read]='\0';
                printf("%d byte letti\n%s \n",read,dataIn);
                i++;
            }
            //Non c'era niente da leggere
            else
                printf("Timeout\n");
            dataOut[0]++;
        }
    }
    //Chiusura della seriale
    serial_close(seriale);
    return 0;

}
