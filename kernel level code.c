#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kthread.h>
#include<linux/sched.h>
#include<linux/time.h>
#include<linux/timer.h>
#include<linux/semaphore.h>
#include<linux/delay.h>

#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (ph_num+4)%N
#define RIGHT (ph_num+1)%N






static struct semaphore mutex;
static struct semaphore S[N];



static struct task_struct *philosopher_thread[5];


int state[N]={THINKING};
unsigned long long int philosopher_number[N]={0,1,2,3,4};

int repeat=0;

int philosopher(void *p_n)
{

      int ph_num=(int)p_n,i; 
      

      //pickup_chopstick(ph_num)
      down(&mutex);
      repeat++;
      state[ph_num]=HUNGRY;
      printk(KERN_INFO "Philosopher %d is Hungry .\n",ph_num+1);


      //test(ph_num)
      if( (state[ph_num]==HUNGRY)&&(state[LEFT]!=EATING)&&(state[RIGHT]!=EATING) )
      {
        state[ph_num]=EATING;
        printk(KERN_INFO "Philosopher %d takes %d and %d chopstick\n",ph_num+1,LEFT+1,ph_num+1);
        printk(KERN_INFO "Philosopher %d is eating .\n",ph_num+1);
        msleep(3);
        up(&S[ph_num]);
      }

      up(&mutex);
      down(&S[ph_num]);


      msleep(1);

      //put_chopstick(ph_num)
      down(&mutex);
      state[ph_num] = THINKING;
      printk(KERN_INFO "Philosopher %d putting fork %d and %d down \n",ph_num+1,LEFT+1,ph_num+1);
      printk(KERN_INFO "Philosopher %d is thinking .\n",ph_num+1);
      
     
      //test(LEFT)
      int p=ph_num;
      ph_num=LEFT;
      if( (state[ph_num]==HUNGRY)&&(state[LEFT]!=EATING)&&(state[RIGHT]!=EATING) )
      {
        state[ph_num]=EATING;
        printk(KERN_INFO "Philosopher %d takes %d and %d chopstick \n",ph_num+1,LEFT+1,ph_num+1);
        printk(KERN_INFO "Philosopher %d is eating .\n",ph_num+1);
        msleep(3);
        up(&S[ph_num]);
      }
      ph_num=p;


      //test(RIGHT)
      p=ph_num;
      ph_num=RIGHT;
      if( (state[ph_num]==HUNGRY)&&(state[LEFT]!=EATING)&&(state[RIGHT]!=EATING) )
      {
        state[ph_num]=EATING;
        printk(KERN_INFO "Philosopher %d takes %d and %d chopstick \n",ph_num+1,LEFT+1,ph_num+1);
        printk(KERN_INFO "Philosopher %d is eating .\n",ph_num+1);
        msleep(3);
        up(&S[ph_num]);
      }
      ph_num=p;

   up(&mutex);  
      
   do_exit(0);
   return 0;
}



asmlinkage long sys_dining(void)
{
  int i;
  char our_thread[8]="thread1";
  sema_init(&mutex,1);

  for(i=0;i<N;i++)
  {
     sema_init(&S[i],0);
  }

  for(i=0;i<N;i++)
  {
    philosopher_thread[i]=kthread_create(philosopher,(void *)philosopher_number[i],our_thread);
    if(philosopher_thread[i])
    {
       wake_up_process(philosopher_thread[i]);
    }
    printk(KERN_INFO "Philosopher %lld is Thinking .\n",philosopher_number[i]+1);
  }

 

  if(repeat>=4)
  {
     printk(KERN_INFO "Cleaning up kthread");
     for(i=0; i<N; i++)
     {
       kthread_stop(philosopher_thread[i]);
       printk(KERN_INFO "philossopher %d killed\n", i+1);		
     }
  }
		
 return 0;
 
}






