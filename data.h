#ifndef DATA_H_
#define DATA_H_

typedef struct Process
{
  
  int id;                   
  int arrival_time;         
  int total_execution_time; 
  int io_burst_time;        
  int io_frequency;         

  
  int execution_time_left; 
  int io_burst_timer;      
  int io_burst_time_left;  
  int time_quantum_left;   
  int queue_index;

  
  int start_end_array_size; 
  int start_time[500];
  int end_time[500];
  char queue_id[500][10];
  int waiting_time;
  int turnaround_time;
} process;

typedef struct Node
{
  process *p;
  struct Node *next;
} node;

typedef struct Queue
{
  int id;           
  int priority;     
  int time_quantum; 

  node *front;
  node *rear;
} queue;

#endif
