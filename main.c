#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <error.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#define MAXSIZE 50000
#define TOK_DEL "\t\a\r\n"

char* FILEONE ;
char* FILETWO ;
char* FILETHREE ;
int **mat1 ;int x ; int  y; int z ;
int **mat2 ;
int **mat3 ;
int **data ;
int counter_Raw , counter_Element ;

int main(int args , char *argv[])
{

//change file name if exists
if ( argv[1] != NULL  )
{
     FILEONE   = argv[1];
     FILETWO   = argv[2];
     FILETHREE = argv[3];
}else{
     FILEONE   = "a.txt";
     FILETWO   = "b.txt";
     FILETHREE = "c.txt";
}
//open files and update mat1

    UpdateMat1() ;

//open files and update mat2 and check if condition of multiplication is exists

    UpdateMat2() ;

// get mat3 byrow
struct timeval stop, start;
gettimeofday(&start, NULL); //start checking time
   executeRow() ;//your code goes here
gettimeofday(&stop, NULL);//end checking time
printf("Raw WAy : Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);

// get mat3 byElement

gettimeofday(&start, NULL); //start checking time
       executeElement() ;//your code goes here
gettimeofday(&stop, NULL);//end checking time
printf("Element WAy : Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);

//print matOut in filel
    printMatOut();

return 1 ;
}

void UpdateMat1(){

FILE *fp1 = fopen(FILEONE, "r");

int n,m;
if(fp1 == NULL)
{
 fprintf(stderr,"Error reading file");
 }
 // get rows and columns size
fscanf(fp1,"row=%d col=%d",&n,&m);
x= n ;
y = m ;
//printf("%d \t%d\n",x,y) ;

//Allocate memory for Matrix x*y
mat1 = (int *)malloc(n *sizeof(int *));
    for (int i=0; i<n; i++)
    {
         mat1[i] = (int *)malloc(m * sizeof(int));
    }
//Read elements from file
for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            fscanf(fp1,"%d\t",&mat1[i][j]);
           // printf("%d\t" ,mat1[i][j]) ;
        }
       // printf("\n") ;
    }
fclose(fp1) ;


return 0 ;
}

void UpdateMat2(){
FILE *fp2 = fopen(FILETWO, "r");

int n,m;

if(fp2 == NULL)
{
 fprintf(stderr,"Error reading file");
 }
 // get rows and columns size
fscanf(fp2,"row=%d col=%d",&n,&m);
z = m ;
//printf("%d \t%d\n",y,z) ;

//Allocate memory for Matrix x*y
mat2 = (int *)malloc(n *sizeof(int *));
    for (int i=0; i<n; i++)
    {
         mat2[i] = (int *)malloc(m * sizeof(int));
    }
//Read elements from file
for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            fscanf(fp2,"%d\t",&mat2[i][j]);
           // printf("%d\t" ,mat2[i][j]) ;
        }
        //printf("\n") ;
    }
fclose(fp2) ;

//check for condition
if(n != y){
    perror("Error!condition of multiplication didn't exists");
    exit(-1);
}
return 0 ;
}
//threads for every row
void *mulInThread (void *data){

int counter = (int ) data ;
// exeute here
for(int i =0 ; i < z ; i++){
    for(int j = 0 ; j < y ; j ++ ){
          mat3[counter][i] += mat1[counter][j]*mat2[j][i] ;
    }
}

pthread_exit(NULL) ;

return NULL ;
}

void executeRow() {

mat3 = (int *)malloc(x *sizeof(int *));
    for (int i=0; i<x; i++)
    {
         mat3[i] = (int *)malloc(z * sizeof(int));
    }

for (int i =0 ; i < x ; i++ )
{

    for(int j = 0 ; j <z ; j++)
        mat3[i][j] = 0 ;

}

int noThreads = x ;
pthread_t *tid ;
tid = (pthread_t *)malloc ( noThreads * sizeof(pthread_t));
void * data = NULL ;
for(int i = 0 ; i < x ; i++){
    counter_Raw++ ;
  if( pthread_create(&tid[i] , NULL , mulInThread ,  i ) != 0 )
  {
        perror("error with threads");
        exit(-1) ;
    }
}

for(int i = 0 ; i < noThreads ; i++){
void *k ;
        pthread_join(tid[i], &k) ;
  }
 /* for(int i=0;i<x;i++){
        for(int j=0;j<z;j++){
           printf("%d\t" ,mat3[i][j]) ;
        }
        printf("\n\n") ;
    }*/

return 0 ;
}
//threads for every element
void *mulInThreadElement (void *args){

int counter = (int) args ;
int d = data[counter][0] ;
 int f = data[counter][1] ;

// exeute here
    for(int j = 0 ; j < y ; j ++ ){
          mat3[d][f] += mat1[d][j]*mat2[j][f] ;
    }

pthread_exit(NULL) ;

return NULL ;
}
void executeElement () {


mat3 = (int *)malloc(x *sizeof(int *));
    for (int i=0; i<x; i++)
    {
         mat3[i] = (int *)malloc(z * sizeof(int));
    }

for (int i =0 ; i < x ; i++ )
{
    for(int j = 0 ; j <z ; j++)
        mat3[i][j] = 0 ;
}
int countTHread = 0 ;
int noThreads = x*z ;
pthread_t *tid ;
tid = (pthread_t *)malloc ( noThreads * sizeof(pthread_t));
data = (int *)malloc( noThreads *sizeof(int *)) ;
    for(int s = 0 ; s < noThreads ; s++ ){
        data[s] = (int * )malloc(countTHread *sizeof(int));
    }

for(int i = 0 ; i < x ; i++){
    for(int j = 0 ; j < z ; j++){
            data[countTHread][0] = i ;
            data[countTHread][1] = j ;
            counter_Element++ ;
          if( pthread_create(&tid[countTHread] , NULL , mulInThreadElement ,countTHread) != 0 )
          {
                perror("error with threads");
                exit(-1) ;
          }
          countTHread++ ;
    }
}

for(int i = 0 ; i < noThreads ; i++){
        void *k ;
        pthread_join(tid[i], &k) ;
}


/*for(int i=0;i<x;i++){
        for(int j=0;j<z;j++){
           printf("%d\t" ,mat3[i][j]) ;
        }
        printf("\n\n") ;
    }
*/
return 0 ;

}
void printMatOut ()
{
FILE *fp ;
fp = fopen(FILETHREE,"w+");

fprintf(fp,"row=%d  col=%d",x,z);
fprintf(fp , "\n\n" , NULL) ;

for(int i=0;i<x;i++){
        for(int j=0;j<z;j++){
           fprintf(fp,"%d\t" ,mat3[i][j]) ;
        }
        fprintf(fp , "\n\n" , NULL) ;
    }


fclose(fp);

// print number of threads per RAW
printf("No.Of thhread per Raw : %d \n" , counter_Raw) ;
//print number of threads per element
printf("No.Of thhread per element : %d\n" , counter_Element) ;

return 0 ;
}
