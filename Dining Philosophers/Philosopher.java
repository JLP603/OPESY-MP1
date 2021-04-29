public class Philosopher implements Runnable {

    private Object leftchopstick;
    private Object rightchopstick;

    public Philosopher(Object leftchopstick, Object rightchopstick) {
        this.leftchopstick = leftchopstick;
        this.rightchopstick = rightchopstick;
    }
    private void doAction(String action) throws InterruptedException {
        System.out.println(
          //gets name of currently running thread
          Thread.currentThread().getName() + " " + action);
        Thread.sleep(((int) (Math.random() * 10000))); //sleeps for a random duraton between 0 and 10 secs note 10000 is 10000 miliseconds
    }

     @Override
     public void run() {
         try {
             while (true) {
                 
                 //thinking
                 //doAction(System.nanoTime() + "is thinking");
                 doAction("is thinking");
                 synchronized (leftchopstick) {
                   //picks up left chopstick
                     synchronized (rightchopstick) {
                         //picked up right chopstick
                         System.out.println(Thread.currentThread().getName() + " acquired its left and right chopsticks.");
                         // eating
                         doAction("is eating"); 
                         //puts down right chopstick
                     }
                     //puts down left chopstick
                     System.out.println(Thread.currentThread().getName() + " released its left and right chopsticks.");
                     //thinking
                 }
             }
         } catch (InterruptedException e) {
             Thread.currentThread().interrupt();
             return;
         }
     }
 }