import java.util.concurrent.Semaphore;
import java.util.Random;

public class Agente extends Thread{
    
    private Semaphore [] sf;
    private Semaphore mutex;

    public Agente(Semaphore [] sf, Semaphore mutex){
        this.sf = sf;
        this.mutex = mutex;
    }

    public void run(){
        
        Random r = new Random();

        try{
            while(true){
                mutex.acquire();
                int f = r.nextInt(3);
                Thread.sleep(1000);
                System.out.printf("Vou liberar o fumante %d\n",f);
                sf[f].release();
            }

       }catch(InterruptedException e){
            e.printStackTrace();
        }
            

    }
}