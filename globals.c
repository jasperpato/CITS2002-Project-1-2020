#include "schedule.h"

char actions[][MAX_ARG_LENGTH] = {
  "compute",
  "sleep",
  "pipe",
  "fork",
  "readpipe",
  "writepipe",
  "wait",
  "exit",
};

int NUM_ACTIONS = sizeof actions / sizeof actions[0];

char states[][MAX_ARG_LENGTH] = {
  "Ready",
  "Running",
  "Sleeping",
  "Waiting",
  "Writing",
  "Reading",
};

int NUM_STATES = sizeof states / sizeof states[0];

process processes[MAX_PROCESSES] = {};
int num_processes = 0;

process *queue[MAX_PROCESSES] = {};
int first = 0;
int last = 0;

process *blocked[MAX_PROCESSES] = {};
int num_blocked;

int switch_state_time = DEF_SWITCH_STATE_TIME; // usecs
int pipe_size = DEF_PIPE_SIZE; // bytes
int transfer_time = DEF_TRANSFER_TIME; // per byte
int time_quantum = DEF_TIME_QUANTUM; // usecs

int time = 0;

int num_state_changes = 0;