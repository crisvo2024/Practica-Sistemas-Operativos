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
    FILE *myf, *tabla;
    struct Dog Actual;
    struct Dog anterior;
    myf = fopen("dataDogs.dat","rb+");
    tabla = fopen("TablaHash.dat","wb");
    int primero[1010],ultimo[1010];
    for (size_t i = 0; i < 1009; i++)
    {
        primero[i]=102;
    }
    for (size_t i = 0; i < 10000001; i++)
    {
        fseek(myf,sizeof(struct Dog)*i,SEEK_SET);
        fread(&Actual,sizeof(struct Dog),1,myf);
        if (primero[Actual.hash]==102)
        {
            primero[Actual.hash]=i;
            ultimo[Actual.hash]=i;
        }else{
            fseek(myf,sizeof(struct Dog)*ultimo[Actual.hash],SEEK_SET);
            fread(&anterior,sizeof(struct Dog),1,myf);
            anterior.next=i;
            fseek(myf,sizeof(struct Dog)*ultimo[Actual.hash],SEEK_SET);
            ultimo[Actual.hash]=i;
            fwrite(&anterior,sizeof(struct Dog),1,myf);
        }
    }
    fwrite(primero,sizeof(primero),1,tabla); 
    for (size_t i = 0; i < 100; i++)
    {
        fseek(myf,sizeof(struct Dog)*i,SEEK_SET);
        fread(&Actual,sizeof(struct Dog),1,myf);
        printf("%d",i);
        printf("Nombre: %s\n",Actual.nombre);
        printf("Tipo: %s \n",Actual.tipo);
        printf("Edad: %d \n", Actual.edad);
        printf("Raza: %s \n", Actual.raza);
        printf("Estatura: %d \n", Actual.estatura);
        printf("Peso: %.2f \n", Actual.peso);
        printf("Sexo: %c \n", Actual.sexo);
        printf("Hash: %d \n", Actual.hash);
        printf("Next: %d \n", Actual.next);
    }
    
    fclose(myf);
    fclose(tabla);   
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