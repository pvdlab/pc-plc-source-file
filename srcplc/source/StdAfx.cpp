#include "StdAfx.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <QSqlError>
#include <QDebug>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "MainCostanti.h"
#include <dirent.h>
#include "string"

int TipoStep;
void setTipoStep(int newTipoStep){
    TipoStep=newTipoStep;
}
int getTipoStep(){
    return TipoStep;
}

bool IsMacchinaInVuoto;
void setMacchinaInVuoto(bool newIsMacchinaInVuoto){
    IsMacchinaInVuoto=newIsMacchinaInVuoto;
}
bool getMacchinaInVuoto(){
    return IsMacchinaInVuoto;
}


bool AcquaErrore;
void setAcquaErrore(bool newAcquaErrore){
    AcquaErrore=newAcquaErrore;
}
bool getAcquaErrore(){
    return AcquaErrore;
}


double Penning;
void setPenning(double newPenning){
    Penning=newPenning;
}

double getPenning(){
    return Penning;
}

double Baratron;
void setBaratron(double newBaratron){
    Baratron=newBaratron;
}

double getBaratron(){
    return Baratron;
}

int TipoApp;

void setTipoApp(int newTipoApp){
    TipoApp=newTipoApp;
}
int getTipoApp(){
    return TipoApp;
}

bool VersioneRidotta;

void setVersioneRidotta(bool newVersioneRidotta){
    VersioneRidotta=newVersioneRidotta;
}
bool getVersioneRidotta(){
    return VersioneRidotta;
}

int w_foglio;
int h_foglio;

void getFoglio(int *w, int *h ){
    *w=w_foglio;
    *h=h_foglio;
}

void setFoglio(int w, int h ){
    w_foglio=w;
    h_foglio=h;
}


int getdir (std::string dir,std::string ext, std::vector<std::string> &files)
{
    DIR *dp;
    dir.append("/.");
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        return -1;
    }

    while ((dirp = readdir(dp)) != NULL) {
        std::string nome=dirp->d_name;
        int index = nome.find(ext);
        if (index!=-1 )
          files.push_back(dirp->d_name);
    }
    closedir(dp);
    return 0;
}

void creaDir(){

    struct stat st ;

    std::string nome=getRunPath();
    nome.append(PLC_CICLO_PATH);
    if (stat(nome.data(), &st) == -1) {
        mkdir(nome.data(), 0700);
    }

    nome=getRunPath();
    nome.append(PLC_PROCESSO_PATH);
    if (stat(nome.data(), &st) == -1) {
        mkdir(nome.data(), 0700);
    }

    nome=getRunPath();
    nome.append(PLC_TARGET_PATH);
    if (stat(nome.data(), &st) == -1) {
        mkdir(nome.data(), 0700);
    }

}


bool errore;

bool getErrore(){
   return errore;
}

void setErrore(bool newerrore){
   errore=newerrore;
}

bool completo;

bool IsCompleto(){
    return completo;
}
void setIsCompleto(bool newval){
    completo=newval;
}


WORD Impianto;

int  getImpianto(){
    return Impianto;
}

void setImpianto(int newImpianto){
    Impianto=newImpianto;
}

WORD SorgenteCount;

int  getSorgenteCount(){
    return SorgenteCount;
}

void setSorgenteCount(int newSorgenteCount){
    SorgenteCount=newSorgenteCount;
}

int GasCount;
int  getGasCount(){
    return GasCount;
}
void setGasCount(int newGasCount){
    GasCount=newGasCount;
}

int ResistenzaCount;
int  getResistenzaCount(){
    return ResistenzaCount;
}
void setResistenzaCount(int newResistenzaCount){
    ResistenzaCount=newResistenzaCount;
}

DWORD   ActClock=0;
DWORD   OffSetClock=0;
DWORD   StartClock=0;

int lanno,lmese;

void setLicenza(int newanno,int newmese){
   lanno=newanno;
   lmese=newmese;
}

bool isLicenza(){

    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    if(lanno-(timeinfo->tm_year+1900)<=0){
        if(lmese-(timeinfo->tm_mon+1)<=0){
            MostraErrore("LICENZA SCADUTA");
            return false;
        }
    }
    return true;
}


