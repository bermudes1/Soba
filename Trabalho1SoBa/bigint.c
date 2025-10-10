/* Felipe Bermudes de Oliveira Leite 2421786 3WA*/
/* Thiago Von Randow Fernandes 2420001 3WA*/

#include "bigint.h"
#include <stdio.h>

#define NUM_BITS 128
typedef unsigned char BigInt[NUM_BITS / 8];

/* criamos o res = val (extensao com sinal) seguido por uma variavel de contagem 
Percorremos o long colocando os bytes menos significativos vao para as primeiras 
posições do array e o Right shift (com sinal) passa para o próximo byte do long */

void big_val(BigInt res, long val) {
  
	int i = 0;
  

  for (; i < sizeof(val) ; i++) {


    res[i] = val | 0x00; 


    val >>= 8;      
  }
  
  /*os casos que byte mais significativo passado para res for 1, as 8 primeiras posições 
  do array  serão preenchidas com esse byte, se não, preenchemos com 0 */
  
  if((res[7] & 0x80) == 0x80) { 

     for(; i < (NUM_BITS/8) ; i++){

        res[i] = 0xFF;

     }

  }
     
     
  else{

     for(; i < (NUM_BITS/8) ; i++){

        res[i] = 0x00;

     }
      
  }

}

void big_comp2(BigInt res, BigInt a) {

/*Passamos os bytes para o array de resposta, em seguida invertemos
todos os bits de cada byte do array de resposta*/

  for (int i = 0; i < (NUM_BITS / 8) ; i++) {

    res[i] = a[i];

  }

  for (int i = 0; i < (NUM_BITS / 8); i++) {

    res[i] = ~res[i];

  }

  for (int i = 0; i < (NUM_BITS / 8) ; i++) {

/* Se o byte for 0xFF sua organização em binário é 1111 1111, se somarmos 1,sabemos que
 1 + 1 = 0,logo se fizermos isso neste número o resultado será 0000 0000, que equivale a 
 0x00 em hexadecimal */

    if (res[i] == 0xFF) {

    	res[i] = 0x00;

    }

/*Caso contrário, guardamos no byte o resultado da sua soma com 1 e paramos o loop */

    else {

      res[i]++;

      break;

    }

  }
}

/* res = a + b */

void big_sum(BigInt res, BigInt a, BigInt b) {

/*criamos uma variavel para guardar o valor do bit mais significativo de cada byte e para controlar o 
overflow da soma, alem disso, outra para guardar o resultado da soma dos bytes (um short tem 2 bytes, 
então usamos um short para guardar o resultado da soma de 2 bytes)
para só ai percorrer todo o array para fazer a soma dos bytes */

  unsigned char overflow = 0;


  unsigned short soma;


  for (int i = 0; i < (NUM_BITS/8) ; i++) {

/* Faz a soma dos bytes com o overflow e guarda no short soma */

    soma = a[i] + b[i] + overflow;

/* Pega o byte menos significativo do resultado da soma e guarda no array de resposta das menores até as maiores posições */

    res[i] = soma & 0xFF;

/* Guarda o valor do bit mais significativo da soma para controlar o overflow que sera passado para o próximo byte */

    overflow = soma >> 8;
  }

}

/* res = a - b */

void big_sub(BigInt res, BigInt a, BigInt b) {

/*A subtração pode ser pensada como a + (-b) para isso, 
fazemos a soma de a com o complemento a 2 de b Usei um array BigInt diferente para fazer
o complemento a 2 para não alterar o array original passado como parâmetro */
  
  BigInt b2;
    
/*Guarda o resultado do complemento a 2 de b em um novo array BigInt b2 usando a função 
big_comp2*/
  
  big_comp2(b2, b);
    
/*O novo array é somado a a e guarda o resultado no array de resposta usando a função
big_sum */
    
  big_sum(res, a, b2);
   
}

/* res = a * b */

void big_mul(BigInt res, BigInt a, BigInt b) {

/*Variável int para guarda a multiplicação dos 2 bytes */

  unsigned int mult;

/*Inicializa o array de resposta com 0 para que posteriormente possamos alocar os bytes*/

  for(int i = 0; i < (NUM_BITS/8) ; i++){

    res[i] = 0x00;

  }

/*Percorre todo o array para fazer a multiplicação dos bytes */

  for(int i = 0; i < (NUM_BITS/8) ; i++){
        
    for(int j = 0; j < (NUM_BITS/8) ; j++){
          
      if((i+j) < (NUM_BITS/8)){
            
    /* Multiplica os bytes e guarda em uma variavel short (2 bytes, 1 byte para cada byte multiplicado),
    guardamos o byte menos significativo do resultado da multiplicação no array de resposta e
    depois o byte mais significativo do resultado da multiplicação no array de resposta */
            
        mult = a[i] * b[j];
            
            
        res[i+j] += mult & 0xFF;
            
            
        res[i+j+1] += mult >> 8;
      }
    
    }
  }

    /*tratamos o overflow */

  for(int i = 0; i < (NUM_BITS/8) -1 ; i++){

    /*Guarda o byte menos significativo do resultado da multiplicação no array de resposta*/
        
    res[i] &= 0xFF;

    /*Soma na proxima posição do array de resposta o bit mais significativo do byte anterior
    na posição de bit menos significativo para permitir futuras somas, já que os outros 
    bits estarão todos em 0 */

    res[i+1] += res[i] >> 8;
  }

}

