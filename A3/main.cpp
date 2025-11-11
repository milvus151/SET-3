#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <fstream>
class ArrayGenerator {
public:
  static std::vector<int> genRandom(int n) {
    std::mt19937 gen(12345);
    std::uniform_int_distribution<> dist(0, 10000);
    std::vector<int> generated(n);
    for (int i = 0; i < n; i++) {
      generated[i] = dist(gen);
    }
    return generated;
  }

  static std::vector<int> genSorted(int n) {
    std::vector<int> generated(n);
    for (int i = 0; i < n; i++) {
      generated[i] = i;
    }
    return generated;
  }
  static std::vector<int> genReverseSorted(int n) {
    std::vector<int> generated(n);
    for (int i = 0; i < n; i++) {
      generated[i] = n - i - 1;
    }
    return generated;
  }

  static std::vector<int> genNearlySorted(int n) {
    std::vector<int> generated = genSorted(n);
    std::mt19937 gen(12345);
    std::uniform_int_distribution<int> dist(0, n - 2);

    int swaps = n / 25;
    for (int i = 0; i < swaps; i++) {
      int pos = dist(gen);
      std::swap(generated[pos], generated[pos + 1]);
    }
    return generated;
  }

  static std::vector<int> getSubArray(const std::vector<int> &baseArray, int exactSize) {
    std::vector<int> result(baseArray.begin(), baseArray.begin() + exactSize);
    return result;
  }
};

int partition(std::vector<int> &arr, int low, int high) {
  static std::mt19937 gen(19454651);
  std::uniform_int_distribution<> dist(low, high);
  int randomIndex = dist(gen);
  std::swap(arr[randomIndex], arr[high]);

  int pivot = arr[high];
  int i = low - 1;
  for (int j = low; j < high; j++) {
    if (arr[j] <= pivot) {
      i++;
      std::swap(arr[i], arr[j]);
    }
  }
  std::swap(arr[i + 1], arr[high]);
  return i + 1;
}

void quicksortHelper(std::vector<int> &arr, int low, int high) {
  if (low < high) {
    int pi = partition(arr, low, high);
    quicksortHelper(arr, low, pi - 1);
    quicksortHelper(arr, pi + 1, high);
  }
}

void quickSort(std::vector<int> &arr) {
  int n = static_cast<int>(arr.size());
  if (n <= 1) {
    return;
  }
  quicksortHelper(arr, 0, n - 1);
}

void heapify(std::vector<int> &arr, int low, int n, int i) {
  int largest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;
  if (left < n && arr[low + left] > arr[low + largest]) {
    largest = left;
  }
  if (right < n && arr[low + right] > arr[low + largest]) {
    largest = right;
  }
  if (largest != i) {
    std::swap(arr[low + i], arr[low + largest]);
    heapify(arr, low, n, largest);
  }
}

void heapSort(std::vector<int> &arr, int low, int high) {
  int n = high - low + 1;
  for (int i = n / 2 - 1; i >= 0; i--) {
    heapify(arr, low, n, i);
  }
  for (int i = n - 1; i > 0; i--) {
    std::swap(arr[low], arr[low + i]);
    heapify(arr, low, i, 0);
  }
}

