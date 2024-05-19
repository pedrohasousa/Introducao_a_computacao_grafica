//le uma imagem .ppm e aplica uma transformacao projetiva qualquer nela, dados os quadrilateros de entrada e saida.
//e recomendavel que o quadrilatero de entrada seja a propria imagem

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


float modulo(float a){
  if(a<0) a=-a;
  return a;
}

//Caso a cor saia do intervalo [0,255] ao fazer a interpolação,
//essa função a coloca de volta no intervalo. 
int tratar_cor(int cor){
  if(cor < 0) cor = 0;
  else if(cor > 255) cor = 255;
  return cor;
}


// matrizes 3x3
//Multiplica A por B e guarda em C
void multiplicar_matrizes(float (*A)[3], float (*B)[3], float (*C)[3]){
  int i,j,k;

  for(i=0; i<3; i++){
    for(j=0; j<3; j++){
      for(k=0; k<3; k++){
        C[i][j] += A[i][k]*B[k][j];
      }
    }
  }
}


// matriz 3x3
// Calcula a adjunta de A e guarda em C
void adjunta(float (*A)[3], float (*C)[3]){
  int i,j,k,l,a=0,b=0, lista_i[3] = {0,1,2}, lista_j[3] = {0,1,2};
  float auxiliar[2][2];

  for(i=0; i<3; i++){
    for(j=0; j<3; j++){

      lista_i[i]=-1;
      lista_j[j]=-1;
      for(k=0; k<3; k++){
        for(l=0; l<3; l++){
          if(lista_i[k] != -1 && lista_j[l] != -1){
            auxiliar[a][b] =  A[k][l];
            if(a==0 && b==0) b=1;
            else if(a==0 && b==1){a=1; b=0;}
            else if(a==1 && b==0) b=1;
            else if(a==1 && b==1){a=0; b=0;}
          }
        }
      }
      C[j][i] = pow(-1,i+j)*(auxiliar[0][0]*auxiliar[1][1] - auxiliar[0][1]*auxiliar[1][0]);

      lista_i[i]=i;
      lista_j[j]=j;
    } 
  }

}

// vetor 1x3 e matriz 3x3
// multiplica v por A e guarda em u
void vetorXmatriz(float *v, float (*A)[3], float *u){
  int i;  

  for(i=0; i<3; i++){
    u[i] = v[0]*A[0][i] + v[1]*A[1][i] + v[2]*A[2][i];
  }
}




