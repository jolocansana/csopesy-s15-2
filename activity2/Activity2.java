import java.util.concurrent.Semaphore;

public class Activity2 {
  public static void main () {
    final static private int n =5;
    final static DiningPhilosophers[] philosophers;
    final static Semaphore mutex = new Semaphore(1);
    public static void main(String [] args){

        philosophers[i] = new DiningPhilosophers(i);

        for(int j=0; j<philosophers.length;j++){
            philosophers[j].start();
        }
    }
  }
}