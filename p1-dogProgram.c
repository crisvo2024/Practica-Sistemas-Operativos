#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>


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
int ingresar(char nombre[32],char tipo[32],int edad,char raza[16],int estatura,float peso, char sexo);
int Listar(int Hash);
int eliminar (int id);
int Buscar(char nombre[32]);
int mygetch ( void );
FILE *myf,*tablaHash,*eliminados;
int n, tabla[1010],offset,total;
struct dogType *lista;
int maxid;

int main(int argc, char const *argv[])
{
    //inicializacion de variables y apertura de archivos
    int opcion=0;
    myf = fopen("dataDogs.dat","rb+");
    tablaHash = fopen("TablaHash.dat","rb+");
    eliminados = fopen("Eliminados.dat","rb+");
    int deleted, id;
    int salir=0;
    char continuar;
    offset=-1;
    char nombre[32];
    struct dogType Anterior;
    //verificacion de cantidad de registros eliminados
    while(!feof(eliminados))
    {
        fread(&deleted,sizeof(int),1,eliminados);
        offset++;
    }
    //calculo de registros totales
    fseek(myf,-sizeof(struct dogType),SEEK_END);
    fread(&Anterior,sizeof(struct dogType),1,myf);
    maxid=Anterior.id;
    total=Anterior.id+1-offset;
    //menu
    do
    {
        printf("\nMenu: ","%s\n" );
        printf("\n 1. Ingresar Registro " ,"%s\n");
        printf("\n 2. Ver Registro ","%s\n" );
        printf("\n 3. Borrar Registro ","%s\n");
        printf("\n 4. Buscar Registro ","%s\n ");
        printf("\n 0. Salir" ,"%s\n ");
        printf("\nIngrese el numero de la opcion: ","%s\n");
        scanf("%d", &opcion);
        switch(opcion)
        {
            case 0:break;
            case 1:;
                char tipo[32];
                int edad;
                char raza[16];
                int estatura;
                float peso;
                char sexo;
                printf("Ingrese el nombre de la mascota: ","%s\n");
                //limpieza de cadena nombre
                memset(nombre,'\0',32);
                scanf("%s",&nombre);
                strcat(nombre,"\n");
                printf("Ingrese el tipo de la mascota: ","%s\n");
                scanf("%s",&tipo);
                printf("Ingrese la edad de la mascota: ","%s\n");
                scanf("%d",&edad);
                printf("Ingrese la raza de la mascota: ","%s\n");
                scanf("%s",&raza);
                printf("Ingrese la estatura de la mascota: ","%s\n");
                scanf("%d",&estatura);
                printf("Ingrese el peso de la mascota: ","%s\n");
                scanf("%f",&peso);
                printf("Ingrese el sexo de la mascota M o H: ","%s\n");
                scanf(" %c",&sexo);
                int x= ingresar(nombre,tipo,edad,raza,estatura,peso,sexo);
                printf("Oprima cualquier tecla para continuar");
                mygetch();
                break;

            case 2:;
                salir=0;
                id=-1;
                struct dogType Nuevo;
                char desition;
                do
                {
                    salir=0;
                    printf("Hay %d registros, ingrese el id del registro que desea ver\n",total);
                    scanf("%d",&id);
                    //verificacion de que el id no este eliminado
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
                } while (id>maxid||id<0||salir==1);
                //viajar a la posicion id en el archivo si existe, sino a la posicion menos los eliminados
                if(id>total){
                    fseek(myf,sizeof(struct dogType)*(id-offset),SEEK_SET);

                }else{
                    fseek(myf,sizeof(struct dogType)*id,SEEK_SET);
                }
                //comprobar que la posicion corresponde con el id
                fread(&Nuevo,sizeof(struct dogType),1,myf);
                while(Nuevo.id!=id)
                {
                    fseek(myf,-sizeof(struct dogType)*2,SEEK_CUR);
                    fread(&Nuevo,sizeof(struct dogType),1,myf);
                }
                printf("\nID: %d\n",Nuevo.id);
                printf("Nombre: %s\n",Nuevo.nombre);
                printf("Tipo: %s \n",Nuevo.tipo);
                printf("Edad: %d \n", Nuevo.edad);
                printf("Raza: %s \n", Nuevo.raza);
                printf("Estatura: %d \n", Nuevo.estatura);
                printf("Peso: %.2f \n", Nuevo.peso);
                printf("Sexo: %c \n", Nuevo.sexo);
                printf("Hash: %d \n", Nuevo.hash);
                printf("Next: %d \n", Nuevo.next);
                //confirmacion de apertura de historia clinica
                do
                {                 
                    printf("Desea ver la historia clinica de %s (S/N)\n",Nuevo.nombre);
                    scanf(" %c",&desition);
                    if(desition=='S'||desition=='s')
                    {
                        //llamada a nano con archivo de historia clinica identifi cado por el id
                        char comando[32];
                        sprintf(comando,"nano %d.txt",id);
                        system(comando);
                        break;
                    }
                    if(desition!='N'&&desition!='n')printf("Opcion no valida");
                }while(desition!='n' && desition!='N');
                printf("Oprima cualquier tecla para continuar");
                mygetch();  
                break;

            case 3:;
                struct dogType eliminado;
                do{
                    salir=0;
                    printf("\n Hay %d registros, ingrese el id del registro que desea eliminar: \n",total);
                    scanf("%d",&id);
                    //verificacion de que el id no este eliminado
                    fseek(eliminados,0,SEEK_SET);
                    while(!feof(eliminados))
                    {
                        fread(&deleted,sizeof(int),1,eliminados);
                        if(deleted==id||id>maxid||id<0){
                            printf("No hay mascota con el id %d", id);
                            salir=1;
                            break;
                        }
                    }
                }while (salir==1||id>maxid||id<0);
                {
                    /* code */
                }
                
                //viajar a la posicion de id si existe o a la posicion menos los eliminados
                if(id>total){
                    fseek(myf,sizeof(struct dogType)*(id-offset),SEEK_SET);

                }else{
                    fseek(myf,sizeof(struct dogType)*id,SEEK_SET);
                }
                //verificar que el id corresponda con la posicion devolviendose hasta encontrarlo
                fread(&eliminado,sizeof(struct dogType),1,myf);
                while(eliminado.id!=id)
                {
                    fseek(myf,-sizeof(struct dogType)*2,SEEK_CUR);
                    fread(&eliminado,sizeof(struct dogType),1,myf);
                }
                printf("\n Datos del registro que desea eliminar: \n");
                printf("\nID: %d\n",eliminado.id);
                printf("Nombre: %s\n",eliminado.nombre);
                printf("Tipo: %s \n",eliminado.tipo);
                printf("Edad: %d \n", eliminado.edad);
                printf("Raza: %s \n", eliminado.raza);
                printf("Estatura: %d \n", eliminado.estatura);
                printf("Peso: %.2f \n", eliminado.peso);
                printf("Sexo: %c \n", eliminado.sexo);
                printf("Hash: %d \n", eliminado.hash);
                printf("Next: %d \n", eliminado.next);
                //confirmacion de eliminacion
                do
                {
                    printf("Desea eliminar el registro de %s (S/N): \n",eliminado.nombre);
                    scanf(" %c",&desition);
                    if(desition=='S'||desition=='s'){
                        eliminar(eliminado.id); 
                        break; 
                    }
                    if(desition=='n' && desition=='N')printf("Operacion abortada \n");
                }while(desition!='n' && desition!='N');
                
                printf("Oprima cualquier tecla para continuar");
                mygetch();
                break;
            case 4:;
                printf("Ingrese el nombre a buscar: ");
                //limpieza de cadena nombre
                memset(nombre,'\0',32);
                scanf("%s",&nombre);
                strcat(nombre,"\n");
                Buscar(nombre);
                printf("Oprima cualquier tecla para continuar");
                mygetch();
                break;
            default:
                printf("opcion no valida");
        }
    }while(opcion!=0);
    fclose(myf);
    fclose(tablaHash);
    fclose(eliminados);
    return 0;
}
int ingresar(char nombre[32],char tipo[32],int edad,char raza[16],int estatura,float peso, char sexo)
{
    struct dogType Anterior, Nuevo;
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
            break;
        }
    }
    Nuevo.id=id;
    memset(Nuevo.nombre,'\0',32);
    strcpy(Nuevo.nombre,nombre);
    Nuevo.edad=edad;
    memset(Nuevo.raza,'\0',32);
    strcpy(Nuevo.raza,raza);
    Nuevo.estatura=estatura;
    memset(Nuevo.tipo,'\0',32);
    strcpy(Nuevo.tipo,tipo);
    Nuevo.peso=peso;
    Nuevo.sexo=sexo;
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
        total++;
        return 0;
    }else{
        //añadiendo Nuevo en la lista encadenada
        Anterior=lista[n];
        Anterior.next=(Nuevo.id)-offset;
        fseek(myf,-sizeof(struct dogType),SEEK_CUR);
        fwrite(&Anterior,sizeof(struct dogType),1,myf);
    }
    //liberacion de memoria de la lista encadenada
    free(lista);
    //impresion de datos de la nueva mascota
    printf("\nID: %d\n",Nuevo.id);
    printf("Nombre: %s\n",Nuevo.nombre);
    printf("Tipo: %s \n",Nuevo.tipo);
    printf("Edad: %d \n", Nuevo.edad);
    printf("Raza: %s \n", Nuevo.raza);
    printf("Estatura: %d \n", Nuevo.estatura);
    printf("Peso: %.2f \n", Nuevo.peso);
    printf("Sexo: %c \n", Nuevo.sexo);
    printf("Hash: %d \n", Nuevo.hash);
    printf("Next: %d \n", Nuevo.next);
    total++;
    maxid++;
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
int eliminar (int id)
{
    struct dogType buffer, deleted;
    int posdel=id;
    //viajar a la posicion de id si existe o a la posicion menos los eliminados
    if(id>total){
        fseek(myf,sizeof(struct dogType)*(id-offset),SEEK_SET);

    }else{
        fseek(myf,sizeof(struct dogType)*id,SEEK_SET);
    }
    //se busca en el archivo a partir de la posicion actual y se mueve hasta el registro con el id correspondiente    
    fread(&deleted,sizeof(struct dogType),1,myf);
    while(deleted.id!=id)
    {
        fseek(myf,-sizeof(struct dogType)*2,SEEK_CUR);
        fread(&deleted,sizeof(struct dogType),1,myf);
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
        while(pos<n)
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
    //ir a la posicion del id del eliminado en el en datadogs y en su copia
    fseek(myf,sizeof(struct dogType)*id,SEEK_SET);
    fread(&buffer,sizeof(struct dogType),1,myf);
    fseek(New,sizeof(struct dogType)*id,SEEK_SET);
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
    
}

int Buscar(char nombre[32]){
    int registros=0;
    //se genera la lista del hash del nombre y se verifica su existencia
    int con= Listar(PolyHash(nombre));
    if(con==1){
        printf("%s no existe \n",nombre);
        return 0;
    }
    for(int i=0;i<=n;i++)
    {
        //se imprimen los registros de la lista ecadenada cuyo nombre corresponda con el buscado 
        //y se contabilizan la cantidad de registros que coninciden
        if(strcoll(lista[i].nombre,nombre)==0){
            registros++;
            printf("\nID: %d\n",lista[i].id);
            printf("Nombre: %s\n",lista[i].nombre);
            printf("Tipo: %s \n",lista[i].tipo);
            printf("Edad: %d \n", lista[i].edad);
            printf("Raza: %s \n", lista[i].raza);
            printf("Estatura: %d \n", lista[i].estatura);
            printf("Peso: %.2f \n", lista[i].peso);
            printf("Sexo: %c \n", lista[i].sexo);
            printf("Hash: %d \n", lista[i].hash);
            printf("Next: %d \n", lista[i].next);
        }
        
    }
    //si la cantidad de registros que coinciden es 0 se imprime mensaje de que no existe 
    if(registros==0)printf("%s no existe \n",nombre);
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