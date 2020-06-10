#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h> 
#include <time.h>

#define PORT 3535
#define BACKLOG 2
#define MAX_THREADS 33

struct dogType{
    int id;
    char nombre[32];
    char tipo[32];
    int edad;
    char raza[16];
    int estatura;
    float peso;
    char sexo;
    int hash;
    int next;
    };

int PolyHash (char cadena[]);
int ingresar(struct dogType n,int clientfd);
int Listar(int Hash);
int eliminar (int id, int clientefd);
int Buscar(char nombre[32],int clientfd);
int mygetch ( void );
int verRegistro(int id, int clientefd, int confVer);
FILE *myf,*tablaHash,*eliminados,*consultas;
int n, tabla[1010],offset,total;
struct dogType *lista;
int maxid;
//nuevo
int serverfd, r, opt = 1;
struct sockaddr_in server, client;
socklen_t tamano;
pthread_t hilo[MAX_THREADS];
pthread_t hiloControl;
int numThreads;
int state[MAX_THREADS];
void sighandler(int signum);

char fecha[64];

struct datosH{
    int clientfd;
    int threadIndex;
};

int* atender(void *datos){
    //incicializacion de variables, archivos y el stream
    struct datosH *in;
    in=(struct datosH*)datos;
    int clientfd =in->clientfd;
    int opcion=0;
    time_t date=time(&date);
    struct tm timeInfo;
    myf = fopen("dataDogs.dat","rb+");
    tablaHash = fopen("TablaHash.dat","rb+");
    eliminados = fopen("Eliminados.dat","rb+");
    
    int deleted, id;
    int salir=0;
    char continuar;
    offset=-1;
    char nombre[32];
    struct dogType Anterior;
    int buffer;
    int datos_envio[2];
    while(!feof(eliminados))
    {
        fread(&deleted,sizeof(int),1,eliminados);
        offset++;
    }
    fseek(myf,-sizeof(struct dogType),SEEK_END);
    fread(&Anterior,sizeof(struct dogType),1,myf);
    maxid=Anterior.id;
    total=Anterior.id+1-offset;
    datos_envio[0]=maxid;
    datos_envio[1]=total;
    //envio del total de registros y el id maximo al cliente
    r=send(clientfd,datos_envio,2*sizeof(int),0);
    if(r < 0){
        perror("\n-->Error en send(): ");
        exit(-1);
    }
        do{ 
            //recibe la opcion del menu del cliente
            r=recv(clientfd,&buffer,sizeof(int),0);
            if(r<0){
                perror("error receive"); 
                exit(EXIT_FAILURE);
            }
            switch (buffer)
            {
            case 1:;
                //recibe la estructura a guardar
                struct dogType n;
                r=recv(clientfd,&n,sizeof(struct dogType),0);
                if(r<0){
                perror("error receive"); 
                exit(EXIT_FAILURE);
                }
                //llamamos la funcion ingresar con la estructura n
                ingresar(n, clientfd);
                break;
            case 2:
                //verificacion de cantidad de registros eliminados
                offset=-1;
                fseek(eliminados,0,SEEK_SET);
                while(!feof(eliminados))
                {
                    fread(&deleted,sizeof(int),1,eliminados);
                    offset++;
                }
                fseek(myf,-sizeof(struct dogType),SEEK_END);
                fread(&Anterior,sizeof(struct dogType),1,myf);
                maxid=Anterior.id;
                total=Anterior.id+1-offset;
                datos_envio[0]=maxid;
                datos_envio[1]=total;
                //envio del total de registros y el id maximo al cliente
                r=send(clientfd,datos_envio,2*sizeof(int),0);
                if(r < 0){
                    perror("\n-->Error en send(): ");
                    exit(-1);
                }
                do{
                    //recibe el id a buscar
                    salir=0;
                    r=recv(clientfd,&id,sizeof(int),0);
                    if(r<0){
                        perror("error receive"); 
                        exit(EXIT_FAILURE);
                    }
                    //confirmamos que el id no este entre los eliminados
                    fseek(eliminados,0,SEEK_SET);
                    while(!feof(eliminados))
                    {
                        fread(&deleted,sizeof(int),1,eliminados);
                        if(deleted==id||id>maxid||id<0){
                            printf("No hay mascota con el id %d \n", id);
                            salir=1;
                            break;
                        }   
                    }
                    //enviamos a cliente la validez del id
                    r=send(clientfd,&salir,sizeof(int),0);
                    if(r < 0){
                        perror("\n-->Error en send(): ");
                        exit(-1);
                    }
                    //el ciclo continua hasta que el cliente envie un id valido
                }while(salir!=0);
                //llamamos la funcion verRegistro con el id
                verRegistro(id, clientfd,0);
                //se registra la operacion en el log
                consultas= fopen("serverDogs.log","a");
                timeInfo=*localtime(&date);
                fprintf(consultas,"\n[ fecha: %d-%d-%d %2d:%2d:%2d ]",timeInfo.tm_year +1900,timeInfo.tm_mon+1,timeInfo.tm_mday,timeInfo.tm_hour,timeInfo.tm_min,timeInfo.tm_sec); 
                fprintf(consultas,"[ cliente 127.0.0.1 ]");
                fprintf(consultas,"[lectura]");
                fprintf(consultas,"[registro: %d]\n",id);
                fclose(consultas);
                break;
            case 3:
                //verificacion de cantidad de registros eliminados
                offset=-1;
                fseek(eliminados,0,SEEK_SET);
                while(!feof(eliminados))
                {
                    fread(&deleted,sizeof(int),1,eliminados);
                    offset++;
                }
                fseek(myf,-sizeof(struct dogType),SEEK_END);
                fread(&Anterior,sizeof(struct dogType),1,myf);
                maxid=Anterior.id;
                total=Anterior.id+1-offset;
                datos_envio[0]=maxid;
                datos_envio[1]=total;
                //envio del total de registros y el id maximo al cliente
                r=send(clientfd,datos_envio,2*sizeof(int),0);
                if(r < 0){
                    perror("\n-->Error en send(): ");
                    exit(-1);
                }
                do{
                    //recibe el id a eliminar
                    salir=0;
                    r=recv(clientfd,&id,sizeof(int),0);
                    if(r<0){
                        perror("error receive"); 
                        exit(EXIT_FAILURE);
                    }
                    //confirmamos que el id no este entre los eliminados
                    fseek(eliminados,0,SEEK_SET);
                    while(!feof(eliminados))
                    {
                        fread(&deleted,sizeof(int),1,eliminados);
                        if(deleted==id||id>maxid||id<0){
                            printf("No hay mascota con el id %d \n", id);
                            salir=1;
                            break;
                        }   
                    }
                    //enviamos a cliente la validez del id
                    r=send(clientfd,&salir,sizeof(int),0);
                    if(r < 0){
                        perror("\n-->Error en send(): ");
                        exit(-1);
                    }
                    //el ciclo continua hasta que el cliente envie un id valido
                }while(salir!=0);
                //llamamos la funcion verRegistra para confirmar con el cliente
                verRegistro(id, clientfd,1);
                int conf;
                //recibe de cliente la confirmacion de eliminacion
                r=recv(clientfd,&conf,sizeof(int),0);
                if(r<sizeof(int)){
                        perror("error receive"); 
                        exit(EXIT_FAILURE);
                }
                if(conf==1){
                    //llamar la funcion eliminar con el id
                    eliminar(id,clientfd);
                    //resgistro de la operacion en el log
                    consultas= fopen("serverDogs.log","a");
                    timeInfo=*localtime(&date);
                    fprintf(consultas,"\n[ fecha: %d-%d-%d %2d:%2d:%2d ]",timeInfo.tm_year +1900,timeInfo.tm_mon+1,timeInfo.tm_mday,timeInfo.tm_hour,timeInfo.tm_min,timeInfo.tm_sec);
                    fprintf(consultas,"[ cliente 127.0.0.1 ]");
                    fprintf(consultas,"[borrado]");
                    fprintf(consultas,"[registro: %d ]\n",id);
                    fclose(consultas);
                }
                break;
            case 4:;
                char nombreBusqueda[32];
                r=recv(clientfd,&nombreBusqueda,sizeof(char)*32,MSG_WAITALL);
                if(r<0){
                        perror("error receive de busqueda"); 
                        exit(EXIT_FAILURE);
                }
                Buscar(nombreBusqueda,clientfd);
                consultas= fopen("serverDogs.log","a");
                fprintf(consultas,"\n[ %s ]",fecha);
                fprintf(consultas,"[ cliente 127.0.0.1 ]");
                fprintf(consultas,"[busqueda]");
                fprintf(consultas,"[cadena buscada: %s ]\n",nombreBusqueda);
                fclose(consultas);
                break;
            default:
                break;
            }
        }while(buffer!=0);
        //marca el hilo como desocupado
        state[in->threadIndex]=0;
        close(clientfd);

}

