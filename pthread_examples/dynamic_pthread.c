#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <unistd.h>




#define MILLISECOND 1000
#define SECOND 100*MILLISECOND //1000*MILLISECOND <- One Second

typedef struct Threads {
    pthread_t *threads;
    int thread_count;
} thread_pool;

thread_pool *tids;

pthread_mutex_t lock;


void* doSomeThing(int *pId)
{
    pthread_mutex_lock(&lock);
    unsigned long i = 0;
    //pthread_t id = pthread_self();

    /*for(i=0; i<(0xF);i++){
        printf("%d from %d ",i,pId);
        printf("\n");
        fflush(stdout);
        usleep(SECOND);
    };*/


    const char progress[] = "|/-\\";

    for (i = 0; i < 100; i += 10) {
        printf("Processing: %3d%%\r",i); /* \r returns the caret to the line start */
        fflush(stdout);
        usleep(SECOND);
    }
    printf("\n"); /* goes to the next line */
    fflush(stdout);

    printf("Processing: ");
    for (i = 0; i < 100; i += 10) {
        printf("%c\b", progress[(i/10)%sizeof(progress)]); /* \b goes one back */
        fflush(stdout);
        usleep(SECOND);
    }
    printf("\n"); /* goes to the next line */
    fflush(stdout);


    printf("\nThread %d processing done\n",pId);


    pthread_mutex_unlock(&lock);

    return NULL;
}



int main(void)
{
    int i = 0;
    int err;

    int thread_counter=0;

    tids=(thread_pool *)malloc(1 * sizeof(thread_pool ));
    tids->threads=(pthread_t *)malloc(1 * sizeof(pthread_t ));
    tids->thread_count = 0;
    //MUTEX
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    //
    while(i < 9)
    {
        if(tids->thread_count!=0)
        {
            tids->threads =(pthread_t *)realloc(tids->threads,sizeof ( pthread_t )*(tids->thread_count+1)) ;
            if(tids==NULL)
                printf("error with realloc");
        }
        err = pthread_create(&(tids->threads[i]), NULL, &doSomeThing, tids->thread_count);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        else
            printf("\n Thread %d created successfully\n",tids->thread_count);

        tids->thread_count++;
        i++;
    }

    i = 0;
    while(i < tids->thread_count)
    {
        pthread_join(tids->threads[i], NULL);
        pthread_join(tids->threads[i], NULL);
        i++;
    }

    pthread_mutex_destroy(&lock);



    return 0;
}