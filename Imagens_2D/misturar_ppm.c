//le duas imagens .ppm e mistura seus pixeis no sentido horizontal
//as imagens devem ter as mesmas dimensoes

#include <stdio.h>
#include <stdlib.h>

//funcao auxiliar para a mistura
//dados um intervalo [x,y] de numeros inteiros, uma escala [0,p] e uma posicao t nessa escala, retorna o valor do intervalo [x,y] correspondente ao t na escala [0,p]
//por exemplo, se [x,y] = [1,10], t = 40 e p = 100, o retorno e 4.
int valor_intermediario(int x, int y, int t, int p){
  int m;
  m=(x+y)>>1;
  if(t<0 || t>p){
    printf("Posição incompatível com a escala.");
    return;
  }
  
  else if(t==0) return x;
    
  else if(t==p) return y;
    
  else if(t>((p+1)>>1)) return valor_intermediario(m,y,t-(p>>1),p>>1);
    
  else if(t<(p>>1)) return valor_intermediario(x,m,t,p>>1);
  else return m;
}




int main(void) {
  int i,j,l,h;
  unsigned char type,cmax,caractere;
  FILE *fp;

  
  //guarda a primeira imagem em uma matriz
  fp = fopen("gato_preto.ppm","r");
  while((caractere=getc(fp))=='#')
    while((caractere=getc(fp))!='\n');
  ungetc(caractere,fp);

  fscanf(fp,"P%hhu\n",&type);
  while((caractere=getc(fp))=='#')
    while((caractere=getc(fp))!='\n');
  ungetc(caractere,fp);

  fscanf(fp,"%u %u %hhu\n",&l,&h,&cmax);

  unsigned char preto[h][l][3];

  if(type==6){
    for(i=0;i<h;i++)
      for(j=0;j<l;j++)
        fscanf(fp,"%c%c%c",&preto[i][j][0],&preto[i][j][1],&preto[i][j][2]);
    fclose(fp);
  }
  else{
    printf("Formato inválido!");
      fclose(fp);
    exit(0);
  }


  //guarda a segunda imagem em uma matriz
  fp=fopen("gatobranco.ppm","r");
  while((caractere=getc(fp))=='#')
    while((caractere=getc(fp))!='\n');
  ungetc(caractere,fp);

  fscanf(fp,"P%hhu\n",&type);
  while((caractere=getc(fp))=='#')
    while((caractere=getc(fp))!='\n');
  ungetc(caractere,fp);

  fscanf(fp,"%u %u %hhu\n",&l,&h,&cmax);

  unsigned char branco[h][l][3];

  if(type==6){
    for(i=0;i<h;i++)
      for(j=0;j<l;j++)
        fscanf(fp,"%c%c%c",&branco[i][j][0],&branco[i][j][1],&branco[i][j][2]);
    fclose(fp);
  }
  else{
    printf("Formato inválido!");
    fclose(fp);
    exit(0);
  }


  
  //mistura as imagens
  //o canto esquerdo da imagem sera igual ao canto da imagem 'preto', e o canto direito sera igual a imagem 'branco'.
  //no meio elas serao misturadas usando interpolacao linear para 'transitar' de uma imagem para a outra gradualmente
  unsigned char mistura[h][l][3];
  int x,y,partes;
  partes=3;
  x=l/partes;
  y=x*(partes-1)+l%partes;  
  for(i=0;i<h;i++){
    for(j=0;j<x;j++){
      mistura[i][j][0]=preto[i][j][0];
      mistura[i][j][1]=preto[i][j][1];
      mistura[i][j][2]=preto[i][j][2];

      mistura[i][j+y][0]=branco[i][j+y][0];
      mistura[i][j+y][1]=branco[i][j+y][1];
      mistura[i][j+y][2]=branco[i][j+y][2];
    }
  }
  
  int t=0;
  
  for(i=0;i<h;i++){
    for(j=x;j<y;j++){
      mistura[i][j][0]=valor_intermediario(preto[i][j][0],branco[i][j][0],t,y-x);
      mistura[i][j][1]=valor_intermediario(preto[i][j][1],branco[i][j][1],t,y-x);
      mistura[i][j][2]=valor_intermediario(preto[i][j][2],branco[i][j][2],t,y-x);
      
      t++;
    }
    t=0;
  }


  //retorno da imagem misturada
  fp=fopen("misturado.ppm","w");
  fprintf(fp,"P6\n%u %u %hhu\n",l,h,cmax);
  for(i=0;i<h;i++)
    for(j=0;j<l;j++)
      fprintf(fp,"%c%c%c",mistura[i][j][0],mistura[i][j][1],mistura[i][j][2]);

  fclose(fp);

  return 0;
}