std::string ComandoToString(char *msg)
{



    int funcCode =msg[2];
    std::string ppBuffer;

    switch( funcCode )
    {
        case  CTipoComando::StendBy:
          ppBuffer="ComStendBy";
          break;
        case  CTipoComando::Vacuum:
          ppBuffer="ComVacuum";
          break;
        case  CTipoComando::Load:
          ppBuffer="ComLoad";
          break;
        case  CTipoComando::Elimina:
          ppBuffer="Elimina";
          break;
        case  CTipoComando::StartProcesso:
          ppBuffer="ComStartProcesso";
          break;
        case  CTipoComando::Manuale:
          ppBuffer="ComManuale";
          break;
        case  CTipoComando::StopVuoto:
          ppBuffer="ComStopVuoto";
          break;
        /*case  CTipoComando::UnLoad:
          ppBuffer="ComUnLoad";
          break;*/
        case  CTipoComando::ResetSegnali:
          ppBuffer="ComResetSegnali";
          break;
        case  CTipoComando::Reset:
          ppBuffer="ComReset";
          break;
        case  CTipoComando::Preset:
          ppBuffer="ComPreset";
          break;
        case  CTipoComando::resetSorgente:
          ppBuffer="ComresetSorgente";
          break;
        case  CTipoComando::ResetStendBy:
          ppBuffer="ComResetStendBy";
          break;
        case  CTipoComando::NextStep:
          ppBuffer="ComNextStep";
          break;
        case CTipoComando::Enabled:
          ppBuffer="ComEnabled";
          break;
        case CTipoComando::Ventila:
          ppBuffer="ComVentila";
          break;
        case  CTipoComando::Impara:
          ppBuffer="ComImpara";
          break;
        case  CTipoComando::GiraTavola:
          ppBuffer="ComGiraTavola";
          break;
        case  CTipoComando::StopVentila:
          ppBuffer="ComStopVentila";
          break;
        case  CTipoComando::StopProcesso:
          ppBuffer="ComStopProcesso";
          break;
        case  CTipoComando::ReStart:
          ppBuffer="ComReStart";
          break;
        case  CTipoComando::CaricaProcesso:
          ppBuffer="Carica Processo";
          break;
        case  CTipoComando::PlcManule:
           ppBuffer="PlcManule";
           break;
        default:
          ppBuffer="Nussuno";
          break;

    }



    return ppBuffer;
}


int PlcListenPort;

void setPlcListenPort(int newporta){

    if(newporta!=0)
      PlcListenPort=newporta;
    else
      PlcListenPort=PLC_LISTEN_PORTA;

}

int getPlcListenPort(){

    if(!PlcListenPort)
       return PLC_LISTEN_PORTA;
    else
       return PlcListenPort;
}

int PcListenPort;

void setPcListenPort(int newporta){

    if(!newporta)
       PcListenPort=newporta;
    else
       PcListenPort=PC_LISTEN_PORTA;

}

int getPcListenPort(){

    if(!PcListenPort)
       return PC_LISTEN_PORTA;
    else
       return PcListenPort;
}


char RunPath[PATH_SIZE];
const char  mEventoPath[]= "/wandplc";
char HomePath[PATH_SIZE];

char *getHomePath(){

    return HomePath;

}

void setHomePath(const char *path){

    char *pHomePath=HomePath;
    memset(pHomePath,0,PATH_SIZE);
    int len=strlen(path);
    if(len>PATH_SIZE)
        len=PATH_SIZE;
    memcpy(pHomePath,path,len);
    MostraEvento(HomePath,Color::FG_DEFAULT);


}



char  *getRunPath(){

    return RunPath;

}

void setRunPath(const char *path){

    char *pRunPath=RunPath;
    memset(pRunPath,0,PATH_SIZE);
    int len=strlen(path);
    if(len>PATH_SIZE)
        len=PATH_SIZE;
    memcpy(pRunPath,path,len);

    MostraEvento(RunPath,Color::FG_DEFAULT);
}

int simula;

void setSimula(int newval){
    simula=newval;
}


int getSimula(){
    return simula;
}


bool debug;

void setDebug(bool newval){
    debug=newval;
}


bool getDebug(){
    return debug;
}


bool BiasIsLV;

void setBiasIsLV(bool newval){
    BiasIsLV=newval;
}


bool getBiasIsLV(){
    return BiasIsLV;
}

double Bias_V;

void setBias_V(double newBias_V){
    Bias_V=newBias_V;
}


