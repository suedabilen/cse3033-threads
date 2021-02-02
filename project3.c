#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h> 
#include <semaphore.h>

//Sueda Bilen 150117044
//Feyza Nur Bulgurcu 150117033 
//Edanur Öztürk 150117007
//Project3

//publisher struct
typedef struct publisher {
	int pubType;
	int pubID;//number of threads in a pubtype
	int bookTotal;
}publisher;


//packager struct 
typedef struct packager {
	int packSize;
	int packID;
}packager;

//buffer struct
typedef struct buffer {
int pubType;
int *buffer;
int in;
int out;
int bufferSize;
//sem_t empty;
//sem_t full;
//pthread_mutex_t mutex;
}buffer;

//books struct
typedef struct book{
	int bookType;
	int bookNum;
}books;




//Globals
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t empty;
sem_t full;
int in = 0;
int out = 0;

//publisher func
void *publish(void *publisher_n){
	struct publisher *come;
	come = malloc(sizeof(struct publisher));
	//come =  *(publisher *)publisher_n;
	come-> pubType = publisher_n->pubType;
	int loc;
	loc = come->pubType - 1;
	//int loc =*((int *)publisher_n->pubType)-1;
	sem_wait(&empty);
	pthread_mutex_lock(&mutex);
	if(buffer_arr[in]==
	buffer_arr[loc]->buffer[in]= books_arr[in]->bookNum;//book putting
	printf("Publisher %d of type %d, Book%d_%d is published and put into the buffer %d",
	publisher_n->pubID, publisher_n->pubType ,publisher_n->pubType,buffer_arr[loc]->buffer[in] ,buffer_arr[loc]->pubType);
	in = (in+1)%buffer_arr[loc]->bufferSize;
	
	pthread_mutex_unlock(&mutex);
	sem_post(&full);
	
		


}

//packager func
void *package(void *packager_n){

	
	sem_wait(&full);
        pthread_mutex_lock(&mutex);
        books_arr[out]->bookNum = buffer[out];
        
        
        printf("Packager %d Put Book%d_%d into the package",packager_n>packID,books_arr[out]->bookType,books[out]->bookNum);
        
        
        out = (out+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);


}





int main(int argc, char*argv[]){

int pubType;
int pubCount;
int packCount;
int bookNum;
int packageSize;
int bufferSize;


sem_init(&empty, 0, bufferSize);
sem_init(&full, 0, 0);
/*input ex*/
/*./project3.out -n 2 3 4 -b 5 -s 6 7*/
	//argument taking 
  	if(argc == 10 && strcmp(argv[1],"-n")==0 && strcmp(argv[5],"-b")==0 && strcmp(argv[7],"-s")==0){
        
         	pubType = atoi(argv[2]);
        	pubCount = atoi(argv[3]);
        	packCount = atoi(argv[4]);
        	bookNum = atoi(argv[6]);
       	packageSize = atoi(argv[8]);
        	bufferSize = atoi(argv[9]);
    	}

    	else
        	printf("not valid input! Control your arguments.");
	
//main thread
int totalThread = (pubType*pubCount)+packCount;
int totalPublisher=pubType*pubCount;
pthread_t thread[totalThread];


int k;
//buffer struct calling
buffer buffer_arr[pubType];
//for buffer creating
for (k = 0; k < pubType; k++){
    buffer_arr[k]->buffer = (int*)malloc(bufferSize * sizeof(int));
    buffer_arr[k]->bufferSize = bufferSize;
    buffer_arr[k]->pubType = k;
}


publisher publisher_arr[totalPublisher];
packager packager_arr[packCount];
books books_arr[bookNum*pubType*pubCount];


int i,j;

//for pthread creating
for(i = 0; i<pubCount*pubType;i++){

		publisher_arr[i].pubID=(i%pubCount)+1;
		publisher_arr[i].pubType=(i/pubCount)+1;
		publisher_arr[i].bookTotal=bookNum;
		pthread_create(&thread[i],NULL,publish,&publisher_arr[i]);
	if(i== (pubCount*pubType)-1){
		//for packager threads
		for(j = 0; j<packCount ; j++){
		packager_arr[j].packID=j+1;
		packager_arr[j].packSize=packageSize;
		
		pthread_create(&thread[i+j+1],NULL,package,&packager_arr[j]);
		}
	}	
}	



//publisher thread join
for(i = 0; i < pubCount*pubType; i++) {
        pthread_join(&thread[i], NULL);

	if(i == (pubCount*pubType)-1){
		//packager thread join
		for(j = 0; j < packCount; j++) {
       		pthread_join(&thread[i+j+1], NULL);
		}
	}
	
}
    
	printf("Exiting the system.");
	pthread_exit(NULL);
	
	
}


