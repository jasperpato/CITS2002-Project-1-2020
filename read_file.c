#include "schedule.h"

void print_event(event *event) {
  printf("%d %s", event->pid, actions[event->action]);
  switch (event->action) {
    case 0:
    case 1:
      printf(" %d", event->usecs);
      break;
    case 2:
      printf(" %d", event->descriptor);
      break;
    case 3:
    case 6:
      printf(" %d", event->other_pid);
      break;
    case 4:
    case 5:
      printf(" %d %d", event->descriptor, event->bytes);
      break;
  }
  printf("\n");
}

void print_processes() {
  for (int i = 0 ; i < num_processes; ++i) {
    process *process = &processes[i];
    for (int j = 0; j < process->num_events; ++j) {
      print_event(&process->events[j]);
    }
  }
}

void process_line(char *line) {
  event event;
  char delim[] = " \n";
  char *arg = strtok(line, delim);
  int arg_num = 0;
  
  while (arg != NULL) {
    if (arg_num == 0) {
      event.pid = atoi(arg);
    }

    else if (arg_num == 1) {
      event.action = get_action(arg);
    }

    else if (arg_num == 2) {
      switch (event.action) {
        case 0: 
        case 1:
          event.usecs = atoi(arg);
          break;
        case 2:
        case 4:
        case 5:
          event.descriptor = atoi(arg);
          break;
        case 3:
        case 6:
          event.other_pid = atoi(arg);
      }
    }

    else if (arg_num == 3) {
      event.bytes = atoi(arg);
    }

    arg = strtok(NULL, delim);
    ++arg_num;
  }
  
  process *process = NULL;

  // check if process already exists
  for (int i = 0; i < num_processes; ++i) {
    if (event.pid == processes[i].pid) {
      process = &processes[i];
    }
  }

  if (process == NULL) {
    process = &processes[num_processes];
    process->pid = event.pid;
    process->state = get_state("Ready");
    process->current_event = &process->events[0];

    enqueue(process);
    ++num_processes;
  }

  process->events[process->num_events++] = event;
}

void read_file(char *fname) {
  FILE *f = fopen(fname, "r");
  char line[MAX_LINE_LENGTH];
  while(fgets(line, sizeof line, f) != NULL) {
    process_line(line);
  }
  fclose(f);
}
