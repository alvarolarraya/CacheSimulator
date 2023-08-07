#include <stdio.h>
#include <stdlib.h>
//tamaño de palabra 48

unsigned long int rangobits(unsigned long int n, int bitmenor, int
bitmayor);
int main()
{
	long entrada = 0x42ad34f1,salida;
	int menor = 0,mayor = 47;
	salida = rangobits(entrada,menor,mayor);
	printf("La salida es: %ld\n", salida);
}

unsigned long int rangobits(unsigned long int n, int bitmenor, int
bitmayor)
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
