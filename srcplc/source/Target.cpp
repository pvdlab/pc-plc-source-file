#include "Target.h"
#include "MainCostanti.h"
#include <sys/stat.h>

CTargetLista::CTargetLista(int newTargetCount)
{

    m_TargetCount=newTargetCount;

    pTarget = new CTarget[m_TargetCount];
    for(int i=0; i< m_TargetCount;i++){
        pTarget[i].Cambia=false;
        pTarget[i].TipoTarget=0;
        pTarget[i].OnTime=0;
    }


}

CTargetLista::~CTargetLista()
{
    delete[] pTarget;

}


CScorta::CScorta(int newTargetCount){

    m_TargetCount=newTargetCount;
    m_Index=0;

}

bool CScorta::Cerca(BYTE **pStr){

    BYTE *pBuffer=*pStr;

    CTargetLista *pTargetListaCerca = new CTargetLista(m_TargetCount);
    for(int i=0; i<m_TargetCount;i++){
        pTargetListaCerca->pTarget[i].TipoTarget=*pBuffer++;
    }

    *pStr=pBuffer;


    for(int i=0;i<(int)pScorta.size();i++){
        bool trovato=true;
        CTargetLista *pTargetLista=pScorta[i];
        for(int j=0; j<m_TargetCount; j++){
            if(pTargetListaCerca->pTarget[j].TipoTarget!=0){
                if( pTargetListaCerca->pTarget[j].TipoTarget==pTargetLista->pTarget[j].TipoTarget)
                   trovato=__min(trovato,true);
                else
                   trovato=__min(trovato,false);
            }
        }
        if(trovato){
            m_Index=i;
            return true;
        }
    }

    for(int i=0;i< (int)pScorta.size();i++){
        CTargetLista *pTargetLista=pScorta[i];
        for(int j=0; j<m_TargetCount; j++){
            if(pTargetListaCerca->pTarget[j].TipoTarget!=0){
                if( pTargetListaCerca->pTarget[j].TipoTarget==pTargetLista->pTarget[j].TipoTarget){
                   pTargetListaCerca->pTarget[j].OnTime=pTargetLista->pTarget[j].OnTime;
                }
            }
        }

    }
    pScorta.push_back(pTargetListaCerca);
    m_Index=pScorta.size()-1;

    return true;
}

bool CScorta::Apri(){

    std::string nomeFile=getRunPath();
    nomeFile.append(PLC_TARGET_PATH);
    nomeFile.append(PLC_TARGET);

    struct stat buf;
    stat(nomeFile.data(), &buf);
    int RecordCount=buf.st_size;
    RecordCount/=m_TargetCount*sizeof(CTarget);

    FILE* fd = fopen(nomeFile.data(), "r");
    if (fd){
        for(int i=0; i< RecordCount; i++){
            CTargetLista *pTargetLista = new CTargetLista(m_TargetCount);
            for(int j=0;j<m_TargetCount; j++){
                fread(&pTargetLista->pTarget[j],sizeof(CTarget),1, fd) ;
            }
            pScorta.push_back(pTargetLista);
        }
        fclose(fd);
        return true;
    }

    return false;
}


int CScorta::TipoTraget(int index){

    if(pScorta.size()==0)
        return 0;

    return pScorta[m_Index]->pTarget[index].TipoTarget;

}

void CScorta::Print(){

    std::string str;
    str.append("OnTime ");
    for(int i=0; i< m_TargetCount;i++){
        str.append(" [");
        str.append(intToString(i));
        str.append("] ");
        str.append(doubleToString(pScorta[m_Index]->pTarget[i].OnTime));
        str.append(" ");
    }
    MostraEventoProcesso(str,Color::FG_GREEN);

}

bool CScorta::Salva(){

    std::string nomeFile=getRunPath();
    nomeFile.append(PLC_TARGET_PATH);
    nomeFile.append(PLC_TARGET);

    CTargetLista *pTargetListaAttuale=pScorta[m_Index];
    for(int i=0;i< (int)pScorta.size();i++){
        if(i!=m_Index){
            CTargetLista *pTargetLista=pScorta[i];
            for(int j=0; j<m_TargetCount; j++){
                if(pTargetListaAttuale->pTarget[j].TipoTarget!=0){
                    if( pTargetListaAttuale->pTarget[j].TipoTarget==pTargetLista->pTarget[j].TipoTarget){
                        pTargetLista->pTarget[j].OnTime=pTargetListaAttuale->pTarget[j].OnTime;
                    }

                }

            }
        }
    }

    FILE* fd = fopen(nomeFile.data(), "w");
    if (fd){
        for(int i=0; i< (int)pScorta.size(); i++){
            for(int j=0;j<m_TargetCount; j++){
                fwrite(&pScorta[i]->pTarget[j],sizeof(CTarget),1, fd) ;
            }
        }
        fclose(fd);
        return true;
    }

    return false;
}


double CScorta::OnTime(int index,double onTime){

    if(pScorta.size()==0)
        return 0;


    pScorta[m_Index]->pTarget[index].OnTime=onTime;
    return pScorta[m_Index]->pTarget[index].OnTime;

}

double CScorta::OnTime(int index){

    if(pScorta.size()==0)
        return 0;

    return pScorta[m_Index]->pTarget[index].OnTime;

}

CTarget *CScorta::Target(int index){

    if(pScorta.size()!=0)
       return &pScorta[m_Index]->pTarget[index];

}


int CScorta::Cambia(char *Buffer){

    char  *pBuffer=Buffer;
    pBuffer++;
    int idMacchina=StrByte(&pBuffer);
    pBuffer+=3;

    CTargetLista *pTargetListaAttuale=pScorta[m_Index];
    for(int i=0; i< m_TargetCount;i++){
        BYTE cambia=StrByte(&pBuffer);
        if(cambia==1)
           pTargetListaAttuale->pTarget[i].OnTime=0;;
    }


    Salva();

    int len=0;
    pBuffer=Buffer;
    pBuffer=ByteStr(pBuffer,CTipoMessaggio::CambiaTarget,&len);
    pBuffer=ByteStr(pBuffer,idMacchina,&len);
    pBuffer=ByteStr(pBuffer,0,&len);
    pBuffer=WORDStr(pBuffer,0,&len);
    for(int i=0; i< m_TargetCount;i++){
        pBuffer=ByteStr(&pTargetListaAttuale->pTarget[i].TipoTarget,1,&len);
    }

    memcpy(pBuffer,FINE_MSG,strlen(FINE_MSG));
    len+=strlen(FINE_MSG);

    MostraEvento("Cambio Target" , Color::FG_GREEN);

    return len;
}

