/*
- Cria um arquivo .off para um toro, usando retângulos como faces.

- Ajusta a malha em seu domínio para que não haja sobreposição de pontos e para 
preservar a característica de Euler do toro (que é 0).
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float f(float u, float v){
  return (3*cos(u) + 7)*cos(v);
}

float g(float u, float v){
  return (3*cos(u) + 7)*sin(v);
}

float h(float u, float v){
  return 3*sin(u);
}


void main(){
  int divisoes_x, divisoes_y, vertices, faces, arestas, i, j;
  int indice_0;
  float x_min, x_max, y_min, y_max, xi, yi, passo_x, passo_y;

  divisoes_x = 15;
  divisoes_y = 15;

  //Veja que a característica de Euler é zero (vertices - arestas + faces = 0)
  vertices = divisoes_x*divisoes_y;
  arestas = 3*divisoes_x*divisoes_y;
  faces = 2*divisoes_x*divisoes_y;
  
  x_min = 0;
  x_max = 6.2831853071796;
  y_min = 0;
  y_max = 6.2831853071796;


  printf("OFF\n%i %i %i\n",vertices, faces, arestas);
  

  //VERTICES
  passo_x = (x_max-x_min) / divisoes_x;
  passo_y = (y_max-y_min) / divisoes_y;
  xi = x_min;
  yi = y_min;

  for(i=0; i < divisoes_y; i++){
    for(j=0; j < divisoes_x; j++){
      printf("%f %f %f\n",f(xi,yi), g(xi,yi), h(xi,yi));

      xi += passo_x;
    }
    yi += passo_y;  
    xi = x_min;
  }



  //FACES ORIENTADAS
  indice_0 = 0;


  for(i=0; i < divisoes_y; i++){
    for(j=0; j < divisoes_x; j++){
      
      if(j == divisoes_x-1 && i == divisoes_y-1){
        printf("3 %i %i %i\n", indice_0, divisoes_x - 1, 0);
        printf("3 %i %i %i\n", indice_0, 0, (divisoes_y-1)*divisoes_x);
      }
      
     else if(j == divisoes_x-1){
        printf("3 %i %i %i\n", indice_0 + 1 - divisoes_x, indice_0, indice_0 + divisoes_x);
        printf("3 %i %i %i\n", indice_0 + 1 - divisoes_x, indice_0 + divisoes_x, indice_0 + 1);
      }

      else if(i == divisoes_y-1){
        printf("3 %i %i %i\n", indice_0 + 1, indice_0, j);
        printf("3 %i %i %i\n", indice_0 + 1, j, j + 1);
      }

      else{
        printf("3 %i %i %i\n", indice_0 + 1, indice_0, indice_0 + divisoes_x);
        printf("3 %i %i %i\n", indice_0 + 1, indice_0 + divisoes_x, indice_0 + divisoes_x + 1);
      }

      indice_0 += 1;
    }
  }
}