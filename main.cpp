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
 *  \brief Esempio di utilizzo della porta seriale per la lettura
 *  Questa versione non è portabile ed è relativa unicamente al S.O. Windows.
 *  Riferimenti: Serial port programming in Windows and linux, Maxwell Walter, 2003
 *  \author Alessandro Bugatti
 *
 *  \version 0.1
 *  \date  Creazione  26/05/2010
 *  \date  Ultima modifica 28/05/2018
 *
 */

#include <stdio.h>
#include <stdlib.h>
#ifdef __WIN32__
    #include <windows.h>
#endif // __WIN32__
#include "seriale.h"

#define SIZE 1024

char dataIn[SIZE];

int main()
{
    int seriale;
    //Apertura della porta seriale, in questo esempio la COM3
    seriale = serial_open("COM3", SERIAL_READ, 9600, 'n', 8, 1);
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
    //Chiusura della seriale
    serial_close(seriale);
    return 0;

}
