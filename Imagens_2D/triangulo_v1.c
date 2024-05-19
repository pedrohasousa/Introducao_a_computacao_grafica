#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int min(int a, int b){
  if(a<b) return a;
  else return b;
}

int max(int a, int b){
  if(a>b) return a;
  else return b;
}

//Bresenham
void reta(unsigned char (**imagem)[3], int *p0, int *p1, unsigned char *cor, int l, int h){
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
      if(x >= 0 && x < l && y >= 0 && y < h){
        imagem[x][y][0] = cor[0]; imagem[x][y][1] = cor[1]; imagem[x][y][2] = cor[2];
      }  

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
      if(x >= 0 && x < l && y >= 0 && y < h){
        imagem[x][y][0] = cor[0]; imagem[x][y][1] = cor[1]; imagem[x][y][2] = cor[2];
      }  

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
      if(x >= 0 && x < l && y >= 0 && y < h){
        imagem[x][y][0] = cor[0]; imagem[x][y][1] = cor[1]; imagem[x][y][2] = cor[2];
      }  

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
      if(x >= 0 && x < l && y >= 0 && y < h){
        imagem[x][y][0] = cor[0]; imagem[x][y][1] = cor[1]; imagem[x][y][2] = cor[2];
      }

      if(d > 0){
        d += incrSE;
        x++;
      }
      else d += incrS;
    }
  }
}


/*Ponto medio (nao esta funcionando bem)
void reta(unsigned char (**imagem)[3], int *p0, int *p1, unsigned char cor[3], int l, int h){
  int p2[2];
  
  if((p0[0] < l) && (p0[0] > 0) && (p0[1] < h) && (p0[1] > 0)){
    imagem[p0[0]][p0[1]][0] = cor[0];
    imagem[p0[0]][p0[1]][1] = cor[1];
    imagem[p0[0]][p0[1]][2] = cor[2];
  }
  
  if((p1[0] < l) && (p1[0] > 0) && (p1[1] < h) && (p1[1] > 0)){
    imagem[p1[0]][p1[1]][0] = cor[0];
    imagem[p1[0]][p1[1]][1] = cor[1];
    imagem[p1[0]][p1[1]][2] = cor[2];
  }  

  if ((abs(p0[0]-p1[0])>1) || (abs(p0[1]-p1[1])>1)){
    p2[0]=(p0[0]+p1[0])>>1; // divisão por 2 arredondada para baixo
    p2[1]=(p0[1]+p1[1])>>1;
    
    if((p2[0] < l) && (p2[0] > 0) && (p2[1] < h) && (p2[1] > 0)){
      imagem[p2[0]][p2[1]][0] = cor[0];
      imagem[p2[0]][p2[1]][1] = cor[1];
      imagem[p2[0]][p2[1]][2] = cor[2];
    }

    reta(imagem, p0, p2, cor, l, h);
    reta(imagem, p2, p1, cor, l, h);
  }
}

*/




// Produto vetorial
void desenhar_triangulo(unsigned char (**imagem)[3], int *p0, int *p1, int *p2, unsigned char *cor_faces, unsigned char *cor_arestas, int l, int h){
  int v0[2], v1[2], v2[2], u0[2], u1[2], u2[2];
  int x_min, x_max, y_min, y_max, i, j;
  
  //Para garantir que a orientacao e positiva, na ordem p0, p1, p2
  if((p1[0]-p0[0]) * (p2[1]-p0[1]) - (p1[1]-p0[1]) * (p2[0]-p0[0]) < 0){
    v1[0] = p1[0]; v1[1] = p1[1];
    p1[0] = p2[0]; p1[1] = p2[1];
    p2[0] = v1[0]; p2[1] = v1[1];
  }

  v0[0] = p1[0] - p0[0]; v0[1] = p1[1] - p0[1];
  v1[0] = p2[0] - p1[0]; v1[1] = p2[1] - p1[1];
  v2[0] = p0[0] - p2[0]; v2[1] = p0[1] - p2[1];
  
  x_min = min(min(p0[0],p1[0]), p2[0]);
  x_max = max(max(p0[0],p1[0]), p2[0]);

  y_min = min(min(p0[1],p1[1]), p2[1]);
  y_max = max(max(p0[1],p1[1]), p2[1]);

  for(j = y_min; j <= y_max; j++){
    for(i = x_min; i <= x_max; i++){
      u0[0] = i - p0[0]; u0[1] = j-p0[1];
      u1[0] = i - p1[0]; u1[1] = j-p1[1];
      u2[0] = i - p2[0]; u2[1] = j-p2[1];

      if((v0[0]*u0[1] - v0[1]*u0[0] >= 0) && (v1[0]*u1[1] - v1[1]*u1[0] >= 0) && (v2[0]*u2[1] - v2[1]*u2[0] >= 0)){

        if(i >= 0 && i < l && j >= 0 && j < h){
          imagem[i][j][0] = cor_faces[0];
          imagem[i][j][1] = cor_faces[1];
          imagem[i][j][2] = cor_faces[2];
        }
      }
    }
  }

  reta(imagem, p0, p1, cor_arestas, l, h);
  reta(imagem, p1, p2, cor_arestas, l, h);
  reta(imagem, p2, p0, cor_arestas, l, h);
}


