/*
- Cria um arquivo .off qualquer, determinado pelas funcoes f(u,v), g(u,v) e h(u,v)
a partir de um domínio retangular.

- A característica de Euler da figura não é considerada, sendo mantida como 1 (que
é a característica de um plano).
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float f(float u, float v){
  return u;
}

float g(float u, float v){
  return v;
}

float h(float u, float v){
  return u*u + v*v;
}


void main(){
  int divisoes_x, divisoes_y, vertices, faces, arestas, i, j;
  int indice_0, indice_1;
  float x_min, x_max, y_min, y_max, xi, yi, passo_x, passo_y;

  divisoes_x = 15;
  divisoes_y = 15;
  vertices = (divisoes_x+1) * (divisoes_y+1);
  arestas = 3*divisoes_x*divisoes_y+ divisoes_x + divisoes_y;
  faces = 2*divisoes_x*divisoes_y;
  
  x_min = -0.7;
  x_max = 0.7;
  y_min = -0.7;
  y_max = 0.7;


  printf("OFF\n%i %i %i\n",vertices, faces, arestas);
  

  //VERTICES
  passo_x = (x_max-x_min) / divisoes_x;
  passo_y = (y_max-y_min) / divisoes_y;
  xi = x_min;
  yi = y_min;

  for(i=0; i <= divisoes_y; i++){
    for(j=0; j <= divisoes_x; j++){
      printf("%f %f %f\n",f(xi,yi), g(xi,yi), h(xi,yi));

      xi += passo_x;
    }
    yi += passo_y;  
    xi = x_min;
  }



  //FACES ORIENTADAS
  indice_0 = 0;
  indice_1 = divisoes_x + 2;

  for(i=0; i < divisoes_y; i++){
    for(j=0; j < divisoes_x; j++){
      printf("3 %i %i %i\n", indice_0, indice_0 + 1, indice_1);
      printf("3 %i %i %i\n", indice_0, indice_1, indice_1 - 1);
      indice_0 += 1;
      indice_1 += 1;
    }
  indice_0 += 1;
  indice_1 += 1;
  }  
}