/* res = a << n */

void big_shl(BigInt res, BigInt a, int n) {

/*contadora*/
   
   int j = 0;

/*Variáveis para guardar o byte a ser passado nas operações */

   unsigned char shiftRight,anterior=0x00;

/*Passando os bytes recebidos para o array de resposta */

   for (int i = 0; i < (NUM_BITS/8); i++) {
      res[i] = a[i];
    }

/*Para o número se shifts de 8 completos, preenchemos os bytes a direita com 0 do byte mais 
significativo para o byte menos significativo */

   
   for(; j < (n/8); j++){
   
    for(int k = (NUM_BITS/8) - 1; k >= 0; k--){

      /*se for o primeiro byte, preenchemos com 0 */
   	
   		if (k == 0){
   			res[k] = 0x00;
   		}

      /* Passamos os bytes para a esquerda */
   		
   		else{
   			res[k] = res[k-1];
   		}
   	
   	}
   	
  }

/* Caso a quantidade de shifts não seja um múltiplo de 8,continuamos o deslocamento */
   
   
  if ((n % 8) != 0){

    /*continuamos o shift do multiplo de 8 */
    
    for(;j < (NUM_BITS/8); j++){

    /* Guardamos o bit mais significativo do byte anterior com o shift de quanto "Falta" 
    para chegar a 8, o byte menos significativo do byte atual com o shift de quanto "Sobra" 
    para chegar a 8 e Fazemos uma operação  de ou aritmético entre o byte atual e o byte 
    anterior para juntar os bits que foram separados*/
    	
    	shiftRight= res[j] >> (8-(n % 8));
   	
    	res[j] <<= (n%8);
    		
    	res[j] |= anterior;

    /* Definimos o que passar para o próximo byte, guardando o valor de quanto "Falta" para 
    chegar a 8 na variável anterior */
    	
    	anterior = shiftRight;
    	
    }
    
  }
  
}

/* res = a >> n (logico) */

void big_shr(BigInt res, BigInt a, int n){

  /*outro contador*/

  int j = 0;

/*(vai guardar o byte a ser passado nas operações)*/

  unsigned char shiftLeft, posterior = 0x00;

/* Passando os bytes recebidos para o array de resposta */

  for (int i = 0; i < (NUM_BITS/8); i++){
    res[i] = a[i]; 
  }

/* Para o número se shifts de 8 completos, passamos os bytes para a esquerda do byte menos 
para o mais siginificativo do array */

  for (int i = 0; i < (NUM_BITS/8); i++){

    for (; j < (n / 8); j++) {

        for (int k = 0; k  < (NUM_BITS/8); k++) {

          res[k] = res[k + 1];

        }
      }
  }

/*Caso a quantidade de shifts não seja um múltiplo de 8, continuamos o deslocamento */
    
  if ((n % 8) != 0){

/*Do byte mais significativo para o menos significativo */

    for (j = (NUM_BITS/8) - 1; j >= 0; j--){

    /* Guardamos o bit menos significativo do byte posterior com o shift de quanto "Falta" 
    para chegar a 8, o byte mais significativo do byte atual com o shift de quanto 
    "Sobra" para chegar a 8 e fazemos uma operação de ou aritmético entre o byte atual e o 
    byte posterior para juntar os bits que foram separados */

      shiftLeft = res[j] << (8 - (n % 8));


      res[j] >>= (n%8);

      
      res[j] |= posterior;

    /* Definimos o que passar para o byte anterior, guardando o valor de quanto "Falta" para chegar a 8 na variável posterior */

      posterior = shiftLeft;

    }
  }

}
    
/* res = a >> n (aritmetico) */

void big_sar(BigInt res, BigInt a, int n){

  /*cont*/

  int i = 0;

  /* Passamos os bytes recebidos como parâmetro para o array de resposta */

  for (; i < (NUM_BITS/8) ; i++) {
    res[i] = a[i];
  }

  /*número de shifts*/

  for (i = 0; i < n ; i++) {

    /* Para cada byte do menos significativo para o mais significativo do array, no byte atual,
    fazemos um shift a direita de 1 bit para adicionar o bit mais significativo do próximo 
    byte e depois guardamos o bit menos significativo do byte seguinte com o 
    shift de 7 e fazemos uma operação de ou aritmético com o byte atual para juntar os bits 
    que foram separados */

    for (int j = 0; j < (NUM_BITS/8) - 1 ; j++) {


      res[j] >>= 1;


      res[j] |= (res[j+1] & 0x01) << 7;
    }

    /* Caso seja o último byte, fazemos um shift a direita de 1 bit para adicionar o bit mais significativo dele mesmo*/ 
    
    res[(NUM_BITS/8) - 1] >>= 1;

    res[(NUM_BITS/8) - 1] |= (res[(NUM_BITS/8) - 1] & 0x40) << 1;
  }
  
}