//Cria uma figura fechada de n lados a partir de uma sequencia de vertices dada.

/*
  A imagem sera separada em regioes da seguinte forma: dada uma seguencia de vertices p1,...pn,
sera calculado, para cada ponto p da imagem, a soma dos angulos entre os vetores (pi-p) e (p[i+1] - p),
para i=1,...,n , considerando p[n+1] = p0. 

  Uma vez que a figura e fechada, essa soma de angulos resultara em um multiplo de 2*pi radianos,digamos
2*k*pi. Cada valor de k define uma regiao na imagem, por exemplo, k=0 e a parte que esta fora da figura.

  Cada regiao sera preenchida com uma cor, que serao dadas numa lista ordenada de cores: a primeira cor
preenche a regiao 2*0*pi,..., a n-esima cor preenche a regiao 2*(n-1)*pi.

  Por fim, um contorno sera desenhado ao longo das intersecoes de regioes.
*/



#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>


float modulo(float a){
  if(a<0) a=-a;
  return a;
}


//define uma orientacao para o angulo entre u e v
//usando o produto vetorial entre u e v postos em R3 com cota nula.
int orientacao(int u[2], int v[2]){
  if(u[0]*v[1]-u[1]*v[0] >= 0)
    return 1;
  else
    return -1;
}


  // Uma alternativa para essa funcao e o algoritmo de Bresenham
  //a matriz de cores tem dimensoes [regioes][3]
  //a cor de cada pixel p0 e p1 devem estar na lista de cores dada
  //o pixel cuja cor tiver indice mais alto na lista de cores sera pintado com a cor de cor_borda
void pintar_borda(unsigned char *p0, unsigned char *p1, unsigned char (*cores)[3], int regioes, unsigned char *cor_borda){
  int i, cor_p0, cor_p1;

  for(i=0;i<regioes;i++){
    if(p0[0] == cores[i][0] && p0[1] == cores[i][1] && p0[2] == cores[i][2])
      cor_p0 = i;
      break;
  }
  for(i=0;i<regioes;i++){
    if(p1[0] == cores[i][0] && p1[1] == cores[i][1] && p1[2] == cores[i][2])
      cor_p1 = i;
      break;
  }

  if(cor_p0>cor_p1){
    p0[0] = cor_borda[0];
    p0[1] = cor_borda[1];
    p0[2] = cor_borda[2];
  }
  else{
    p1[0] = cor_borda[0];
    p1[1] = cor_borda[1];
    p1[2] = cor_borda[2];
  }
}





