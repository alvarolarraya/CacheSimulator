#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

unsigned long int rangobits(unsigned long int, int, int);
void actualizarTrasFallo (unsigned long int[], int,unsigned long int);
int primeraPosLibre (unsigned long int[]);
bool estaEnCache (unsigned long int[], int, int);
bool yaEstaActualizadaTrasAcierto(unsigned long int[], int, int);
void actualizarTrasAcierto(unsigned long int [], int, int);
char** fragmenta(const char* cadena);

int main()
{
	int accesosMC = 0, fallosMC = 0;
	float tasaFallos;
	FILE* fichero;
    char texto[100];
	char ** cifra;
    if((fichero = fopen("config.txt","r")) == NULL)
        perror("ha habido un fallo abriendo la traza");
    fgets(texto,100,fichero);
	cifra = fragmenta(texto);
	int Nlin = atoi(cifra[1]);
	fgets(texto,100,fichero);
	cifra = fragmenta(texto);
	int Tlin = atoi(cifra[1]);
	fgets(texto,100,fichero);
	cifra = fragmenta(texto);
	int Asoc = atoi(cifra[1]);
	fgets(texto,100,fichero);
	cifra = fragmenta(texto);
	int lineasVictim = atoi(cifra[1]);
	fclose(fichero);
	int numeroConjuntos = Nlin / Asoc;
	unsigned long int cache[numeroConjuntos][Asoc];
	for(int i = 0; i < numeroConjuntos; i++)
		for(int j = 0; j < Asoc; j++)
			cache[i][j] = -1;
	unsigned long int victimCache[lineasVictim];
	for(int i = 0; i < lineasVictim; i++)
		victimCache[i] = -1;
	unsigned long int direccion, linea, conjunto;
    if((fichero = fopen("traza.txt","r")) == NULL)
    	perror("ha habido un fallo abriendo la traza");
	int bitInicialConjunto, bitFinalConjunto;
	bitInicialConjunto = log(Tlin) / log(2);
	bitFinalConjunto = bitInicialConjunto + (log(numeroConjuntos) / log(2)) - 1;
	fscanf(fichero, "%lx", &direccion);
	while(feof(fichero) == 0)
    {
		linea = rangobits(direccion, bitInicialConjunto, 47);
		conjunto = rangobits(direccion, bitInicialConjunto, bitFinalConjunto);
		accesosMC++;
		if(estaEnCache(cache[conjunto], linea, Asoc))
		{
			if(!yaEstaActualizadaTrasAcierto(cache[conjunto], linea, Asoc))
				actualizarTrasAcierto(cache[conjunto], linea, Asoc);
		}
		else if(!estaEnCache(cache[conjunto], linea, Asoc) && lineasVictim != 0)
		{
			//puede haber lineas repetidas en MC y VC, pero creo que da igual
			actualizarTrasFallo(victimCache, lineasVictim, cache[conjunto][0]);
			actualizarTrasFallo(cache[conjunto], Asoc, linea);
			if(!estaEnCache(victimCache, linea, lineasVictim))
				fallosMC++;
		}
		else if(!estaEnCache(cache[conjunto], linea, Asoc) && lineasVictim == 0)
		{
			fallosMC++;
			actualizarTrasFallo(cache[conjunto], Asoc, linea);
		}
        fscanf(fichero, "%lx", &direccion);
    }
	tasaFallos = ((float) fallosMC / accesosMC) * 100;
    fclose(fichero);
	printf("El programa accede %d veces a memoria y falla %d de ellas.\n", accesosMC, fallosMC);
	printf("Esta caché, al ejecutar este programa tiene una tasa de fallos de %.2f por ciento.\n", tasaFallos);
    return 0;
}

unsigned long int rangobits(unsigned long int n, int bitmenor, int bitmayor)
{
	unsigned long int bit2=1, bit1=1;
	if (bitmenor < 0 || bitmenor > 47) {
		printf("Error en Bitmenor: %d\n", bitmenor);
		exit(0);
	}
	else if (bitmayor < 0 || bitmayor > 47) {
		printf("Error en Bitmayor: %d\n", bitmayor);
		exit(0);
	}
	else if (bitmayor < bitmenor) {
		printf("Error en Orden\n");
		exit(0);
	}
	else if(bitmayor-bitmenor+1 < 48 ) {
		/* calcula valor 2 elevado a bitmenor */
		bit1= bit1 << bitmenor;
		/* calcula valor 2 elevado a (bitmayor-bitmenor + 1) **/
		bit2 =bit2 << (bitmayor-bitmenor + 1);
		n= n/bit1;
		n= n%bit2;
	}
	return n;
}

bool estaEnCache (unsigned long int cache[], int direccion, int Nlin)
{
	int i = 0;
	while(i < Nlin-1 && cache[i] != direccion)
		i++;
	if(cache[i] == direccion)
		return true;
	else
		return false;
}

bool yaEstaActualizadaTrasAcierto(unsigned long int cache[], int direccion, int Nlin)
{
	int i = Nlin-1;
	while (i>-1 && cache[i] == -1)
		i--;
	if(cache[i] == direccion)
		return true;
	else
		return false;
}

void actualizarTrasAcierto(unsigned long int *cache, int direccion, int Nlin)
{
	int i = 0;
	while(cache[i] != direccion)
		i++;
	int j = Nlin-1;
	while (j>-1 && cache[j] == -1)
		j--;
	while(i < j+1)
	{
		cache[i] = cache[i+1];
		i++;
	}
	cache[j] = direccion;
}

void actualizarTrasFallo (unsigned long int cache[], int Nlin,unsigned long int nuevaDireccion)
{
	if(cache[Nlin-1] == -1)
	{
		unsigned long int primeraLibre = primeraPosLibre(cache);
		cache[primeraLibre] = nuevaDireccion;
	}
	else
	{
		for(int i = 1;i < Nlin; i++)
			cache[i-1] = cache[i];
		cache[Nlin-1] = nuevaDireccion;
	}
}

int primeraPosLibre (unsigned long int cache[])
{
	int i = 0;
	while(cache[i] != -1)
	{
		i++;
	}
	return i;
}

char** fragmenta(const char* cadena)
{
    int cuantosCaracteres = strlen(cadena),cuantasPalabras = 0,i = 0;
    char* cadenaAuxiliar;
    while(i<cuantosCaracteres)
    {
        if(cadena[i] == ' ')
        {
            while(cadena[i] == ' ')
                i++;
            cuantasPalabras++;
        }
        i++;
    }
    if ((cadenaAuxiliar = malloc((cuantosCaracteres)*sizeof(char))) == NULL)
        perror("no hay memoria para reservar la cadena auxiliar en fragmenta\n");
    i = 0;
    while(cadena[i] != '\n')
    {
        cadenaAuxiliar[i] = cadena[i];
        i++;
    }
    cadenaAuxiliar[i] = ' ';
    char** vectorDeCadenas;
    if ((vectorDeCadenas = malloc(cuantasPalabras*sizeof(char*))) == NULL)
        perror("no hay memoria para reservar el vector de cadenas en fragmenta\n");
    char* argumento;
    if ((argumento = malloc(sizeof(char*))) == NULL)
        perror("no hay memoria para reservar el argumento en fragmenta\n");
    argumento = strtok(cadenaAuxiliar," ");
    vectorDeCadenas[0] = argumento;
    i = 1;
    while(argumento != NULL)
    {
        argumento = strtok(NULL," ");
        vectorDeCadenas[i] = argumento;
        i++;
    }
    free(argumento);
    return vectorDeCadenas;
}
