// Integer factorization in O(N^{1/4}
// uses squfof from msieve https://github.com/radii/msieve
// with fixes to work for n = p^3
// works up to 10^18
// probably fails on 5003^5 which is ~10^{18.5}

#include "factor.hh"

namespace Factor {
    template<typename T, typename SFINAE = void> struct bigger_type{};
    template<typename T> using bigger_type_t = typename bigger_type<T>::type;
    template<typename T> struct bigger_type<T, typename enable_if<is_integral<T>::value && is_signed<T>::value && sizeof(T) == 4>::type>{using type = long long;};
    template<typename T> struct bigger_type<T, typename enable_if<is_integral<T>::value && !is_signed<T>::value && sizeof(T) == 4>::type>{using type = unsigned long long;};
    template<typename T> struct bigger_type<T, typename enable_if<is_integral<T>::value && is_signed<T>::value && sizeof(T) == 8>::type>{using type = __int128;};
    template<typename T> struct bigger_type<T, typename enable_if<is_integral<T>::value && !is_signed<T>::value && sizeof(T) == 8>::type>{using type = unsigned __int128;};

    template<typename int_t = unsigned long long>
    struct Mod_Int{
        static inline int_t add(int_t const&a, int_t const&b, int_t const&mod){
            int_t ret = a+b;
            if(ret>=mod) ret-=mod;
            return ret;
        }
        static inline int_t sub(int_t const&a, int_t const&b, int_t const&mod){
            return add(a, mod-b);
        }
        static inline int_t mul(int_t const&a, int_t const&b, int_t const&mod){
            return a*static_cast<bigger_type_t<int_t>>(b)%mod;
        }
        static inline int_t pow(int_t const&a, int_t const&b, int_t const&mod){
            int_t ret = 1;
            int_t base = a;
            for(int i=0;b>>i;++i){
                if((b>>i)&1) ret = mul(ret, base, mod);
                base = mul(base, base, mod);
            }
            return ret;
        }
    };

    template<typename T>
    bool is_prime(T x){
        static_assert(is_integral<T>::value);
        if(x<T(4)) return x>T(1);
        for(T i=2;i*i<=x;++i){
            if(x%i == 0) return false;
        }
        return true;
    }

    template<typename T>
    bool miller_rabin_single(T const&x, T base){
        static_assert(is_integral<T>::value);
        if(x<T(4)) return x>T(1);
        if(x%2 == 0) return false;
        base%=x;
        if(base == 0) return true;

        T xm1 = x-1;
        int j = 1;
        T d = xm1/2;
        while(d%2 == 0){ // could use __builtin_ctz
            d/=2;
            ++j;
        }
        T t = Mod_Int<T>::pow(base, d, x);
        if(t==T(1) || t==T(xm1)) return true;
        for(int k=1;k<j;++k){
            t = Mod_Int<T>::mul(t, t, x);
            if(t == xm1) return true;
            if(t<=1) break;
        }
        return false;
    }

    template<typename T>
    bool miller_rabin_multi(T const&){return true;}
    template<typename T, typename... S>
    bool miller_rabin_multi(T const&x, T const&base, S const&...bases){
        static_assert(is_integral<T>::value);
        if(!miller_rabin_single(x, base)) return false;
        return miller_rabin_multi(x, bases...);
    }

    template<typename T>
    bool miller_rabin(T const&x){
        static_assert(is_integral<T>::value);
        if(x < 316349281) return miller_rabin_multi(x, T(11000544), T(31481107));
        if(x < 4759123141ull) return miller_rabin_multi(x, T(2), T(7), T(61));
        return miller_rabin_multi(x, T(2), T(325), T(9375), T(28178), T(450775), T(9780504), T(1795265022));
    }

