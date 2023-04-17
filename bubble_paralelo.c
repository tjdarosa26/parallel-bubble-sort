// Compilar:
// gcc bubble_paralelo_tid.c -pthread -o bubble_paralelo_tid 
// Executar:
// ./bubble_seq 1024 256 16 (num_arr/num_elem/num_threads)

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

// #define IMPRIME

int **elementos, n, n_threads, tam, i, j;    
 
struct thread_args {
  int ** array;
  int size;
  int arr_per_t;
  int tid;
};

void imprime(int **array, int num_arrays, int size){
  int i,j;
  for(i=0; i < num_arrays; i++){
	 for(j=0; j < size; j++){
		printf("%d ", array[i][j]);
	 }
	 printf("\n");
  }
}

int bubble(int *array,unsigned int size){
  int i,j;
  int temp;
  for(i = 0; i < size-1; i++){       
    for(j = 0; j < size-i-1; j++){          
      if(array[j] > array[j+1]){               
        temp = array[j];
        array[j] = array[j+1];
        array[j+1] = temp;
      }
    }
  }
}

void *bubble_paralelo(void* t_args){
  struct thread_args *args = (struct thread_args*)t_args;

  int start = args->tid * args->arr_per_t;
  int end = start + args->arr_per_t;

  #ifdef IMPRIME
  printf("executing thread #%d, start:%d, end:%d\n",
  args->tid,
  start,
  end);
  #endif

  for (start; start<end; start++){
    bubble(args->array[start], args->size);
  }
}

int main(int argc, char **argv){
  struct timeval t1, t2;
  double t_total;

  if(argc!=4){
	 printf("Digite %s Num_arrays Num_elementos Num_threads\n", argv[0]);
	 exit(0);
  }
  n = atoi(argv[1]);
  tam = atoi(argv[2]);
  n_threads = atoi(argv[3]);	

  /*Para simplificar a lógica do programa, são apenas aceitas quantidades 
  de arrays que sejam divisíveis pelo número de threads informadas (sem resto)*/
  if(n % n_threads != 0){
    printf("O número de arrays deve ser divisivel pelo numero de threads! (sem resto)\n");
    exit(0);
  }

  /*Aloca memória para os vetores*/
  elementos=(int**)malloc(n*sizeof(int*));
  for(i = 0; i < n; i++)
    elementos[i]=(int*)malloc(tam*sizeof(int));

  /*Popula os arrays com elementos aleatorios entre 0 e 1000*/
  for(i = 0; i < n; i++) 
    for (j = 0; j < tam; j++)
      elementos[i][j] = rand() % 1000;

  #ifdef IMPRIME
  printf("Antes da ordenacao!\n");
  imprime(elementos, n, tam);
  printf("\n");
  #endif

  pthread_t th[n_threads];
  int n_arrays_per_thread = n / n_threads;
  int t;
  int creating_code;
  int joining_code;

  // Aloca memória para todos os argumentos de todas as threads
  struct thread_args **t_args_adresses;
  t_args_adresses = (struct thread_args**)malloc(
    n_threads * sizeof(struct thread_args*)
  );

  for (int index = 0; index < n_threads; index++){
    struct thread_args *ta = (struct thread_args*)malloc(
      sizeof(struct thread_args)
    );

    ta->size = tam;
    ta->arr_per_t = n_arrays_per_thread;
    ta->array = elementos;

    /* Guarda o endereço da memória alocada */ 
    t_args_adresses[index] = ta;
  }

  gettimeofday(&t1, NULL);

  for(t = 0; t<n_threads; t++){

    // guarda na struct os valores a serem enviados como argumentos
    t_args_adresses[t]->tid = t;


    #ifdef IMPRIME 
    printf("\nMain: creating thread %d\n", t);
    #endif

    // É passado para a thread um ponteiro para a struct criada
    creating_code = pthread_create(&th[t], NULL, *bubble_paralelo, (void*)t_args_adresses[t]);
    if (creating_code){
      printf("ERROR: return code from pthread_create() is %d\n", creating_code);
      exit(-1);
    }
  }

  for(t = 0; t<n_threads ;t++){
    joining_code = pthread_join(th[t], NULL);
    if (joining_code){
      printf("ERROR: return code from pthread_join() is %d\n", joining_code);
    }
  }

  gettimeofday(&t2, NULL);

  // Libera memória dos argumentos das threads
  for (int i = 0; i<n_threads; i++){
    free(t_args_adresses[i]);
  }
  free(t_args_adresses);

  
  t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec)/1000000.0);
 
  #ifdef IMPRIME
  printf("apos a ordenacao!\n");
  imprime(elementos, n, tam);
  printf("\n");
  #endif

  /*Libera memoria alocada*/
  for(i=0;i<n;i++){
		free(elementos[i]);
	}
	free(elementos);

  printf("tempo total = %f\n", t_total);

  return 0;
}