int main(void) {
  int i, j, k, l, h;
  unsigned char (**imagem)[3];
  FILE *fp;

  l = 500;
  h = 500;

  imagem=malloc(l*sizeof(char*));
  for(i=0; i<l; i++)
    imagem[i] = malloc(3*h);




  //DEFINICAO DA FIGURA
    //numero de vertices e lista ordenada dos vertices da figura. (atencao com as dimensoes da imagem)

  int vertices = 5;  //colocar esse valor no 1º indice da lista de pontos abaixo
  int pontos[5][2] = {{450,250},{50,250},{350,450},{250,100},{150,450}};
  
  /*
  Exemplos de figuras:
    -hexagono: pontos[6][2] = {{250,30},{50,130},{50,330},{250,440},{450,330},{450,130}} (total de 2 regioes na imagem)
    -estrela: pontos[5][2] = {{450,250},{50,250},{350,450},{250,100},{150,450}} (total de 3 regioes na imagem)
    -figura com mais regioes: pontos[15][2] = {{250,100},{350,150},{250,350},{150,150},{250,130},{350,200},{250,330},{150,200},{250,150},{350,250},{250,300},{150,250},{250,180},{350,275},{150,275}} (total de 5 regioes na imagem)
    -pentagono saindo da tela(imagem 500x500): pontos[5][2] = {{450,50},{250,150},{300,350},{600,350},{650,150}} (total de 2 regioes na imagem)
  */


  //LISTA DE CORES 
    /*
      A variavel "regioes" indica quantas regioes serao consideradas ao pintar a imagem (ver explicacao 
    no inicio do programa).

      A pintura da imagem e feita mod(regioes), entao se o valor de "regioes" for menor que o numero de 
    regioes definidas pela figura desenhada, a lista de cores sera percorrida novamente para pintar as
    regioes extras. (Ex: se "regioes" = 3 mas existem 5 regioes na imagem, entao a regioes 2*3*pi e 2*4*pi, 
    excedentes, serao pintadas como 2*0*pi e 2*1*pi, respectivamente)

      A lista de cores deve ter ao menos o numero de cores dado pela variavel "regioes".
    */

  int regioes=5;  //colocar esse valor no 1º indice da lista de pontos abaixo
  unsigned char cores[5][3] = {{255,255,255},{0,0,255},{0,255,0},{255,255,0},{120,70,150}};

  //cor para as retas que ligam os vértices da figura
  unsigned char contorno[3] = {0,0,0};



  //PINTURA DA IMAGEM
  float angulo, norma0, norma1, cosseno;
  int u[2], v[2], voltas;

  for(j=0; j<h; j++){
    for(i=0; i<l; i++){
      angulo = 0;
      for(k=0; k<vertices; k++){
        u[0] = pontos[k][0]-i;
        u[1] = pontos[k][1]-j;
        v[0] = pontos[(k+1)%vertices][0]-i;
        v[1] = pontos[(k+1)%vertices][1]-j;
        
        
        norma0 = sqrt(u[0]*u[0] + u[1]*u[1]);
        norma1 = sqrt(v[0]*v[0] + v[1]*v[1]);

        //quando (i,j) for um vertice, ou u ou v podera ter norma 0, gerando erro no calculo do cosseno.
        //como os vertices sao pontos da borda, pintamos ele com a cor da borda e seguimos para o proximo pixel
        if(norma0==0 || norma1==0){
          imagem[i][j][0] = contorno[0];
          imagem[i][j][1] = contorno[1];
          imagem[i][j][2] = contorno[2];
          break;
        }  

        //devido as aproximacoes, o valor dessa expressao para o cosseno pode sair (um pouco) do intervalo [-1,1], gerando erro no acos.
        cosseno = (u[0]*v[0] + u[1]*v[1])/(norma0*norma1);
        if(cosseno>1) cosseno = 1;
        else if(cosseno<-1) cosseno = -1;
        
        //apos todas as iteracoes desse for, o angulo sera aproximadamente um multiplo de 2*pi.
        angulo += orientacao(u,v)*acos(cosseno);
      }
      
      voltas = modulo(angulo)/(2*M_PI) + 0.5;
    
      imagem[i][j][0] = cores[voltas%regioes][0];
      imagem[i][j][1] = cores[voltas%regioes][1];
      imagem[i][j][2] = cores[voltas%regioes][2];
    }   
  }
  
 
  //RETAS PARA LIGAR OS VERTICES
  unsigned char red_abaixo, red_direita;
  unsigned char green_abaixo, green_direita;
  unsigned char blue_abaixo, blue_direita;

  //detecta as bordas da figura (fronteiras entre regioes) e pinta com a cor de contorno escolhida.
  for(j=0; j<h-1; j++){
    for(i=0; i<l-1; i++){
      if(imagem[i][j][0]==contorno[0] && imagem[i][j][1]==contorno[1] && imagem[i][j][2]==contorno[2])
        continue;

      red_direita = imagem[i+1][j][0];
      green_direita = imagem[i+1][j][1];
      blue_direita = imagem[i+1][j][2];

      red_abaixo = imagem[i][j+1][0];
      green_abaixo = imagem[i][j+1][1];
      blue_abaixo = imagem[i][j+1][2];

      if(imagem[i][j][0]!=red_direita || imagem[i][j][1]!=green_direita || imagem[i][j][2]!=blue_direita)
        if(red_direita!=contorno[0] || green_direita!=contorno[1] || blue_direita!=contorno[2])
          pintar_borda(imagem[i][j], imagem[i+1][j], cores, regioes, contorno);


      if(imagem[i][j][0]!=red_abaixo || imagem[i][j][1]!=green_abaixo || imagem[i][j][2]!=blue_abaixo)
        if(red_abaixo!=contorno[0] || green_abaixo!=contorno[1] || blue_abaixo!=contorno[2])
          pintar_borda(imagem[i][j], imagem[i][j+1], cores, regioes, contorno);
    }
  }





  fp=fopen("poligono.ppm","w");
  fprintf(fp,"P6\n%u %u\n255\n",l,h);

  for(j=0; j<h; j++)
    for(i=0; i<l; i++)
      fprintf(fp,"%c%c%c",imagem[i][j][0],imagem[i][j][1],imagem[i][j][2]);
  


  for(i=0;i<l;i++)
    free(imagem[i]);
  free(imagem);

  return 0;
}