double getBias_V(){
    return Bias_V;
}

double Bias_I;

void setBias_I(double newBias_I){
    Bias_I=newBias_I;
}


double getBias_I(){
    return Bias_I;
}


DWORD mMainTime;

void setMainTime(DWORD time){
    mMainTime=time;
}

DWORD getMainTime(){
    return mMainTime;
}


DWORD getTick() {

#ifdef QT_APP
    struct timespec ts;
    unsigned theTick = 0U;
    clock_gettime( CLOCK_REALTIME, &ts );
    theTick  = ts.tv_nsec / 1000000;
    theTick += ts.tv_sec * 1000;
    return theTick;
#endif

}





DWORD GetTickCount()
{

  ActClock=getTick();
  if(getSimula())
     ActClock*=10;
  return  ActClock-StartClock;
   
}



void ResetTickCount()
{	

    StartClock=getTick();
    if(getSimula())
       StartClock*=10;
    ActClock=StartClock;
	OffSetClock=0;
   
}



BYTE StrByte(BYTE **pStr)
{
	BYTE *Tmp=*pStr;
	BYTE SVal=0;

    SVal=*Tmp++;

    *pStr=Tmp;
	return SVal;
}


BYTE *ByteStr(BYTE *pStr,BYTE SVal,int *len)
{
    BYTE CVal=SVal;
    int  NewLen=*len;

    memcpy(pStr++,&CVal,sizeof(BYTE));
    NewLen++;
    *len=NewLen;

    return pStr;
}


WORD StrWORD(BYTE **pStr)
{
    BYTE *Tmp=*pStr;
    WORD SVal=0;

    memcpy( &SVal,Tmp,sizeof(WORD));
    Tmp+=2;
    *pStr=Tmp;

    return (WORD)SVal;
}

BYTE *WORDStr(BYTE *pStr,WORD SVal,int *len)
{
    int   NewLen=*len;
    BYTE *pBuffer=pStr;

    memcpy(pBuffer,&SVal,sizeof(WORD));
    NewLen+=2;
    pBuffer+=2;
    pStr=pBuffer;
    *len=NewLen;

    return pStr;
}

short StrShort(BYTE **pStr)
{
    BYTE *Tmp=*pStr;
    short SVal=0;

    memcpy( &SVal,Tmp,sizeof(short));
    Tmp+=2;
    *pStr=Tmp;

    return (short)SVal;
}

BYTE *ShortStr(BYTE *pStr,short SVal,int *len)
{
    int   NewLen=*len;
    BYTE *pBuffer=pStr;

    memcpy(pBuffer,&SVal,sizeof(short));
    NewLen+=2;
    pBuffer+=2;
    pStr=pBuffer;
    *len=NewLen;

    return pStr;
}

DWORD StrDWORD(BYTE **pStr)
{
	BYTE *Tmp=*pStr;
    DWORD SVal=0;

    memcpy(&SVal,Tmp,sizeof(DWORD));
    Tmp+=4;
    *pStr=Tmp;

	return SVal;
}

BYTE *DWORDStr(BYTE *pStr,DWORD LVal,int *len)
{
    int  NewLen=*len;

    memcpy(pStr,&LVal,sizeof(DWORD));
    NewLen+=4;
    pStr+=4;
    *len=NewLen;

    return pStr;
}

double StrDouble(BYTE **pStr)
{
    BYTE *Tmp=*pStr;
	double DVal=0;

    memcpy(&DVal,Tmp,sizeof(double));
    Tmp+=8;
    *pStr=Tmp;

	return (double)DVal;
}

void StrDescrizione(char *Descrizione,BYTE **pStr)
{
   BYTE *Tmp=*pStr;
   int len;

   len=(int)*Tmp++;
   memcpy(Descrizione,Tmp,len);
   Tmp+=len;
   *pStr=Tmp;

   return;
}



BYTE *DoubleStr(BYTE *pStr, double DVal,int *len)
{

	int   NewLen=*len;

	memcpy(pStr,&DVal,sizeof(double));
	NewLen+=8;
	pStr+=8;
	*len=NewLen;

	return pStr;
}

