#define NC 50
#define NR 50
#define NITER 2000

#include <omp.h>

#include <iostream>
#include <map>
#include <vector>

#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

void set_bcs(vector<vector<float>> &t);
void plot(vector<vector<float>> &t, int iter);

int main(int argc, char **argv) {
    vector<vector<float>> t(NR + 2, vector<float>(NC + 2, 0));
    vector<vector<float>> told(NR + 2, vector<float>(NC + 2, 0));
    int i, j, iter;

    set_bcs(t);
    for (i = 0; i <= NR + 1; i++)
        for (j = 0; j <= NC + 1; j++) told[i][j] = t[i][j];
    plot(t, 0);
    for (iter = 1; iter <= NITER; iter++) {
#pragma omp parallel for private(j) num_threads(8)
        for (i = 1; i <= NR; i++)
            for (j = 1; j <= NC; j++)
                t[i][j] = 0.25 * (told[i + 1][j] + told[i - 1][j] +
                                  told[i][j + 1] + told[i][j - 1]);

#pragma omp parallel for private(j) num_threads(8)
        for (i = 0; i <= NR + 1; i++)
            for (j = 1; j <= NC; j++) told[i][j] = t[i][j];

        // if (iter % 1 == 0) {
        //     plot(t, iter);
        // }
    }
    plt::plot(t, 0);
    plt::close();
}

void set_bcs(vector<vector<float>> &t) {
    int i, j;
#pragma omp parallel for num_threads(8)
    for (i = 0; i <= NR + 1; i++) {
        t[i][NC + 1] = 25 + (100.0 / NR) * i;
        t[i][0] = (25.0 / NR) * i;
    }
#pragma omp parallel for num_threads(8)
    for (j = 0; j <= NC + 1; j++) {
        t[NR + 1][j] = 25 + (100.0 / NC) * j;
        t[0][j] = (25.0 / NC) * j;
    }
}

void plot(vector<vector<float>> &t, int iter) {
    int i, j;
    PyObject *obj;
    static vector<vector<float>> x(NR + 2, vector<float>(NC + 2, 0));
    static vector<vector<float>> y(NR + 2, vector<float>(NC + 2, 0));
#pragma omp parallel for private(j) num_threads(8)
    for (i = 0; i <= NR + 1; i++)
        for (j = 0; j <= NR + 1; j++) {
            x[i][j] = i;
            y[i][j] = j;
        }
    plt::clf();
    plt::plot_surface(x, y, t, {{"cmap", "jet"}, {"antialiased", "True"}});
    plt::xlabel("X");
    plt::ylabel("Y");
    plt::save("img/" + to_string(10000 + iter));
    plt::close();
}
