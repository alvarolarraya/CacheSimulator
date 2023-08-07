#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    FILE* fichero;
    unsigned long int direccion;
    if((fichero = fopen("traza.txt","r")) == NULL)
    perror("ha habido un fallo abriendo la traza");
    fscanf(fichero, "%lx", &direccion);
    while(feof(fichero) == 0)
    {
        printf("%ld\n", direccion);
        fscanf(fichero, "%lx", &direccion);
    }
    fclose(fichero);
    return 0;
}
