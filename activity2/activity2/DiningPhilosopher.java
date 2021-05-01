package activity2;

public class DiningPhilosopher {

  void pickup(int i) {
    test(i);
    if (Activity2.philosophers[i].state == Status.EATING)
    {
      try {
        Activity2.philosophers[(i + 4) % 5].semaphore.acquire();
      } catch (InterruptedException e) {
        e.printStackTrace();
      }
      try {
        Activity2.philosophers[(i + 1) % 5].semaphore.acquire();
      } catch (InterruptedException e) {
        e.printStackTrace();
      }
      System.out.println("Philosopher " + i + " acquired its left and right chopsticks.");
    }
  }

  void putdown(int i) {
    Activity2.philosophers[i].state = Status.THINKING;
    Activity2.philosophers[(i + 4) % 5].semaphore.release();
    Activity2.philosophers[(i + 1) % 5].semaphore.release();
    System.out.println("Philosopher " + i + " released its left and right chopsticks.");
  }

  void test(int i) {
    // System.out.println(Activity2.philosophers[(i + 4) % 5].state + " " + p.state + " " + Activity2.philosophers[(i + 1) % 5].state);
    if ((Activity2.philosophers[(i + 4) % 5].state != Status.EATING) &&
        (Activity2.philosophers[i].state == Status.HUNGRY) &&
        (Activity2.philosophers[(i + 1) % 5].state != Status.EATING)) {
        Activity2.philosophers[i].state = Status.EATING;
    }
  }
}