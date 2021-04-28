#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "errno.h"
#include "pthread.h"
#include "dirent.h"
#include "limits.h"
#include "stdatomic.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "unistd.h"

pthread_mutex_t lock;
pthread_cond_t notEmpty;
pthread_cond_t threadState;
atomic_int numOfFilesFound;
char * term;
atomic_int numOfErrorThreads;
atomic_int numOfSleepingThreads;


/////////////////////Queue implementation/////////////////////////
struct Element{
    DIR *dir;
    char * path;
    struct Element * next;
};

struct Queue{
    struct Element * head;
    struct Element * tail;
    int length;
};

//is queue empty
int isEmptyQueue(struct Queue queue){
    if(queue.length==0){
      return 1;
    }
    return 0;
}

int QueueInit(struct Queue * queue,DIR * dir, char * path){//called only when queue is empty(called from main or enqueue where lock is already handled)
    struct Element * elementFirst=malloc(sizeof(struct Element));
    if(elementFirst==NULL){
        fprintf(stderr,"%s\n",strerror(errno));
        return -1;
    }

    elementFirst->dir=dir;
    elementFirst->path=malloc(PATH_MAX);
    if(elementFirst->path==NULL){
        fprintf(stderr,"%s\n",strerror(errno));
        return -1;
    }
    strcpy(elementFirst->path,path);
    elementFirst->next=NULL;
    queue->head=elementFirst;
    queue->tail=elementFirst;
    queue->length=1;
    return 0;
}

//add element at the end of the queue
int enqueue(struct Queue * queue,DIR * dir, char * path){
    pthread_mutex_lock(&lock);
    if(isEmptyQueue(*queue)){//if empty Queue init
        if(QueueInit(queue,dir,path)<0){
            pthread_mutex_unlock(&lock);
            return -1;
        }
        pthread_mutex_unlock(&lock);
        return 0;
    }
    struct Element * element =malloc(sizeof(struct Element));
    if(element==NULL){
        pthread_mutex_unlock(&lock);
        fprintf(stderr,"%s\n",strerror(errno));
        return -1;
    }
    element->dir=dir;
    element->path=malloc(PATH_MAX);
    if(element->path==NULL){
        pthread_mutex_unlock(&lock);
        fprintf(stderr,"%s\n",strerror(errno));
        return -1;
    }
    strcpy(element->path,path);
    element->next=NULL;
    queue->tail->next=element;
    queue->tail=queue->tail->next;
    queue->length++;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&notEmpty);
    return 0;
}

//only called on a non empty Queue
struct Element * dequeue(struct Queue * queue){
    pthread_mutex_lock(&lock);
    while(isEmptyQueue(*queue)){// while empty wait for signal
        numOfSleepingThreads++;
        pthread_cond_wait(&notEmpty,&lock);
        numOfSleepingThreads--;
    }
    struct Element * temp = queue->head;
    if(queue->length==1){
        queue->head=NULL;
    }
    else{
        queue->head=queue->head->next;
    }
    queue->length--;
    pthread_mutex_unlock(&lock);
    return temp;
}

