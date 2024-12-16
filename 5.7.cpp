#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>

using namespace std;

struct Generator {
    int duration;
    int recovery;
    vector<double> signal;
};

void read_input(int &N, vector<double> &input_signal, vector<Generator> &generators) {
    cin >> N;
    input_signal.resize(N);
    for (int i = 0; i < N; ++i) {
        cin >> input_signal[i];
    }
    for (int i = 0; i < 3; ++i) {
        int N_g, M_g;
        cin >> N_g >> M_g;
        vector<double> signal(N_g);
        for (int j = 0; j < N_g; ++j) {
            cin >> signal[j];
        }
        generators.push_back({N_g, M_g, signal});
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    vector<double> input_signal;
    vector<Generator> generators;

    read_input(N, input_signal, generators);

    vector<double> cumulative_signal(N, 0.0);
    vector<pair<int, int>> activations;
    vector<int> gen_available_times(3, 0);

    for (int t = 0; t < N; ++t) {
        double best_error_increase = numeric_limits<double>::infinity();
        int best_gen = -1;
        int best_actual_duration = 0;


        for (int i = 0; i < 3; ++i) {
            if (gen_available_times[i] <= t) {
                Generator &gen = generators[i];
                int remaining_time = N - t;
                int max_possible_duration = min(gen.duration, remaining_time);


                for (int try_d = 1; try_d <= max_possible_duration; ++try_d) {
                    double error_before = 0.0;
                    double error_after = 0.0;

                    for (int k = 0; k < try_d; ++k) {
                        double diff_before = input_signal[t+k] - cumulative_signal[t+k];
                        error_before += diff_before * diff_before;

                        double diff_after = input_signal[t+k] - (cumulative_signal[t+k] + gen.signal[k]);
                        error_after += diff_after * diff_after;
                    }

                    double error_increase = error_after - error_before;
                    if (error_increase < best_error_increase) {
                        best_error_increase = error_increase;
                        best_gen = i;
                        best_actual_duration = try_d;
                    }
                }
            }
        }


        if (best_gen != -1 && best_error_increase < 0.0) {
            Generator &gen = generators[best_gen];
            for (int k = 0; k < best_actual_duration; ++k) {
                cumulative_signal[t+k] += gen.signal[k];
            }
            activations.push_back({t, best_gen + 1});
            gen_available_times[best_gen] = t + best_actual_duration + gen.recovery;
        }
    }

    cout << (int)activations.size() << "\n";
    for (auto &act : activations) {
        cout << act.first << " " << act.second << "\n";
    }

    return 0;
}