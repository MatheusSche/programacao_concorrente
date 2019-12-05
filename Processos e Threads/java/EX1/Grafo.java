import java.util.concurrent.Semaphore;

public class Grafo extends Thread{

    private int id;

    private static Semaphore s0 = new Semaphore(0);
    private static Semaphore s1 = new Semaphore(0);
    private static int A,B,C,D;

    public Grafo(int id){
        this.id = id;
    }

    public void run(){
        try{
            switch (id){
                case 1: A = 1;
                    s0.acquire();
                    B = A + C;
                    D = B;
                    s1.release();
                    A = A + D;
                    break;
                case 2: C = 3;
                    s0.release();
                    s1.acquire();
                    C = B * D;
                    break;
                }
        } catch(InterruptedException e){
            
            e.printStackTrace();
        }
        
        System.out.printf("Fluxo %d\n", id);
    }

    public static void main(String [] args){
        try{
            
            Grafo g1 = new Grafo(1);
            g1.start();
       
            Grafo g2 = new Grafo(2);
            g2.start();

            g1.join();
            g2.join();
        
        }catch(InterruptedException e){
            e.printStackTrace();
        }
        
        System.out.printf("A= %d B= %d C= %d D= %d\n",A,B,C,D);
        
    }
}