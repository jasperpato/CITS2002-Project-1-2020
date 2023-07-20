#define SCHEDULE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_PROCESSES 20
#define MAX_EVENTS 50 // per process
#define MAX_PIPE_DESCRIPTORS 10 // per process

#define MAX_LINE_LENGTH 100
#define MAX_ARG_LENGTH 20

#define DEF_SWITCH_STATE_TIME 5
#define DEF_PIPE_SIZE 400
#define DEF_TRANSFER_TIME 1
#define DEF_TIME_QUANTUM 1000

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

#define NUM_ACTIONS sizeof(actions) / sizeof(actions[0])

char states[][MAX_ARG_LENGTH] = {
  "Ready",
  "Running",
  "Sleeping",
  "Waiting",
  "Writing",
  "Reading",
};

#define NUM_STATES sizeof(states) / sizeof(states[0])

struct event {
  int pid;
  int action;
  int usecs;
  int descriptor;
  int other_pid;
  int bytes;
  int completed;
} typedef event;

struct process {
  int pid;
  int state;
  event events[MAX_EVENTS];
  event *current_event;
  int num_events;
  int sleep_length;
} typedef process;

process processes[MAX_PROCESSES];
int num_processes;

process *queue[MAX_PROCESSES];
int first= 0;
int last = 0;

int switch_state_time = DEF_SWITCH_STATE_TIME; // usecs
int pipe_size = DEF_PIPE_SIZE; // bytes
int transfer_time = DEF_TRANSFER_TIME; // per byte
int time_quantum = DEF_TIME_QUANTUM; // usecs

int time = 0;

// read file

int get_action(char *);

void print_event(event *);

void print_processes(void);

void read_file(char *);

// queue

int is_empty(void);

void enqueue(process *);

process *dequeue(void);

void test_queue(void);

// util

int min(int, int);

int max(int, int);

int get_action(char *);

int get_state(char *);