int main(int argc, char const *argv[])
{
     //inicializacion del socket
    int clientfd;
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverfd < 0){
        perror("\n-->Error en socket():");
        exit(-1);
    }
    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8); 
    
    setsockopt(serverfd,SOL_SOCKET,SO_REUSEADDR,(const char *)&opt,sizeof(int));

    r = bind(serverfd, (struct sockaddr *)&server, sizeof(struct sockaddr));
    if(r < 0){
        perror("\n-->Error en bind(): ");
        exit(-1);
    }
    
    r = listen(serverfd, BACKLOG);
    if(r < 0){
        perror("\n-->Error en Listen(): ");
        exit(-1);
    }
    for (size_t i = 0; i < MAX_THREADS; i++)
    {
        state[i]=0;
    }
    

    //se añade la función signal para que se cierren los archivos en caso de interrupción del servidor
    signal(SIGINT,sighandler);
    int i=0;
    while(1){
        clientfd = accept(serverfd, (struct sockaddr *)&client, &tamano);
        if(clientfd < 0)
        {
            perror("\n-->Error en accept: ");
            exit(-1);
        }
        //verifica que hilo esta desocupado
        for (size_t h = 0; i < MAX_THREADS; i++)
        {
            if(state[h]==0){
                i=h;
                break;
            }
        }   
        //si todos estan ocupados espera al primer hilo
        if(state[i]==1){
            i=1;
            int *rh0;
            pthread_join(hilo[i], (void **)&rh0);
            if(r != 0)
            {
                perror("\n-->pthread_join error: ");
                exit(-1);
            }
        }     
        //inicializa los datos para pasarle al hilo y lo marca como ocupado
        struct datosH *datos=malloc(sizeof(struct datosH));
        datos->clientfd=clientfd;
        datos->threadIndex=i;
        state[i]=1;
        atender((void *)datos);
        //crea el hilo para atender al cliente  
        /*r=pthread_create(&hilo[i++],NULL,(void *)atender,(void *)datos);
        if(r != 0)
        {
            perror("\n-->pthread_create error: ");
            exit(-1);
        }  */
    }
    return 0;
}
void sighandler(int signum){
    //se cierran los servidores y los archivos
    fclose(myf);    
    close(serverfd); 
    fclose(tablaHash);
    fclose(eliminados);
    exit(0);
}
int ingresar(struct dogType Nuevo, int clientfd)
{
    struct dogType Anterior;
    int id, deleted;
    //encontrar y leer estructura final en anterior
    fseek(myf,-sizeof(struct dogType),SEEK_END);
    fread(&Anterior,sizeof(struct dogType),1,myf);
    //inicializar Nuevo con los datos ingresados limpiando los campos necesarios
    id=Anterior.id+1;
    //comprobar que el nuevo id no corresponda con uno que se halla eliminado antes
    fseek(eliminados,0,SEEK_SET);
    while(!feof(eliminados))
    {
        fread(&deleted,sizeof(int),1,eliminados);
        if(deleted==id){
            id++;
            fseek(eliminados,0,SEEK_SET);
        }
    }
    Nuevo.id=id;
    Nuevo.hash=PolyHash(Nuevo.nombre);
    Nuevo.next=0;
    //guardar nuevo registro al final del archivo
    fwrite(&Nuevo,sizeof(struct dogType),1,myf);
    //confirmacion de existencia del hash en la tabla hash y generacion de la lista encadenada
    int con = Listar(Nuevo.hash);
    if(con==1){
        //creacion de la entrada de hash en la tabla hash
        tabla[Nuevo.hash]=total;
        fseek(tablaHash,0,SEEK_SET);
        fwrite(&tabla,sizeof(tabla),1,tablaHash);
    }else{
        //añadiendo Nuevo en la lista encadenada
        Anterior=lista[n];
        Anterior.next=(Nuevo.id)-offset;
        fseek(myf,-sizeof(struct dogType),SEEK_CUR);
        fwrite(&Anterior,sizeof(struct dogType),1,myf);
    }
    //liberacion de memoria de la lista encadenada
    free(lista);
    r=send(clientfd,&Nuevo,sizeof(struct dogType),0);
    if(r<0){
        perror("error envio");
        exit(-1);
    }
    total++;
    maxid=id;
    //escritura de la operacion en el log
    time_t date=time(&date);
    struct tm timeInfo;
    consultas= fopen("serverDogs.log","a");
    timeInfo=*localtime(&date);
    fprintf(consultas,"\n[ fecha: %d-%d-%d %2d:%2d:%2d ]",timeInfo.tm_year +1900,timeInfo.tm_mon+1,timeInfo.tm_mday,timeInfo.tm_hour,timeInfo.tm_min,timeInfo.tm_sec);
    fprintf(consultas,"[ cliente 127.0.0.1 ]");
    fprintf(consultas,"Insercion");
    fprintf(consultas,"registro: %d]\n",Nuevo.id);
    fclose(consultas); 
}
int verRegistro(int id, int clientfd, int confver){
    struct dogType Nuevo;
    int buff;
    //viajar a la posicion id en el archivo si existe, sino a la posicion menos los eliminados
    if(id>=total){
        fseek(myf,sizeof(struct dogType)*(id-offset),SEEK_SET);
        //comprobar que la posicion corresponde con el id
        fread(&Nuevo,sizeof(struct dogType),1,myf);
        while(Nuevo.id!=id)
        {
            fread(&Nuevo,sizeof(struct dogType),1,myf);
        }
    }else{
        fseek(myf,sizeof(struct dogType)*id,SEEK_SET);
        //comprobar que la posicion corresponde con el id
        fread(&Nuevo,sizeof(struct dogType),1,myf);
        while(Nuevo.id!=id)
        {
            fseek(myf,-sizeof(struct dogType)*2,SEEK_CUR);
            fread(&Nuevo,sizeof(struct dogType),1,myf);
        }
    }
    //enviamos al cliente la estructura corespondiente al id
    r=send(clientfd,&Nuevo,sizeof(struct dogType),0);
    if(r<0){
        perror("error envio");
        exit(-1);
    }
    //recibe de cliente el deseo de ver la historia clinica
    if(confver==1)return 0;
    r=recv(clientfd,&buff,sizeof(int),0);
    if(r<0){
        perror("error recv() ");
        exit(-1);
    }
    if(buff==1){
        char comando[32];
        sprintf(comando,"%d_server.txt",Nuevo.id);
        //busqueda de la historia correspondiente al id
        FILE *historia=fopen(comando,"rb");
        int respuesta=0;
        if(historia==NULL){
            respuesta=1;
        }
        //envio al cliente sobre la existencia del archivo
        r=send(clientfd,&respuesta,sizeof(int),0);
        if(r<0){
            perror("error envio");
            exit(-1);
        }
        if(historia!=NULL){
            char filedata[1024];
            fseek(historia, 0L, SEEK_SET);
            int sz=0;
            while((sz=fread(&filedata,sizeof(char),1024,historia))>0){
                //envio a cliente de todos lo caracteres dentro de la historia clinica
                r=send(clientfd,&sz,sizeof(int),0);
                if(r<0){
                    perror("error envio");
                    exit(-1);
                }
                r=send(clientfd,&filedata,sizeof(char)*sz,0);
                if(r<0){
                    perror("error envio");
                    exit(-1);
                }
            }
            sz=0;
            r=send(clientfd,&sz,sizeof(int),0);
            if(r<0){
                perror("error envio");
                exit(-1);
            }
            fclose(historia);

        }
        //recibe de cliente si se realizaron cambios en la historia
        r=recv(clientfd,&respuesta,sizeof(int),0);
        if(r<0){
            perror("error recibir");
            exit(-1);
        }
        if (respuesta==1){
            //termina si no hay cambios nuevos
            return 0;
        }
        //se elimina el archivo anterior si ya existia
        if(historia!=NULL){
            sprintf(comando,"rm %d_server.txt",Nuevo.id);
            system(comando);
        }
        
        sprintf(comando,"%d_server.txt",Nuevo.id);
        historia = fopen(comando,"w");
        char info[1024];
        int sz=0;
        r= recv(clientfd,&sz,sizeof(int),0);
        if(r<0){
            perror("\n-->Error en recv info historia clinica");
            exit(-1);
        }
        do{
            memset(info,'\0',1024);
            //recibe de cliente todos los caraceres de la historia para actualizarla
            r= recv(clientfd,&info,sizeof(char)*sz,MSG_WAITALL);
            if(r<0){
                perror("\n-->Error en recv info historia clinica");
                exit(-1);
            }
            fprintf(historia,"%s",info);
            r= recv(clientfd,&sz,sizeof(int),0);
            if(r<0){
                perror("\n-->Error en recv info historia clinica");
                exit(-1);
            }
        }while(sz>0);
        fclose(historia);  
    }
}

