/*
***************************************************************
Names: Pascual, Jeremy
       Pua, Shaun
       Soler, Javi
Group: 41
Section: S16
***************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data.h"

// GLOBAL VAR
int number_of_processes;
process p[100];
int number_of_queues;
queue q[5];
int priority_boost_time;
queue io;
float avg_waiting_time;

#include "datafunc.h"

int isStringDigitsOnly(const char *str)
{
  while (*str)
  {
    if (*str != '\n' && *str != ' ' && isdigit(*str++) == 0)
      return 0;

    *str++;
  }
  return 1;
}

void enqueue_to_lower(process *p1)
{
  if (p1->queue_index == number_of_queues - 1)
  {
    enqueue(&q[p1->queue_index], p1);
  }
  else
  {
    p1->queue_index++;
    enqueue(&q[p1->queue_index], p1);
  }
  p1->time_quantum_left = q[p1->queue_index].time_quantum;
}

void execute_priority_boost()
{
  int i;
  for (i = 1; i < number_of_queues; i++)
  {
    while (!is_queue_empty(&q[i]))
    {
      process *p1 = dequeue(&q[i]);
      p1->queue_index = 0;
      p1->time_quantum_left = q[0].time_quantum;
      enqueue(&q[0], p1);
    }
  }
}

int are_processes_done()
{
  int flag = 1; //TRUE (processes are done)
  int i;
  for (i = 0; i < number_of_processes; i++)
  {
    if (p[i].execution_time_left != 0)
    {
      flag = 0; //FALSE (processes are not done)
      break;
    }
  }
  return flag;
}

void enqueue(queue *q, process *p1)
{
  node *qNode = create_node(p1);

  if (is_queue_empty(q))
  {
    q->front = qNode;
    q->rear = qNode;
  }
  else
  {
    q->rear->next = qNode;
    q->rear = qNode;
  }
}

int is_queue_empty(queue *q)
{
  return q->rear == NULL;
}

void update_burst_left_in_IO()
{
  queue *temp = createQueue();
  process *p1;

  while (!is_queue_empty(&io))
  {
    p1 = dequeue(&io);
    p1->io_burst_time_left--;
    enqueue(temp, p1);
  }

  while (!is_queue_empty(temp))
  {
    p1 = dequeue(temp);
    enqueue(&io, p1);
  }
}

void remove_completed_IO(int current_time)
{
  int i;
  queue *temp = createQueue();
  process *p1;

  while (!is_queue_empty(&io))
  {
    p1 = dequeue(&io);
    enqueue(temp, p1);
  }

  while (!is_queue_empty(temp))
  {
    p1 = dequeue(temp);
    if (p1->io_burst_time_left != 0)
      enqueue(&io, p1);
    else
    {
      //list end time for IO and update array size
      p1->end_time[p1->start_end_array_size] = current_time;
      p1->start_end_array_size++;

      if (p1->time_quantum_left == 0)
        enqueue_to_lower(p1);
      else
      {
        //reset time quantum left if already in the lowest queue
        if (p1->queue_index == number_of_queues - 1)
          p1->time_quantum_left = q[p->queue_index].time_quantum;

        enqueue(&q[p1->queue_index], p1);
      }
    }
  }
}

int has_ready_higher_priority_job(process *p1)
{
  int flag = 0; //FALSE
  int i;
  for (i = 0; i < p1->queue_index; i++)
    if (!is_queue_empty(&q[i]))
    {
      flag = 1; //TRUE
      break;
    }

  return flag;
}

void show_processes()
{
  int i;
  for (i = 0; i < number_of_processes; i++)
    printf("P[%d] %d %d %d %d \n", p[i].id, p[i].arrival_time, p[i].total_execution_time,
           p[i].io_burst_time, p[i].io_frequency);
}

int are_all_queues_empty()
{
  int flag = 1; //TRUE
  int i;
  for (i = 0; i < number_of_queues; i++)
    if (!is_queue_empty(&q[i]))
    {
      flag = 0;
      break;
    }

  return flag;
}

void record_end_time(process *p1, int current_time)
{
  p1->end_time[p1->start_end_array_size] = current_time;
  p1->start_end_array_size++;
}

void record_start_time(process *p1, int current_time)
{
  char str[10];
  sprintf(str, "%d", q[p1->queue_index].id);
  strcpy(p1->queue_id[p1->start_end_array_size], str);
  p1->start_time[p1->start_end_array_size] = current_time;
}

void record_IO_start_time(process *p1, int current_time)
{
  strcpy(p1->queue_id[p1->start_end_array_size], "IO");
  p1->start_time[p1->start_end_array_size] = current_time;
}

void initialize_io_timer(process *p1)
{
  p1->io_burst_timer = 0;
  p1->io_burst_time_left = p1->io_burst_time;
}

int main(void)
{

  FILE *fp;
  char filepath[261] = "./";
  char filename[261];
  char line[256];
  char *token;
  int i, j;

  int current_time = 0;
  process *p1 = NULL;
  int priority_boost_flag = 0;

  float X, Y, S;
  float A[100], B[100], C[100];
  float F[100], G[100], H[100], I[100], J[100];
  int dX, dY;

  avg_waiting_time = 0;

  printf("Enter filename : ");
  scanf("%s", filename);

  strcat(filepath, filename);

  fp = fopen(filepath, "r");

  if (fp == NULL)
  {
    printf("cannot find file \"%s\"", filename);
    exit(1);
  }

  if (fgets(line, 256, fp) != NULL)
  {
    long int filePosition;
    int tokenCounterPerLine;
    token = strtok(line, " ");
    tokenCounterPerLine = 0;

    while (token != NULL)
    {
      tokenCounterPerLine++;
      token = strtok(NULL, " ");
    }

    fseek(fp, 0, SEEK_SET);
    fscanf(fp, " %f %f %f", &X, &Y, &S);
    dX = X;
    dY = Y;
    fseek(fp, 0, SEEK_SET);
    fgets(line, 256, fp);

    filePosition = ftell(fp);

    for (i = 0; i < dX; i++)
    {
      if (fgets(line, 256, fp) != NULL)
      {
        token = strtok(line, " ");
        tokenCounterPerLine = 0;

        while (token != NULL)
        {

          tokenCounterPerLine++;

          token = strtok(NULL, " ");
        }
      }
      else
      {

        exit(1);
      }
    }

    fseek(fp, filePosition, SEEK_SET);
    for (i = 0; i < dX; i++)
      fscanf(fp, " %f %f %f", &A[i], &B[i], &C[i]);

    fseek(fp, filePosition, SEEK_SET);
    for (i = 0; i < dX; i++)
      fgets(line, 256, fp);

    filePosition = ftell(fp);

    for (i = 0; i < dY; i++)
    {
      if (fgets(line, 256, fp) != NULL)
      {
        token = strtok(line, " ");
        tokenCounterPerLine = 0;

        while (token != NULL)
        {
          tokenCounterPerLine++;
          token = strtok(NULL, " ");
        }
        if (tokenCounterPerLine != 5)
        {
          exit(1);
        }
      }
      else
      {
        exit(1);
      }
    }

    fseek(fp, filePosition, SEEK_SET);
    for (i = 0; i < dY; i++)
      fscanf(fp, " %f %f %f %f %f", &F[i], &G[i], &H[i], &I[i], &J[i]);

    fseek(fp, filePosition, SEEK_SET);
    for (i = 0; i < dY; i++)
      fgets(line, 256, fp);
  }
  else
  {

    exit(1);
  }

  number_of_queues = X;
  number_of_processes = Y;
  priority_boost_time = S;

  fclose(fp);

  for (i = 0; i < number_of_queues; i++)
  {
    q[i].id = (int)A[i];
    q[i].priority = (int)B[i];
    q[i].time_quantum = (int)C[i];
  }

  for (j = 0; j < number_of_processes; j++)
  {
    p[j].id = (int)F[j];
    p[j].arrival_time = (int)G[j];
    p[j].total_execution_time = (int)H[j];
    p[j].io_burst_time = (int)I[j];
    p[j].io_frequency = (int)J[j];
  }

  // SETUP PROCESS AND QUEUES

  for (i = 0; i < number_of_processes; i++)
  {
    p[i].execution_time_left = p[i].total_execution_time;
    p[i].start_end_array_size = 0;
  }

  for (j = 0; j < number_of_queues; j++)
  {
    q[j].front = NULL;
    q[j].rear = NULL;
  }
  io.front = NULL;
  io.rear = NULL;

  // SORT QUEUES VIA PRIORITY
  for (i = 0; i < number_of_queues - 1; i++)
  {
    for (j = 0; j < number_of_queues - i - 1; j++)
      if (q[j].priority > q[j + 1].priority)
      {
        queue temp = q[j];
        q[j] = q[j + 1];
        q[j + 1] = temp;
      }
  }

  int previous = -1;
  for (i = 0; i < number_of_queues; i++)
  {
    if (previous == q[i].priority)
    {
      exit(1);
    }
    else
    {
      previous = q[i].priority;
    }
  }

  // sort_queues_by_priority();

  while (are_processes_done() == 0)
  {

    for (i = 0; i < number_of_processes; i++)
    {
      if (p[i].arrival_time == current_time)
      {
        p[i].queue_index = 0;
        p[i].time_quantum_left = q[0].time_quantum;
        enqueue(&q[0], &p[i]);
      }
    }

    if (p1 && has_ready_higher_priority_job(p1))
    {
      record_end_time(p1, current_time);

      if (p1->queue_index == number_of_queues - 1)
        p1->time_quantum_left = q[p1->queue_index].time_quantum;
      enqueue(&q[p1->queue_index], p1);
      p1 = NULL;
    }

    remove_completed_IO(current_time);

    if (p1 && has_ready_higher_priority_job(p1))
    {
      record_end_time(p1, current_time);

      if (p1->queue_index == number_of_queues - 1)
        p1->time_quantum_left = q[p1->queue_index].time_quantum;
      enqueue(&q[p1->queue_index], p1);
      p1 = NULL;
    }

    if (current_time % priority_boost_time == 0)
      priority_boost_flag = 1;
    if (priority_boost_flag && p1 == NULL)
    {
      execute_priority_boost();
      priority_boost_flag = 0;
    }

    if (p1 == NULL && !are_all_queues_empty())
    {
      p1 = get_highest_priority_process();
      record_start_time(p1, current_time);
    }
    current_time++;
    update_burst_left_in_IO();

    if (p1)
    {
      p1->execution_time_left--;
      p1->time_quantum_left--;
      p1->io_burst_timer++;

      if (p1->io_burst_timer == p1->io_frequency && p1->io_burst_time != 0 && p1->execution_time_left != 0)
      {
        record_end_time(p1, current_time);
        initialize_io_timer(p1);
        enqueue(&io, p1);
        record_IO_start_time(p1, current_time);
        p1 = NULL;
      }

      else if (p1->time_quantum_left == 0 && p1->execution_time_left != 0)
      {
        record_end_time(p1, current_time);
        enqueue_to_lower(p1);
        p1 = NULL;
      }

      else if (p1->execution_time_left == 0)
      {
        record_end_time(p1, current_time);
        p1 = NULL;
      }
    }
  }

  for (i = 0; i < number_of_processes; i++)
  {

    int time_in_IO = 0;
    for (j = 0; j < p[i].start_end_array_size; j++)
    {
      if (strcmp(p[i].queue_id[j], "IO") == 0)
        time_in_IO += (p[i].end_time[j] - p[i].start_time[j]);
    }

    p[i].turnaround_time = p[i].end_time[p[i].start_end_array_size - 1] - p[i].arrival_time;
    p[i].waiting_time = p[i].turnaround_time - p[i].total_execution_time - time_in_IO;
    avg_waiting_time += p[i].waiting_time;
  }
  avg_waiting_time = avg_waiting_time / number_of_processes;

  printf("\n************************************************************* \n");
  for (i = 0; i < number_of_processes; i++)
  {
    printf("P[%d]\n\n", p[i].id);
    for (j = 0; j < p[i].start_end_array_size; j++)
    {
      if (strcmp(p[i].queue_id[j], "IO") == 0)
      {
        printf("[IO] ");
      }
      else
      {
        printf("Q[%s] ", p[i].queue_id[j]);
      }

      printf("Start time: %d \t End time: %d \n", p[i].start_time[j], p[i].end_time[j]);
    }

    printf("\nWait time: %d \n", p[i].waiting_time);
    printf("TA time: %d \n", p[i].turnaround_time);
    printf("\n************************************************************* \n");
  }
  printf("Average wait time: %f \n", avg_waiting_time);

  return 0;
}
