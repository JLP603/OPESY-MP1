/*
***************************************************************
Names: Pascual, Jeremy
       Pua, Shaun
       Soler, Javier
Group: 41
Section: S16
***************************************************************
*/
//Driver Class

public class Activity2 {

    public static void main(String[] args){

        final Philosopher[] philosophers_arr=new Philosopher[5];
        Object[] chopsticks_arr=new Object[philosophers_arr.length];

        for (int i=0; i<chopsticks_arr.length; i++) {
            chopsticks_arr[i]=new Object();
        }

        for (int i=0; i<philosophers_arr.length; i++) {
            Object leftchopstick=chopsticks_arr[i];
            Object rightchopstick=chopsticks_arr[(i+1)%chopsticks_arr.length];

            if (i== philosophers_arr.length-1) {
                
                //last philosopher picks up the right chopstick to prevent deadlock
                philosophers_arr[i]=new Philosopher(rightchopstick, leftchopstick); 
            } else {
                philosophers_arr[i]=new Philosopher(leftchopstick, rightchopstick);
            }
            
            Thread t 
              = new Thread(philosophers_arr[i],"Philosopher "+(i+1));
            t.start();
        }
    }
}