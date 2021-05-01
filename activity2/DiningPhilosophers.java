public class DiningPhilosophers
{
  enum State {THINKING, HUNGRY, EATING};
  State state[] = new State[5];
  condition self[5];

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
  
  void initialization_code() {
    for (int i = 0; i < 5; i++)
      state[i] = State.THINKING;
  }

  public synchronized void safeMethod() {

  }
}