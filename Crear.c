#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
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



int main(int argc, char const *argv[])
{
    FILE *myf, *nombres, *razas, *razasgato, *razasconejo;
    struct dogType lista;
    nombres=fopen("Nombres.txt","r");
    razas=fopen("razas.txt","r");
    razasgato=fopen("razasgato.txt","r");
    razasconejo=fopen("razasconejo.txt","r");
    char arr_nombres[1717][32];
    char arr_razas[242][16];
    char arr_razasgato[56][16];
    char arr_razasconejo[207][16];
 
    //Lee línea a línea y escribe en pantalla hasta el fin de fichero
    for (size_t i = 0; i < 1717; i++)
    {
        memset(arr_nombres[i],'\0',32);
        fgets(arr_nombres[i], 32, (FILE*) nombres);
    }
    
    fclose(nombres);
    for(size_t i = 0; i < 242; i++)
    {
        memset(arr_razas[i],'\0',16);
        fgets(arr_razas[i], 16, (FILE*) razas);
    }
    
    fclose(razas);
    for(size_t i = 0; i < 56; i++)
    {
        memset(arr_razasgato[i],'\0',16);
        fgets(arr_razasgato[i], 16, (FILE*) razasgato);
    }
    
    fclose(razasgato);
    for(size_t i = 0; i < 207; i++)
    {
        memset(arr_razasconejo[i],'\0',16);
        fgets(arr_razasconejo[i], 16, (FILE*) razasconejo);
    }
    srand(time(NULL));
    fclose(razasconejo);
    for (size_t i = 0; i < 3333333; i++)
    {
        myf = fopen("dataDogs.dat","ab");
        lista.id=i;
        int num=rand() %1717;
        memset(lista.nombre,'\0',32);
        strcpy(lista.nombre,arr_nombres[num]);
        num=rand() %242;
        strcpy(lista.raza,arr_razas[num]);
        strcpy(lista.tipo,"Perro\0");
        num=rand() %20;
        lista.edad=num;
        num=rand() %70;
        lista.estatura=num;
        num=rand() %85;
        lista.peso=num;
        num= rand()%2;
        if(num==1){
            lista.sexo='M';
        }else{
            lista.sexo='H';
        }
        lista.hash=PolyHash(lista.nombre);
        fwrite(&lista,sizeof(struct dogType),1,myf);
        fclose(myf);  
    }
    for (size_t i = 3333333; i < 6666666; i++)
    {
        myf = fopen("dataDogs.dat","ab");
        lista.id=i;
        int num=rand() %1717;
        memset(lista.nombre,'\0',32);
        strcpy(lista.nombre,arr_nombres[num]);
        num=rand() %56;
        strcpy(lista.raza,arr_razasgato[num]);
        strcpy(lista.tipo,"Gato\0");
        num=rand() %20;
        lista.edad=num;
        num=rand() %70;
        lista.estatura=num;
        num=rand() %85;
        lista.peso=num;
        num= rand()%2;
        if(num==1){
            lista.sexo='M';
        }else{
            lista.sexo='H';
        }
        lista.hash=PolyHash(lista.nombre);
        fwrite(&lista,sizeof(struct dogType),1,myf);
        fclose(myf);  
    }
    for (size_t i = 6666666; i < 10000001; i++)
    {
        myf = fopen("dataDogs.dat","ab");
        lista.id=i;
        int num=rand() %1717;
        memset(lista.nombre,'\0',32);
        strcpy(lista.nombre,arr_nombres[num]);
        num=rand() %207;
        strcpy(lista.raza,arr_razasconejo[num]);
        strcpy(lista.tipo,"Conejo\0");
        num=rand() %20;
        lista.edad=num;
        num=rand() %70;
        lista.estatura=num;
        num=rand() %85;
        lista.peso=num;
        num= rand()%2;
        if(num==1){
            lista.sexo='M';
        }else{
            lista.sexo='H';
        }
        lista.hash=PolyHash(lista.nombre);
        fwrite(&lista,sizeof(struct dogType),1,myf);
        fclose(myf);  
    }
    /*
    for (size_t i = 0; i < 10; i++)
    {
        printf("Nombre: %s\n",lista[i].nombre);
        printf("Tipo: %s \n",lista[i].tipo);
        printf("Edad: %d \n", lista[i].edad);
        printf("Raza: %s \n", lista[i].raza);
        printf("Estatura: %d \n", lista[i].estatura);
        printf("Peso: %.2f \n", lista[i].peso);
        printf("Sexo: %c \n", lista[i].sexo);
        printf("Hash: %d \n", lista[i].hash);
        
    }*/
    
    
      
    
                   
    return 0;
}
int PolyHash (char cadena[])
{
    int i = 0;    
    while (cadena[i] != '\0') 
    {
        if (cadena[i] >= 'a' && cadena[i] <= 'z') {
            cadena[i] = cadena[i] - 32;
        }
    i++;
    }
    int Hash=0;
    for (int o = 31; o >= 0; o--)
    {
        if(cadena[o] >= 'A' && cadena[o] <= 'Z')Hash=(Hash*31+(int)cadena[o])% 1009;
    }
    if(Hash<0){
        Hash=-Hash;
    }
    return Hash;        
}
