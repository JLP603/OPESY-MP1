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
      int prcs_wt_time[100],delay_boost=0,i,j;
      float avg_wt;
      for(i = 0; i < queue_num; i++){
        fscanf(fptr, "%d %d %d\n", &queue_id[i], &queue_prio[i], &quantum[i]);
        prcs_wt_time[i]=0;
        ProcessCheck[i] = 0;
        prcs_startend_len[i]=0;
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
      }
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
      prevProcess=101;
      //runs until the process counter is equal to # of processes
      for(time=0;count!=process_num;time++){
            //check for all the tasks to see what arrived and is also not complete
            for(i=0;i<process_num;i++){
                  if(arrival_time[i]<=time &&burst_time[i]>0){
                        //add task to q
                        ordered_q_contents[curr_q][ordered_q_content_len[curr_q]]=i;
                        ordered_q_content_len[curr_q]++;
                  }
            }
            //check for any tasks / processes coming out of IO freeze
            for(i=0;i<IO_prcs_len;i++){
                  //move IO process to a Queue
                  if(IO_prcs[i][1]==0){
                        ordered_q_contents[0][ordered_q_content_len[0]]=i;
                        ordered_q_content_len[0]++;
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
            //might need to change or remove this segment below
            //start checking for tasks at highest priority queue 
            curr_q=0;
            //check if the curr_q is empty, if it is go down all the qs until you reach the end
            if(ordered_q_content_len[curr_q]==0)
            {
                  while(ordered_q_content_len[curr_q]==0){
                        curr_q++;
                        curr_quant=0;
                        if(curr_q==queue_num&&ordered_q_content_len[curr_q]==0)
                        {
                              //not processes are available to do skip time and start at the highest queue again for jobs
                              //each process has its own quantum counter for each queue?
                              prcsfound=0;
                              break;
                        }
                  }
            }
            //If the process is in IO during prio boost do not include it in prioboost
            //schedule new tasks with prioboost like normal
            /*prio boost time if a process in ANY q is still running during prio boost, delay the scheduling 
            until the task is finished then schedule the tasks using prio boost*/
            if(time%prio_boost_time==0){
                  delay_boost=1;
            }
            //if you found any arrived processes for any of the queues 
            if(prcsfound==1){
                  //set the curr process to work on the task at the start of curr_q
                  curr_process=ordered_q_contents[curr_q][0];
                  
                  //check if it is the first process ever
                  if(curr_process!=prevProcess&&prevProcess==101){
                        curr_quant=0;
                        //update proccess start time for new process
                        process_start[curr_process][prcs_startend_len[curr_process]]=time;
                  }
                  //check if it was the same process as last time to reset the queue's quantum
                  if(curr_process!=prevProcess&&prevProcess!=101&&ProcessCheck[prevProcess]==1){
                        curr_quant=0;
                        //update proccess start time for new process
                        process_start[curr_process][prcs_startend_len[curr_process]]=time;
                        //update process end time for previous process
                        process_end[prevProcess][prcs_startend_len[prevProcess]]=time;
                        //assign what q the task finished in
                        prcs_q[prevProcess][prcs_startend_len[prevProcess]]=curr_q;
                        //the process is no longer waiting
                        ProcessCheck[prevProcess]=0;
                  }
                  //check if the process has exceeded the queue's quantum
                  if(curr_quant>quantum[curr_q])
                  {
                        //update curr process end time
                        process_end[curr_process][prcs_startend_len[curr_process]]=time;
                        //move the process to a lower queue
                        ///if u are already at the lowest quque
                        demote_pos=curr_q+1;
                        if(demote_pos==queue_num){
                              demote_pos=curr_q;
                        }
                        ordered_q_contents[demote_pos][ordered_q_content_len[demote_pos]]=curr_process;
                        ordered_q_content_len[demote_pos]++;
                        //process is waiting
                        ProcessCheck[curr_process]=1;
                        //prioboost
                        //If the process is in IO during prio boost do not include it in prioboost
                        /*prio boost time if a process in ANY q is still running during prio boost, delay the scheduling 
                        until the task is finished then schedule the tasks using prio boost*/
                        if(time%prio_boost_time==0||delay_boost==1){
                              //schedule new tasks with prioboost like normal
                              for(i=1;i<queue_num;i++){
                                    for(j=0;j<ordered_q_content_len[i];j++)
                                    {
                                          ordered_q_contents[0][ordered_q_content_len[0]]=ordered_q_contents[i][j];
                                          ordered_q_content_len[0]++;
                                    }
                                    ordered_q_content_len[i]=0;
                              }
                              delay_boost=0;
                        }
                  }
                  //if the quantum isnt over
                  else{
                        //if it is IO burst interval time
                        if(curr_quant%burst_interval[curr_process]==0){
                              IO_prcs[IO_prcs_len][0]=curr_process;
                              IO_prcs[IO_prcs_len][1]=IO_burst_time[curr_process];
                              IO_prcs_len++;
                              //remove from ordered q content
                        }
                        else{
                              burst_time[curr_process]--;
                              prevProcess=curr_process;
                              curr_quant++;
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
                        
                              
                  }
                  //update wait time (wait time is how long it waits in a q)
                  for(i=0;i<process_num;i++){
                        if(arrival_time[i]<=time&&burst_time[i]>0&&ProcessCheck[i]==1){
                              prcs_wt_time[i]++;
                        }
                  }
                  
            }
            else{
                  //prioboost
                  //If the process is in IO during prio boost do not include it in prioboost
                  /*prio boost time if a process in ANY q is still running during prio boost, delay the scheduling 
                  until the task is finished then schedule the tasks using prio boost*/
                  if(time%prio_boost_time==0||delay_boost==1){
                        //schedule new tasks with prioboost like normal
                        for(i=1;i<queue_num;i++){
                              for(j=0;j<ordered_q_content_len[i];j++)
                              {
                                    ordered_q_contents[0][ordered_q_content_len[0]]=ordered_q_contents[i][j];
                                    ordered_q_content_len[0]++;
                              }
                              ordered_q_content_len[i]=0;
                        }
                        delay_boost=0;
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
      fclose(fptr);

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