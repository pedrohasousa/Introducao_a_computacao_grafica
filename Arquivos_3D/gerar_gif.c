//Usa a camera virtual feita no arquivo Foto_otimizado.c para tirar fotos sequenciais de um objeto, que podem ser usadas para fazer um gif.

#include <stdio.h>
#include <stdlib.h>

#define pi 3.14159265
#include <math.h>
#include "/home/pedro/Documentos/GitHub/comp_graf-matrizlib/matriz.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//vertices guardara os indices dos vertices da face, na lista do arquivo off
//angulo sera o angulo entre o vetor normal da face e o vetor que vai da camera ate o baricentro da face
typedef struct{
  int vertices[9];
  float angulo;
}Face;

int abs(int a){
  if(a<0) a=-a;
  return a;
}

int min(int a, int b){
  if(a<b) return a;
  else return b;
}

int max(int a, int b){
  if(a>b) return a;
  else return b;
}


// Funcao auxiliar para o merge_sort adaptada
// A e B sao partes de uma lista de enderecos para faces de um arquivo .off ja ordenadas, com a e b elementos respectivamente.
// C e uma lista vazia com a+b espaços, que sera usada para juntar A e B em apenas uma lista ordenada
// Z e uma lista com as coordenadas z dos vertices do arquivo .off, e a juncao de A e B sera feita com base nela.
// Para cada face de A e B, e calculada a media das coordenadas z de seus vertices. A ordenacao seguira a ordem
//crescente dessas medias.
void merge(float *Z, Face **A, Face **B, Face **C, int a, int b){
  int i = 0, j = 0, k = 0, acabou = 0, vertices_na_face, d;
  float media_A = 0, media_B = 0;
  

  for(k = 0; k < a + b; k++){
    if(i == a) acabou = 1;
    if(j == b) acabou = 2;
    media_A = 0;
    media_B = 0;

    switch(acabou){
      case 0:
        vertices_na_face = (**(A+i)).vertices[0];

        for(d = 1; d <= vertices_na_face; d++)
          media_A += Z[(**(A+i)).vertices[d]];
        media_A /= vertices_na_face; 


        vertices_na_face = (**(B+j)).vertices[0];

        for(d = 1; d <= vertices_na_face; d++)
          media_B += Z[(**(B+j)).vertices[d]];
        media_B /= vertices_na_face;   

        if(media_A < media_B){
          C[k] = A[i];
          i++;
        }
        else{
          C[k] = B[j];
          j++;
        }
        break;
          
      case 1:
        C[k] = B[j];
        j++;
        break;
      
      case 2:
        C[k] = A[i];
        i++;
        break;
    }
  }
}

// Funcao de ordenacao usada para projetar as faces do objeto na ordem certa, adaptada para esse problema
// A é uma lista de faces de um arquivo .off a serem ordenadas.
// B deve ter a*sizeof(Face*) de espaço alocado, pois sera usada para guardar os enderencos das faces de A ordenados.
// Z e uma lista com as coordenadas z dos vertices do arquivo .off, e a ordenacao das faces sera feita com base nela.
// Para cada face f de A, calculamos a media das coordenadas z de seus vertices, e a ordenacao sera feita seguindo a ordem
//crescente dessas medias.
void merge_sort(float *Z, Face *A, Face **B, int a){
  int d, vertices_na_face;
  float media_A0 = 0, media_A1 = 0;
  Face **C;
  C = malloc(a*sizeof(Face*));

  if(a == 2){
    vertices_na_face = (*A).vertices[0];
    
    for(d = 1; d <= vertices_na_face; d++)
      media_A0 += Z[(*A).vertices[d]];
    media_A0 /= vertices_na_face; 


    vertices_na_face = (*(A+1)).vertices[0];

    for(d = 1; d <= vertices_na_face; d++)
      media_A1 += Z[(*(A+1)).vertices[d]];
    media_A1 /= vertices_na_face;  


    B[0] = A; B[1] = A+1;
    
    if(media_A1 < media_A0){
      B[0] = A+1; B[1] = A;
    }  
  }

  else if(a == 1)
    B[0] = A;

  else{
    merge_sort(Z, A, B, a/2);
    merge_sort(Z, &A[a/2], &B[a/2], a/2 + a%2);
    merge(Z, B, &B[a/2], C, a/2, a/2 + a%2);

    for(int i = 0; i < a; i++)
      B[i] = C[i];

  }
}


