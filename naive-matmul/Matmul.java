import java.util.Random;

public class Matmul {
    static int n = 1024;
    static double[][] a = new double[n][n];
    static double[][] b = new double[n][n];
    static double[][] c = new double[n][n];

    public static void main(String[] args) {
        Random rand = new Random();

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = rand.nextDouble();
                b[i][j] = rand.nextDouble();
                c[i][j] = 0;
            }
        }

        long start = System.nanoTime();

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++)
                    c[i][j] += a[i][k] * b[k][j];
                
        double diff = (System.nanoTime() - start) * 1e-9;
        System.out.println(diff);
    }
}