char *DescrizioneStr(char *pStr, char *Descrizione,int *len)
{
    BYTE  CVal;
	int NewLen=*len;

	CVal=(BYTE)strlen(Descrizione);
    memcpy(pStr++,&CVal,sizeof(BYTE));
    memcpy(pStr,Descrizione,CVal);
    NewLen=CVal+1;
    pStr+=CVal;
	*len=NewLen;

    return pStr;
}

bool GetBit(BYTE b, BYTE position)
{
	return ((b & (BYTE)(1 << position)) != 0);
}



BYTE GetBits(BYTE x, BYTE p,int n)
{
    return (x>>(p-1)) & ~(~0<<n) ;
}

BYTE SetBit(BYTE b, BYTE position, bool newBitValue)
{
	BYTE mask;
		
	mask=(BYTE)(1 << position);
	if(newBitValue){
		return (BYTE)(b | mask);
	}else{
		return (BYTE)(b & ~mask);
	}
}

void SetBit(BYTE *b, BYTE Point, bool Segnale){
    BYTE CVal;

    BYTE *pBuffer =b;

    Segnale=__min(Segnale,1);

    if(Point<8){
      CVal=*pBuffer;
      CVal=SetBit(CVal, (BYTE)Point, (bool)Segnale);
      memcpy(pBuffer,&CVal,sizeof(BYTE));
    }
    else{
      pBuffer++;
      CVal=*pBuffer;
      CVal=SetBit(CVal, (BYTE)(Point-8), (bool)Segnale);
      memcpy(pBuffer,&CVal,sizeof(BYTE));
      }

}



void SetChrBit(BYTE *b, BYTE position, bool newBitValue)
{
  BYTE BitPos;
  BYTE BytePos;
  BYTE Bit;
  BYTE *pB=b;

  BytePos=(BYTE)position/8;
  BitPos=position-BytePos*8;
  pB+=BytePos;
  
  if(pB){
    Bit=(BYTE)*pB;
	Bit=SetBit(Bit, BitPos, newBitValue);
	memcpy(pB,&Bit,sizeof(BYTE));
  }
	
}

bool GetChrBit(BYTE *b, BYTE position)
{
  BYTE BitPos;
  BYTE BytePos;
  BYTE Bit;
  BYTE *pB=b;

  BytePos=(BYTE)position/8;
  BitPos=position-BytePos*8;
  pB+=BytePos;

  Bit=(BYTE)*pB;
  return GetBit(Bit, BitPos);

	
}

#ifdef QT_APP

QSqlDatabase *DataBase;
QString utente;
QString passoword;
QString host;
int mErrorCount;

bool ConnettiDB(QString nome, QString newUtente,QString newPassoword,QString newhost){

    utente=newUtente;
    passoword=newPassoword;
    host=newhost;
    mErrorCount=0;

    DataBase = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL","connectProcesso"));
    DataBase->setDatabaseName(nome);
    DataBase->setHostName(host);
    DataBase->setPort(-1);
    if (!DataBase->open(utente, passoword)) {
        std::string str;
        str.append(DataBase->lastError().text().toUtf8());
        MostraErrore(str);
        return false;
    }else{
        MostraEvento("CONNESSO CON SUCCESSO AL DataBase",Color::FG_DEFAULT);
        return true;
    }

}

std::vector<std::string> mSystem(std::string msg){

    FILE *cmd=popen(msg.data(), "r");
    char result[1024]={0x0};
    std::vector<std::string> strVec;
    while (fgets(result, sizeof(result), cmd) !=NULL){
           strVec.push_back(result);
    }
    pclose(cmd);
    return strVec;

}

bool mProcessoIsOn;

void setProcessoIsOn(bool val){

    mProcessoIsOn=val;
}
bool getProcessoIsOn(){

    return mProcessoIsOn;
}

bool SalvaSuFile;

void setSalvaSuFile(bool salva){

    SalvaSuFile=salva;
}

std::string EventoFileName;

void setEventoFileName(std::string nome){

     EventoFileName=nome;

}

std::string EventoProcessoFileName;
WORD EventoProcessoCount;

std::string getEventoProcessoFileName(){

    return EventoProcessoFileName;

}

