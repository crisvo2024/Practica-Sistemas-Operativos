#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Dog{
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
    struct Dog lista;
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
        fgets(arr_nombres[i], 31, (FILE*) nombres);
    }
    
    fclose(nombres);
    for(size_t i = 0; i < 242; i++)
    {
        memset(arr_razas[i],'\0',16);
        fgets(arr_razas[i], 15, (FILE*) razas);
    }
    
    fclose(razas);
    for(size_t i = 0; i < 56; i++)
    {
        memset(arr_razasgato[i],'\0',16);
        fgets(arr_razasgato[i], 15, (FILE*) razasgato);
    }
    
    fclose(razasgato);
    for(size_t i = 0; i < 207; i++)
    {
        memset(arr_razasconejo[i],'\0',16);
        fgets(arr_razasconejo[i], 15, (FILE*) razasconejo);
    }
    srand(time(NULL));
    fclose(razasconejo);
    for (size_t i = 0; i < 33; i++)
    {
        myf = fopen("dataDogsTest.dat","ab");
        lista.id=i;
        int num=rand() %1717;
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
        fwrite(&lista,sizeof(struct Dog),1,myf);
        lista.id=i+100;
        fwrite(&lista,sizeof(struct Dog),1,myf);
        fclose(myf);  
    }
    for (size_t i = 33; i < 66; i++)
    {
        myf = fopen("dataDogsTest.dat","ab");
        lista.id=i;
        int num=rand() %1717;
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
        fwrite(&lista,sizeof(struct Dog),1,myf);
        lista.id=i+100;
        fwrite(&lista,sizeof(struct Dog),1,myf);
        fclose(myf);  
    }
    for (size_t i = 66; i < 101; i++)
    {
        myf = fopen("dataDogsTest.dat","ab");
        lista.id=i;
        int num=rand() %1717;
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
        fwrite(&lista,sizeof(struct Dog),1,myf);
        lista.id=i+100;
        fwrite(&lista,sizeof(struct Dog),1,myf);
        fclose(myf);  
    }          
    return 0;
}
int PolyHash (char cadena[])
    {
        int Hash=0;
        for (size_t o = 31; o > 0; o--)
        {
            Hash=(Hash*38+(int)cadena[o])% 1009;
        }
        Hash=(Hash*38+(int)cadena[0])% 1009;
        return Hash;        
    }