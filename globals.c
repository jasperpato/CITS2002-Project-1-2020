#include "schedule.h"

process processes[MAX_PROCESSES];
int num_processes = 0;

process *queue[MAX_PROCESSES];
int first = 0;
int last = 0;

process *sleepers[MAX_PROCESSES];
int num_sleepers = 0;

int switch_state_time = DEF_SWITCH_STATE_TIME; // usecs
int pipe_size = DEF_PIPE_SIZE; // bytes
int transfer_time = DEF_TRANSFER_TIME; // per byte
int time_quantum = DEF_TIME_QUANTUM; // usecs

int time = 0;