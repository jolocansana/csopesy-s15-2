import java.util.concurrent.Semaphore;

public class DiningPhilosophers
{
  enum State {THINKING, HUNGRY, EATING};
  State state[] = new State[5];
  Condition self[] = new Condition[5];

  void pickup(int i) {
    state[i] = State.HUNGRY;
    test(i);
    if (state[i] != State.EATING)
      self[i].wait();
  }
  void putdown(int i) {
    state[i] = State.THINKING;
    test((i + 4) % 5);
    test((i + 1) % 5);
  }
  void test(int i) {
    if ((state[(i + 4) % 5] != State.EATING) &&
      (state[i] == State.HUNGRY) &&
      (state[(i + 1) % 5] != State.EATING)) {
        state[i] = State.EATING;
        self[i].signal();
    }
  }

  void start() {
    
  }
  
  DiningPhilosophers(int max) {
    for (int i = 0; i < max; i++)
      state[i] = State.THINKING;
  }
}

class Condition {
  Semaphore mutex = new Semaphore(1);
  Semaphore next = new Semaphore(0);
  Semaphore x_sem = new Semaphore(0);
  int x_count = 0;
  int next_count = 0;

  void wait() {
    x_count++;
    if (next_count > 0)
      signal(next);
    else
      signal(mutex);
    wait(x_sem);
    x_count--;
  }

  void signal() {
    if (x_count > 0) {
      next_count++;
      signal(x_sem);
      wait(next);
      next_count--;
      }
  }
}