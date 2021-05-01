package ref;

import java.util.concurrent.Semaphore;

public class Main{
    
    final static private int n =5;
    final static Philosopher[] philosophers = new Philosopher[n];
    final static Semaphore mutex = new Semaphore(1);
    public static void main(String [] args){
        for(int i = 0; i<n; i++){
            philosophers[i] = new Philosopher(i);
        }

        for(int j=0; j<philosophers.length;j++){
            philosophers[j].start();
        }
    }
}