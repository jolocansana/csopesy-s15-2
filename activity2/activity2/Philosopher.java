package activity2;
import java.util.concurrent.Semaphore;

public class Philosopher extends Thread
{

  int id;
  Status state;
  Semaphore semaphore;

  
  Philosopher(int id) {
    this.id = id;
    state = Status.THINKING;
    semaphore = new Semaphore(1);
  }

  private void sleep() {
    try{
      Thread.sleep(Math.round(Math.random() * 1000));
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  public void run() {
    while(Activity2.isContinue){
      if(this.state == Status.THINKING){
        System.out.println("Philosopher " + this.id + " is thinking.");
        sleep();
        this.state = Status.HUNGRY;
      }
      else if (this.state == Status.HUNGRY){
        Activity2.diningPhilosopher.pickup(this.id);
      }
      else if(this.state == Status.EATING){
        System.out.println("Philosopher " + this.id + " is eating.");
        sleep();
        Activity2.diningPhilosopher.putdown(this.id);
      }
    }
  }
}