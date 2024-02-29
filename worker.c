#include<stdio.h>
#include<math.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<stdlib.h>


#define SHMKEY1 2031535
#define SHMKEY2 2031536
#define SHMKEY3 2041537
#define BUFF_SZ sizeof(int)


int main(int argc, char** argv){
    
    
    //Set up shared memory pointer for struct
    int shm_id = shmget(SHMKEY1, BUFF_SZ, IPC_CREAT | 0777);
    if(shm_id <= 0){
        fprintf(stderr, "Shared memory get failed\n");
        exit(1);
    }
    int* sharedSeconds = shmat(shm_id, 0, 0);

    //Set up shared memory pointer
    shm_id = shmget(SHMKEY2, BUFF_SZ, IPC_CREAT | 0777);
    if(shm_id <= 0){
        fprintf(stderr, "Shared memory get failed\n");
        exit(1);
    }
    int* sharedNano = shmat(shm_id, 0, 0);
    



    //Work
    
    int pid = getpid();
    int ppid = getppid();

    //Parse out current SysClock and SysNano time
    int sysClockS = *sharedSeconds; //Starting seconds
    int sysClockNano = *sharedNano; //Starting nanoseconds
    int timeLimitSeconds = atoi(argv[1]) + sysClockS; //upper bound, passed from calling parent
    int timeLimitNano = sysClockNano; 
    int seconds;
    int nano;   
    int timeElapsed;
    int timer = 0;

    printf("WORKER PID: %d PPID: %d SysClockS: %d SysClockNano: %d TermTimeS: %d TermTimeNano: %d\n--Just Starting\n"
            , pid, ppid, *sharedSeconds, *sharedNano, timeLimitSeconds, timeLimitNano);

     
    while(timeLimitSeconds > (*sharedSeconds) || ((timeLimitSeconds == (*sharedSeconds)) && (timeLimitNano > (*sharedNano)))){
        

        timeElapsed = *sharedSeconds - sysClockS;
        
        
        
        if((*sharedSeconds) - timer == 1){
            timer = *sharedSeconds;
            printf("WORKER PID: %d PPID: %d SysClockS: %d SysClockNano: %d TermTimeS: %d TermTimeNano: %d\n--%d seconds have passed since starting\n"
            , pid, ppid, *sharedSeconds, *sharedNano, timeLimitSeconds, timeLimitNano, timeElapsed);
        }
        

            

    }
    
    
   
    printf("WORKER PID: %d PPID: %d SysClockS: %d SysClockNano: %d TermTimeS: %d TermTimeNano: %d\n--Terminating\n"
            , pid, ppid, *sharedSeconds, *sharedNano, timeLimitSeconds, timeLimitNano);




    //Unattach shared memory pointer
    shmdt(sharedSeconds);
    shmdt(sharedNano);
      
    return EXIT_SUCCESS;
}
