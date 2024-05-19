//Cria um arquivo .off para uma faixa de mobius, cujas faces são triângulos
//Usa uma malha específica, que fica uniforme ao longo de toda a superfície, inclusive na 'colagem'.
//A característica de Euler da faixa é respeitada.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define pi 3.1415926535898

float f(float u, float v){
  return (1 + (v/2)*cos(u/2)) * cos(u);
}

float g(float u, float v){
  return (1 + (v/2)*cos(u/2)) * sin(u);
}

float h(float u, float v){
  return (v/2)*sin(u/2);
}


void main(){
  int divisoes_u, divisoes_v, vertices, faces, arestas, i, j;
  float u_min, u_max, v_min, v_max, ui, vi, passo_u, passo_v;

  divisoes_u = 100;
  divisoes_v = 2;
  vertices = (divisoes_v+1) * divisoes_u;
  arestas = 3*divisoes_u*divisoes_v + divisoes_u;
  faces = 2*divisoes_u*divisoes_v;
  
  u_min = 0;
  u_max = 2*pi;
  v_min = -1;
  v_max = 1;


  printf("OFF\n%i %i %i\n",vertices, faces, arestas);
  

  //VERTICES
  passo_u = (u_max-u_min) / divisoes_u;
  passo_v = (v_max-v_min) / divisoes_v;
  ui = u_min;
  vi = v_min;

  for(i=0; i <= divisoes_v; i++){
    for(j=0; j < divisoes_u; j++){
      printf("%f %f %f\n",f(ui,vi), g(ui,vi), h(ui,vi));

      ui += passo_u;
    }
    vi += passo_v;  
    ui = u_min;
  }



  //FACES ORIENTADAS
  for(i=0; i <= (divisoes_u*divisoes_v - 1)/2 ; i++){

    if(i%2 != 0){
      if((i+1)%divisoes_u == 0){
        printf("3 %i %i %i\n", i, divisoes_u*divisoes_v - i + divisoes_u - 1 , divisoes_v*divisoes_u - i - 1);
        printf("3 %i %i %i\n", i, divisoes_v*divisoes_u - i - 1, i + divisoes_u);
      }  

      else{
        printf("3 %i %i %i\n", i, i + 1, i + divisoes_u + 1);
        printf("3 %i %i %i\n", i, i + divisoes_u + 1, i + divisoes_u);
      }  
    }

    else{
      if((i+1)%divisoes_u == 0){
        printf("3 %i %i %i\n", i, divisoes_v*divisoes_u - i + divisoes_u - 1, i + divisoes_u);
        printf("3 %i %i %i\n", divisoes_v*divisoes_u - i + divisoes_u - 1,  divisoes_u*divisoes_v - i - 1, divisoes_u + i);
      }

      else{
        printf("3 %i %i %i\n", i, i + 1, i + divisoes_u);
        printf("3 %i %i %i\n", i + 1, i + divisoes_u + 1, i + divisoes_u);
      }  
    }


  }




  for(i=(divisoes_u*divisoes_v - 1)/2 + 1; i <= divisoes_u*divisoes_v - 1 ; i++){

    if(i%2 == 0){
      if((i+1)%divisoes_u == 0){
        printf("3 %i %i %i\n", i, divisoes_u*divisoes_v - i + divisoes_u - 1 , divisoes_v*divisoes_u - i - 1);
        printf("3 %i %i %i\n", i, divisoes_v*divisoes_u - i - 1, i + divisoes_u);
      }  

      else{
        printf("3 %i %i %i\n", i, i + 1, i + divisoes_u + 1);
        printf("3 %i %i %i\n", i, i + divisoes_u + 1, i + divisoes_u);
      }  
    }

    else{
      if((i+1)%divisoes_u == 0){
        printf("3 %i %i %i\n", i, divisoes_v*divisoes_u - i + divisoes_u - 1, i + divisoes_u);
        printf("3 %i %i %i\n", divisoes_v*divisoes_u - i + divisoes_u - 1,  divisoes_u*divisoes_v - i - 1, divisoes_u + i);
      }

      else{
        printf("3 %i %i %i\n", i, i + 1, i + divisoes_u);
        printf("3 %i %i %i\n", i + 1, i + divisoes_u + 1, i + divisoes_u);
      }  
    }


  }
}
