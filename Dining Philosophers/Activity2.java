
//Driver Class

public class Activity2 {

    public static void main(String[] args){

        final Philosopher[] philosophers = new Philosopher[5];
        Object[] forks = new Object[philosophers.length];

        for (int i = 0; i < forks.length; i++) {
            forks[i] = new Object();
        }

        for (int i = 0; i < philosophers.length; i++) {
            Object leftFork = forks[i];
            Object rightFork = forks[(i + 1) % forks.length];

            if (i == philosophers.length - 1) {
                
                // The last philosopher picks up the right fork first
                philosophers[i] = new Philosopher(rightFork, leftFork); 
            } else {
                philosophers[i] = new Philosopher(leftFork, rightFork);
            }
            
            Thread t 
              = new Thread(philosophers[i], "Philosopher " + (i + 1));
            t.start();
        }
    }
}
/*

// We must make a solution to the dining philosopher problem using monitors that follow sthe structure of chapter 5.8.2 of the book
Create five philosophers, each identified by a number 0...4. Each philosopher  should run as a separate thread. 
When a philosopher wishes to eat, it should call the method pickup(i), where i identifies the number of the philosopher wishing to eat. 
When a philosopher finishes eating, it invokes putdown(i). A philosopher may pick up the chopsticks only if both of them are available. 
2. When a philosopher is thinking the philosopher should print: Philosopher i is thinking." with i being the number of the philosopher who is thinking. After printing the philosopher thread should sleep for a random amount of time. 

3. When a philosopher is eating the philosopher should print: Philosopher i is eating." with i being the number of the philosopher who is eating. After printing the philosopher thread should sleep for a random amount of time.

4.  When a philosopher acquires both of its chopsticks the philosopher should print: Philosopher i acquired its left and right chopsticks." with i being the number of the philosopher who acquired its chopsticks. 
When a philosopher releases both of its chopsticks the philosopher should print: Philosopher i released its left and right chopsticks." with i being the number of the philosopher who released its chopsticks

The Java API provides support for semaphores, condition variables, and mutex locks (among other concurrency mechanisms) in the java.util.concurrent package.
 You can use them for your solution. Do not use other packages/your own packages so I can run it independent of the file organization in your computer or whatever IDE you used.

Dining philosopher code answer using threads
https://www.baeldung.com/java-dining-philoshophers

*/