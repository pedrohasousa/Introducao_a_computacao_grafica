//le uma imagem .ppm e aplica nela uma transformacao linear, isto e, uma mistura de rotacao, homotetia e cisalhamento.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


float modulo(float a){
  if(a<0) a=-a;
  return a;
}


void main(int argc, char **argv){
  int i, j, l, h;
  unsigned char type, cmax, caractere;
  FILE *fp;

  //leitura da imagem
  fp = fopen("gatobranco.ppm", "r");
  while ((caractere=getc(fp))=='#')
    while((caractere=getc(fp))!='\n');
  ungetc(caractere,fp);

  fscanf(fp, "P%hhu\n", &type);
  while ((caractere=getc(fp))=='#')
    while((caractere=getc(fp))!='\n');
  ungetc(caractere,fp);

  fscanf(fp, "%u %u\n%hhu\n", &l, &h, &cmax);


  unsigned char (**imagem)[3];

  j=l*sizeof(char*);
  imagem = malloc(j);

  j=h*3;
  for (i=0; i<l; i++)
    imagem[i] = malloc(j);


  if(type==3){
    for(j=0; j<h; j++)
      for(i=0; i<l; i++)
        fscanf(fp, "%hhu %hhu %hhu", &imagem[i][j][0],&imagem[i][j][1],&imagem[i][j][2]);
    fclose(fp);
  }
  else if(type==6){
    for(j=0; j<h; j++)
      for(i=0; i<l; i++)
        fscanf(fp, "%c%c%c", &imagem[i][j][0],&imagem[i][j][1],&imagem[i][j][2]);
    fclose(fp);
  }       
  else{
    printf("Formato inválido!");
    fclose(fp);
    exit(0);
  }

  
  //DEFINICAO DA TRANSFORMACAO E CRIACAO DA MATRIZ PARA A IMAGEM TRANSFORMADA
  unsigned char (**transformada)[3];
  float matriz_inversa[2][2], a, b, c, d, det, i1, j1;
  int ic, jc, i1_aprox, j1_aprox, L, H, IC, JC;

  //matriz[[a,b],[c,d]] da transformacao
  a=cos(1.3); b=-sin(1.3); c=sin(1.3); d=cos(1.3);
  det=a*d-b*c;
  
  //transformacao inversa
  matriz_inversa[0][0]=d/det;
  matriz_inversa[0][1]=-b/det;
  matriz_inversa[1][0]=-c/det;
  matriz_inversa[1][1]=a/det;

  //dimensoes da imagem transformada
  L=modulo(a)*l+modulo(b)*h;
  H=modulo(c)*l+modulo(d)*h;

  j=L*sizeof(char*);
  transformada = malloc(j);

  j=H*3;
  for (i=0; i<L; i++)
    transformada[i] = malloc(j);

  //Centros das imagens
  ic=l>>1; jc=h>>1;
  IC=L>>1; JC=H>>1;

  
  //APLICACAO DA TRANSFORMACAO COM INTERPOLACAO LINEAR
  for(j=0; j<H; j++){
    for(i=0; i<L; i++){
      i1=matriz_inversa[0][0]*(i-IC) + matriz_inversa[0][1]*(j-JC);
      j1=matriz_inversa[1][0]*(i-IC) + matriz_inversa[1][1]*(j-JC);
      i1_aprox=i1;
      j1_aprox=j1;

      if(i1_aprox<-ic);
      else if(i1_aprox>=ic+l%2);
      else if(j1_aprox<-jc);
      else if(j1_aprox>=jc+h%2);
      else if(i1_aprox==ic+l%2-1 || j1_aprox==jc+h%2-1){
        transformada[i][j][0]=imagem[i1_aprox+ic][j1_aprox+jc][0];
        transformada[i][j][1]=imagem[i1_aprox+ic][j1_aprox+jc][1];
        transformada[i][j][2]=imagem[i1_aprox+ic][j1_aprox+jc][2];
      }
      else{
        transformada[i][j][0]=imagem[i1_aprox+ic][j1_aprox+jc][0] + (imagem[i1_aprox+ic+1][j1_aprox+jc][0] - imagem[i1_aprox+ic][j1_aprox+jc][0])*(modulo(i1)-modulo(i1_aprox));

        transformada[i][j][1]=imagem[i1_aprox+ic][j1_aprox+jc][1] + (imagem[i1_aprox+ic+1][j1_aprox+jc][1] - imagem[i1_aprox+ic][j1_aprox+jc][1])*(modulo(i1)-modulo(i1_aprox));
        
        transformada[i][j][2]=imagem[i1_aprox+ic][j1_aprox+jc][2] + (imagem[i1_aprox+ic+1][j1_aprox+jc][2] - imagem[i1_aprox+ic][j1_aprox+jc][2])*(modulo(i1)-modulo(i1_aprox));
      }  
    }
  }
 

 /*
  //APLICACAO DA TRANSFORMACAO SEM INTERPOLACAO
  for(j=0; j<H; j++){
    for(i=0; i<L; i++){
      i1=matriz_inversa[0][0]*(i-IC) + matriz_inversa[0][1]*(j-JC);
      j1=matriz_inversa[1][0]*(i-IC) + matriz_inversa[1][1]*(j-JC);
      
      i1_aprox=i1;
      j1_aprox=j1;

      if(i1_aprox<-ic);
      else if(i1_aprox>=ic+l%2);
      else if(j1_aprox<-jc);
      else if(j1_aprox>=jc+h%2);
        
      else{
        transformada[i][j][0]=imagem[i1_aprox+ic][j1_aprox+jc][0];
        transformada[i][j][1]=imagem[i1_aprox+ic][j1_aprox+jc][1];
        transformada[i][j][2]=imagem[i1_aprox+ic][j1_aprox+jc][2];
      }
    }
  }
*/
  

  fp = fopen("transformada.ppm", "w");
    fprintf(fp, "P6\n");
      fprintf(fp, "%u %u\n255\n", L, H);
      for (j=0;j<H;j++)
        for (i=0;i<L;i++)
          fprintf(fp,"%c%c%c", transformada[i][j][0],transformada[i][j][1],transformada[i][j][2]);
  fclose(fp);

  for(i=0;i<l;i++)
    free(imagem[i]);
  free(imagem);

  for(i=0;i<L;i++)
    free(transformada[i]);
  free(transformada);
}  