////////////////////////Thread work///////////////////////////////////
void * thread_work(void * arg){
    //wait until called to start
    pthread_mutex_lock(&lock);
    numOfSleepingThreads++;
    pthread_cond_wait(&threadState, &lock);
    numOfSleepingThreads--;
    pthread_mutex_unlock(&lock);

    struct Queue *queue = (struct Queue *) arg;

    while(1==1) {
        struct stat status;
        char path[PATH_MAX];

        //2 - dequeue the directory
        struct Element *temp = dequeue(queue);
        DIR *dir = temp->dir;
        char *relativePath = temp->path;
        free(temp);

        //3 - search for the term in all files in the directory
        errno = 0;//to distinguish between error and no more files
        struct dirent *file = readdir(dir);
        if (errno != 0) {//readdir failed
            fprintf(stderr, "%s\n", strerror(errno));
            numOfErrorThreads++;
            pthread_exit(NULL);
        }
        while (file != NULL) {
            if (strcmp(file->d_name, ".") && strcmp(file->d_name, "..")) {
                if (getcwd(path, PATH_MAX) == NULL) {//to get path
                    fprintf(stderr, "%s\n", strerror(errno));
                    numOfErrorThreads++;
                    pthread_exit(NULL);
                }

                strcat(path, "/");
                strcat(path, relativePath);
                strcat(path, "/");
                strcat(path, file->d_name);
                if (lstat(path, &status) != 0) {
                    fprintf(stderr, "%s\n", strerror(errno));
                    numOfErrorThreads++;
                    pthread_exit(NULL);
                }


                if (S_ISDIR(status.st_mode)) {//if directory
                    if (access(file->d_name, R_OK) && access(file->d_name, X_OK)) {//if accessible add to queue
                        char newDirPath[PATH_MAX] = "";
                        strcat(newDirPath, relativePath);
                        strcat(newDirPath, "/");
                        strcat(newDirPath, file->d_name);
                        DIR *newDir;
                        newDir = opendir(newDirPath);

                        if (newDir == NULL && errno==EACCES) {
                            printf("Directory %s: Permission denied.\n", file->d_name);
                        }
                        else if (newDir == NULL &&errno!=EACCES) {
                            fprintf(stderr, "%s\n", strerror(errno));
                            numOfErrorThreads++;
                            pthread_exit(NULL);
                        }
                        else {
                            if (enqueue(queue, newDir, newDirPath) < 0) {//error in enqueue(already printed)
                                numOfErrorThreads++;
                                pthread_exit(NULL);
                            }
                            if (numOfSleepingThreads > 0) {//if there are sleeping threads wake one to do the work
                                pthread_cond_signal(&threadState);
                            }
                        }
                    } else {//directory not accessible
                        printf("Directory %s: Permission denied.\n", file->d_name);
                    }
                } else {// look for term in file name
                    if (strstr(file->d_name, term) != NULL) {// meaning the term is in the filename
                        numOfFilesFound++;
                        char filePath[PATH_MAX];
                        strcpy(filePath,"");
                        strcat(filePath, relativePath);
                        strcat(filePath, "/");
                        strcat(filePath, file->d_name);
                        printf("%s\n", filePath);
                    }
                }
            }

            errno = 0;
            file = readdir(dir);
            if (errno != 0) {//readdir failed
                fprintf(stderr, "%s\n", strerror(errno));
                numOfErrorThreads++;
                pthread_exit(NULL);
            }
        }

        if (closedir(dir) < 0) {//error on closing directory
            fprintf(stderr, "%s\n", strerror(errno));
            numOfErrorThreads++;
            pthread_exit(NULL);
        }
    }
}

///////////////////////main work//////////////////////////////////////
int main(int argc, char *argv[]) {
    int result;
    char *endptr;
    int numOfThreads;

    if(argc!=4){// check that exactly 3 arguments were passed
        fprintf(stderr,"%s\n",strerror(EINVAL));
        exit(1);
    }

    // Initialize mutex and condition variables objects
    result=pthread_mutex_init(&lock, NULL);
    if(result){
        fprintf(stderr,"%s\n",strerror(errno));
        exit(1);
    }
    result=pthread_cond_init(&notEmpty, NULL);
    if(result){
        fprintf(stderr,"%s\n",strerror(errno));
        exit(1);
    }
    result=pthread_cond_init(&threadState, NULL);
    if(result){
        fprintf(stderr,"%s\n",strerror(errno));
        exit(1);
    }

    //set number of threads
    numOfThreads=strtol(argv[3],&endptr,10);
    if((*endptr)!='\0'){//strtol failure
        fprintf(stderr,"%s\n",strerror(errno));
        exit(1);
    }

    pthread_t threads[numOfThreads];
    DIR * dir;
    term=argv[2];
    numOfErrorThreads=0;
    numOfSleepingThreads=0;

    //open given directory
    dir = opendir(argv[1]);
    if(dir==NULL){
        fprintf(stderr,"%s\n",strerror(errno));
        exit(1);
    }

    //queue init
    struct Queue  * queue=malloc(sizeof(struct Queue));
    if(queue==NULL){
        fprintf(stderr,"%s\n",strerror(errno));
        exit(1);
    }
    if(QueueInit(queue,dir,argv[1])<0){//error already printed in QueueInit
        exit(1);
    }

    //create threads
    for(int i=0; i < numOfThreads; i++){
        result=pthread_create(&threads[i],NULL,&thread_work,(void *)queue);
        if(result){
            fprintf(stderr,"%s\n",strerror(errno));
            exit(1);
        }
    }

    //signal threads to start searching
    while(numOfSleepingThreads!=numOfThreads){}//wait for all threads to be created
    pthread_cond_signal(&threadState);



    //run until all threads exited due to an error or there are no more directories in the queue
    //and all live searching threads are idle
    while(numOfErrorThreads<numOfThreads && (!isEmptyQueue(*queue) || numOfSleepingThreads+numOfErrorThreads!=numOfThreads)){
    }


    for(int i=0; i < numOfThreads; i++){
        pthread_cancel(threads[i]);
    }

    pthread_mutex_trylock(&lock);
    pthread_mutex_unlock(&lock);


    //exit the program
    result=pthread_cond_destroy(&notEmpty);
    result=pthread_cond_destroy(&threadState);
    result=pthread_mutex_destroy(&lock);

    free(queue);
    printf("Done searching, found %d files\n",numOfFilesFound);

    if(numOfErrorThreads==0){
        exit(0);
    }
    else{
        exit(1);
    }
}

