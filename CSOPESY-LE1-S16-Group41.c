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

#define N 10 
 
typedef struct 
{ 
      int process_id, arrival_time, burst_time, priority;
      int q, ready; 
}process_structure; 
 
int Queue(int t1) 
{ 
      if(t1 == 0 || t1 == 1 || t1 == 2 || t1 == 3) 
      { 
            return 1; 
      } 
      else
      {
            return 2; 
      }
} 
 
int multilvl(FILE *fptr, int x, int process_num, int z) 
{ 
      int limit, count, temp_process, time, j, y; 
      process_structure temp; 
      /*
      printf("Enter Total Number of Processes:\t"); 
      scanf("%d", &limit);
      */
      limit=process_num;
/*
//code to keep on reading lines of code with 3 variables
for(i = 0; i < limit; i++){
        fscanf(fptr, "%d %d %d", &process_id[i], &arrival_time[i], &burst_time[i]);
    }
*/
fclose(fptr);

      process_structure process[limit]; 
      for(count = 0; count < limit; count++) 
      { 
            printf("\nProcess ID:\t"); 
            scanf("%d", &process[count].process_id); 
            printf("Arrival Time:\t"); 
            scanf("%d", &process[count].arrival_time); 
            printf("Burst Time:\t"); 
            scanf("%d", &process[count].burst_time); 
            printf("Process Priority:\t"); 
            scanf("%d", &process[count].priority); 
            temp_process = process[count].priority; 
            process[count].q = Queue(temp_process);
            process[count].ready = 0; 
      }
      time = process[0].burst_time; 
      for(y = 0; y < limit; y++) 
      { 
            for(count = y; count < limit; count++) 
            { 
                  if(process[count].arrival_time < time) 
                  {
                        process[count].ready = 1; 
                  } 
            } 
            for(count = y; count < limit - 1; count++) 
            {
                  for(j = count + 1; j < limit; j++) 
                  { 
                        if(process[count].ready == 1 && process[j].ready == 1) 
                        { 
                              if(process[count].q == 2 && process[j].q == 1) 
                              { 
                                    temp = process[count]; 
                                    process[count] = process[j]; 
                                    process[j] = temp; 
                              } 
                        } 
                  } 
            } 
            for(count = y; count < limit - 1; count++) 
            { 
                  for(j = count + 1; j < limit; j++) 
                  {
                        if(process[count].ready == 1 && process[j].ready == 1) 
                        { 
                              if(process[count].q == 1 && process[j].q == 1) 
                              { 
                                    if(process[count].burst_time > process[j].burst_time) 
                                    { 
                                          temp = process[count]; 
                                          process[count] = process[j]; 
                                          process[j] = temp; 
                                    } 
                                    else 
                                    { 
                                          break; 
                                    } 
                              } 
                        } 
                  } 
            } 
            printf("\nProcess[%d]:\tTime:\t%d To %d\n", process[y].process_id, time, time + process[y].burst_time); 
            time = time + process[y].burst_time; 
            for(count = y; count < limit; count++) 
            { 
                  if(process[count].ready == 1) 
                  { 
                        process[count].ready = 0; 
                  } 
            } 
      } 
      return 0; 
}
/*
//Shaun's Round Robin Code
void roundRobin (FILE *fptr, int y, int quantum) {
	
    int i, sum=0, itercount =0, waiting_time = 0, turnaround_time =0, limit=y, qt = quantum;;
    int process_id[100], arrival_time[100], burst_time[100], temp[100], sorted_arrival[100], sorted_burst[100];
    float average_weight_time; 
    
    for(i=0;i<limit;i++)
    {
        fscanf(fptr, "%d %d %d\n", &process_id[i], &arrival_time[i], &burst_time[i]);
        temp[i] = burst_time[i];
        sorted_arrival[i] = arrival_time[i];
        sorted_burst[i] = burst_time[i];
    }
      
      
    int startTime[100];
    selectionSort(sorted_arrival, limit, sorted_burst);
    //selectionSort(sorted_burst, limit);
    
    int startVal = 0;
    for (i = 0; i < limit; i++) {
    	
    	if (i == 0) {
    		startVal = sorted_arrival[i];
    		startTime[i] = sorted_arrival[i];
    		if (qt > sorted_burst[i]) {
				startVal += sorted_burst[i];
				
			} else {
				startVal += qt;
				
			}
    		
		} else {
			if (qt > sorted_burst[i]) {
				
				startTime[i] = startVal;
				startVal += sorted_burst[i];
			} else {
				
				startTime[i] = startVal;
				startVal += qt;
			}
		}
	}
    
	for (sum = 0, i = 0; limit !=0;) {  
		if((temp[i] > 0) &&  (temp[i] <= qt)) {
			itercount = 1;  
		    sum = sum + temp[i];  
		    temp[i] = 0;  
		    
		}  else if(temp[i] > 0) {  
		    temp[i] = temp[i] - qt;  
		    sum = sum + qt;    
		}  if(itercount ==1 &&  temp[i]==0)  {  
		    limit--; 
		    waiting_time = waiting_time + sum-arrival_time[i]-burst_time[i];  
		    turnaround_time  = turnaround_time +sum-arrival_time[i];  
		    printf("P[%d]\n", i+1);
		    printf("Start time: %d   End time: %d\n", startTime[i], arrival_time[i]+ (sum-arrival_time[i]));
		    printf("Waiting time: %d\n", sum-arrival_time[i]-burst_time[i]);
		    printf("Turn around time: %d\n", sum-arrival_time[i]);
		    printf("********************\n");
		    itercount =0;
			if ((i+1) == y) {
				break;
			}    
		}  if(i==y-1) {  
		    i=0;  
		}  else if(arrival_time[i+1]<=sum)  {  
		    i++;  
		}  else  {  
			i=0;  
		}  
	}  
	
	average_weight_time = waiting_time * 1.0/y;   
	printf("Average Waiting Time: %f", average_weight_time);  
	
	 
}
*/

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