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
#include <stdlib.h>
#include<conio.h>   // For exit() function
void PreemptSJF(FILE *fptr, int y){
      int limit=y,waitingVar[y];
      int process_id[100], arrival_time[100], burst_time[101], temp[100], ProcessCheck[y], prevProcess;
      int waitTimeOfTask[y],startTime[y][100],endTime[y][100],StartEndTimeLen[y],turnTimeOfTask=0;
      int i,j, smallest, count = 0, time;
      double wait_time = 0, end;
      float average_waiting_time;
      for(i=0;i<limit;i++)
      {
            fscanf(fptr, "%d %d %d\n", &process_id[i], &arrival_time[i], &burst_time[i]);
            temp[i] = burst_time[i];
            waitTimeOfTask[i] = 0;
            ProcessCheck[i] = 0;
            waitingVar[i]=0;
            StartEndTimeLen[i]=0;
      }
      prevProcess=101;
      burst_time[limit]=9999;
      for(time=0;count!=limit;time++){
           smallest = limit;
           for(i=0;i<limit;i++){
                 if(arrival_time[i]<=time && burst_time[i] <burst_time[smallest]&&burst_time[i]>0){
                       
                       smallest = i;
                 }
           }
           if(smallest!=prevProcess&&prevProcess!=101&&ProcessCheck[prevProcess]==1){
                printf("\nprevProcess:%d startEndTimeLen %d\n",prevProcess, StartEndTimeLen[prevProcess]);
                 endTime[prevProcess][StartEndTimeLen[prevProcess]]=time;
                 printf("\nENDED! %d HERE!\n",endTime[prevProcess][StartEndTimeLen[prevProcess]]);
                 StartEndTimeLen[prevProcess]++;
                 ProcessCheck[prevProcess]=0;
           }
           
           if(ProcessCheck[smallest]==0){
                  printf("\nsmallest:%d startEndTimeLen %d\n",smallest, StartEndTimeLen[smallest]);
                  startTime[smallest][StartEndTimeLen[smallest]]=time;
                  printf("\nSTARTED! %d HERE!\n",startTime[smallest][StartEndTimeLen[smallest]]);
                  ProcessCheck[smallest]=1;
           }
           
           for(i=0;i<limit;i++){
                  if(arrival_time[i]<=time&&burst_time[i]>0&&ProcessCheck[i]==0){
                       waitTimeOfTask[i]++;
                 }
           }
           
           prevProcess=smallest;

           
           burst_time[smallest]=burst_time[smallest]-1;
           if(burst_time[smallest] == 0)
            {
                  count++;
                  end = time + 1;
                  wait_time = wait_time + end - arrival_time[smallest] - temp[smallest];
                  printf("P[%d]\n",process_id[smallest]);
                  endTime[smallest][StartEndTimeLen[smallest]]=end;
                  ProcessCheck[smallest]=0;
                  StartEndTimeLen[smallest]++;
                  for(i=0;i<StartEndTimeLen[smallest];i++){
                        printf("Start time: %d End time: %d\n",startTime[smallest][i],endTime[smallest][i]);
                  }
                  printf("Waiting time: %d\n",waitTimeOfTask[smallest]);
                  for(i=0;i<StartEndTimeLen[smallest];i++)
                  {
                        turnTimeOfTask=turnTimeOfTask+endTime[smallest][i];
                  }
                  for(i=0;i<StartEndTimeLen[smallest];i++)
                  {
                        turnTimeOfTask=turnTimeOfTask-startTime[smallest][i];
                  }
                  turnTimeOfTask=turnTimeOfTask+waitTimeOfTask[smallest];
                  printf("Turnaround time: %d\n",turnTimeOfTask);
                  printf("************************************\n");
                  ProcessCheck[smallest]=3;
                  turnTimeOfTask=0;
            }
     }
      average_waiting_time=wait_time/limit; 
      printf("Average waiting time: %lf\n",average_waiting_time);
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
      
    
    for(sum=0, i = 0; limit!=0; )  
	{  
		if(temp[i] <= qt && temp[i] > 0) {
		
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
		    limit--; 
		    //printf("\nProcess No[%d] \t\t %d\t\t\t\t %d\t\t\t %d", i+1, burst_time[i], sum-arrival_time[i], sum-arrival_time[i]-burst_time[i]);
		    printf("P[%d]\n", i+1);
		    printf("Start time: %d   End time: %d\n", arrival_time[i], arrival_time[i]+ (sum-arrival_time[i]));
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
	
	avg_wt = wt * 1.0/y;   
	printf("\n Average Waiting Time: \t%f", avg_wt);  
	
	getch();  
}

void fcFs(FILE *fptr, int y) {
	
	int limit =y;
	
	int process_id[20], arrival_time[20], burst_time[20], temp[20],wt[20],tat[20],i,j;
	float avwt=0,avtat=0;
	
	for(i=0;i<limit;i++)
      {
            fscanf(fptr, "%d %d %d\n", &process_id[i], &arrival_time[i], &burst_time[i]);
            temp[i] = burst_time[i];
      }
	
	wt[0]=0;    //waiting time for first process is 0

      //calculating waiting time
      for(i=1;i<y;i++)
      {
            wt[i]=0;
            for(j=0;j<i;j++)
                  wt[i]+=burst_time[j];
      }
 
    //calculating turnaround time
    
    int startTime = 0;
    int endTime = burst_time[0];
    for(i=0;i<y;i++)
    {
        
		tat[i]=burst_time[i]+wt[i];
            avwt+=wt[i];
            //printf("\nP[%d]\t\t%d\t\t%d\t\t%d",i+1,burst_time[i],wt[i],tat[i]);
       	printf("P[%d]\n", i+1);
		printf("Start time: %d   End time: %d\n", wt[i], wt[i]+burst_time[i]);
		printf("Waiting time: %d\n", wt[i]);
		printf("Turn around time: %d\n", tat[i]);
		printf("********************\n\n");
		
    }
 
    avwt/=i;
    printf("\nAverage Waiting Time:%.2f",avwt);
	
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
		if (x == 0) {
			fcFs(fptr, y);
		}
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
