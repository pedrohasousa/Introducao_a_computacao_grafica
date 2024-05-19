//Implementaçãoo do algoritmo de ordenação merge sort com algumas melhorias

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//A e B são listas ordenadas com a e b elementos, respectivamente
//C é uma lista vazia com a+b espaços, que sera usada para juntar A e B em apenas uma lista ordenada
void merge(float* A, float* B, float* C, int a, int b){
  int i = 0, j = 0, k = 0, acabou = 0;
  
  for(k = 0; k < a + b; k++){
    if(i == a) acabou = 1;
    if(j == b) acabou = 2;

    switch(acabou){
      case 0:
        if(A[i] < B[j]){
          C[k] = A[i];
          i++;
        }
        else{
          C[k] = B[j];
          j++;
        }
        break;
          
      case 1:
        C[k] = B[j];
        j++;
        break;
      
      case 2:
        C[k] = A[i];
        i++;
        break;
    }
  }
}



//A é uma lista com a elementos
//B deve ter a*sizeof(float) de espaço alocado
//i é o indice de A e B a prtir do qual a ordenacao comeca
void merge_sort(float* A, float* B, int a){
  float *C;
  C = malloc(a*sizeof(float));

  if(a == 2){
    float max, min;
    
    max = A[0]; min = A[1];
    if(max < min){
      max = A[1]; min = A[0];
    }

    B[0] = min; B[1] = max;
  }

  else if(a == 1){
    B[0] = A[0]; 
  }

  else{
    merge_sort(A, B, a/2);
    merge_sort(&A[a/2], &B[a/2], a/2 + a%2);
    merge(B, &B[a/2], C, a/2, a/2 + a%2);

    for(int i = 0; i < a; i++)
      B[i] = C[i];

  }
}



//Gera uma lista aleatória de floats e aplica o merge sort nela.
void main(){
  int a = 19, limite = 500;
  float A[a], C[a], b;
  srand(time(NULL));

  for(int i = 0; i < a; i++){
    if(rand()%2 == 1){
      A[i] = rand()%limite;
      
      b = rand()%15;
      if(b) A[i] /= b;
    }
    else{
      A[i] = -rand()%limite;

      b = rand()%15;
      if(b) A[i] /= b;
    }  
  }


  merge_sort(A, C, a);
  

  for(int i = 0; i < a; i++)
    printf("%f\n", C[i]);  
}