// Funcao para desenhar uma reta em um arquivo ppm. Sera usada na de desenhar triangulos.
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


// Funcao para desenhar um triangulo em um arquivo ppm
//p0, p1, e p2 sao os vertices do triangulo.
//cor_faces e cor_arestas sao listas com o rgb das faces e arestas.
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


float modulo(float x){
  if(x < 0) x = -x;
  return x;
}


void foto(char nome[15], float *posicao, float* foco, unsigned char *cor_faces, unsigned char *cor_arestas){
  int i,j;
  //LEITURA DO ARQUIVO OFF
  unsigned int V,F,A; 
  Face *faces; //Matriz da conectividade Fx9. Limitada a faces com no máximo 8 lados
  float (*vertices)[3]; //Matriz Vx3
  FILE *file = fopen("/home/pedro/Área de Trabalho/arquivos 3D/block.off","r");

  //Lendo cabeçalho (Apenas cabecalhos sem comentários)
  fscanf(file,"OFF\n%u %u %u\n",&V,&F,&A);

  //Alocando memória para a matrix com os vértices
  vertices = malloc(3*V*sizeof(float));

  //carregando os vértices
  for(i=0;i<V;i++) fscanf(file,"%f %f %f\n",&vertices[i][0],&vertices[i][1],&vertices[i][2]);

  //Alocando memória para a matrix com a conectividade
  faces = malloc(F*sizeof(Face));

  //Carregando a conectividade [(k) a b c d e f g h]. k: número de vértices. a,b,c,d,e,f,g,h: índices dos vértices
  for(i=0;i<F;i++){
    fscanf(file,"%u",&(faces[i].vertices[0]));
    for(j=1;j<=faces[i].vertices[0];j++) fscanf(file," %u",&faces[i].vertices[j]);
    fscanf(file,"%*[^\n]"); //Descartando o restante da linha (cor, transparência)
  }
  fclose(file);

 



  
 
  //GERANDO A FOTO
  // Aqui, juuntamos todas as transformacoes em uma unica matriz e a aplicamos no objeto
  // A camera estara no ponto posicao olhando para o foco (que sera seu eixo z)
  // O eixo x sera horizontal, e o eixo y sera dado pelo produto vetorial dos outros
  float z[3] = {foco[0]-posicao[0], foco[1]-posicao[1], foco[2]-posicao[2]};
  float x[3] = {-z[1], z[0], 0};
  float y[3] = {z[1]*x[2] - z[2]*x[1], z[2]*x[0] - z[0]*x[2], z[0]*x[1] - z[1]*x[0]};
  float norma_x = sqrt(y[2]), 
        norma_y = sqrt(y[2] * (y[2] + z[2]*z[2])),
        norma_z = sqrt(y[2] + z[2]*z[2]);

  for(i = 0; i < 3; i++){
    x[i] /= norma_x;
    y[i] /= norma_y;
    z[i] /= norma_z;
  }      

  //Transformacao inicial a ser feita no objeto caso necessario
  Matriz4x4 ajuste = {{{1, 0, 0, 0},
                       {0, 1, 0, 0},
                       {0, 0, 1, 0},
                       {0, 0, 0, 1}}};


  //Matriz que aplica a mudanca de base
  Matriz4x4 rotacionar_camera = {{{x[0], x[1], x[2], 0},
                                 {y[0], y[1], y[2], 0},
                                 {z[0], z[1], z[2], 0},
                                 {0, 0, 0, 1}}};
  
  
  Matriz4x4 transladar_camera = {{{1, 0, 0, -posicao[0]},
                                 {0, 1, 0, -posicao[1]},
                                 {0, 0, 1, -posicao[2]},
                                 {0, 0, 0, 1}}};


  float altura = 0.5;
  Matriz4x4 transformacao_projetiva = {{{1, 0, 0, 0},
                                        {0, 1, 0, 0},
                                        {0, 0, 1/(1-altura), -altura/(1-altura)},
                                        {0, 0, 1, 0}}};


  Matriz4x4 gerar_foto = multiplicar4x4Por4x4(rotacionar_camera, transladar_camera);
  gerar_foto = multiplicar4x4Por4x4(transformacao_projetiva, gerar_foto);
  



  // Calculo dos angulos entres os vetores normais as faces e os que vao da camera ate cada face
  // Esses angulos serao usados para variar a cor das faces no ppm. Faces que
  //estejam mais "de lado" em relacao ao vetor_face_camera serao mais escuras
  // Eles precisam ser caculados antes da aplicacao das transformacoes.
  float normal[3], vetor_face_camera[3], *vertice_1, *vertice_2, *vertice_3, angulo, distancia;
  

  
  for(i = 0; i < F; i++){
    vertice_1 = vertices[faces[i].vertices[1]];
    vertice_2 = vertices[faces[i].vertices[2]];
    vertice_3 = vertices[faces[i].vertices[3]];

    normal[0] = (vertice_3[1] - vertice_1[1]) * (vertice_2[2] - vertice_1[2]) - (vertice_3[2] - vertice_1[2]) * (vertice_2[1] - vertice_1[1]);
    normal[1] = -(vertice_3[0] - vertice_1[0]) * (vertice_2[2] - vertice_1[2]) + (vertice_3[2] - vertice_1[2]) * (vertice_2[0] - vertice_1[0]);
    normal[2] = (vertice_3[0] -vertice_1[0]) * (vertice_2[1] - vertice_1[1]) - (vertice_3[1] - vertice_1[1]) * (vertice_2[0] - vertice_1[0]);
    
    vetor_face_camera[0] = -(vertice_1[0] + vertice_2[0] + vertice_3[0])/3 - posicao[0];
    vetor_face_camera[1] = -(vertice_1[1] + vertice_2[1] + vertice_3[1])/3 - posicao[1];
    vetor_face_camera[2] = -(vertice_1[2] + vertice_2[2] + vertice_3[2])/3 - posicao[2];
    distancia = sqrt(pow(vetor_face_camera[0],2) + pow(vetor_face_camera[1],2) + pow(vetor_face_camera[2],2));

    angulo = (vetor_face_camera[0]*normal[0] + vetor_face_camera[1]*normal[1] + vetor_face_camera[2]*normal[2]) / (sqrt(pow(normal[0],2) + pow(normal[1],2) + pow(normal[2],2)) * distancia);

    faces[i].angulo = angulo;
  }





  //aplicando a transformacao nos pontos do objeto
  Matriz4x1 ponto_RP3, ponto_foto;
  ponto_RP3.elemento[3][0] = 1;
  
  for(i = 0; i < V; i++){
    ponto_RP3.elemento[0][0] = vertices[i][0];
    ponto_RP3.elemento[1][0] = vertices[i][1];
    ponto_RP3.elemento[2][0] = vertices[i][2];

    ponto_foto = multiplicar4x4Por4x1(gerar_foto, ponto_RP3);

    if(ponto_foto.elemento[3][0] == 0){
      printf("um ponto do objeto foi para o infinito\n");
      exit(0);
    }  

    vertices[i][0] = ponto_foto.elemento[0][0]/ponto_foto.elemento[3][0];
    vertices[i][1] = ponto_foto.elemento[1][0]/ponto_foto.elemento[3][0];
    vertices[i][2] = ponto_foto.elemento[2][0]/ponto_foto.elemento[3][0];
  }

  


  //TIRANDO A FOTO
  // Nessa parte, projetamos o objeto gerado no plano xy, finalizando a foto.
  // A partir dessa projecao, criamos um arquivo ppm para guardar a foto em 2D.
  // Para isso, devemos ordenar as faces e adaptdar as coordenadas x,y do objeto.
  // Alem disso, precisamos de formas de desenhar retas e pintar triangulos em 
  //arquivos ppm.

  // Ordenacao da lista de faces
  // Devemos desenhar primeiro no ppm as faces mais distantes do plano xy.
  // A lista ordem_das_faces guardara os enderecos das faces do objeto na ordem crescente das medias das 
  //coordenadas z dos vertices de cada face.
  // A lista Z sera uma lista auxiliar com a coordenada z de todos os vertices, sera usada na ordenacao.
  Face **ordem_das_faces = malloc(F*sizeof(Face*));
  float *Z = malloc(V*sizeof(float));
  
  for(i = 0; i < V; i++)
    Z[i] = vertices[i][2];
  
  merge_sort(Z, faces, ordem_das_faces, F);
  

  // Matriz para guardar a foto
  // A foto tera altura h e largura l
  // Aqui tambem alocamos espaco para a foto e a pintamos de branco (plano de fundo)
  unsigned char (**foto_ppm)[3];
  int l = 1000, h = 1000;
  
  
  foto_ppm = malloc(l*sizeof(char*));
  for(i = 0; i < l; i++)
    foto_ppm[i] = malloc(3*h*sizeof(char));
  
  for(j = 0; j < h; j++)
    for(i = 0; i < l; i++)
      foto_ppm[i][j][0] = foto_ppm[i][j][1] = foto_ppm[i][j][2] = 255;



  // Adaptacao das coordenadas
  // Para fazer a imagem .ppm, precisamos adaptar as coordenadas x,y dos pontos, pois no ppm elas serao
  //inteiros, por exemplo de 0 a 499, ou de 0 a 999.
  // Para isso, consideramos que o campo de visao da camera e o tronco de cone que passa pela origem e pelo quadrado
  //(1,1,1), (1,-1,1), (-1,1,1), (-1,-1,1) e transformamos esse quadrado na imagem inteira usando uma homotetia nos 
  //eixos x e y. Entao, transladamos a imagem para que os vertices desse quadrado vao para (l,h), (l,0), (h,0) e (0,0) 


  for(i = 0; i < V; i++){
    vertices[i][0] = vertices[i][0]*l/2 + l/2;
    vertices[i][1] = vertices[i][1]*h/2 + h/2;
  }



  /* Expandir: verificar se todo o objeto fica na foto
  float x_max, y_max, x_min, y_min, range_x, range_y;

  x_max = x_min = vertices[0][0];
  y_max = y_min = vertices[0][1];
  for(i = 1; i < V ; i++){
    if(vertices[i][0] > x_max) x_max = vertices[i][0];
    else if (vertices[i][0] < x_min) x_min = vertices[i][0];

    if(vertices[i][1] > y_max) y_max = vertices[i][1];
    else if (vertices[i][1] < y_min) y_min = vertices[i][1];
  }

  range_x = x_max-x_min;
  range_y = y_max-y_min;
  printf("range x: %f , range y: %f\nx_max: %f x_min: %f\ny_max: %f y_min: %f\n", range_x, range_y, x_max, x_min, y_max, y_min);
  */
  
  



  // Escrevendo o ppm
  // Para finalizar, pintamos todos os triangulos (faces) no ppm.
  // Para cada face, definimos as coordenadas de seus vertices (deverao ser transformadas em inteiros),
  //desenhamos retas ligando esses vertices e pintamos dentro do triangulo. Isso deve ser feito seguindo
  //ordem de faces determinada acima, e so os pontos que estiverem no campo de visao da camera sera pintados
  // Obs: No ppm a coordenada y vai de cima para baixo, entao cada vertices[i] deve ser desenhado com
  //sua coordenada y dada por (h - vertices[i][1]), em vez de vertices[i][1].
  
  int p0[2], p1[2], p2[2], vertices_na_face;
  float media = 0, media_comp = 0;
  
  for(i = F-1; i >= 0; i--){
    p0[0] = vertices[(**(ordem_das_faces+i)).vertices[1]][0] + 0.5; p0[1] = h - vertices[(**(ordem_das_faces+i)).vertices[1]][1] + 0.5;
    p1[0] = vertices[(**(ordem_das_faces+i)).vertices[2]][0] + 0.5; p1[1] = h - vertices[(**(ordem_das_faces+i)).vertices[2]][1] + 0.5;
    p2[0] = vertices[(**(ordem_das_faces+i)).vertices[3]][0] + 0.5; p2[1] = h - vertices[(**(ordem_das_faces+i)).vertices[3]][1] + 0.5;
    
    if((p0[0] < 0 || p0[0] >= l) && (p1[0] < 0 || p1[0] >= l) && (p2[0] < 0 || p2[0] >= l))
      continue;
    else if((p0[1] < 0 || p0[1] >= h) && (p1[1] < 0 || p1[1] >= h) && (p2[1] < 0 || p2[1] >= h))  
      continue;
  /* Expandir: teste de funcao de ordenacao
  media_comp = media;  
  media = 0;
  vertices_na_face = (**(ordem_das_faces+i)).vertices[0];
    
  for(j = 1; j <= vertices_na_face; j++)
    media += Z[(**(ordem_das_faces+i)).vertices[j]];
  media /= vertices_na_face;

  if(media_comp < media){
    printf("erro de ordenacao. i = %i media: %f media_comp: %f\n", i, media, media_comp);
  }
  */

    if(vertices[(**(ordem_das_faces+i)).vertices[1]][2] > 0 && vertices[(**(ordem_das_faces+i)).vertices[2]][2] > 0 && vertices[(**(ordem_das_faces+i)).vertices[3]][2] > 0){  
      cor_faces[0] = cor_faces[1] = cor_faces[2] = modulo(255*(**(ordem_das_faces+i)).angulo);
      desenhar_triangulo(foto_ppm, p0, p1, p2, cor_faces, cor_arestas, l, h);
    }
  }
  


  file = fopen(nome,"w");
  fprintf(file, "P6\n%i %i 255\n", l, h);

  for(j = 0; j < h; j++)
    for(i = 0; i < l; i++)
      fprintf(file, "%c%c%c", foto_ppm[l-i-1][j][0], foto_ppm[l-i-1][j][1], foto_ppm[l-i-1][j][2]);

  
  free(faces);
  free(vertices);
  free(Z);
  free(ordem_das_faces);
  for(i = 0; i < h; i++)
    free(foto_ppm[i]);
  free(foto_ppm);
}




