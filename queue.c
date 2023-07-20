#ifndef SCHEDULE
#include "schedule.h"
#endif

int is_empty() {
  return last == first;
}

void enqueue(process *process) {
  queue[last] = process;
  last = (last + 1) % MAX_PROCESSES;
}

process *dequeue() {
  process *process = queue[first];
  first = (first + 1) % MAX_PROCESSES;
  return process;
}

void test_queue() {
  for (int i = 0; i < num_processes; ++i) {
    enqueue(&processes[i]);
  }
  for (int i = 0; i < num_processes / 2; ++i) {
    process *process = dequeue();
    printf("%p %d\n", process, is_empty());
  }
  for (int i = 0; i < num_processes / 2; ++i) {
    enqueue(&processes[i]);
  }
  for (int i = 0; i < num_processes; ++i) {
    process *process = dequeue();
    printf("%p %d\n", process, is_empty());
  }
}