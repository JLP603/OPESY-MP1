/*
***************************************************************
Names: Pascual, Jeremy
       Pua, Shaun
       Soler, Javi
Group: 41
Section: S16
***************************************************************
*/

/*
Rule 1: If Priority(A) > Priority(B), A runs (B doesn’t).
Rule 2: If Priority(A) = Priority(B), A & B run in RR
Rule 3: When a job enters the system, it is placed at the highest priority (the topmost
queue).
Rule 4: Once a job uses up its time quantum at a given level (regardless of how many
times it has given up the CPU e.g. during an IO burst), its priority is reduced (i.e., it
moves down one queue).
Rule 5: After some time period S, move all the jobs in the system to the topmost queue.
This is what we refer to as a priority boost.
*/
/*
program takes the filename of an input text file with the following:
X Y S 
A B C}- X number of lines where A is queue ID B is thr priority and C is the quantum
F G H I J} Y lines based from the input file


All queues should use Round Robin time Scheduling and times are in the unit of mili seconds
where X is the num of queues and is 2<=X<=5
where Y is the number of processes 3<=Y<=100
where S is time time period for tjhe priority boost

Where F is the process ID, G is the arrival time, H is the total execution time and I is the length of IO burst time
J dictates how often the IO burst happens.
If it is a CPU bound process with no I/O variables I and J are 0 and context switch time is 0
*/
/*
Output:
P[A]
Q[X] Start time: <S1> End time: <E1>
Q[X] Start time: <S2> End time: <E2>
[IO] Start time: <S3> End time: <E4>
…
Q[X] Start time: <SN> End time: <EN>
Waiting time: <WT>
Turnaround time: <TT>
************************************
P[A]
Q[X] Start time: <S1> End time: <E1>
Q[X] Start time: <S2> End time: <E2>
…
Q[X] Start time: <SN> End time: <EN>
Waiting time: <WT>
Turnaround time: <TT>
************************************
Average waiting time: <AWT>
*/
#include<stdio.h> 
#include <stdlib.h>
#include<conio.h> 