void main(){
  char nome[100];
  float posicao[3], foco[3] = {30,0,0};
  unsigned char cor_faces[3] = {0,0,0}, cor_arestas[3] = {0,0,0};
  int i, n, n2;
  
  //Criando uma pasta para guardar as fotos do gif
  //Seu nome deve ser colocado no penultimo espaco do endereco da foto mais abaixo
  struct stat st = {0};
	if (stat("/home/pedro/Área de Trabalho/arquivos 3D/gif5", &st) == -1)
    mkdir("/home/pedro/Área de Trabalho/arquivos 3D/gif5", 0700);
  
  else{
    printf("pasta ja existente\n");
    exit(0);
  }

  // Aqui devemos parametrizar duas curvas, uma para a posicao da camera e outra para o foco.
  // Entao, fazemos a camera andar um pouco na curva a cada iteracao.
  float t;
  n = 40;
  n2 = (n>>1) - 1;

  for(i = 0; i < n; i++){
    


   /* Uma parametrizacao, usada no cavalo
    posicao[0] = 250*cos(i*2*pi/n);
    posicao[1] = 300*sin(i*2*pi/n);

    if(i <= n2){
      posicao[2] = 100*(n2-i)/n2;
    }  
    else{
      posicao[2] = 100*(i-n2-1)/n2;
    }  
    */
    if(i < 20){
      t = (float)i/20;
      posicao[0] = -40*(1-t) + 2*t;
      posicao[2] = posicao[0]*posicao[0]/45;
    }

    else{
      t = (float)(i-20)/20;
      posicao[0] = 2*(1-t) - 40*t;
      posicao[2] = posicao[0]*posicao[0]/45;
    }  

    sprintf(nome, "/home/pedro/Área de Trabalho/arquivos 3D/gif5/foto%.2d.ppm", i);
    foto(nome, posicao, foco, cor_faces, cor_faces);
  }
  
}