int Listar(int Hash){
    struct dogType prueba;
    //lee la tabla hash y la carga en el arreglo tabla
    fseek(tablaHash,0,SEEK_SET);
    fread(&tabla,sizeof(tabla),1,tablaHash);
    //comprobacion de la existencia del hash y retorno en caso contrario
    if(tabla[Hash]==-1){
        return 1;
    }
    //busqueda en el archivo de la primera posicion de la lista encadenada
    fseek(myf,(tabla[Hash])*sizeof(struct dogType),SEEK_SET);
    fread(&prueba,sizeof(struct dogType),1,myf);
    //conteo de cantidad de elementos en la lista encadenada
    n=1;
    while(prueba.next>0)
    {
        n++;
        fseek(myf,(prueba.next)*sizeof(struct dogType),SEEK_SET);
        fread(&prueba,sizeof(struct dogType),1,myf);
    }
    //reserva de memoria de la lista encadenada y guardado en lista
    lista=(struct dogType *)malloc(n*sizeof(struct dogType));
    fseek(myf,(tabla[Hash])*sizeof(struct dogType),SEEK_SET);
    fread(&lista[0],sizeof(struct dogType),1,myf);
    n=0;
    while(lista[n].next>0)
    {
        fseek(myf,(lista[n].next)*sizeof(struct dogType),SEEK_SET);
        fread(&lista[++n],sizeof(struct dogType),1,myf);
    }    
    return 0;
}
int eliminar (int id, int clientfd)
{
    struct dogType buffer, deleted;
    int posdel=id;
    //viajar a la posicion de id si existe o a la posicion menos los eliminados
    if(id>=total){
        fseek(myf,sizeof(struct dogType)*(id-offset),SEEK_SET);
        //se busca en el archivo a partir de la posicion actual y se mueve hasta el registro con el id correspondiente    
        fread(&deleted,sizeof(struct dogType),1,myf);
        while(deleted.id!=id)
        {
            fread(&deleted,sizeof(struct dogType),1,myf);
        }
    }else{
        fseek(myf,sizeof(struct dogType)*id,SEEK_SET);
        //se busca en el archivo a partir de la posicion actual y se mueve hasta el registro con el id correspondiente    
        fread(&deleted,sizeof(struct dogType),1,myf);
        while(deleted.id!=id)
        {
            fseek(myf,-sizeof(struct dogType)*2,SEEK_CUR);
            fread(&deleted,sizeof(struct dogType),1,myf);
        }
    }
    
    //se crea la lista encadenada del hash del id a borrar y se verifica si su tamaño es 1
    Listar(deleted.hash);
    if(n==1)
    {
        //si solo hay uno en la lista ecadenada se elimina de la tabla hash esa posicion
        tabla[deleted.hash]=-1;
    }else
    {
        //si hay mas se verifica en que posicion de la lista encadenada esta el id a borrar
        int pos=0;
        while(pos<=n)
        {
            if(lista[pos++].id==id)break;
        }
        if(pos==1)
        {
            //si la posicion es la primera se remplaza en la tabla hash con el next del eliminado
            tabla[deleted.hash]=deleted.next;
        }else
        {
            //vamos a la posicion en el archivo del id de la posicion anterior al que vamos a eliminar 
            fseek(myf,sizeof(struct dogType)*lista[pos-2].id,SEEK_SET);
            fread(&buffer,sizeof(struct dogType),1,myf);
            //verificar que corresponda la posicion con el id de la posicion anterior al que vamos a eliminar
            while(buffer.id!=lista[pos-2].id)
            {
                fseek(myf,-sizeof(struct dogType)*2,SEEK_CUR);
                fread(&buffer,sizeof(struct dogType),1,myf);
            }
            fseek(myf,-sizeof(struct dogType),SEEK_CUR);
            //reemplazamos el next del id anterior por el next del eliminado
            lista[pos-2].next=lista[pos-1].next;
            fwrite(&lista[pos-2],sizeof(struct dogType),1,myf);
        }
        
    }
    //se cierra el archivo para aegurarnos de que los cambios hechos se guarden
    fclose(myf);
    myf=fopen("dataDogs.dat","rb+");
    //se copia el archivo con la funcion cp del sistema operativo
    system("cp dataDogs.dat dataDogs1.dat");
    //se habre la copia del archivo
    FILE *New= fopen("dataDogs1.dat","rb+");
    //ir a la posicion del id del eliminado en el en datadogs y en su copia, o al ultimo registro si el id es mayor a la cantidad de registros
    if(id>=total){
        fseek(myf,-sizeof(struct dogType),SEEK_END);
        fseek(New,-sizeof(struct dogType),SEEK_END);
        posdel=total-1;
    }else{
        fseek(myf,sizeof(struct dogType)*id,SEEK_SET);
        fseek(New,sizeof(struct dogType)*id,SEEK_SET);
    }   
    fread(&buffer,sizeof(struct dogType),1,myf);
    //verificamos que la posicion corresponda con el id a eliminar
    while (buffer.id!=id)
    {
        //se corrige la posicion del id a eliminar 
        posdel--;
        fseek(myf,-sizeof(struct dogType)*2,SEEK_CUR);
        fseek(New,-sizeof(struct dogType),SEEK_CUR);
        fread(&buffer,sizeof(struct dogType),1,myf);
    }    
    //se escriben todas las posiciones despues del id a eliminar en el nuevo archivo 
    //saltando el eliminado y reduciendo en uno el next de cada uno
    while(!feof(myf))
    {
        fread(&buffer,sizeof(struct dogType),1,myf);
        buffer.next--;
        fwrite(&buffer,sizeof(struct dogType),1,New);
    }
    //truncamos el archivo nuevo con el tamaño del archivo original menos una estructura
    fseek(New, 0L, SEEK_END);
    int sz=ftell(myf)-sizeof(struct dogType);
    ftruncate(fileno(New),sz);
    //eliminamos el archivo anterior con ma funcion rm del sistema operativo
    system("rm dataDogs.dat");
    //guardamos los cambios en el nuevo archivo y lo renombramos con el nombre del antiguo
    //utilizando la funcion mv del sistema operativo
    fclose(New);
    system("mv dataDogs1.dat dataDogs.dat");
    //liberamos la memoria de lista 
    free(lista);
    //añadimos en el archivo eliminados el id de la estructura eliminada
    fseek(eliminados,0,SEEK_END);
    fwrite(&id,sizeof(int),1,eliminados);
    struct dogType actual; 
    //reabriendo dataDogs.dat para que tome el nuevo archivo
    fclose(myf);
    myf=fopen("dataDogs.dat","rb+");
    //reducimos en 1 todos los next de las posiciones anteriores a la eliminada cuyo next
    //va despues de la eliminada
    fseek(myf,0,SEEK_SET);
    fread(&actual,sizeof(struct dogType),1,myf);
    for (size_t i = 0; actual.id<id&&!feof(myf); i++){
        if(actual.next>id){
            actual.next--;
            fseek(myf,-sizeof(struct dogType),SEEK_CUR);
            fwrite(&actual,sizeof(struct dogType),1,myf);
        }
        fread(&actual,sizeof(struct dogType),1,myf);
    }
    //actualizamos la tabla hash reduciendo en uno todos los que estan despues de eliminado
    for (size_t i = 0; i < 1010; i++)
    {
        if(tabla[i]>posdel)tabla[i]--;
    }
    fseek(tablaHash,0,SEEK_SET);
    fwrite(&tabla,sizeof(tabla),1,tablaHash);
    //se suma uno al offset y se actualizan los totales
    offset++;
    total--;
    int a=1;
    //envio de la confirmacion de eliminacion al cliente
    r=send(clientfd,&a,sizeof(int),0);
    if(r < 0){
        perror("\n-->Error en send(): ");
        exit(-1);
    }
}

