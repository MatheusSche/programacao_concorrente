import java.util.concurrent.Semaphore;

public class Principal{
    public static void main(String [] args){
         
        Semaphore mutex = new Semaphore(1);
        Semaphore [] sf = new Semaphore[3];
        for(int i=0; i<3; i++){
            sf[i] = new Semaphore(0);
        }

        Agente a = new Agente(sf, mutex);
        a.start();  


        Fumante [] f = new Fumante[3];
        for(int i=0; i<3;i++){
            f[i] = new Fumante(i, sf[i], mutex);
            f[i].start();
        }

        try{
            a.join();
        }catch(InterruptedException e){
            e.printStackTrace();
        }
    }
}