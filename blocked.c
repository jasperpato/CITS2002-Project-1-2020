#include "schedule.h"

void block(process *process) {
  blocked[num_blocked++] = process;
}

int unblock(process *process) {
  int shuffle = 0;
  for (int i = 0; i < num_blocked; ++i) {
    if (blocked[i] == process) {
      shuffle = 1;
    }
    if (shuffle && i < num_blocked-1) {
      blocked[i] = blocked[i+1];
    }
  }
  if (shuffle) --num_blocked;
  return shuffle;
}

void print_blocked() {
  for (int i = 0; i < num_blocked; ++i) {
    print_event(blocked[i]->current_event);
  }
}

void test_blocked() {
  process p0 = { .pid = 0 };
  process p1 = { .pid = 1 };
  process p2 = { .pid = 2 };

  block(&p0); block(&p1); block(&p2);
  unblock(&p0);
  for (int i = 0; i < num_blocked; ++i) {
    printf("%d\n", blocked[i]->pid);
  }
}