#include <iostream>
#include <cmath>
#include <iomanip>
#include "implementations.h"
#include <fstream>

using namespace std;

typedef pair<double, double> Points;

int MAX_THREAD_NUMBER = omp_get_max_threads();

void parallel_implementations(Points &dots, double eps, ofstream &out) {
    Result result;

    out << "\n<----------Reduction method---------->" << endl;
    for (int i = 2; i <= MAX_THREAD_NUMBER; i++) {
        result = reduction_implementation(dots.first, dots.second, eps, i);
        out << result.thread_number << " threads' time: " << result.time << endl;
    }
    out << "Step number: " << result.step_number << endl;
    cout << "Reduction completed" << endl;

    out << "\n<----------Atomic method---------->" << endl;
    for (int i = 2; i <= MAX_THREAD_NUMBER; i++) {
        result = atomic_implementation(dots.first, dots.second, eps, i);
        out << result.thread_number << " threads' time: " << result.time << endl;
    }
    out << "Step number: " << result.step_number << endl;
    cout << "Atomic completed" << endl;

    out << "\n<----------Critical method---------->" << endl;
    for (int i = 2; i <= MAX_THREAD_NUMBER; i++) {
        result = critical_implementation(dots.first, dots.second, eps, i);
        out << result.thread_number << " threads' time: " << result.time << endl;
    }
    out << "Step number: " << result.step_number << endl;
    cout << "Critical completed" << endl;

    out << "\n<----------Locks method---------->" << endl;
    for (int i = 2; i <= MAX_THREAD_NUMBER; i++) {
        result = locks_implementation(dots.first, dots.second, eps, i);
        out << result.thread_number << " threads' time: " << result.time << endl;
    }
    out << "Step number: " << result.step_number << endl;
    cout << "Locks completed" << endl;
}


void run_own_experiment() {
    Points points[]{
            make_pair(1e-5, 1e-4),
            make_pair(1e-4, 1e-3),
            make_pair(1e-3, 1e-2),
            make_pair(1e-2, 1e-1),
            make_pair(1e-1, 1),
            make_pair(1, 10),
            make_pair(10, 100)
    };
    double eps = 1e-5;
    Result result;

    for (Points &dots: points) {
        cout << endl << dots.first << " " << dots.second << endl;

        ofstream log_file;
        log_file.open(to_string(dots.first) + ".txt");
        log_file << "\nA = " << dots.first << " " << "\tB = " << dots.second << "\tEps = " << eps << endl << endl;
        log_file << "Real value: " << J(dots.first, dots.second) << endl;
        result = serial_implementation(dots.first, dots.second, eps);
        cout << "Serial completed" << endl;
        log_file << endl;
        log_file << result.thread_number << " threads' time: " << result.time << endl;

        parallel_implementations(dots, eps, log_file);
    }
}

int main() {
    cout.precision(15);

    run_own_experiment();

    return 0;
}