    template<typename T>
    T isqrt(T const&x){
        static_assert(is_integral<T>::value);
        assert(x>=T(0));
        T ret = static_cast<T>(sqrtl(x));
        while(ret>0 && ret*ret>x) --ret;
        while(x-ret*ret>2*ret)
            ++ret;
        return ret;
    }
    template<typename T>
    T icbrt(T const&x){
        static_assert(is_integral<T>::value);
        assert(x>=T(0));
        T ret = static_cast<T>(cbrt(x));
        while(ret>0 && ret*ret*ret>x) --ret;
        while(x-ret*ret*ret>3*ret*(ret+1))
            ++ret;
        return ret;
    }
    uint64_t isqrt(unsigned __int128 const&x){
        unsigned __int128 ret = sqrtl(x);
        while(ret>0 && ret*ret>x) --ret;
        while(x-ret*ret>2*ret)
            ++ret;
        return ret;
    }
    vector<uint16_t> saved;
    // fast prime factorization from
    // https://github.com/radii/msieve
    uint64_t squfof_iter_better(uint64_t const&x, uint64_t const&k, uint64_t const&it_max, uint32_t cutoff_div){
        if(__gcd((uint64_t)k, x)!=1) return __gcd((uint64_t)k, x);
        //cerr << "try: " << x << " " << k << "\n";
        saved.clear();
        uint64_t scaledn = k*x;
        if(scaledn>>62) return 1;
        uint32_t sqrtn = isqrt(scaledn);
        uint32_t cutoff = isqrt(2*sqrtn)/cutoff_div;
        uint32_t q0 = 1;
        uint32_t p1 = sqrtn;
        uint32_t q1 = scaledn-p1*p1;

        if(q1 == 0){
            uint64_t factor = __gcd(x, (uint64_t)p1);
            return factor==x ? 1:factor;
        }

        uint32_t multiplier = 2*k;
        uint32_t coarse_cutoff = cutoff * multiplier;
        //cerr << "at: " << multiplier << "\n";

        uint32_t i, j;
        uint32_t p0 = 0;
        uint32_t sqrtq = 0;

        for(i=0;i<it_max;++i){
            uint32_t q, bits, tmp;

            tmp = sqrtn + p1 - q1;
            q = 1;
            if (tmp >= q1)
                q += tmp / q1;

            p0 = q * q1 - p1;
            q0 = q0 + (p1 - p0) * q;

            if (q1 < coarse_cutoff) {
                tmp = q1 / __gcd(q1, multiplier);

                if (tmp < cutoff) {
                    saved.push_back((uint16_t)tmp);
                }
            }

            bits = 0;
            tmp = q0;
            while(!(tmp & 1)) {
                bits++;
                tmp >>= 1;
            }
            if (!(bits & 1) && ((tmp & 7) == 1)) {

                sqrtq = (uint32_t)isqrt(q0);

                if (sqrtq * sqrtq == q0) {
                    for(j=0;j<saved.size();++j){
                        if(saved[j] == sqrtq) break;
                    }
                    if(j == saved.size()) break;
                    //else cerr << "skip " << i << "\n";;
                }
            }
            tmp = sqrtn + p0 - q0;
            q = 1;
            if (tmp >= q0)
                q += tmp / q0;

            p1 = q * q0 - p0;
            q1 = q1 + (p0 - p1) * q;

            if (q0 < coarse_cutoff) {
                tmp = q0 / __gcd(q0, multiplier);

                if (tmp < cutoff) {
                    saved.push_back((uint16_t) tmp);
                }
            }
        }

        if(sqrtq == 1) { return 1;}
        if(i == it_max) { return 1;}

        q0 = sqrtq;
        p1 = p0 + sqrtq * ((sqrtn - p0) / sqrtq);
        q1 = (scaledn - (uint64_t)p1 * (uint64_t)p1) / (uint64_t)q0;

        for(j=0;j<it_max;++j) {
            uint32_t q, tmp;

            tmp = sqrtn + p1 - q1;
            q = 1;
            if (tmp >= q1)
                q += tmp / q1;

            p0 = q * q1 - p1;
            q0 = q0 + (p1 - p0) * q;

            if (p0 == p1) {
                q0 = q1;
                break;
            }

            tmp = sqrtn + p0 - q0;
            q = 1;
            if (tmp >= q0)
                q += tmp / q0;

            p1 = q * q0 - p0;
            q1 = q1 + (p0 - p1) * q;

            if (p0 == p1)
                break;
        }
        if(j==it_max) {cerr << "RNG\n"; return 1;} // random fail

        uint64_t factor = __gcd((uint64_t)q0, x);
        if(factor == x) factor=1;
        return factor;
    }
    uint64_t squfof(uint64_t const&x){
        static array<uint32_t, 16> multipliers{1, 3, 5, 7, 11, 3*5, 3*7, 3*11, 5*7, 5*11, 7*11, 3*5*7, 3*5*11, 3*7*11, 5*7*11, 3*5*7*11};

        uint64_t cbrt_x = icbrt(x);
        if(cbrt_x*cbrt_x*cbrt_x == x) return cbrt_x;

        //uint32_t iter_lim = isqrt(isqrt(x))+10;
        uint32_t iter_lim = 300;
        for(uint32_t iter_fact = 1;iter_fact<20000;iter_fact*=4){
            for(uint32_t const&k : multipliers){
                if(numeric_limits<uint64_t>::max()/k<=x) continue; //would overflow
                uint32_t const it_max = iter_fact*iter_lim;
                uint64_t factor = squfof_iter_better(x, k, it_max, 1);
                if(factor==1 || factor==x) continue;
                return factor;
            }
        }
        cerr << "failed to factor: " << x << "\n";
        assert(0);
        return 1;
    }

