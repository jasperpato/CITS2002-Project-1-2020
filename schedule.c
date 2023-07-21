#include "schedule.h"

void update_sleepers(int elapsed) {
  for (int i = 0; i < num_processes; ++i) {
    process *process = &processes[i];
    if (process->state == get_state("Sleeping")) {
      process->current_event->usecs -= elapsed;
    }
  }
}

void change_state(process *process, char *state) {
  time += switch_state_time;
  update_sleepers(switch_state_time);
  if (process != NULL) {
    process->state = get_state(state);
  }
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
    print_event(event);
    
    if (process->state == get_state("Ready")) {
      // sleeper = -1;
      change_state(process, "Running");
    }

    // ----- COMPUTE -----
    if (event->action == get_action("compute")) {
      int elapsed = min(event->usecs, time_quantum);
      time += elapsed;
      event->usecs -= elapsed;
      update_sleepers(elapsed);

      if (event->usecs <= 0) {
        ++process->current_event;
      }

      //sleeper = -1;
      change_state(process, "Ready");
    }

    // ----- SLEEP -----
    else if (event->action == get_action("sleep")) {
      // become the sleeper
      if (sleeper == -1) {
        sleeper = process->pid;
      }
      if (process->state != get_state("Sleeping")) {
        change_state(process, "Sleeping");
      }
      // check if slept enough
      else if (event->usecs <= 0) {
        //sleeper = -1;
        change_state(process, "Ready");
        ++process->current_event;
      }
      else if (process->pid == sleeper) {
        // got back to the sleeper without anything else happening
        increment_time(1);
      }
    }

    // ----- FORK -----
    else if (event->action == get_action("fork")) {
      //sleeper = -1;
      change_state(process, "Ready");
      change_state(NULL, "Ready"); // simulating changing child's state
      ++process->current_event;
    }

    // ----- WAIT -----
    else if (event->action == get_action("wait")) {
      if (process->state == get_state("Running")) {
        //sleeper = -1;
        change_state(process, "Waiting");
      }
      else if (get_process(event->other_pid)->terminated) {
        //sleeper = -1;
        change_state(process, "Ready");
        ++process->current_event;
      } 
    }

    // ----- EXIT -----
    if (event->action == get_action("exit")) {
      process->terminated = 1;
    }
    else {
      enqueue(process);
    }

    if (process->state != get_state("Waiting") && process->state != get_state("Sleeping")) {
      // reset sleeper
      sleeper = -1;
    }
  }
  printf("Time taken: %d usecs\n", time);
}

int main(int argc, char *argv[]) {
  if (argc < 1) exit(EXIT_FAILURE);
  
  read_file(argv[1]);
  print_processes();

  schedule();
  // test_blocked();

  exit(EXIT_SUCCESS);
  return 0;
}