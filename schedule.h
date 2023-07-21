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

extern char actions[][MAX_ARG_LENGTH];
extern int NUM_ACTIONS;

extern char states[][MAX_ARG_LENGTH];
extern int NUM_STATES;

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
  int terminated;
} typedef process;

extern process processes[MAX_PROCESSES];
extern int num_processes;

extern process *queue[MAX_PROCESSES];
extern int first, last;

extern process *blocked[MAX_PROCESSES];
extern int num_blocked;

extern int switch_state_time; // usecs
extern int pipe_size; // bytes
extern int transfer_time; // per byte
extern int time_quantum; // usecs

extern int time;

// read file

extern int get_action(char *);
extern void print_event(event *);
extern void print_processes(void);
extern void read_file(char *);

// queue

extern int is_empty(void);
extern void enqueue(process *);
extern process *dequeue(void);
extern void test_queue(void);

// blocked

extern void block(process *);
extern void unblock(process *);
extern void test_blocked(void);

// util

extern int min(int, int);
extern int max(int, int);

extern int get_action(char *);
extern int get_state(char *);
extern process *get_process(int);