void insertionSort(std::vector<int> &arr, int low, int high) {
  for (int i = low + 1; i <= high; i++) {
    int key = arr[i];
    int j = i - 1;
    while (j >= low && arr[j] > key) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
}

void introSortHelper(std::vector<int> &arr, int low, int high, int recDepth) {
  int n = high - low + 1;
  if (n <= 16) {
    insertionSort(arr, low, high);
    return;
  }
  if (recDepth == 0) {
    heapSort(arr, low, high);
    return;
  }
  int pi = partition(arr, low, high);
  introSortHelper(arr, low, pi - 1, recDepth - 1);
  introSortHelper(arr, pi, high, recDepth - 1);
}

void introSort(std::vector<int> &arr) {
  int n = static_cast<int>(arr.size());
  if (n <= 1) {
    return;
  }
  int maxRecDepth = 2 * static_cast<int>(std::log2(n));
  introSortHelper(arr, 0, n - 1, maxRecDepth);
}

class SortTester {
public:
  static long long measureTime(std::vector<int> &arr, bool isStandard) {
    if (isStandard) {
      using clock = std::chrono::steady_clock;
      auto start = clock::now();
      quickSort(arr);
      auto elapsed = clock::now() - start;
      long long ms = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
      return ms;
    } else {
      using clock = std::chrono::steady_clock;
      auto start = clock::now();
      introSort(arr);
      auto elapsed = clock::now() - start;
      long long ms = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
      return ms;
    }
  }
};

int main() {
  std::vector<int> sizes;
  for (int i = 500; i <= 100000; i += 100) {
    sizes.push_back(i);
  }
  // --- Экспериментальные замеры для обычного QuickSort ---
  // тест 1 (случайные массивы)
  std::vector<int> base = ArrayGenerator::genRandom(100000);
  std::ofstream outRandom("quicksort_random.csv");
  outRandom << "size;time_run1_ms;time_run2_ms;time_run3_ms;time_run4_ms;time_run5_ms;time_run6_ms;time_run7_ms;time_run8_ms;time_run9_ms;med_ms\n";
  for (int size: sizes) {
    std::vector<long long> times;
    times.reserve(9);
    std::vector<int> base_copy = ArrayGenerator::getSubArray(base, size);
    for (int i = 0; i < 9; i++) {
      std::vector<int> work_mas = base_copy;
      long long ms = SortTester::measureTime(work_mas, true);
      times.push_back(ms);
    }
    outRandom << size << ";" << times[0] << ";" << times[1] << ";" << times[2] << ";" << times[3] << ";"
    << times[4] << ";" << times[5] << ";" << times[6] << ";" << times[7] << ";" << times[8] << ";";
    std::sort(times.begin(), times.end());
    long long med = times[4];
    outRandom << med << "\n";
  }
  outRandom.close();
  std::cout<<"QuickSort random done!\n";

  // тест 2 (отсортированные в обратном порядке массивы)
  std::vector<int> base2 = ArrayGenerator::genReverseSorted(100000);
  std::ofstream outReverseSorted("quicksort_reversed.csv");
  outReverseSorted << "size;time_run1_ms;time_run2_ms;time_run3_ms;time_run4_ms;time_run5_ms;time_run6_ms;time_run7_ms;time_run8_ms;time_run9_ms;med_ms\n";
  for (int size: sizes) {
    std::vector<long long> times;
    times.reserve(9);
    std::vector<int> base2_copy = ArrayGenerator::getSubArray(base2, size);
    for (int i = 0; i < 9; i++) {
      std::vector<int> work_mas = base2_copy;
      long long ms = SortTester::measureTime(work_mas, true);
      times.push_back(ms);
    }
    outReverseSorted << size << ";" << times[0] << ";" << times[1] << ";" << times[2] << ";" << times[3] << ";"
    << times[4] << ";" << times[5] << ";" << times[6] << ";" << times[7] << ";" << times[8] << ";";
    std::sort(times.begin(), times.end());
    long long med = times[4];
    outReverseSorted << med << "\n";
  }
  outReverseSorted.close();
  std::cout<<"QuickSort reverse done!\n";

  // тест 3 ("почти" отсортированные массивы)
  std::vector<int> base3 = ArrayGenerator::genNearlySorted(100000);
  std::ofstream outNearlySorted("quicksort_nearly_sorted.csv");
  outNearlySorted << "size;time_run1_ms;time_run2_ms;time_run3_ms;time_run4_ms;time_run5_ms;time_run6_ms;time_run7_ms;time_run8_ms;time_run9_ms;med_ms\n";
  for (int size: sizes) {
    std::vector<long long> times;
    times.reserve(9);
    std::vector<int> base3_copy = ArrayGenerator::getSubArray(base3, size);
    for (int i = 0; i < 9; i++) {
      std::vector<int> work_mas = base3_copy;
      long long ms = SortTester::measureTime(work_mas, true);
      times.push_back(ms);
    }
    outNearlySorted << size << ";" << times[0] << ";" << times[1] << ";" << times[2] << ";" << times[3] << ";"
    << times[4] << ";" << times[5] << ";" << times[6] << ";" << times[7] << ";" << times[8] << ";";
    std::sort(times.begin(), times.end());
    long long med = times[4];
    outNearlySorted << med << "\n";
  }
  outNearlySorted.close();
  std::cout<<"QuickSort nearly sorted done!\n";

    // --- Экспериментальные замеры для гибридного IntroSort ---
  // тест 4 (случайные массивы)
  std::vector<int> base4 = ArrayGenerator::genRandom(100000);
  std::ofstream outRandomIntro("introsort_random.csv");
  outRandomIntro << "size;time_run1_ms;time_run2_ms;time_run3_ms;time_run4_ms;time_run5_ms;time_run6_ms;time_run7_ms;time_run8_ms;time_run9_ms;med_ms\n";
  for (int size: sizes) {
    std::vector<long long> times;
    times.reserve(9);
    std::vector<int> base4_copy = ArrayGenerator::getSubArray(base4, size);
    for (int i = 0; i < 9; i++) {
      std::vector<int> work_mas = base4_copy;
      long long ms = SortTester::measureTime(work_mas, false);
      times.push_back(ms);
    }
    outRandomIntro << size << ";" << times[0] << ";" << times[1] << ";" << times[2] << ";" << times[3] << ";"
    << times[4] << ";" << times[5] << ";" << times[6] << ";" << times[7] << ";" << times[8] << ";";
    std::sort(times.begin(), times.end());
    long long med = times[4];
    outRandomIntro << med << "\n";
  }
  outRandomIntro.close();
  std::cout<<"IntroSort random done!\n";

  // тест 5 (отсортированные в обратном порядке массивы)
  std::vector<int> base5 = ArrayGenerator::genReverseSorted(100000);
  std::ofstream outReverseSortedIntro("introsort_reversed.csv");
  outReverseSortedIntro << "size;time_run1_ms;time_run2_ms;time_run3_ms;time_run4_ms;time_run5_ms;time_run6_ms;time_run7_ms;time_run8_ms;time_run9_ms;med_ms\n";
  for (int size: sizes) {
    std::vector<long long> times;
    times.reserve(9);
    std::vector<int> base5_copy = ArrayGenerator::getSubArray(base5, size);
    for (int i = 0; i < 9; i++) {
      std::vector<int> work_mas = base5_copy;
      long long ms = SortTester::measureTime(work_mas, false);
      times.push_back(ms);
    }
    outReverseSortedIntro << size << ";" << times[0] << ";" << times[1] << ";" << times[2] << ";" << times[3] << ";"
    << times[4] << ";" << times[5] << ";" << times[6] << ";" << times[7] << ";" << times[8] << ";";
    std::sort(times.begin(), times.end());
    long long med = times[4];
    outReverseSortedIntro << med << "\n";
  }
  outReverseSortedIntro.close();
  std::cout<<"IntroSort reverse done!\n";

  // тест 6 ("почти" отсортированные массивы)
  std::vector<int> base6 = ArrayGenerator::genNearlySorted(100000);
  std::ofstream outNearlySortedIntro("introsort_nearly_sorted.csv");
  outNearlySortedIntro << "size;time_run1_ms;time_run2_ms;time_run3_ms;time_run4_ms;time_run5_ms;time_run6_ms;time_run7_ms;time_run8_ms;time_run9_ms;med_ms\n";
  for (int size: sizes) {
    std::vector<long long> times;
    times.reserve(9);
    std::vector<int> base6_copy = ArrayGenerator::getSubArray(base6, size);
    for (int i = 0; i < 9; i++) {
      std::vector<int> work_mas = base6_copy;
      long long ms = SortTester::measureTime(work_mas, false);
      times.push_back(ms);
    }
    outNearlySortedIntro << size << ";" << times[0] << ";" << times[1] << ";" << times[2] << ";" << times[3] << ";"
    << times[4] << ";" << times[5] << ";" << times[6] << ";" << times[7] << ";" << times[8] << ";";
    std::sort(times.begin(), times.end());
    long long med = times[4];
    outNearlySortedIntro << med << "\n";
  }
  outNearlySortedIntro.close();
  std::cout<<"IntroSort nearly sorted done!\n";

  return 0;
}
