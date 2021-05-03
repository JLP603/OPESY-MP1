#ifndef DATAFUNC_H_
#define DATAFUNC_H_

node *create_node(process *p1);
process *dequeue(queue *q);
process *get_highest_priority_process();
queue *createQueue();

node *create_node(process *p1)
{
  node *qNode = (node *)malloc(sizeof(node));
  qNode->p = p1;
  qNode->next = NULL;
  return qNode;
}

process *dequeue(queue *q)
{

  node *qNode = q->front;
  q->front = q->front->next;

  if (q->front == NULL)
    q->rear = NULL;

  process *p = qNode->p;

  free(qNode);

  return p;
}

queue *createQueue()
{
  queue *q = (queue *)malloc(sizeof(queue));
  q->front = NULL;
  q->rear = NULL;
  return q;
}

process *get_highest_priority_process()
{
  process *p1 = NULL;
  int i;
  for (i = 0; i < number_of_queues; i++)
    if (!is_queue_empty(&q[i]))
    {
      p1 = dequeue(&q[i]);
      break;
    }
  return p1;
}

#endif
