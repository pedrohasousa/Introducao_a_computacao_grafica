//Implementecao do algoritmo de Bresenhan para retas e gerador de retas aletorias em uma imagem .ppm

#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int abs(int a){
  if(a<0) a = -a;
  return a;
}


//a equacao da reta e x*dy - y*dx + c = 0, mas c nao e usado no algoritmo
//escolhemos ponto inicial e final de forma que a reta va semore da esquerda para direita, para reduzir o numero de casos
void reta(unsigned char (**imagem)[3], int *p0, int *p1, unsigned char *cor){
  int d, inicio[2], fim[2], x, y;
  float m, dx, dy;
  
  inicio[0] = p0[0]; inicio[1] = p0[1];
  fim[0] = p1[0]; fim[1] = p1[1];

  if(p1[0] < p0[0]){
    inicio[0] = p1[0]; inicio[1] = p1[1];
    fim[0] = p0[0]; fim[1] = p0[1];
  }
  
  dx = fim[0] - inicio[0];
  dy = fim[1] - inicio[1];
  m = dy/dx;
  
  if(m >= 0 && m <= 1){
    int incrE, incrNE;
    y = inicio[1];
    d = 2*dy - dx;
    incrE = 2*dy;
    incrNE = 2*(dy-dx);
    
    for(x = inicio[0]; x <= fim[0]; x++){
      imagem[x][y][0] = cor[0]; imagem[x][y][1] = cor[1]; imagem[x][y][2] = cor[2];

      if(d > 0){
        d += incrNE;
        y++;
      }
      else d += incrE;
    }
  }

  else if(m > 1){
    int incrN, incrNE;
    x = inicio[0];
    d = dy - 2*dx;
    incrN = -2*dx;
    incrNE = 2*(dy-dx);
    
    for(y = inicio[1]; y <= fim[1]; y++){
      imagem[x][y][0] = cor[0]; imagem[x][y][1] = cor[1]; imagem[x][y][2] = cor[2];

      if(d <= 0){
        d += incrNE;
        x++;
      }
      else d += incrN;
    }
  }

  else if(m >= -1){
    int incrE, incrSE;
    y = inicio[1];
    d = 2*dy + dx;
    incrE = 2*dy;
    incrSE = 2*(dy+dx);
    
    for(x = inicio[0]; x <= fim[0]; x++){
      imagem[x][y][0] = cor[0]; imagem[x][y][1] = cor[1]; imagem[x][y][2] = cor[2];

      if(d < 0){
        d += incrSE;
        y--;
      }
      else d += incrE;
    }
  }

  else{

    int incrS, incrSE;
    x = inicio[0];
    d = dy + 2*dx;
    incrS = 2*dx;
    incrSE = 2*(dy+dx);
    
    for(y = inicio[1]; y >= fim[1]; y--){
      imagem[x][y][0] = cor[0]; imagem[x][y][1] = cor[1]; imagem[x][y][2] = cor[2];

      if(d > 0){
        d += incrSE;
        x++;
      }
      else d += incrS;
    }
  }
}







void main(){
  int i, j, l, h;
  FILE *fp;
  srand(time(NULL));
  unsigned char (**imagem)[3], cor[3] = {12,200,28};
  
  l = 200;
  h = 200;

  imagem = malloc(l*sizeof(unsigned char*));
  for(i = 0; i < l; i++)
    imagem[i] = malloc(3*h*sizeof(unsigned char));

  for(j=0; j<h; j++)
    for(i=0; i<l; i++)
      imagem[i][j][0] = imagem[i][j][1] = imagem[i][j][2] = 255;  

  
  int p0[2], p1[2];

  for(i = 0; i < 30; i++){
    p0[0] = abs(rand())%200; p0[1] = abs(rand())%200;
    p1[0] = abs(rand())%200; p1[1] = abs(rand())&200;
    cor[0] = abs(rand())%256; cor[1] = abs(rand())%256; cor[2] = abs(rand())%256;
    printf("p0 = (%i,%i) p1 = (%i,%i) cor = [%u,%u,%u]\n",p0[0],p0[1],p1[0],p1[1],cor[0],cor[1],cor[2]);
    reta(imagem, p0, p1, cor);
  }




  fp = fopen("reta_bresenhan.ppm","w");
  fprintf(fp,"P6\n%u %u\n255\n",l,h);

  for(j=0; j<h; j++)
    for(i=0; i<l; i++)
      fprintf(fp,"%c%c%c", imagem[i][j][0], imagem[i][j][1], imagem[i][j][2]);
  

  //o free esta comentado porque estava gerando um erro, nao sei o motivo
  //for(i=0;i<l;i++)
    //free(imagem[i]);
  //free(imagem);
}