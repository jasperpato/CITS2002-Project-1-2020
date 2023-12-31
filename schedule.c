#include "schedule.h"

void do_unblock(process *to_unblock[], int num_unblock) {
  for (int i = 0; i < num_unblock; ++i) {
    process *process = to_unblock[i];
    if (unblock(process)) {
      change_state(process, "Ready");
      ++process->current_event;
      enqueue(process);
    }
  }
}

void update_sleepers(int elapsed) {
  process *to_unblock[MAX_PROCESSES] = {};
  int num_unblock = 0;
  
  for (int i = 0; i < num_blocked; ++i) {
    process *process = blocked[i];
    if (process->state == get_state("Sleeping")) {
      process->current_event->usecs -= elapsed;
      if (process->current_event->usecs <= 0) {
        to_unblock[num_unblock++] = process;
      }
    }
  }
  do_unblock(to_unblock, num_unblock);
}

void update_waiters() {
  process *to_unblock[MAX_PROCESSES] = {};
  int num_unblock = 0;

  for (int i = 0; i < num_blocked; ++i) {
    process *process = blocked[i];
      if (process->state == get_state("Waiting")) {
        // check child process
        for (int j = 0; j < num_processes; ++j) {
          if (processes[j].pid == process->current_event->other_pid && processes[j].terminated) {
            to_unblock[num_unblock++] = process;
          }
        }
      }
  }
  do_unblock(to_unblock, num_unblock);
}

void change_state(process *process, char *state) {
  time += switch_state_time;
  update_sleepers(switch_state_time);
  // update_pipes(switch_state_time);
  ++num_state_changes;

  // print change state
  print_event(process->current_event);
  printf("[%d] %s->%s (%d usecs)\n\n", num_state_changes, states[process->state], state, time);
  
  process->state = get_state(state);
}

void change_state_pid(int pid, char *state) {
  for (int i = 0; i < num_processes; ++i) {
    if (processes[i].pid == pid) {
      change_state(&processes[i], state);
    }
  }
}

void increment_time(int elapsed) {
  time += elapsed;
  update_sleepers(elapsed);
  // update_pipes(elapsed);
}

void schedule() {
  while (!is_empty() || num_blocked) {
  
    if (is_empty()) {
      increment_time(1);
      continue;
    }

    process *process = dequeue();
    event *event = process->current_event;
    change_state(process, "Running");

    // ----- COMPUTE -----
    if (event->action == get_action("compute")) {
      int elapsed = min(event->usecs, time_quantum);
      time += elapsed;
      event->usecs -= elapsed;
      update_sleepers(elapsed);
      // update_pipes(elapsed);

      change_state(process, "Ready");
      
      if (event->usecs <= 0) {
        ++process->current_event;
      }

      enqueue(process);
    }

    // ----- SLEEP -----
    else if (event->action == get_action("sleep")) {
      change_state(process, "Sleeping");
      block(process);
    }

    // ----- FORK -----
    else if (event->action == get_action("fork")) {
      // child
      enqueue_pid(event->other_pid);
      change_state_pid(event->other_pid, "Ready");

      // parent
      enqueue(process);
      change_state(process, "Ready");
      ++process->current_event;
    }

    // ----- WAIT -----
    else if (event->action == get_action("wait")) {
      change_state(process, "Waiting");
      block(process); 
    }

    // PIPE
    else if (event->action == get_action("pipe")) {
      pipe p = { .descriptor = event->descriptor, .bytes = 0 };
      pipes[num_pipes++] = p;

      enqueue(process);
      change_state(process, "Ready");
      ++process->current_event;
    }

    // WRITE PIPE
    else if (event->action == get_action("writepipe")) {
      pipe *pipe = get_pipe(event->descriptor);

      int written = min(min(event->bytes, pipe_size - pipe->bytes), time_quantum * transfer_speed);
      event->bytes -= written;
      pipe->bytes += written;
      
      int elapsed = written / transfer_speed;
      time += elapsed;
      update_sleepers(elapsed);
      
      if (event->bytes <= 0) {
        ++process->current_event;
        change_state(process, "Ready");
      }
      else {
        change_state(process, "Writing");
      }
      enqueue(process);
    }

    // READ PIPE
    else if (event->action == get_action("readpipe")) {
      pipe *pipe = get_pipe(event->descriptor);

      int read = min(min(event->bytes, pipe->bytes), time_quantum * transfer_speed);
      event->bytes -= read;
      pipe->bytes -= read;
      
      int elapsed = read / transfer_speed;
      time += elapsed;
      update_sleepers(elapsed);
      
      if (event->bytes <= 0) {
        ++process->current_event;
        change_state(process, "Ready");
      }
      else {
        change_state(process, "Reading");
      }
      enqueue(process);
    }

    // ----- EXIT -----
    if (event->action == get_action("exit")) {
      process->terminated = 1;
      update_waiters();
    }
  }
  printf("Time taken: %d usecs\n", time);
}

int main(int argc, char *argv[]) {
  if (argc < 1) exit(EXIT_FAILURE);
  
  if (argc > 2) time_quantum = atoi(argv[2]);
  if (argc > 3) time_quantum = atoi(argv[3]);

  read_file(argv[1]);
  print_processes();
  printf("---\n");
  
  schedule();

  exit(EXIT_SUCCESS);
  return 0;
}