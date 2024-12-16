#include <iostream>
#include <vector>

struct Generator {
    int duration;
    int recovery;
    std::vector<int> signal;
};

int main() {
    int N;
    std::cin >> N;
    std::vector<int> input_signal(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> input_signal[i];
    }

    std::vector<Generator> generators(3);
    for (int i = 0; i < 3; ++i) {
        int N_g, M_g;
        std::cin >> N_g >> M_g;
        std::vector<int> signal(N_g);
        for (int j = 0; j < N_g; ++j) {
            std::cin >> signal[j];
        }
        generators[i] = {N_g, M_g, signal};
    }

    std::vector<int> cumulative_signal(N, 0);
    std::vector<std::pair<int, int>> activations;
    std::vector<int> gen_available_times(3, 0);

    for (int t = 0; t < N; ++t) {
        if (cumulative_signal[t] >= input_signal[t]) {
            continue;
        }

        for (int i = 0; i < 3; ++i) {
            Generator& gen = generators[i];

            if (gen_available_times[i] > t) {
                continue;
            }

            int gen_duration = gen.duration;
            int recovery_time = gen.recovery;

            if (t + gen_duration > N) {
                continue;
            }

            bool overshoot = false;
            for (int k = 0; k < gen_duration; ++k) {
                int idx = t + k;
                int new_value = cumulative_signal[idx] + gen.signal[k];
                if (new_value > input_signal[idx]) {
                    overshoot = true;
                    break;
                }
            }

            if (overshoot) {
                continue;
            }

            // Обновляем cumulative_signal без копирования
            for (int k = 0; k < gen_duration; ++k) {
                int idx = t + k;
                cumulative_signal[idx] += gen.signal[k];
            }

            gen_available_times[i] = t + gen_duration + recovery_time;
            activations.emplace_back(t, i + 1);
            break;  // Переходим к следующему моменту времени
        }
    }

    std::cout << activations.size() << std::endl;
    for (const auto& activation : activations) {
        std::cout << activation.first << " " << activation.second << std::endl;
    }

    return 0;
}