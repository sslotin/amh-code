import java.util.Random;

public class Euclid {
    public static int gcd(int a, int b) {
        if (b == 0)
            return a;
        else
            return gcd(b, a % b);
    }

    public static void main(String[] args) {
        Random rand = new Random(0);

        int n = 1000;
        int k = 2000;
        int lim = 1000 * 1000 * 1000;

        int [] a = new int[k];
        int [] b = new int[k];

        for (int i = 0; i < k; i++) {
            a[i] = rand.nextInt(lim);
            b[i] = rand.nextInt(lim);
        }

        long start = System.nanoTime();

        int s = 0;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < k; j++) {
                s += gcd(a[j], b[j]);
            }
        }
        
        System.out.println(s);
        
        long diff = System.nanoTime() - start;
        float ns = (float) diff / n / k;

        System.out.println(ns);
    }
}