void setEventoProcessoFileName(int Id){

    EventoProcessoFileName=getRunPath();
    EventoProcessoFileName.append(PLC_CICLO_PATH);
    EventoProcessoFileName.append(PLC_EVENTO);
    EventoProcessoFileName.append(intToString(Id));
    EventoProcessoCount=1;
    std::string path=EventoProcessoFileName;

    FILE* fp = fopen(path.data(), "w");
    if (fp){
        MostraEvento(path,Color::FG_GREEN);
        fclose(fp);
    }else{
        std::string str="IMPOSSIBILE APRIRE ";;
        str.append(path);
        MostraErrore(path);
    }

}

void  setMostraEventoProcessoCount(WORD count){
    if(count<MAX_MSG)
       EventoProcessoCount=count;
    else
       EventoProcessoCount=0;
}

WORD  getMostraEventoProcessoCount(){
    return EventoProcessoCount;
}
void MostraEventoProcessoReset(){
    EventoProcessoCount=0;
}

std::string oldmsg;

void MostraEventoProcesso(std::string msg, int colore){

    std::string str;
    str.append(MainTimeToString());
    str.append(" ");
    str.append(intToString(EventoProcessoCount));
    str.append("> ");
    str.append(msg);

    if(str.size()+3>D_MSG){
       str.resize(D_MSG-3);
    }

    if(str==oldmsg){
       return;
    }


    oldmsg=str;

    FILE* Fp = fopen(EventoProcessoFileName.data(), "a");
    if (Fp){
        fwrite(str.data(), D_MSG, 1, Fp);
        fclose(Fp);
        if(EventoProcessoCount+1<MAX_MSG)
           EventoProcessoCount++;
    }


    std::string out="\033[1;";
    out.append(intToString(colore));
    out.append("m");
    out.append(str);
    out.append("\033[0m\n");

    std::cout << out ;



}



void MostraEvento(std::string msg, int colore){

    if(getProcessoIsOn()){
        MostraEventoProcesso(msg,colore);
        return;
    }

    if(oldmsg==msg){
       return;
    }

    oldmsg=msg;

    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    int size=strlen( asctime (timeinfo) ) ;

    if(size-1<0)
       return;

    std::string str=asctime (timeinfo);
    str.replace(size-1,1,"\t");
    str.append(msg);

    std::string out="\033[1;";
    out.append(intToString(colore));
    out.append("m");
    out.append(str);
    out.append("\033[0m\n");

    std::cout << out ;

    if(SalvaSuFile){
        int fd = open(EventoFileName.data(), O_RDWR | O_APPEND |O_CREAT , mode);
        size=str.size();
        if(write (fd, str.data(), size) < 0){ //if(write (fd, str.data(), size) < 0){
              return ;
        }
        close(fd);
    }



}

void MostraEventoClear(){

    oldmsg="";
}

std::string ErrorFileName;

void setErrorFileName(std::string nome){

    ErrorFileName=nome;
}


int ErrorCount(){

    return mErrorCount;

}

void ErrorCountReset(){

    mErrorCount=0;

}



void MostraErrore(std::string msg){

    if(mErrorCount+1>10000 )
        mErrorCount=0;

    mErrorCount++;

    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    int size=strlen( asctime (timeinfo) ) ;
    if(size-1<0)
       return;  

    std::string str=asctime (timeinfo);
    str.replace(size-1,1,"\t");
    str.append(msg);

    // "\033[1;31mbold red text\033[0m\n";
    std::string out="\033[1;41m";
    out.append(str);
    out.append("\033[0m\n");

    std::cout << out ;

    int fd = open(ErrorFileName.data(), O_RDWR | O_APPEND |O_CREAT , mode);

    size=str.size();
    if(write (fd, str.data(), size) < 0){
          close(fd);
          return ;
    }
    close(fd);

}

std::string intToString(int i)
{
    std::stringstream ss;
    std::string s;
    ss << i;
    s = ss.str();

    return s;
}

std::string doubleToString(double i)
{
    std::stringstream ss;
    std::string s;
    ss << i;
    s = ss.str();

    return s;
}

std::string MainTimeToString(){

    std::string s;
    char timeString[9];

    DWORD actempo=getMainTime();

    int val=actempo/1000;
    int msec=actempo-val*1000;
    int ore=val/3600;
    val-=ore*3600;
    int minuti=val/60;
    val-=minuti*60;
    int sec=val;
    sprintf(timeString, "%02d:%02d:%02d %04d", ore, minuti, sec,msec);

    s.append(timeString);
    return s;
}
#else
void MostraEvento(const char * Buffer){

}

#endif


