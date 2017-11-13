//
// Created by kirov on 13.11.17.
//

#ifndef LAB3_IMPLEMENTATIONS_H
#define LAB3_IMPLEMENTATIONS_H

#include <cmath>
#include <iostream>
#include <omp.h>

using namespace std;

typedef long long dlong;


double f(double x);

double J(double a, double b);

struct Result {
    dlong step_number;
    int thread_number;
    double result;
    double time;

    Result(int _thread_number, double _time, double _result, long _step_number) :
            thread_number(_thread_number), time(_time), result(_result), step_number(_step_number) {}

    Result() = default;

    void show() {
        cout << thread_number << " threads' time: " << time << endl;
        cout << "Number of steps: " << step_number << " result : " << result << endl;
    }
};

Result serial_implementation(double a, double b, double eps);

Result serial_implementation_for_task(double a, double b, double eps);

Result reduction_implementation(double a, double b, double eps, int thread_number);

Result atomic_implementation(double a, double b, double eps, int thread_number);

Result critical_implementation(double a, double b, double eps, int thread_number);

Result locks_implementation(double a, double b, double eps, int thread_number);

Result reduction_implementation_for_task(double a, double b, double eps, int thread_number);

Result atomic_implementation_for_task(double a, double b, double eps, int thread_number);

Result critical_implementation_for_task(double a, double b, double eps, int thread_number);

Result locks_implementation_for_task(double a, double b, double eps, int thread_number);


#endif //LAB3_IMPLEMENTATIONS_H
