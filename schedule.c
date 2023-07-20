#include "schedule.h"

void update_sleepers(int elapsed) {
  for (int i = 0; i < num_processes; ++i) {
    process *process = &processes[i];
    if (process->state == get_state("Sleeping")) {
      process->sleep_length += elapsed;
    }
  }
}

void change_state(process *process, char *state) {
  time += switch_state_time;
  update_sleepers(switch_state_time);
  process->state = get_state(state);
}

void increment_time(int elapsed) {
  time += elapsed;
  update_sleepers(elapsed);
}

void schedule() {
  int sleeper = -1;

  while (!is_empty()) {
    process *process = dequeue();
    event *event = process->current_event;

    if (event->action != get_action("sleep")) {
      sleeper = -1;
    }

    if (event->action == get_action("compute")) {
      if (process->state == get_state("Ready")) {
        change_state(process, "Running");
      }

      int elapsed = min(event->usecs, time_quantum);
      time += elapsed;
      event->usecs -= elapsed;
      update_sleepers(elapsed);

      if (event->usecs <= 0) {
        ++process->current_event;
      }
      change_state(process, "Ready");
    }

    else if (event->action == get_action("sleep")) {
      // become the sleeper
      if (sleeper == -1) {
        sleeper = process->pid;
      }
      if (process->state == get_state("Ready")) {
        change_state(process, "Sleeping");
      }
      else if (process->state == get_state("Sleeping")) {
        // check if slept enough
        if (process->sleep_length >= event->usecs) {
          change_state(process, "Ready");
          ++process->current_event;
        }

        else if (process->pid == sleeper) {
          // got back to the sleeper without anything else happening
          increment_time(1);
        }
      }
    }

    if (event->action != get_action("exit")) {
      enqueue(process);
    }
  }
  printf("Time taken: %d usecs\n", time);
}

int main(int argc, char *argv[]) {
  if (argc < 1) exit(EXIT_FAILURE);
  
  read_file(argv[1]);
  print_processes();

  schedule();

  exit(EXIT_SUCCESS);
  return 0;
}