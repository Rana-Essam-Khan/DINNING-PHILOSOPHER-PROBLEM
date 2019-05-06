#include<stdio.h>
#include<semaphore.h>
#include<unistd.h>
#include<pthread.h>

#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (ph_num+4)%N
#define RIGHT (ph_num+1)%N


void *philosopher(void *);
void pickup_chopstick(int);
void test(int);
void put_chopstick(int);

sem_t mutex;
sem_t S[N];


int state[N]={THINKING};
int philosopher_number[N]={0,1,2,3,4};


int main()
{
  int i;
  pthread_t thread_id [N];
  sem_init(&mutex,0,1);

  for(i=0;i<N;i++)
  {
     sem_init(&S[i],0,0);
  }

  for(i=0;i<N;i++)
  {
    pthread_create(&thread_id[i],NULL,philosopher,&philosopher_number[i]);
    printf("Philosopher %d is Thinking .\n",philosopher_number[i]+1);
  }

  for(i=0;i<N;i++)
  {
   pthread_join(thread_id[i],0);
  }
 
}



void *philosopher(void *p_n)
{
   int k=0;
   while(k<5)
   {
      int *i=p_n;
      pickup_chopstick(*i);
      sleep(1);
      put_chopstick(*i);
      k++;
   }
}


void pickup_chopstick(int ph_num)
{
  sem_wait(&mutex);
  state[ph_num]=HUNGRY;
  printf("Philosopher %d is Hungry .\n",ph_num+1);
  test(ph_num);
  sem_post(&mutex);
  sem_wait(&S[ph_num]);
}


void test (int ph_num)
{
  if( (state[ph_num]==HUNGRY)&&(state[LEFT]!=EATING)&&(state[RIGHT]!=EATING) )
  {
    state[ph_num]=EATING;
    printf("Philosopher %d takes %d and %d chopstick \n",ph_num+1,LEFT+1,ph_num+1);
    printf("Philosopher %d is eating .\n",ph_num+1);
    sleep(1);
    sem_post(&S[ph_num]);
  }
}


void put_chopstick(int ph_num)
{
   sem_wait(&mutex);
   state[ph_num] = THINKING;
   printf("Philosopher %d putting fork %d and %d down .\n",ph_num+1,LEFT+1,ph_num+1);
   printf("Philosopher %d is thinking .\n",ph_num+1);
   test(LEFT);
   test(RIGHT);
   sem_post(&mutex);  

}




