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

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#ifndef SERIALE_H_INCLUDED
#define SERIALE_H_INCLUDED

/** \brief Funzione per aprire la porta seriale
 *
 * \param port Numero di porta che deve essere compreso tra 1 e 9
 * \param mode Modalità di lettura/scrittura:
         GENERIC_READ per lettura,
         GENERIC_WRITE per scrittura, possono essere messe
         in OR logico per ottenere la modalità di lettura/scrittura
         (GENERIC_READ | GENERIC_WRITE)
 * \param speed La velocità in bit/secondo
 * \param parity Un carattere che indica
    <ul>
        <li>n : nessuna parità</li>
        <li>e : parità pari</li>
        <li>o : parità dispari</li>
    </ul>
 * \param bits Il numero di bit che compongono l'unità base di trasmissione
 * \param stop Il numero di bit di stop, tipicamente 0,1 o 2
 * \return L'handle alla risorsa, INVALID_HANDLE_VALUE se fallisce
 *
 */


HANDLE COM_open(int port, int mode, unsigned int speed, char parity,
                unsigned int bits,unsigned int stop);

/** \brief Funzione per leggere dalla porta seriale
 *
 * \param com L'handle alla porta ritornato da una COM_open eseguita
 * con successo
 * \param buf Il riferimento al buffer dove verranno scritti i caratteri
 * letti da seriale
 * \return
 *
 */

int COM_read(HANDLE com, char *buf, int n);
void COM_close(HANDLE com);

#endif // SERIALE_H_INCLUDED
