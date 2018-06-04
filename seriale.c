#include "seriale.h"


HANDLE COM_open(int port, int mode, unsigned int speed, char parity,
                unsigned int bits, unsigned int stop)
{



    DCB dcb; //Struttura che contiene i settaggi
    char porta_seriale[] = "COM?";
    //Controllo se viene passato un numero di porta valido
    if (port < 1 || port > 9)
        return (void*)(-1);
    porta_seriale[3] = port + '0';
    HANDLE fileHandle; //Handle alla porta, un po' l'equivalente di FILE* per
                       //la gestione di file ordinari
    //Apertura della porta
    fileHandle = CreateFile(
         porta_seriale, //Nome della porta (COM1, COM2, COM3 ecc.
         mode,//Apertura il lettura / scrittura
            0, //sharing mode, se 0 la porta non può essere condivisa
            0, //attributi di sicurezza, con 0 l'handle non può essere ereditato
            OPEN_EXISTING, //la porta deve esistere per poter essere aperta, altrimenti non può crearla
            0, //Overlapped /non-overlapped mode, con 0 non -overlapped (non si può leggere e scrivere contemporaneamente)
            0 //template file, non definito per le seriali
        );
    //Controllo se l'apertura è fallita
    if (fileHandle == INVALID_HANDLE_VALUE)
        return (void*)(-1);
    //Settaggio dei parametri
    FillMemory(&dcb, sizeof(dcb), 0); //Inizializzo tutti i campi della struttura a zero
    dcb.DCBlength = sizeof(dcb); //Setto il campo lunghezza alla propria dimensione
    //Costruisco una stringa che è il parametro d'ingresso per settare i campi di un DCB
    //La stringa contiene i parametri speed, parity, data size, stop bits separati da virgola
    //speed è in BAUD
    //parity è n per nessuna parità, e(even) per pari, o(odd) per dispari
    //data size è il numero di bit (tipicamente 8)
    //stop bits è il numero di bit di stop, tipicamente 1 o 2
    //Per Arduino i valori di default sono 9600,n,8,1
    //Nel costruirlo controllo anche se ci riesco
    char settaggi[100];
    sprintf(settaggi,"%d,%c,%d,%d",speed,parity,bits,stop);
    if(!BuildCommDCB(settaggi, &dcb))
        return (void*)(-1);
    //Assegno i settaggi alla porta e controllo se ci sono riuscito
    if(!SetCommState(fileHandle, &dcb))
        return (void*)(-1);
    //Imposto la dimensione del buffer (tipicamente una potenza del 2)
    //in questo esempio 1024 e controllo la riuscita
    if(!SetupComm(fileHandle,
        1024,//coda di input
        1024)//coda di output
    )
        return (void*)(-1);
    //Imposto i timeouts
    COMMTIMEOUTS cmt;
    //Timeout in lettura tra la lettura di due caratteri consecutivi in ms
    //Se passa più di quel tempo la lettura è considerata conclusa e la
    //funzione ReadFile ritorna
    cmt.ReadIntervalTimeout = 100;
    //Valore totale del timeout per ogni operazione di lettura rispetto ai byte letti
    //(num byte da leggere) * (timeout) in ms
    //Il numero di byte dal leggere sono quelli specificati dalla ReadFile come
    //terzo parametro, se ad esempio si vogliono leggere 100 byte e il parametro
    //è impostato a 10 il timeout scatta dopo un secondo
    cmt.ReadTotalTimeoutMultiplier = 50;
    //Valore da aggiungere al precedente per ogni singola operazione di lettura
    cmt.ReadTotalTimeoutConstant = 0;
    //lo stesso per la scrittura
    cmt.WriteTotalTimeoutConstant = 1000;
    cmt.WriteTotalTimeoutMultiplier = 1000;
    //Assegno i valori alla porta e controllo la riuscita dell'operazione
    if(!SetCommTimeouts(fileHandle, &cmt))
        return (void*)(-1);
    return fileHandle;

}



int COM_read(HANDLE com, char *buf, int n)
{
    DWORD i;
    ReadFile(
        com,//l'handle al file
        buf, //puntatore al buffer dei dati
        n, //dimensione dei dati che voglio leggere con una singola operazione
        &i, //la variabile dove finiranno il numero di dati realmente letti
        NULL //serve solo per overlapped I/O, nel nostro caso NULL
        );
    if (i == 0) return -1;
    return i;
}



void COM_close(HANDLE com)
{
    CloseHandle(com);
}

