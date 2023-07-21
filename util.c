#include "schedule.h"

int min(int a, int b) {
  return a <= b ? a : b;
}

int max(int a, int b) {
  return a >= b ? a : b;
}

int get_action(char *action) {
  for (int i = 0; i < NUM_ACTIONS; ++i) {
    if (strcmp(action, actions[i]) == 0) {
      return i;
    }
  }
  return -1;
}

int get_state(char *state) {
  for (int i = 0; i < NUM_STATES; ++i) {
    if (strcmp(state, states[i]) == 0) return i;
  }
  return -1;
}

process *get_process(int pid) {
  for (int i = 0; i < num_processes; ++i) {
    if (pid == processes[i].pid) return &processes[i];
  }
  return NULL;
}