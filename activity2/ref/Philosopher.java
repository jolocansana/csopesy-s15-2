package ref;

import java.util.concurrent.Semaphore;

public class Philosopher extends Thread {
    private enum State {Thinking, Eating, Hungry};
    private int id;
    private State currState;
    private Semaphore selfMutex;

    public Philosopher(int id){
        this.id = id;
        this.currState = State.Thinking;
        this.selfMutex = new Semaphore(0);
    }
    
    //checks right neighbor
    private Philosopher neighborRight(){
        if(this.id==0){
            return Main.philosophers[4];
        }else{
            return Main.philosophers[this.id-1];
        }
    }

    //check left neighbor
    private Philosopher neighborLeft(){
        return Main.philosophers[(this.id + 1)%5];
    }

    //Prints the states
    private void print(){
        System.out.println("Philosopher" + this.id + this.currState);
    }

    //randomly generated time of the philosopher in the thinking state or eating state
    private void randomSleep(){
        try{
            Thread.sleep(Math.round(Math.random() * 1000));
        } catch(InterruptedException e){
            e.printStackTrace();
        }
    }

    //check if neighbors are not eating, means chopsticks are available
    private void availableChopsticks(Philosopher p){
        if(p.neighborLeft().currState != State.Eating && p.neighborRight().currState != State.Eating 
        && p.currState == State.Hungry){
            p.currState = State.Eating;
            p.selfMutex.release();
        }
    }

    //start of thread
    public void run(){
        try{
            while(true){
                print();
                if(this.currState == State.Thinking){
                    randomSleep();
                    Main.mutex.acquire();
                    this.currState = State.Hungry;
                }
                else if (this.currState == State.Hungry){
                    availableChopsticks(this);
                    Main.mutex.release();
                    this.selfMutex.acquire();
                    this.currState = State.Eating;
                }
                else if(this.currState == State.Eating){
                    randomSleep();
                    Main.mutex.acquire();
                    availableChopsticks(this.neighborLeft());
                    availableChopsticks(this.neighborRight());
                    this.currState = State.Thinking;
                    Main.mutex.release();
                }
            }

        }catch(InterruptedException e){
            e.printStackTrace();
        }
    }

    
}
