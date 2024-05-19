/*
- Cria um arquivo .off para uma esfera usando retângulos como faces, exceto para
as faces que têm uma das singularidades como vértice (essas são triângulos). 

- Ajusta a malha em seu domínio para que não haja sobreposição de pontos e para 
preservar a característica de Euler da esfera (que é 2).
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float f(float u, float v){
  return 5*sin(u)*cos(v);
}

float g(float u, float v){
  return 5*sin(u)*sin(v);
}

float h(float u, float v){
  return 5*cos(u);
}


void main(){
  int divisoes_x, divisoes_y, vertices, faces, arestas, i, j;
  float x_min, x_max, y_min, y_max, xi, yi, passo_x, passo_y;

  divisoes_x = 20;
  divisoes_y = 20;

  vertices = divisoes_x*divisoes_y - divisoes_y + 2;
  arestas = 2*divisoes_x*divisoes_y - divisoes_y;
  faces = divisoes_x*divisoes_y;
  
  x_min = 0;
  x_max = 3.1415926535898;
  y_min = 0;
  y_max = 6.2831853071796;


  printf("OFF\n%i %i %i\n",vertices, faces, arestas);
  

  //VERTICES
  passo_x = (x_max-x_min) / divisoes_x;
  passo_y = (y_max-y_min) / divisoes_y;
  xi = x_min;
  yi = y_min;

  for(i=0; i <= divisoes_x; i++){
    for(j=0; j < divisoes_y; j++){
      if(i == 0 || i == divisoes_x){
        printf("%f %f %f\n", f(xi,yi), g(xi,yi), h(xi,yi));
        break;
      }
      printf("%f %f %f\n",f(xi,yi), g(xi,yi), h(xi,yi));

      yi += passo_y;
    }
    xi += passo_x;  
    yi = y_min;
  }


  //FACES ORIENTADAS
  for(i=1; i <= (divisoes_x - 1)*divisoes_y; i++){
    if(i < divisoes_y) 
      printf("3 %i %i %i\n", 0, i, i+1);

    else if(i == divisoes_y)
      printf("3 %i %i %i\n", 0, i, 1);

    else if(i%divisoes_y == 0)
      printf("4 %i %i %i %i\n", i - divisoes_y, i, i - divisoes_y + 1, i - 2*divisoes_y + 1);

    else if(i < (divisoes_x - 1)*divisoes_y)
      printf("4 %i %i %i %i\n", i - divisoes_y, i, i + 1, i - divisoes_y + 1);
  }


  for(i = (divisoes_x - 2)*divisoes_y + 1; i <= (divisoes_x - 1)*divisoes_y; i++){
    if(i == (divisoes_x - 1)*divisoes_y)
      printf("3 %i %i %i\n", (divisoes_x - 2)*divisoes_y + 1, i,  divisoes_x*divisoes_y - divisoes_y + 1);

    else
      printf("3 %i %i %i\n", i, divisoes_x*divisoes_y - divisoes_y + 1, i+1);

  }
}