    template<typename T>
    vector<T> factorize(T x){
        static_assert(is_integral<T>::value);
        vector<T> ret;
        const uint32_t trial_limit = 5000;
        auto trial = [&](int i){while(x%i == 0){x/=i; ret.push_back(i);}};
        trial(2); trial(3);
        for(uint32_t i=5, j=2;i<trial_limit && i*i <= x;i+=j, j=6-j){
            trial(i);
        }
        if(x>1){
            static stack<T> s;
            s.push(x);
            while(!s.empty()){
                x = s.top(); s.pop();
                if(!miller_rabin(x)){
                    T factor = squfof(x);
                    if(factor == 1 || factor == x){assert(0); return ret;}
                    s.push(factor);
                    s.push(x/factor);
                } else {
                    ret.push_back(x);
                }
            }
        }
        sort(ret.begin(), ret.end());
        return ret;
    }

    int64_t euclid_invert(int64_t a, int64_t mod){
        a%=mod;
        int64_t b = mod, x = 0, x2 = 1, y = 1, y2 = 0, tmp;

        while(b){
            int64_t q = a / b, r = a % b;

            tmp = x;
            x = x2 - q * x;
            x2 = tmp;

            tmp = y;
            y = y2 - q * y;
            y2 = tmp;

            a = b; b = r;
        }
        if(x2<0) x2+=mod;
        return x2;
    }
    // baby and giant step
    int64_t disc_log(int64_t base, int64_t x, int64_t p, int64_t limit){
        int64_t block = sqrt(limit);
        vector<pair<int64_t, int> > small;
        int64_t a = 1;
        for(int i=0;i<block;++i){
            small.emplace_back(a, i);
            a = Mod_Int<int64_t>::mul(a, base, p);
        }
        sort(small.begin(), small.end());
        int64_t big_base = euclid_invert(a, p);
        a = x;
        for(int j=0;;++j){
            auto it = lower_bound(small.begin(), small.end(), make_pair(a, -1));
            if(it != small.end() && it->first == a){
                return j*block + it->second;
            }
            a = Mod_Int<int64_t>::mul(a, big_base, p);
        }
        assert(0);
        return 0;
    }
    // solves x^exp = a (mod p)
    // exp and p have to be prime
    // returns -1 if there is no solution
    int64_t root_extraction(int64_t a, int64_t exp, int64_t p){
        assert(miller_rabin(exp));
        assert(miller_rabin(p));
        if(a == 0) return 0;
        if(__gcd(exp, p-1) == 1){
            int64_t exp_inv = euclid_invert(exp, p-1);
            assert((p-1 == 1) || exp_inv * (__int128)exp % (p-1) == 1);
            return Mod_Int<int64_t>::pow(a, exp_inv, p);
        }
        // now exp divides (p-1)
        if(Mod_Int<int64_t>::pow(a, (p-1)/exp, p)!=1){
            // no solutions
            return -1;
        }
        // find non-redidue
        int64_t res = 2;
        while(Mod_Int<int64_t>::pow(res, (p-1)/exp, p) == 1) ++res;
        int t = 0;
        int64_t s = p-1;
        while(s%exp == 0){
            s/=exp;
            ++t;
        }
        int64_t alph = euclid_invert(exp, s);
        int64_t A = Mod_Int<int64_t>::pow(res, (p-1)/exp, p);
        int64_t B = Mod_Int<int64_t>::pow(a, alph*exp, p);
        B = Mod_Int<int64_t>::mul(B, euclid_invert(a, p), p);
        int64_t C = Mod_Int<int64_t>::pow(res, s, p);
        int64_t h = 1;
        for(int i=1;i<t;++i){
                int64_t d = Mod_Int<int64_t>::pow(B, Mod_Int<int64_t>::pow(exp, t-1-i, p-1), p);
                int64_t j = 0;
                if(d!=1){
                    j = disc_log(A, d, p, exp);
                }
                B = Mod_Int<int64_t>::mul(B, euclid_invert(Mod_Int<int64_t>::pow(C, Mod_Int<int64_t>::mul(exp, j, p-1), p), p), p);
                h = Mod_Int<int64_t>::mul(h, euclid_invert(Mod_Int<int64_t>::pow(C, j, p), p), p);
                C = Mod_Int<int64_t>::pow(C, exp, p);
        }
        int64_t ret = Mod_Int<int64_t>::mul(Mod_Int<int64_t>::pow(a, alph, p), h, p);

        return ret;
    }
}

u64 find_factor(u64 x) {
    return Factor::factorize(x)[0];
}
