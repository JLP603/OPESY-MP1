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

void nonPreSJF (FILE *fptr, int y){
	int i, j, total=0, pos, temp, limit = y, zero = 0;
	int arrival_time[100], burst_time[100], process_id[100], wait_time[100], ta_time[100], start_time[100], comp_time[100];
	float avg_wait_time;
	
	for(i = 0; i < limit; i++){
        fscanf(fptr, "%d %d %d", &process_id[i], &arrival_time[i], &burst_time[i]);
    }
    
    //sorting of arrival times so that the process with an arrival timem of 0 comes first
    
    for(i = 0; i < limit; i++){
        pos = i;
        // for each 0 it will increment the value of a variable zero
        for(j = i+1; j < limit; j++){
            if(arrival_time[j] < arrival_time[pos]){
            	pos = j;
            	if(arrival_time[j] == 0){
            		zero++;
				}
            }
        }
        temp = burst_time[i];
        burst_time[i] = burst_time[pos];
        burst_time[pos] = temp;
        temp = process_id[i];
        process_id[i] = process_id[pos];
        process_id[pos] = temp;
        temp = arrival_time[i];
        arrival_time[i] = arrival_time[pos];
        arrival_time[pos] = temp;
    }
    
	//sorting of burst times
    for(i = zero; i < limit; i++){
        pos = i;
        for(j = i+1; j < limit; j++){
            if(burst_time[j] < burst_time[pos]){
            	pos = j;
            }
        }
        temp = burst_time[i];
        burst_time[i] = burst_time[pos];
        burst_time[pos] = temp;
        temp = process_id[i];
        process_id[i] = process_id[pos];
        process_id[pos] = temp;
        temp = arrival_time[i];
        arrival_time[i] = arrival_time[pos];
        arrival_time[pos] = temp;
    }
    
    start_time[0] = 0;
	          
    for(i = 1; i < limit; i++){
        start_time[i] = 0;
        for(j = 0; j < i; j++){
            start_time[i] += burst_time[j];
        }
    }
    
    //ta_time = comp_time - arrival_time && wait_time = ta_time - burst_time
    
    for(i = 0; i < limit; i++){
    	comp_time[i] = start_time[i] + burst_time[i];
    	ta_time[i] = comp_time[i] - arrival_time[i];
    	wait_time[i] = ta_time[i] - burst_time[i];
        total += wait_time[i];
        printf("P[%d]\n", process_id[i]);
		printf("Start time: %d   End time: %d\n", start_time[i], comp_time[i]);
		//printf("Burst time: %d\n", burst_time[i]);
		printf("Waiting time: %d\n", wait_time[i]);
		printf("Turn around time: %d\n", ta_time[i]);
		printf("********************\n\n");
    }
    avg_wait_time = (float)total/limit;
    printf("Average Waiting Time = %f", avg_wait_time);
}

void PreemptSJF(FILE *fptr, int y){
      int limit=y;
      int process_id[100], arrival_time[100], burst_time[101], temp[100], ProcessCheck[y], prevProcess;
      int waitTimeOfTask[y],startTime[y][100],endTime[y][100],StartEndTimeLen[y],turnTimeOfTask=0;
      int i, smallest, count = 0, time;
      double wait_time = 0, end;
      float average_waiting_time;
      for(i=0;i<limit;i++)
      {
            fscanf(fptr, "%d %d %d\n", &process_id[i], &arrival_time[i], &burst_time[i]);
            temp[i] = burst_time[i];
            waitTimeOfTask[i] = 0;
            ProcessCheck[i] = 0;
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
                //printf("\nprevProcess:%d startEndTimeLen %d\n",prevProcess, StartEndTimeLen[prevProcess]);
                 endTime[prevProcess][StartEndTimeLen[prevProcess]]=time;
                 //printf("\nENDED! %d HERE!\n",endTime[prevProcess][StartEndTimeLen[prevProcess]]);
                 StartEndTimeLen[prevProcess]++;
                 ProcessCheck[prevProcess]=0;
           }
           
           if(ProcessCheck[smallest]==0){
                  //printf("\nsmallest:%d startEndTimeLen %d\n",smallest, StartEndTimeLen[smallest]);
                  startTime[smallest][StartEndTimeLen[smallest]]=time;
                  //printf("\nSTARTED! %d HERE!\n",startTime[smallest][StartEndTimeLen[smallest]]);
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

void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
// Function to perform Selection Sort
void selectionSort(int arr[], int n, int arr2[])
{
    int i, j, min_idx;
 
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++) {
 
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
 
        // Swap the found minimum element
        // with the first element
        swap(&arr[min_idx], &arr[i]);
        swap(&arr2[min_idx], &arr2[i]);
    }
}


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

void fcFs(FILE *fptr, int y) {
	
	int limit =y, i,j;
	int process_id[100], arrival_time[100], burst_time[100],waiting_time[100],turnaround_time[100];
	float average_weight_time = 0;
	
	for(i=0;i<limit;i++) {
            fscanf(fptr, "%d %d %d\n", &process_id[i], &arrival_time[i], &burst_time[i]);
    }  

    //to get waiting time
    waiting_time[0]=0; 
    for( i= 1;i < y; i++) {
        waiting_time[i]=0;
        for(j=0;j<i;j++) {
        	waiting_time[i] += burst_time[j];
		}
            
    }
    int itercount;
    
    for(i = 0; i<y; i++){
    	
		turnaround_time[i]=burst_time[i]+waiting_time[i];
        average_weight_time+=waiting_time[i];
       	printf("P[%d]\n", i);
		printf("Start time: %d   End time: %d\n", waiting_time[i], waiting_time[i]+burst_time[i]);
		printf("Waiting time: %d\n", waiting_time[i]);
		printf("Turn around time: %d\n", turnaround_time[i]);
		printf("********************\n\n");
		
    }
    
 	itercount = i;
    average_weight_time = average_weight_time / itercount;
    printf("Average Waiting Time:%f",average_weight_time);
	
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