int Buscar(char nombre[32], int clientfd){
    int registros=0;
    //se genera la lista del hash del nombre y se verifica su existencia
    int con= Listar(PolyHash(nombre));
    if(con==1){
        printf("%s no existe \n",nombre);
        r=send(clientfd,&registros,sizeof(int),0);
        return 0;
    }
    for(int i=0;i<=n;i++)
    {
        //se imprimen los registros de la lista ecadenada cuyo nombre corresponda con el buscado 
        //y se contabilizan la cantidad de registros que coninciden
        if(strcoll(lista[i].nombre,nombre)==0){
            registros++;
        }        
    }
    r=send(clientfd,&registros,sizeof(int),0);
    if(r < 0){
        perror("\n-->Error en send(): ");
        exit(-1);
    }
    registros=0;
    for(int i=0;i<=n;i++)
    {
        //se imprimen los registros de la lista ecadenada cuyo nombre corresponda con el buscado 
        //y se contabilizan la cantidad de registros que coninciden
        if(strcoll(lista[i].nombre,nombre)==0){
            r=send(clientfd,&lista[i],sizeof(struct dogType),0);
            if(r < 0){
                perror("\n-->Error en send(): ");
                exit(-1);
            }
        }        
    }
    // se libera la memoria de la lista
    free(lista);
}

