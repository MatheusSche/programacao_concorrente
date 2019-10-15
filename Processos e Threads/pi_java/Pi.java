public class Pi extends Thread{

    private int id;
    private long ntermos;
    private int nthreads;
    private double pi;
    
    public Pi(int id, long ntermos, int nthreads){
        this.id = id;
        this.ntermos = ntermos;
        this.nthreads = nthreads;
    }
    public void run(){
        double pi = 0;
        for(long i=id+1; i<=ntermos; i+=nthreads){
            double termo = 1.0/(2*i-1);
            if(i % 2 == 1){
                pi += termo;
            } else {
                pi -= termo;
            }
        }

       pi*=4;
       this.pi = pi; 
    }

    public double getPi(){
        return pi;
    }
}