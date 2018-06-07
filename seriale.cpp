#include "seriale.h"


int serial_open(string porta_seriale, int mode, unsigned int speed, char parity,
                unsigned int bits, unsigned int stop)
{


#ifdef __WIN32__
{
    DCB dcb; //Struttura che contiene i settaggi
    HANDLE fileHandle; //Handle alla porta, un po' l'equivalente di FILE* per
                       //la gestione di file ordinari
    //Apertura della porta seriale
    fileHandle = CreateFile(
         porta_seriale.c_str(), //Nome della porta (COM1, COM2, COM3 ecc.
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
    //Imposto i timeouts a valori standard
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
#endif // __WIN32__

#ifdef __linux__
{


struct termios options; //Struttura che contiene i settaggi
int fd; //Handle alla porta, un po' l'equivalente di FILE* per
        //la gestione di file ordinari

        //Apertura della porta seriale
fd = open(porta_seriale.c_str(),
            mode);

if (fd == -1)
    return INVALID_VALUE;

//Carica in options gli attributi correnti della porta seriale
tcgetattr(fd, &options);

//Viene settata la velocità di input e output
cfsetispeed(&options, speed);
cfsetospeed(&options, speed);

//Setta il tipo d parità
if (parity == 'n') //disabilità il bit di parità
    options.c_cflag &= ~PARENB;
else if (parity == 'e') //abilità la parità pari
    {
        options.c_cflag |= PARENB;
        options.c_cflag |= ~PARODD;
    }
else        //abilità la parità dispari
    {
        options.c_cflag |= PARENB;
        options.c_cflag |= PARODD;
    }

//Setta il numero di bit di parola
if (bits == 8){
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
}
else if (bits == 7){
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS7;
}
else if (bits == 6){
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS6;
}
else if (bits == 5){
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS5;
}

//Setta il numero di bit di stop
//Attualmente se il numero non è nè 1
//nè 2 il numero di bit viene comunque messo a 1
if (stop == 1)
    options.c_cflag &= ~CSTOPB;
else if (stop == 2)
    options.c_cflag &= CSTOPB;
else
    options.c_cflag &= ~CSTOPB;
//Setta il numero minimo di caratteri che
//possono essere letti in un colpo solo
//attendendo al massimo un certo intervallo di
//tempo (con i settaggi riportati legge 255 caratteri
//aspettanto al massimo un decimo di secondo)
options.c_cc[VMIN] = 255;
options.c_cc[VTIME] = 1;

//Imposta gli attributi nella seriale
if(tcsetattr(fd, TCSANOW, &options)!= 0) {
    return INVALID_VALUE;
}

return fd;
#endif // __linux__

}

}



int serial_read(int serial, char *buf, int n)
{
#ifdef __WIN32__
    DWORD i;
    ReadFile(
        (void *)serial,//l'handle al file
        buf, //puntatore al buffer dei dati
        n, //dimensione dei dati che voglio leggere con una singola operazione
        &i, //la variabile dove finiranno il numero di dati realmente letti
        NULL //serve solo per overlapped I/O, nel nostro caso NULL
        );
    if (i == 0) return -1;
    return i;
#endif // __WIN32__

#ifdef __linux__
    int letti = read(serial, buf, n);
    if (letti == 0)
        return -1;
    return letti;
#endif // __linux__
}



void serial_close(int serial)
{
#ifdef __WIN32__
    CloseHandle((void *)serial);
#endif // __WIN32__
#ifdef __linux__
    close(serial);
#endif // __linux__
}

