#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

int main(int argc, char const *argv[])
{
    FILE *myf, *tabla;
    struct dogType Actual;
    struct dogType anterior;
    myf = fopen("dataDogs.dat","rb+");
    tabla = fopen("TablaHash.dat","wb");
    int primero[1010],ultimo[1010];
    for (size_t i = 0; i < 1010; i++)
    {
        primero[i]=-1;
    }
    for (size_t i = 0; i < 10000001; i++)
    {
        fseek(myf,sizeof(struct dogType)*i,SEEK_SET);
        fread(&Actual,sizeof(struct dogType),1,myf);
        if (primero[Actual.hash]==-1)
        {
            primero[Actual.hash]=i;
            ultimo[Actual.hash]=i;
        }else{
            fseek(myf,sizeof(struct dogType)*ultimo[Actual.hash],SEEK_SET);
            fread(&anterior,sizeof(struct dogType),1,myf);
            anterior.next=i;
            fseek(myf,sizeof(struct dogType)*ultimo[Actual.hash],SEEK_SET);
            ultimo[Actual.hash]=i;
            fwrite(&anterior,sizeof(struct dogType),1,myf);
        }
    }
    fwrite(primero,sizeof(primero),1,tabla); 
    fclose(myf);
    fclose(tabla);   
}

