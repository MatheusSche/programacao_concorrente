public class Main {
    public static void main ( String [] args){
        
        if ( args.length != 2){
            System.out.println("Informe o numero de termos e threads");
            return;
        }
       
        long ntermos = Long.parseLong(args[0]);
        int nthreads = Integer.parseInt(args[1]);

        Pi [] p = new Pi[nthreads];

        for(int i=0; i<nthreads;i++){
            p[i] = new Pi(i, ntermos, nthreads);
            p[i].start();
        }

        double pi = 0;
        for(int i=0; i<nthreads;i++){
            try{
                p[i].join();
            }catch(InterruptedException e){
                e.printStackTrace();
            }
            pi += p[i].getPi();
        }

        System.out.printf("Pi: %.15f\n", pi);
    }
}