/* Produto iterno (esta sem verificar orientacao, funciona apenas para orientacao positiva)
void desenhar_triangulo(unsigned char (**imagem)[3], int *p0, int *p1, int *p2, unsigned char cor_faces[3], unsigned char cor_arestas[3], int l, int h){
  int i, j, x_max = p0[0], x_min = p0[0], y_max = p0[1], y_min = p0[1];
  int v0[2], v1[2], v2[2];
  int n0[2], n1[2], n2[2];
  int u0[2], u1[2], u2[2];

  v0[0]=p1[0] - p0[0]; v0[1] = p1[1] - p0[1];
  v1[0]=p2[0] - p1[0]; v1[1] = p2[1] - p1[1];
  v2[0]=p0[0] - p2[0]; v2[1] = p0[1] - p2[1];

  n0[0] = -v0[1]; n0[1] = v0[0];
  n1[0] = -v1[1]; n1[1] = v1[0];
  n2[0] = -v2[1]; n2[1] = v2[0];


  if(p1[0] < x_min) x_min = p1[0];
  if(p2[0] < x_min) x_min = p2[0];

  if(p1[0] > x_max) x_max = p1[0];
  if(p2[0] > x_max) x_max = p2[0];

  if(p1[1] < y_min) y_min = p1[1];
  if(p2[1] < y_min) y_min = p2[1];

  if(p1[1] > y_max) y_max = p1[1];
  if(p2[1] > y_max) y_max = p2[1];

  if(x_max > l) x_max = l-1;
  if(x_min < 0) x_min = 0;
  if(y_max > h) y_max = h-1;
  if(y_min < 0) y_min = 0;

  for(j = y_min; j <= y_max; j++){
    for(i = x_min; i <= x_max; i++){
      u0[0] = i - p0[0]; u0[1] = j - p0[1];
      u1[0] = i - p1[0]; u1[1] = j - p1[1];
      u2[0] = i - p2[0]; u2[1] = j - p2[1];

      if((u0[0]*n0[0]+u0[1]*n0[1])>=0 && (u1[0]*n1[0]+u1[1]*n1[1])>=0 && (u2[0]*n2[0]+u2[1]*n2[1])>=0){
        imagem[i][j][0]= cor_faces[0]; imagem[i][j][1]= cor_faces[1]; imagem[i][j][2]= cor_faces[2];
      }
    }
  }

  reta(imagem, p0, p1, cor_arestas, l, h);
  reta(imagem, p1, p2, cor_arestas, l, h);
  reta(imagem, p2, p0, cor_arestas, l, h);
}

*/






void main(){
  int i, j, l, h;
  FILE *fp;
  srand(time(NULL));
  unsigned char (**imagem)[3], cor_faces[3], cor_arestas[3] = {0,0,0};
  
  l = 200;
  h = 200;

  imagem = malloc(l*sizeof(unsigned char*));
  for(i = 0; i < l; i++)
    imagem[i] = malloc(3*h*sizeof(unsigned char));

  for(j=0; j<h; j++)
    for(i=0; i<l; i++)
      imagem[i][j][0] = imagem[i][j][1] = imagem[i][j][2] = 255;  

  
  int p0[2], p1[2], p2[2];

  for(i = 0; i < 10; i++){
    p0[0] = abs(rand())%200; p0[1] = abs(rand())%200;
    p1[0] = abs(rand())%200; p1[1] = abs(rand())&200;
    p2[0] = abs(rand())%200; p2[1] = abs(rand())&200;

    
    cor_faces[0] = abs(rand())%256; cor_faces[1] = abs(rand())%256; cor_faces[2] = abs(rand())%256;
    desenhar_triangulo(imagem, p0, p2, p1, cor_faces, cor_arestas, l, h);
  }




  fp = fopen("triangulo_v1.ppm","w");
  fprintf(fp,"P6\n%u %u\n255\n",l,h);

  for(j=0; j<h; j++)
    for(i=0; i<l; i++)
      fprintf(fp,"%c%c%c", imagem[i][j][0], imagem[i][j][1], imagem[i][j][2]);

}      