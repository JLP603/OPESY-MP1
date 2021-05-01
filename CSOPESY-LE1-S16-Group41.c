/*
***************************************************************
Names: <Surname1, Name1>
<Surname2, Name2>
Group: <Group Number>
Section: <Section>s
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
      int queue_id[5], queue_prio[5], quantum[5];
      int tmp;
      int process_id[100], arrival_time[100], burst_time[100],IO_burst_time[100],burst_interval[100];
      for(int i = 0; i < queue_num; i++){
        fscanf(fptr, "%d %d %d\n", &queue_id[i], &queue_prio[i], &quantum[i]);
      }
      //sort queue by prio
      for (int i = 0; i < queue_num; ++i){
            for (int j = i + 1; j < queue_num; ++j){
                if (queue_prio[i] > queue_prio[j]) {
                    tmp =  queue_prio[i];

                    queue_prio[i] = queue_prio[j];

                    queue_prio[j] = tmp;
                    tmp =  queue_id[i];

                    queue_id[i] = queue_id[j];

                    queue_id[j] = tmp;
                    tmp =  quantum[i];

                    quantum[i] = quantum[j];

                    quantum[j] = tmp;
                }
            }
      }
      for(int i = 0; i < process_num; i++){
        fscanf(fptr, "%d %d %d\n", &process_id[i], &arrival_time[i], &burst_time[i], &IO_burst_time, &burst_interval);
      }
      for (int i = 0; i < queue_num; ++i){
            for (int j = i + 1; j < queue_num; ++j){
                if (queue_prio[i] > queue_prio[j]) {
                    tmp =  queue_prio[i];

                    queue_prio[i] = queue_prio[j];

                    queue_prio[j] = tmp;
                    tmp =  queue_id[i];

                    queue_id[i] = queue_id[j];

                    queue_id[j] = tmp;
                    tmp =  quantum[i];

                    quantum[i] = quantum[j];

                    quantum[j] = tmp;
                }
            }
      }
      //schedule tasks by round robin in the queues and the queue's qunantum which is FCFS so...
      //sort by arrival time?
      //note each process gets a quantum instance so if it a process completes b4 the quantum is up the next process starts with a fresh quantum
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