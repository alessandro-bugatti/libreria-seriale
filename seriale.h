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
 *  \brief Semplice libreria per l'utilizzo della porta seriale
 *  *  Riferimenti: Serial port programming in Windows and linux, Maxwell Walter, 2003
 *  \author Alessandro Bugatti
 *
 *  \version 0.1
 *  \date  Creazione  26/05/2010
 *  \date  Ultima modifica 28/05/2018
 *  \attention Questa versione attualmente non è portabile
 *  ed è relativa unicamente al S.O. Windows.
 *  \todo La parte relativa alla scrittura su seriale
 */

#include <cstdio>
#include <cstdlib>
#ifdef __WIN32__
#include <windows.h>
#endif // __WIN32__
#ifdef __linux__
#include <termios.h>
#include <unistd.h>
#endif // __linux__
#include <string>

using namespace std;

#ifndef SERIALE_H_INCLUDED
#define SERIALE_H_INCLUDED

#ifdef __WIN32__

const int SERIAL_READ = GENERIC_READ;
const int SERIAL_WRITE = GENERIC_WRITE;

#endif // __WIN32__

#ifdef __linux__

#include <sys/types.h>
#include <fcntl.h>

const int SERIAL_READ = O_RDONLY;
const int SERIAL_WRITE = O_RDWR;


#endif // __linux__

const int INVALID_VALUE = -1;

/** \brief Funzione per aprire la porta seriale
 *
 * \param port Il nome della porta, potrebbe essere COM3 in Windows e /dev/ttyACM0 in linux
 * \param mode Modalità di lettura/scrittura:
         SERIAL_READ per lettura,
         SERIAL_WRITE per scrittura, possono essere messe
         in OR logico per ottenere la modalità di lettura/scrittura
         (SERIAL_READ | SERIAL_WRITE)
 * \param speed La velocità in bit/secondo
 * \param parity Un carattere che indica
    <ul>
        <li>n : nessuna parità</li>
        <li>e : parità pari</li>
        <li>o : parità dispari</li>
    </ul>
 * \param bits Il numero di bit che compongono l'unità base di trasmissione
 * \param stop Il numero di bit di stop, tipicamente 0,1 o 2
 * \return L'handle alla risorsa, INVALID_VALUE se fallisce
 *
 */


int serial_open(string port, int mode, unsigned int speed, char parity,
                unsigned int bits,unsigned int stop);

/** \brief Funzione per leggere dalla porta seriale
 *
 * \param com L'handle alla porta ritornato da una serial_open eseguita
 * con successo
 * \param buf Il riferimento al buffer dove verranno scritti i caratteri
 * letti da seriale
 * \param n Numero di byte che verranno letti al massimo dalla seriale,
 * in generale è la dimensione del buffer
 * \return
 *
 */

int serial_read(int serial_handle, char *buf, int n);

/** \brief Funzione per scrivere sulla porta seriale
 *
 * \param com L'handle alla porta ritornato da una serial_open eseguita
 * con successo
 * \param buf Il riferimento al buffer dove verranno letti i caratteri
 * da scrivere su seriale
 * \param n Il numero di byte da leggere dal buffer e che andranno scritti sulla seriale
 * \return
 *
 */

int serial_write(int serial_handle, char *buf, int n);


void serial_close(int serial_handle);

#endif // SERIALE_H_INCLUDED
