import sys
sys.stdout = open(sys.stdout.fileno(), mode='w', buffering=1)


def read_input():
    N = int(sys.stdin.readline())
    input_signal = [int(sys.stdin.readline()) for _ in range(N)]
    generators = []
    for _ in range(3):
        N_g, M_g = map(int, sys.stdin.readline().split())
        signal = [int(sys.stdin.readline()) for _ in range(N_g)]
        generators.append({'duration': N_g, 'recovery': M_g, 'signal': signal})
    return N, input_signal, generators


def main():
    N, input_signal, generators = read_input()
    cumulative_signal = [0] * N
    activations = []

    gen_available_times = [0, 0, 0]

    for t in range(N):

        if cumulative_signal[t] >= input_signal[t]:
            continue

        for i, gen in enumerate(generators):

            if gen_available_times[i] > t:
                continue

            gen_signal = gen['signal']
            gen_duration = gen['duration']
            recovery_time = gen['recovery']

            if t + gen_duration > N:
                continue

            overshoot = False

            for k in range(gen_duration):
                idx = t + k
                new_value = cumulative_signal[idx] + gen_signal[k]
                if new_value > input_signal[idx]:
                    overshoot = True
                    break

            if overshoot:
                continue

            for k in range(gen_duration):
                idx = t + k
                cumulative_signal[idx] += gen_signal[k]

            gen_available_times[i] = t + gen_duration + recovery_time
            activations.append((t, i + 1))
            break  # Переходим к следующему моменту времени

    sys.stdout.write(str(len(activations)) + "\n")
    sys.stdout.flush()
    for activation in activations:
        sys.stdout.write(f"{activation[0]} {activation[1]}\n")
        sys.stdout.flush()


if __name__ == "__main__":
    main()