void multilvl(FILE *fptr, int queue_num, int process_num, int prio_boost_time) 
{ 
      int queue_id[5], queue_prio[5], quantum[5], ordered_q_contents[5][100], ordered_q_content_len[5];
      int process_start[100][100],process_end[100][100],prcs_q[100][100],prcs_startend_len[100],prcsfound=1,demote_pos;
      int tmp,time,count=0,curr_q=0,curr_process,curr_quant,prevProcess,ProcessCheck[100],IO_prcs[100][2],IO_prcs_len=0;
      int process_id[100], arrival_time[100], burst_time[100],IO_burst_time[100],burst_interval[100],tmp_IO[100][2],tmp_IO_count=0;
      int prcs_wt_time[100],delay_boost=0,i,j,io_add=0,prcs_quant_q[100][4];
      float avg_wt=0.0;
      for(i = 0; i < queue_num; i++){
        fscanf(fptr, "%d %d %d\n", &queue_id[i], &queue_prio[i], &quantum[i]);
        prcs_wt_time[i]=0;
        ProcessCheck[i] = 0;
        prcs_startend_len[i]=0;
        ordered_q_content_len[i]=0;
      }
      //sort queue by prio
      for (i = 0; i < queue_num; i++){
            for (j = i + 1; j < queue_num; ++j){
                if (queue_prio[i] > queue_prio[j]) {
                    tmp =  queue_prio[i];

                    queue_prio[i] = queue_prio[j];

                    queue_prio[j] = tmp;
                    //
                    tmp =  queue_id[i];

                    queue_id[i] = queue_id[j];

                    queue_id[j] = tmp;
                    //
                    tmp =  quantum[i];

                    quantum[i] = quantum[j];

                    quantum[j] = tmp;
                }
            }
      }
      for(i = 0; i < process_num; i++){
        fscanf(fptr, "%d %d %d %d %d\n", &process_id[i], &arrival_time[i], &burst_time[i], &IO_burst_time[i], &burst_interval[i]);
      }
      for(i=0;i<process_num;i++){
            prcs_wt_time[i]=0;
            for(j=0;j<3;j++){
                  prcs_quant_q[i][j]=0;
            }
      }
      fclose(fptr);

      //schedule tasks by round robin in the queues and the queue's qunantum which is FCFS so...
      //sort by arrival time
      for (i = 0; i < process_num; i++){
            for (j = i + 1; j < process_num; ++j){
                if (arrival_time[i] > arrival_time[j]) {
                    tmp =  arrival_time[i];

                    arrival_time[i] = arrival_time[j];

                    arrival_time[j] = tmp;
                    //
                    tmp =  process_id[i];

                    process_id[i] = process_id[j];

                    process_id[j] = tmp;
                    //
                    tmp =  burst_time[i];

                    burst_time[i] = burst_time[j];

                    burst_time[j] = tmp;
                    //
                    tmp =  IO_burst_time[i];

                    IO_burst_time[i] = IO_burst_time[j];

                    IO_burst_time[j] = tmp;
                    //
                    tmp =  burst_interval[i];

                    burst_interval[i] = burst_interval[j];

                    burst_interval[j] = tmp;
                }
                //if the process has the same arrival time sort them by process id
                if(arrival_time[i] == arrival_time[j])
                {
                      if(process_id[i] > process_id[j]){
                        tmp =  process_id[i];

                        process_id[i] = process_id[j];

                        process_id[j] = tmp;
                      }
                }
            }
      }
      /*
      //test printing
      for(i=0;i<queue_num;i++){
            printf("Queue id: %d prio: %d quantum %d\n",queue_id[i],queue_prio[i],quantum[i]);
      }
      for(i=0;i<process_num;i++){
            printf("Process id: %d arrival time: %d burst time: %d IO burst time: %d IO interval %d\n",process_id[i],arrival_time[i],burst_time[i],IO_burst_time[i],burst_interval[i]);
      }
      */

      prevProcess=101;
      //runs until the process counter is equal to # of processes
      for(time=0;count!=process_num;time++){
            //testing
            printf("\nTIME RUNS! It is now: %d ms\n",time);
            for(i=0;i<process_num;i++){
                  printf("Process id: %d arrival time: %d burst time: %d IO burst time: %d IO interval %d\n",process_id[i],arrival_time[i],burst_time[i],IO_burst_time[i],burst_interval[i]);
            }
            //stopper for testing
            if(time==20){
                 printf("PROCESS FORCE STOP\n");             
                  break;
            }

            //check for all the tasks to see what arrived and is also not complete
            for(i=0;i<process_num;i++){
                  if(arrival_time[i]<=time&&burst_time[i]>0&&ProcessCheck[i]==0){
                        //add task to q
                        ordered_q_contents[0][ordered_q_content_len[0]]=i;
                        ordered_q_content_len[0]++;
                        ProcessCheck[i]=1;
                        //ABA
                        prcs_quant_q[i][3]=0;
                  }
            }

            //check for any tasks / processes coming out of IO freeze
            for(i=0;i<IO_prcs_len;i++){
                  printf("check io freeze\n");
                  
                  for(j=0;j<IO_prcs_len;j++){
                        printf("j= %d IO arr with process %d time remaining: %d\n",j,IO_prcs[j][0],IO_prcs[j][1]);
                  }
                  
                  //move IO process to a Queue
                  if(IO_prcs[i][1]==0){
                        ordered_q_contents[prcs_quant_q[IO_prcs[i][0]][3]][ordered_q_content_len[prcs_quant_q[IO_prcs[i][0]][3]]]=IO_prcs[i][0];
                        ordered_q_content_len[prcs_quant_q[IO_prcs[i][0]][3]]++;
                        ProcessCheck[IO_prcs[i][0]]=1;
                        //should probabbly be replaced by a quantum assigned to every process
                        //ABA
                        curr_quant=0;
                        io_add=1;
                  }
                  //move processes to start of IO array
                  if(IO_prcs[i][1]!=0){
                        tmp_IO[tmp_IO_count][0]=IO_prcs[i][0];
                        tmp_IO[tmp_IO_count][1]=IO_prcs[i][1];
                        tmp_IO_count++;
                  }  
            }
            //revalue IOprcs with tmp_IO and empty tmpIO out
            for (i = 0; i < IO_prcs_len; i++){
                  for (j = 0; j < IO_prcs_len; j++){
                        IO_prcs[i][j]=tmp_IO[i][j];
                        tmp_IO[i][j]=0;
                  }
            }
            IO_prcs_len=tmp_IO_count;
            tmp_IO_count=0;
            
            if(io_add==1){
                  //move the prev process to the end of the queue it inhabits this should actually just be for when a new IO process comes
                  tmp=ordered_q_contents[curr_q][0];
                  for(i=0;i<ordered_q_content_len[curr_q];i++){
                        
                        if(i==(ordered_q_content_len[curr_q]-1)){
                              ordered_q_contents[curr_q][i]=tmp;
                        }
                        else{
                              ordered_q_contents[curr_q][i]=ordered_q_contents[curr_q][i+1];
                        }
                  }
                  tmp=0;
                    
                  io_add=0;
            }

            //might need to change or remove this segment below
            //start checking for tasks at highest priority queue 
            curr_q=0;
            //check if the curr_q is empty, if it is go down all the qs until you reach the end
            if(ordered_q_content_len[curr_q]==0)
            {     
                  while(ordered_q_content_len[curr_q]==0&&prcsfound==1){
                        curr_q++;
                        //ABA
                        //curr_quant=0;
                        if(curr_q==queue_num)
                        {
                              //shouldnt each process have its own quantum counter for each queue?
                              curr_q=0;
                              prcsfound=0;
                        }
                  }
            }

            //testing, check contents of the queues
            for(i=0;i<queue_num;i++){
                  printf("Queue: %d process position put into queue:",i);
                  for(j=0;j<ordered_q_content_len[i];j++){
                        printf(" %d",ordered_q_contents[i][j]);
                  }
                  printf("\n");
            }

            
            //if you found any arrived processes for any of the queues 
            if(prcsfound==1){
                  //If the process is in IO during prio boost do not include it in prioboost
                  //schedule new tasks with prioboost like normal
                  /*prio boost time if a process in ANY q is still running during prio boost, delay the scheduling 
                  until the task is finished then schedule the tasks using prio boost*/
                  if(time%prio_boost_time==0&&time>=prio_boost_time){
                        delay_boost=1;
                  }
                  //set the curr process to work on the task at the start of curr_q
                  curr_process=ordered_q_contents[curr_q][0];
                  
                  //check if it is the first process ever
                  if(curr_process!=prevProcess&&prevProcess==101){
                        //ABA
                        //curr_quant=0;
                        //update proccess start time for new process
                        process_start[curr_process][prcs_startend_len[curr_process]]=time;
                  }
                  //check if it was the same process as last time to reset the queue's quantum
                  if(curr_process!=prevProcess&&prevProcess!=101&&ProcessCheck[prevProcess]==1){
                        //ABA
                        curr_quant=0;
                        //update proccess start time for new process
                        process_start[curr_process][prcs_startend_len[curr_process]]=time;
                        //update process end time for previous process
                        process_end[prevProcess][prcs_startend_len[prevProcess]]=time;
                        //assign what q the task finished in
                        prcs_q[prevProcess][prcs_startend_len[prevProcess]]=curr_q;
                        //the process is now waiting
                        ProcessCheck[prevProcess]=2;
                        
                  }
                  //check if the process has exceeded the queue's quantum
                  if(prcs_quant_q[curr_process][prcs_quant_q[curr_process][3]]+1>quantum[curr_q])
                  {     
                        //for testing
                        printf("\nQUANTUM EXCEEDED\n");
                        //update curr process end time
                        process_end[curr_process][prcs_startend_len[curr_process]]=time;
                        //move the process to a lower queue
                        ///if u are already at the lowest quque
                        demote_pos=curr_q+1;
                        if(demote_pos==queue_num){
                              demote_pos=curr_q;
                        }
                        //ABA
                        prcs_quant_q[curr_process][3]=demote_pos;
                        ordered_q_contents[demote_pos][ordered_q_content_len[demote_pos]]=curr_process;
                        ordered_q_content_len[demote_pos]++;
                        //remove process from queue
                        for(i=0;i<ordered_q_content_len[curr_q];i++){
                              if(i==ordered_q_content_len[curr_q]-1){
                                    ordered_q_contents[curr_q][i]=101;
                              }
                              else{
                                    ordered_q_contents[curr_q][i]=ordered_q_contents[curr_q][i+1];
                              }
                        }
                        ordered_q_content_len[curr_q]--;
                        //process is waiting
                        //ABA
                        ProcessCheck[curr_process]=2;
                        if(delay_boost==1){
                              //schedule new tasks with prioboost like normal
                              printf("BOOOSTTT!!!!\n");
                              for(i=1;i<queue_num;i++){
                                    for(j=0;j<ordered_q_content_len[i];j++)
                                    {
                                          ordered_q_contents[0][ordered_q_content_len[0]]=ordered_q_contents[i][j];
                                          ordered_q_content_len[0]++;
                                          prcs_quant_q[ordered_q_contents[i][j]][0]=0;
                                          prcs_quant_q[ordered_q_contents[i][j]][1]=0;
                                          prcs_quant_q[ordered_q_contents[i][j]][2]=0;
                                          prcs_quant_q[ordered_q_contents[i][j]][3]=0;
                                          //remove lower queue contents
                                          ordered_q_contents[i][j]=0;
                                    }
                                    ordered_q_content_len[i]=0;
                              }
                              delay_boost=0;
                        }

                        //reset curr process
                        curr_process=ordered_q_contents[curr_q][0];
                        //ABA
                        //reset curr quant
                        curr_quant=0;
                        //prcs_quant_q[curr_process][prcs_quant_q[curr_process][3]]=0;
                        if(ordered_q_content_len[curr_q]==0)
                        {
                              while(ordered_q_content_len[curr_q]==0&&prcsfound==1){
                                    curr_q++;
                                    //ABA
                                    //curr_quant=0;
                                    if(curr_q==queue_num)
                                    {
                                          //ABA
                                          //shouldnt each process have its own quantum counter for each queue?
                                          //curr_q=0;
                                          prcsfound=0;
                                          
                                    }
                              }
                              curr_process=ordered_q_contents[curr_q][0];
                        }
                        
                  }
                  //testing
                  printf("curr process = Q%d %d with quantum=%d and currquant of %d burstinterval of current process %d \n",curr_q,curr_process,prcs_quant_q[curr_process][curr_q],curr_quant,burst_interval[curr_process]);
                  //if it is IO burst interval time
                  if(burst_interval[curr_process]>0&&(
                  //ABA
                  //prcs_quant_q[curr_process][curr_q]
                  curr_quant
                  %burst_interval[curr_process])==0&&
                  //ABA
                  curr_quant
                  //prcs_quant_q[curr_process][curr_q]
                  >=burst_interval[curr_process]){
                        //for testing
                        printf("STOP FREEZE IN IO\n");
                        IO_prcs[IO_prcs_len][0]=curr_process;
                        IO_prcs[IO_prcs_len][1]=IO_burst_time[curr_process];
                        IO_prcs_len++;
                        //remove process from queue
                        for(i=0;i<ordered_q_content_len[curr_q];i++){
                              if(i==ordered_q_content_len[curr_q]-1){
                                    ordered_q_contents[curr_q][i]=101;
                              }
                              else{
                                    ordered_q_contents[curr_q][i]=ordered_q_contents[curr_q][i+1];
                              }
                        }
                        ordered_q_content_len[curr_q]--;
                        //process is waiting
                        ProcessCheck[curr_process]=2;
                        //reset curr process
                        //ABA
                        if(delay_boost==1){
                              //schedule new tasks with prioboost like normal
                              printf("BOOOSTTT!!!!\n");
                              for(i=1;i<queue_num;i++){
                                    printf("BOOOSTTT!!!!\n");
                                    for(j=0;j<ordered_q_content_len[i];j++)
                                    {
                                          printf("%d\n",ordered_q_contents[i][j]);
                                          ordered_q_contents[0][ordered_q_content_len[0]]=ordered_q_contents[i][j];
                                          ordered_q_content_len[0]++;
                                          prcs_quant_q[ordered_q_contents[i][j]][0]=0;
                                          prcs_quant_q[ordered_q_contents[i][j]][1]=0;
                                          prcs_quant_q[ordered_q_contents[i][j]][2]=0;
                                          prcs_quant_q[ordered_q_contents[i][j]][3]=0;
                                          //remove lower queue contents
                                          ordered_q_contents[i][j]=0;
                                          
                                    }
                                    ordered_q_content_len[i]=0;
                              }
                              delay_boost=0;
                        }
                        

                        curr_process=ordered_q_contents[curr_q][0];
                        //ABA
                        //reset curr quant
                        curr_quant=0;
                        if(ordered_q_content_len[curr_q]==0)
                        {
                              printf("No task in curr_q %d\n",curr_q);
                              while(ordered_q_content_len[curr_q]==0&&prcsfound==1){
                                    curr_q++;
                                    //ABA
                                    //curr_quant=0;
                                    if(curr_q==queue_num)
                                    {
                                          //shouldnt each process have its own quantum counter for each queue?
                                          curr_q=0;
                                          prcsfound=0;
                                    }
                              }
                              
                        }
                        printf("new curr_q %d\n",curr_q);
                        curr_process=ordered_q_contents[curr_q][0];
                        
                  }
                  if(prcsfound==1){
                        burst_time[curr_process]--;
                        prevProcess=curr_process;
                        //ABA
                        prcs_quant_q[curr_process][curr_q]++;
                        curr_quant++;
                        ProcessCheck[curr_process]=3;
                        //if a process finishes
                        if(burst_time[curr_process]==0){
                              count++;
                              printf("P[%d]\n",process_id[curr_process]);
                              for(i=0;i<prcs_startend_len[curr_process];i++){
                                    //if the process Q is not the available q, then the process had an I/O
                                    if(prcs_q[curr_process][i]==10){
                                          printf("[IO] Start time: %d End time: %d\n",process_start[curr_process][i],process_end[curr_process][i]);
                                    }
                                    else{
                                          printf("Q[%d] Start time: %d End time: %d\n",prcs_q[curr_process][i],process_start[curr_process][i],process_end[curr_process][i]);
                                    }
                              }
                              printf("Waiting time: %d\n",prcs_wt_time[curr_process]);
                              printf("Turnaround time: %d\n",time-arrival_time[curr_process]);
                              printf("************************************\n");
                        }
                  }
                  //update wait time (wait time is how long it waits in a q)
                  for(i=0;i<process_num;i++){
                        if(arrival_time[i]<=time&&burst_time[i]>0&&ProcessCheck[i]==2){
                              prcs_wt_time[i]++;
                        }
                  }
                  
            }
            else{
                  //prioboost
                  //If the process is in IO during prio boost do not include it in prioboost
                  /*prio boost time if a process in ANY q is still running during prio boost, delay the scheduling 
                  until the task is finished then schedule the tasks using prio boost*/
                  if(time%prio_boost_time==0&&time>=prio_boost_time){
                        //schedule new tasks with prioboost like normal
                        printf("BOOOSTTT!!!!\n");
                        for(i=1;i<queue_num;i++){
                              for(j=0;j<ordered_q_content_len[i];j++)
                              {
                                    ordered_q_contents[0][ordered_q_content_len[0]]=ordered_q_contents[i][j];
                                    ordered_q_content_len[0]++;
                                    //remove lower queue contents
                                    ordered_q_contents[i][j]=0;
                                    //not sure if i need this line below
                                    ProcessCheck[ordered_q_contents[i][j]]=1;
                                    prcs_quant_q[ordered_q_contents[i][j]][0]=0;
                                    prcs_quant_q[ordered_q_contents[i][j]][1]=0;
                                    prcs_quant_q[ordered_q_contents[i][j]][2]=0;
                                    prcs_quant_q[ordered_q_contents[i][j]][3]=0;
                                    
                              }
                              ordered_q_content_len[i]=0;
                        }
                  }
            }
            //update IO burst time
            for(i=0;i<IO_prcs_len;i++){
                  IO_prcs[i][1]--;
            }
            prcsfound=1;
      }
      for(i=0;i<process_num;i++){
            avg_wt=avg_wt+prcs_wt_time[i];
      }
      avg_wt=avg_wt/process_num;
      printf("Average waiting time: %lf\n",avg_wt);

      //in test case t3 went to q2 after being processed for two seconds in two different instances
      //move task to lower queue's tail if the task exceeds queue's quantum
      //new process that arrive append to the tail of lower queue
      //if a new process arrives at the higher level then start that first
      //keep checking for prio boost time. if it is prio boost time move all tasks to top queue
      //in what order that is? choose and specify if it is by queue order (and also queue priority) or by process arrival time
      //if a process is not finished with its quantum when it is prio boost time finish the process first before prio boosting
      //if a prio boost is delayed stick to the original time interval of prio boost (donot factor in the prio boost)
      
      //supposedly when your reach IO burst time you are free to do other process and so go to the nxt queue process. 
      //the one that got IO bursted goes to another place where it cools down having the task readded to the original queue's tail after the IO burst time completes 
      //quantum is not used during io wait
      //cpu burst time is execution time
      //the IO interval is based on the times the process executes ex: process cpu burst is 10 and interval is 3. every cpu execution it IO bursts at 3 6 9
      //I/O execution occurs for all processes at the same time
      //arriving process has priority over processes coming back from IO burst
      //wait time is the amount of time waiting for the process for the CPU (I/O does not factor into this)
      //turn around time is the amount of time it takes to complete the process (CPU process time + waiting time)
      

}
int main() 
{
      FILE *fptr;
      int x,y,z;
      char filename[50];
      //enter file name as input
      //checks if the file exists in the location or folder where this c file is located
      //reads data from file, or else it outputs error txt on the console.
      printf("\nEnter the filename of the input file (ex:input.txt):");
      scanf("%s", filename);
      fptr = fopen(filename, "r"); 
      if (fptr == NULL) {
        printf("%s not found.",filename);
        exit(1);
      }
      else{
      	//For testing
            //printf("%s found!\n",filename);
            fscanf(fptr, "%d %d %d\n", &x, &y, &z);
            //For testing
		//printf("%d %d %d found!\n",x,y,z);
		multilvl(fptr,x,y,z);
      }
      
      return 0;
}
