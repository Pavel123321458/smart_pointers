import matplotlib.pyplot as plt
import csv

N = []
raw = []
unq = []
shrd = []
stl_unq = []
stl_shr = []

with open('results.csv') as f:
    reader = csv.DictReader(f)
    for row in reader:
        N.append(int(row['N']))
        raw.append(int(row['Raw']))
        unq.append(int(row['UnqPtr']))
        shrd.append(int(row['ShrdPtr']))
        stl_unq.append(int(row['UniquePtrSTL']))
        stl_shr.append(int(row['SharedPtrSTL']))

plt.figure(figsize=(10, 6))
plt.plot(N, raw, 'o-', label='Raw pointers')
plt.plot(N, unq, 's-', label='UnqPtr')
plt.plot(N, shrd, '^-', label='ShrdPtr')
plt.plot(N, stl_unq, 'd--', label='std::unique_ptr')
plt.plot(N, stl_shr, 'v--', label='std::shared_ptr')

plt.xscale('log')
plt.yscale('log')
plt.xlabel('N (log scale)')
plt.ylabel('Time, ms (log scale)')
plt.title('Smart Pointers Benchmark')
plt.legend()
plt.grid(True)
plt.savefig('benchmark.png')
plt.show()