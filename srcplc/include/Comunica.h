#ifndef COMUNICA_H
#define COMUNICA_H

#include "Ciclo.h"

class CComunica : public CCiclo
{
public:
    CComunica(int newtipoapp, int newTipoImpianto);
    int     Comunica(char *Buffer, int len);
    void    Comunica_Main();
};

#endif // COMUNICA_H