int PolyHash (char cadena[])
{
    //se tranforman todas las letras de la cadena en mayusuculas
    int i = 0;
    while (cadena[i] != '\0') 
    {
        if (cadena[i] >= 'a' && cadena[i] <= 'z') {
            cadena[i] = cadena[i] - 32;
        }
    i++;
    }
    //se calcula el hash ignorando caracteres especiales
    int Hash=0;
    for (int o = 31; o >= 0; o--)
    {
        if(cadena[o] >= 'A' && cadena[o] <= 'Z') Hash=(Hash*31+(int)cadena[o])% 1009;
    }
    //se utiliza valor absoluto en el hash y se retorna el valor
    if(Hash<0){
        Hash=-Hash;
    }
    return Hash;        
}
//funcion tomada de https://faq.cprogramming.com/cgi-bin/smartfaq.cgi?id=1043284385&answer=1042856625
int mygetch ( void ) 
{
  int ch;
  struct termios oldt, newt;
  
  tcgetattr ( STDIN_FILENO, &oldt );//leer los atributos de entrada de la consola
  newt = oldt;//copiarlos en newt
  newt.c_lflag &= ~( ICANON | ECHO );//descativa el modo canonico de la entrada
  tcsetattr ( STDIN_FILENO, TCSANOW, &newt );//fijar atributos de newt
  ch = getchar();//lee el caracter anterior que deja scanf sin leer
  ch = getchar();//espera cualquier entrada
  tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );//resetea los atributos de entrada de la consola
  
  return ch;
}