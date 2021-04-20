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
void PreemptSJF(){
      int arrival_time[10], burst_time[10], temp[10];
      int i, smallest, count = 0, time, limit;
      double wait_time = 0, turnaround_time = 0, end;
      float average_waiting_time, average_turnaround_time;
      printf("\nEnter the Total Number of Processes:\t");
      scanf("%d", &limit); 
      printf("\nEnter Details of %d Processesn", limit);
      for(i = 0; i < limit; i++)
      {
            printf("\nEnter Arrival Time:\t");
            scanf("%d", &arrival_time[i]);
            printf("Enter Burst Time:\t");
            scanf("%d", &burst_time[i]); 
            temp[i] = burst_time[i];
      }
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
}
int main() 
{

      //enter file name as input
      //checks if file exists reads data from file, else output error txt

      int x;
      if(x==2)
      {
            PreemptSJF();
      }
      return 0;
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