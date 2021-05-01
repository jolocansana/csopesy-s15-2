package activity2;

public class Activity2 {
  static int max = 5;
  static Philosopher philosophers[];
  static DiningPhilosopher diningPhilosopher;
  static boolean isContinue = true;

  public static void main (String args[]) {

    philosophers = new Philosopher[max];

    // New DiningPhilosopher Monitor
    diningPhilosopher = new DiningPhilosopher();

    // Instatiate Philosophers
    for(int i=0; i < philosophers.length; i++)
      philosophers[i] = new Philosopher(i);

    // Start Philosophers
    for(int i=0; i < philosophers.length; i++)
      philosophers[i].start();

    // try{
    //   Thread.sleep(5000);
    // } catch (Exception e) {
    //   e.printStackTrace();
    // }

    // isContinue = false;
    // System.out.println("STOP");
  }
}