void main(int argc, char **argv){
  int i, j, l, h;
  unsigned char type, cmax, caractere;
  FILE *fp;

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
  //TRANSFORMACAO PROJETIVA
  unsigned char (**transformada_proj)[3];
  int L, H, ic, jc, IC, JC, i_pixel, j_pixel;
  float T_v[3] = {0,0,1}, v[3];

  L = l;
  H = h;

  j=L*sizeof(char*);
  transformada_proj = malloc(j);

  j=H*3;
  for (i=0; i<L; i++)
    transformada_proj[i] = malloc(j);

  for (i = 0; i < L; i++)
    for (j = 0; j < H; j++)
        transformada_proj[i][j][0] = transformada_proj[i][j][1] = transformada_proj[i][j][2] = 0;



  
  float quadrilatero_entrada[4][3] = {{0,0,1},{L,0,1},{0,H,1},{L,H,1}}, quadrilatero_saida[4][3] = {{160,80,1},{499,53,1},{124,418,1},{418,303,1}};
  float abc[3], alfabetagama[3];
  float A[3][3], B[3][3], Adj_A[3][3]={}, Adj_B[3][3]={}, T[3][3]={}, Adj_T[3][3] = {};
  int incremento_red, incremento_green, incremento_blue;

  for(i=0; i<3; i++){
    for(j=0; j<3; j++){
      A[i][j] = quadrilatero_entrada[i][j];
      B[i][j] = quadrilatero_saida[i][j];
    } 
  }
  //encontrando a,b,c e alfa,beta,gama
  adjunta(A, Adj_A);
  adjunta(B, Adj_B);

  vetorXmatriz(quadrilatero_entrada[3], Adj_A, abc);
  
  vetorXmatriz(quadrilatero_saida[3], Adj_B, alfabetagama); //a.alfa, b.beta, c.gama

  alfabetagama[0]/=abc[0]; 
  alfabetagama[1]/=abc[1];
  alfabetagama[2]/=abc[2];

  
  //encontrando T
  for(i=0; i<3; i++){
    for(j=0; j<3; j++){
      B[i][j]*=alfabetagama[i];
    } 
  }
  

  multiplicar_matrizes(Adj_A,B,T);
  adjunta(T, Adj_T);



  //APLICACAO DA TRANSFORMACAO NA IMAGEM
  /* Cada ponto de coordenada (x,y) e representado com coordenadas (x,y,1) 
  para a aplicacao da transformacao projetiva. Entao, dado (a,b,c) = T(x,y,1), as
  novas coordenadas de (x,y) serao (a/c, b/c) 
  */

  for(j=0; j<H; j++){
    for(i=0; i<L; i++){
      T_v[0] = i;
      T_v[1] = j;
      vetorXmatriz(T_v, Adj_T, v);

      i_pixel = v[0]/v[2];
      j_pixel = v[1]/v[2];

      if(i_pixel<0);
      else if(i_pixel>=l);
      else if(j_pixel<0);
      else if(j_pixel>=h);
      else if(i_pixel==l-1 || j_pixel==h-1){
        transformada_proj[i][j][0]=imagem[i_pixel][j_pixel][0];
        transformada_proj[i][j][1]=imagem[i_pixel][j_pixel][1];
        transformada_proj[i][j][2]=imagem[i_pixel][j_pixel][2];
      }
      else{

        incremento_red = (imagem[i_pixel+1][j_pixel][0] - imagem[i_pixel][j_pixel][0])*(modulo(v[0]/v[2])-modulo(i_pixel)) + (imagem[i_pixel][j_pixel+1][0] - imagem[i_pixel][j_pixel][0])*(modulo(v[1]/v[2])-modulo(j_pixel));
        
        transformada_proj[i][j][0]=tratar_cor(imagem[i_pixel][j_pixel][0] + incremento_red);

        incremento_green = (imagem[i_pixel+1][j_pixel][1] - imagem[i_pixel][j_pixel][1])*(modulo(v[0]/v[2])-modulo(i_pixel)) + (imagem[i_pixel][j_pixel+1][1] - imagem[i_pixel][j_pixel][1])*(modulo(v[1]/v[2])-modulo(j_pixel));
        
        transformada_proj[i][j][1]=tratar_cor(imagem[i_pixel][j_pixel][1] + incremento_green);
        
        incremento_blue = (imagem[i_pixel+1][j_pixel][2] - imagem[i_pixel][j_pixel][2])*(modulo(v[0]/v[2])-modulo(i_pixel)) + (imagem[i_pixel][j_pixel+1][2] - imagem[i_pixel][j_pixel][2])*(modulo(v[1]/v[2])-modulo(j_pixel));
        
        transformada_proj[i][j][2]=tratar_cor(imagem[i_pixel][j_pixel][2] + incremento_blue);
      }
    } 
  }



  
  

  fp = fopen("transformada_proj.ppm", "w");
    fprintf(fp, "P6\n");
      fprintf(fp, "%u %u\n255\n", L, H);
      for (j=0;j<H;j++)
        for (i=0;i<L;i++)
          fprintf(fp,"%c%c%c", transformada_proj[i][j][0],transformada_proj[i][j][1],transformada_proj[i][j][2]);
  fclose(fp);

  for(i=0;i<l;i++)
    free(imagem[i]);
  free(imagem);

  for(i=0;i<L;i++)
    free(transformada_proj[i]);
  free(transformada_proj);
}