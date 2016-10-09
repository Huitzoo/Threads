#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <unistd.h>
#include  <pthread.h>
int **matriz;
int **matriz2;
int n;
struct param{
  int *resultado;
  int *filas;
  int repeticiones;
};
void *multiplicacion(void *hilo_mandado)
{
  int i;
  int j;
  int l=0;
  int r=0;
  int p=0;
  int h;
  struct param *hilo = (struct param*)hilo_mandado;
  int *k= (int*)malloc((n*hilo->repeticiones)*sizeof(int));
  for(i=0;i<hilo->repeticiones;i++)
  {
    for(h=0;h<n;h++)
    {
      for(j=0;j<n;j++)
      {
        p=(matriz[hilo->filas[i]][j]*matriz2[j][h]);
        r=p+r;
      }
      k[h+l] = r;
      p=0;
      r=0;
    }
    l=j+l;
  }
  hilo->resultado=k;
  return((void*)hilo);
}
int main(int argc, char **v)
{
  //OBTENCION DE DATOS
  srand(time(NULL));
  int hilitos;
  int i=0;
  int r=0;
  int j=0;
  int k=0;
  int m=0;
  int cont=0;
  int x=rand()%5;
  printf("Ingresa el tamaño de tus matrices a multiplicar: \n");
  scanf("%d", &n);
  //LOS DATOS FUERON ACEPTADOS
  printf("Ingresa cuantos hilos quieres: \n");
  scanf("%d",&hilitos);
  if(hilitos>n)
  {
      printf("Es un desperdicio de memoria invocar mas hilos que el tamaño de tu matriz, lo siento :(\n");
      exit(0);
  }
  //inicializacion de variables
  struct param *cajitadehilos=(struct param*)malloc(hilitos*sizeof(struct param));
  pthread_t *id = (pthread_t *)malloc(hilitos*sizeof(pthread_t));
  matriz = (int**)malloc(sizeof(int*)*n);
  matriz2  = (int**)malloc(sizeof(int*)*n);
  for(i=0;i<n;i++)
  {
    matriz[i]=(int*)malloc(sizeof(int)*n);
    matriz2[i]=(int*)malloc(sizeof(int)*n);
  }
  printf("Matriz \n");
  for(i=0;i<n;i++)
  {
    for(j=0;j<n;j++)
    {
      x=rand()%4;
      matriz[i][j]=x;
      printf("%d ",matriz[i][j]);
    }
    printf("\n");
  }
  printf("Matriz 2\n");
  for(i=0;i<n;i++)
  {
    for(j=0;j<n;j++)
    {
      x=rand()%4;
      matriz2[i][j]=x;
      printf("%d ",matriz2[i][j]);
    }
    printf("\n");
  }
  //Decide cuantas veces se van a repetir los hilos.
  for(i=0;i<hilitos;i++)
  {
    cajitadehilos[i].repeticiones=n/hilitos;
  }
  //Si en la división sobraron columnas, entonces se agrega uno por uno las que hayan sobrado mediante el modulo
  if(n%hilitos!=0)
  {
    for(i=0;i<(n%hilitos);i++)
    {
      cajitadehilos[i].repeticiones=cajitadehilos[i].repeticiones+1;
    }
  }
  //Asignación de filas que multiplicaran
  for(i=0;i<hilitos;i++)
  {
    for(j=0;j<cajitadehilos[i].repeticiones;j++)
    {
      cajitadehilos[i].filas=(int*)malloc((cajitadehilos[i].repeticiones)*sizeof(int));
    }
  }
  for(i=0;i<hilitos;i++)
  {
    for(j=0;j<cajitadehilos[i].repeticiones;j++)
    {
      cajitadehilos[i].filas[j]=cont;
      cont = cont +1;
    }
  }
  for(i=0;i<hilitos;i++)
  {
      printf("hilo %d: %d filas\n",i,cajitadehilos[i].repeticiones);
      cajitadehilos[i].resultado=(int*)malloc((n*cajitadehilos[i].repeticiones)*sizeof(int));
  }
  //Invoca a los hilos para que hagan las cosas
  for(i=0;i<hilitos;i++)
  {
    pthread_create(&id[i], NULL, multiplicacion, (void*)&cajitadehilos[i]);
    
  }
  //Espera a los hilos a que terminen
  for (i=0;i<hilitos;i++)
  {
      pthread_join(id[i], (void**)&cajitadehilos[i]);
  }
  k=n;
  printf("\nMatriz resultante \n");
  for(i=0;i<hilitos;i++)
  {
    if(cajitadehilos[i].repeticiones!=1)
    {
      for(r=0;r<cajitadehilos[i].repeticiones;r++)
      {
        for(;m<k;m++)
        {
          printf("%d ",cajitadehilos[i].resultado[m]);
        }
        k=k+n;
        printf("\n");
      }
      m=0;
      k=n;
    }
    else
    {
      for(j=0;j<n;j++)
      {
        printf("%d ",cajitadehilos[i].resultado[j]);
      }
      printf("\n");
    }
  }
	printf("termine");
}
