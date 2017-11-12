#include <iostream>
#include <cmath>
#include <iomanip>
#include "omp.h"

using namespace std;

double f(double x) {
    return pow(sin(1 / x) / x, 2);
}

double J(double a, double b) {
    return 0.25 * (2 * (b - a) / (a * b) + sin(2 / b) - sin(2 / a));
}

struct Result {
    long n;
    double result1;
    double result2;
    double time;

    Result(long _n, double _result1, double _result2, double _time) : n(_n), result1(_result1), result2(_result2),
                                                                      time(_time) {}

    Result() = default;

    void show() {
        cout << "Calc1:  " << result1 << endl;
        cout << "Calc2:  " << result2 << endl;
        cout << "Number: " << n << endl;
        cout << "Time:   " << time << endl;
    }

};

Result implementation_serial(double a, double b, double e, long n = 1, long dn = 1) {
    double result1 = (f(a) + f(b)) / 2;
    double d, result2, start, finish;
    long n_final;

    start = omp_get_wtime();

    while (true) {
        result2 = (f(a) + f(b)) / 2;
        d = (b - a) / n;

        for (long i = 1; i < n; i++) {
            double x = a + d * i;
            result2 += f(x);
        }
        result2 *= d;

        if (abs(result1 - result2) <= e * abs(result2)) {
            n_final = n;
            break;
        } else {
            result1 = result2;
            n += dn;
        }
    }

    finish = omp_get_wtime();

    return Result(n_final, result1, result2, finish - start);
}

Result implementation_with_reduction(double a, double b, double e, long n = 1, long dn = 1) {
    double result1 = (f(a) + f(b)) / 2;
    double d, result2, start, finish;
    long n_final;

    start = omp_get_wtime();

    while (true) {
        result2 = (f(a) + f(b)) / 2;
        d = (b - a) / n;

#pragma omp parallel for reduction(+:result2)
        for (long i = 1; i < n; i++) {
            double x = a + d * i;
            result2 += f(x);
        }
        result2 *= d;

        if (abs(result1 - result2) <= e * abs(result2)) {
            n_final = n;
            break;
        } else {
            result1 = result2;
            n += dn;
        }
    }

    finish = omp_get_wtime();

    return Result(n_final, result1, result2, finish - start);
}

Result implementation_with_atomic(double a, double b, double e, long n = 1, long dn = 1) {
    double result1 = (f(a) + f(b)) / 2;
    double d, result2, start, finish;
    long n_final;

    start = omp_get_wtime();

    while (true) {
        result2 = (f(a) + f(b)) / 2;
        d = (b - a) / n;

#pragma omp parallel for
        for (long i = 1; i < n; i++) {
            double x = a + d * i;
#pragma omp atomic
            result2 += f(x);
        }
        result2 *= d;

        if (abs(result1 - result2) <= e * abs(result2)) {
            n_final = n;
            break;
        } else {
            result1 = result2;
            n += dn;
        }
    }

    finish = omp_get_wtime();

    return Result(n_final, result1, result2, finish - start);
}

Result implementation_with_critical(const double a, const double b, const double e,
                                    long n = 1, const long dn = 1) {
    double result1 = (f(a) + f(b)) / 2;
    double d, result2, start, finish;
    long n_final;

    start = omp_get_wtime();

    while (true) {
        result2 = (f(a) + f(b)) / 2;
        d = (b - a) / n;

#pragma omp parallel for
        for (long i = 1; i < n; i++) {
            double x = a + d * i;
#pragma omp critical
            result2 += f(x);
        }
        result2 *= d;

        if (abs(result1 - result2) <= e * abs(result2)) {
            n_final = n;
            break;
        } else {
            result1 = result2;
            n += dn;
        }
    }

    finish = omp_get_wtime();

    return Result(n_final, result1, result2, finish - start);
}

Result implementation_with_locks(const double a, const double b, const double e,
                                 long n = 1, const long dn = 1) {
    double result1 = (f(a) + f(b)) / 2;
    double d, result2, start, finish;
    long n_final;

    start = omp_get_wtime();

    omp_lock_t writelock;

    omp_init_lock(&writelock);

    while (true) {
        result2 = (f(a) + f(b)) / 2;
        d = (b - a) / n;

#pragma omp parallel for
        for (long i = 1; i < n; i++) {
            double x = a + d * i;

            omp_set_lock(&writelock);
            result2 += f(x);
            omp_unset_lock(&writelock);

        }
        result2 *= d;

        if (abs(result1 - result2) <= e * abs(result2)) {
            n_final = n;
            break;
        } else {
            result1 = result2;
            n += dn;
        }
    }

    finish = omp_get_wtime();

    return Result(n_final, result1, result2, finish - start);
}

int main() {
    double e = 2.22 * pow(10, -12);
    double a = 0.01, b = 0.1;
    long n = 10000;
    cout.precision(15);

    Result result;

    cout << "Reduction" << endl;
    result = implementation_with_reduction(a, b, e, n);
    cout << "A: " << a << " B: " << b << endl;
    cout << "True:   " << J(a, b) << endl;
    result.show();
    cout << "Error:  " << scientific << e << endl;
/*
    cout << endl << "Atomic" << endl;
    result = implementation_with_atomic(a, b, e, n);
    cout << fixed << "A: " << a << " B: " << b << endl;
    cout << "True:   " << J(a, b) << endl;
    result.show();
    cout << "Error:  " << scientific << e << endl;
*/
    cout << endl << "Serial" << endl;
    result = implementation_serial(a, b, e, n);
    cout << fixed << "A: " << a << " B: " << b << endl;
    cout << "True:   " << J(a, b) << endl;
    result.show();
    cout << "Error:  " << scientific << e << endl;
    /*
    cout << endl << "Locks" << endl;
    result = implementation_with_locks(a, b, e, n);
    cout << fixed << "A: " << a << " B: " << b << endl;
    cout << "True:   " << J(a, b) << endl;
    result.show();
    cout << "Error:  " << scientific << e << endl;
*/
    return 0;
}