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
	
	int i, sum=0,count=0, quant, wt=0, tat=0, limit=y;
	int qt = quantum;
    int process_id[100], arrival_time[100], burst_time[100], temp[100];
    float avg_wt, avg_tat; 
    
    for(i=0;i<limit;i++)
    {
        fscanf(fptr, "%d %d %d\n", &process_id[i], &arrival_time[i], &burst_time[i]);
        temp[i] = burst_time[i];
    }
      
    /*
    for(i=0;i<limit;i++)
	{
        printf("%d %d %d\n", process_id[i], arrival_time[i], burst_time[i]);
        
    }
    */
    for(sum=0, i = 0; limit!=0; )  
	{  
		if(temp[i] <= qt && temp[i] > 0) // define the conditions   
		{  
		    sum = sum + temp[i];  
		    temp[i] = 0;  
		    count=1;  
		    }     
		else if(temp[i] > 0)  
		{  
		    temp[i] = temp[i] - qt;  
		    sum = sum + qt;    
		}  
		if(temp[i]==0 && count==1)  
		{  
		    limit--; //decrement the process no.  
		    //printf("\nProcess No[%d] \t\t %d\t\t\t\t %d\t\t\t %d", i+1, burst_time[i], sum-arrival_time[i], sum-arrival_time[i]-burst_time[i]);
			printf("P[%d]\n", i+1);
			printf("Start time: %d   End time: %d\n", arrival_time[i], arrival_time[i]+ sum);
			printf("Waiting time: %d\n", sum-arrival_time[i]-burst_time[i]);
			printf("Turn around time: %d\n", sum-arrival_time[i]);
			printf("********************\n\n");
		    wt = wt+sum-arrival_time[i]-burst_time[i];  
		    tat = tat+sum-arrival_time[i];  
		    count =0;     
		}  
		if(i==y-1)  
		{  
		    i=0;  
		}  
		else if(arrival_time[i+1]<=sum)  
		{  
		    i++;  
		}  
		else  
		{  
			i=0;  
		}  
	}  
	// represents the average waiting time and Turn Around time  
	avg_wt = wt * 1.0/y;  
	avg_tat = tat * 1.0/y;  
	printf("\n Average Turn Around Time: \t%f", avg_wt);  
	printf("\n Average Waiting Time: \t%f", avg_tat);  
	
	getch();  
    
	
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
			if (x == 3) {
            	roundRobin(fptr, y, z);
			}
            fclose(fptr);
      }
      
      return 0;
}
