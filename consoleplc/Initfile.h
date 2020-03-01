#ifndef CCINITFILE_H
#define CCINITFILE_H
#include <QString>
#include <QList>

#define SUDO "sudo"
#define PASSWORD "flicgatto"
#define SET_HOST "-o"
#define SET_COMPLETO "-c"
#define SET_IMPIANTO "-i"
#define SET_UTENTE_LOCALE "-u"
#define SET_PASS_LOCALE "-p"
#define SET_PORTA "-d"
#define SET_HOME "-h"
#define SET_LICENZA "-l"

#define SET_COM "-c"
#define SET_COM1 "-c1"
#define SET_COM2 "-c2"
#define SET_COM3 "-c3"
#define SET_COM4 "-c4"
#define RESET_LICENZA "-rl"
#define SET_SIMULA "simula"
#define SET_DEBUG "debug"
#define SET_DOG "dog"
#define SET_SEGNALE "segnale"
#define SET_MANUALE "manuale"
#define SET_PORTA_APERTA "porta"

#define TEST "test"
#define TEST_POWER "power"


class CArg
{
public:
    QString  Comando;
    QString  Descrizione;
    QString  daSalvare;
};


class CCInitFile
{
public:
    CCInitFile();
    CCInitFile(QString newTitolo,QString newNomeFile);
    void     appaned(QString newComando,QString newDescrizione);
    void     mostra();
    QString  leggiInput();
    bool     getFile();
    void     setFile();
    QString  at(QString comando);
    bool     isSudo(int argc, char *argv[]);
    bool     isSimula(int argc, char *argv[]);
    int      isMacchina(int argc, char *argv[]);
    bool     isDebug(int argc, char *argv[]);
    bool     isManuale(int argc, char *argv[]);
    bool     isPorta(int argc, char *argv[]);
    bool     isDog(int argc, char *argv[]);
    bool     isSetSegnale(int argc, char *argv[],int *slot, int *point, int *valore);

private:
    QList<CArg *> Arg;
    QString Titolo;
    QString NomeFile;
};

#endif // CCINITFILE_H
