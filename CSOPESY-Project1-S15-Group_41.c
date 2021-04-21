/*
***************************************************************
Names: Pascual, Jeremy
       Pua, Shaun
       Soler, Javier
Group: CPU Scheduling 41
Section: CSOPESY-S15
***************************************************************
*/
/*
Input file format:
X Y Z
A B C - Y number of ABC lines
Where X == 0 is First Come First Serve
      X == 1 is Non Preemptive Shortest Job First
      x == 2 is Preemptive Shortest Job First
      x == 3 is Round Robin
      Y is >= 3 && <=100
      Z is a time slice value set to 1 when not Round Robin is not being used, otherwise is >=1 && <=100
Where A == Process ID, B == arival time, C == total execution time.

If the text file
exists, its contents will be processed, the output will be displayed, and the program
terminates. If the text file does not exist, the program outputs “<FILENAME.TXT> not
found.” error message and then terminates
*/
#include <stdio.h>
#include <stdlib.h> // For exit() function
void PreemptSJF(FILE *fptr, int y){
      int limit=y;
      int process_id[100], arrival_time[100], burst_time[100], temp[100];
      int i, smallest, count = 0, time;
      double wait_time = 0, turnaround_time = 0, end;
      float average_waiting_time, average_turnaround_time;
      for(i=0;i<limit;i++)
      {
            fscanf(fptr, "%d %d %d\n", &process_id[i], &arrival_time[i], &burst_time[i]);
            temp[i] = burst_time[i];
      }
      //for testing
      /*
      for(i=0;i<limit;i++)
      {
            printf("%d %d %d\n", process_id[i], arrival_time[i], burst_time[i]);
        
      }
      */
      burst_time[9] = 9999;  
      for(time = 0; count != limit; time++)
      {
            smallest = 9;
            for(i = 0; i < limit; i++)
            {
                  if(arrival_time[i] <= time && burst_time[i] < burst_time[smallest] && burst_time[i] > 0)
                  {
                        smallest = i;
                  }
            }
            burst_time[smallest]--;
            if(burst_time[smallest] == 0)
            {
                  count++;
                  end = time + 1;
                  wait_time = wait_time + end - arrival_time[smallest] - temp[smallest];
                  turnaround_time = turnaround_time + end - arrival_time[smallest];
            }
      }
      average_waiting_time = wait_time / limit; 
      average_turnaround_time = turnaround_time / limit;
      printf("\n\nAverage Waiting Time:\t%lf\n", average_waiting_time);
      printf("Average Turnaround Time:\t%lf\n", average_turnaround_time);
       /*
      Sample output:
            P[A]
            Start time: <S1> End time: <E1>
            Start time: <S2> End time: <E2>
            …
            Start time: <SN> End time: <EN>
            Waiting time: <WT>
            Turnaround time: <TT>
            ************************************
            P[A]
            Start time: <S1> End time: <E1>
            Start time: <S2> End time: <E2>
            …
            Start time: <SN> End time: <EN>
            Waiting time: <WT>
            Turnaround time: <TT>
            ************************************
            Average waiting time: <AWT>
      */
}

void roundRobin (FILE *fptr, int y, int quantum) {
	
	int limit=y;
    int process_id[100], arrival_time[100], burst_time[100], temp[100];
    
	printf("you selected round robin\n");
}
int main() 
{
      FILE *fptr;
      int x,y,z;
      char filename[50];
      //enter file name as input
      //checks if file exists reads data from file, else output error txt
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
            if(x==2)
            {
              PreemptSJF(fptr, y);
            } 
			if (x == 4) {
            	roundRobin(fptr, y, z);
			}
            fclose(fptr);
      }
      
      return 0;
}
