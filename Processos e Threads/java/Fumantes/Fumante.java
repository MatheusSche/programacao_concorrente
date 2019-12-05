import java.util.concurrent.Semaphore;

public class Fumante extends Thread{

    private int id;
    private Semaphore sf;
    private Semaphore mutex;
    
    public Fumante(int id, Semaphore sf, Semaphore mutex){
        this.id = id;
        this.sf = sf;
        this.mutex = mutex;
    }

    public void run(){
        
        try{
            while(true){
                sf.acquire();
                System.out.printf("Sou um fumante %d\n", id);
                Thread.sleep(1000);
                mutex.release();
            }
        }catch(InterruptedException e){
            e.printStackTrace();
        }
     
    }
}