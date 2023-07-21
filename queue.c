#include "schedule.h"

int is_empty() {
  return last == first;
}

void enqueue(process *process) {
  queue[last] = process;
  last = (last + 1) % MAX_PROCESSES;
}

void enqueue_pid(int pid) {
  for (int i = 0; i < num_processes; ++i) {
    if (processes[i].pid == pid) {
      enqueue(&processes[i]);
    }
  }
}

process *dequeue() {
  process *process = queue[first];
  first = (first + 1) % MAX_PROCESSES;
  return process;
}

void print_queue() {
  if (first < last) {
    for (int i = first; i < last; ++i) {
      print_event(queue[i]->current_event);
    }
  }
  else if (first < last) {
    for (int i = first; i < MAX_PROCESSES + last; ++i) {
      print_event(queue[i % MAX_PROCESSES]->current_event);
    }
  }
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