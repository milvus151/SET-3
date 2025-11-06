#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
class ArrayGenerator {
public:
  static std::vector<int> genRandom(int n) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> dist(0, 10000);
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
    std::mt19937 gen(std::random_device{}());
    int swap = n / 25;
    std::uniform_real_distribution<> dist(0, n - 1);
    for (int i = 0; i < swap; i++) {
      int x1 = dist(gen);
      int x2 = dist(gen);
      int temp = generated[x2];
      generated[x2] = generated[x1];
      generated[x1] = temp;
    }
    return generated;
  }
  static std::vector<int> getSubArray(const std::vector<int> &baseArray, int exactSize) {
    std::vector<int> result(baseArray.begin(), baseArray.begin() + exactSize);
    return result;
  }
};

void insertionSort(std::vector<int> &arr, const int left, const int right) {
  for (int i = left + 1; i <= right; i++) {
    int key = arr[i];
    int j = i - 1;
    while (j >= left && arr[j] > key) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
}

void merge(std::vector<int> &arr, const int left, const int mid, const int right) {
  std::vector<int> temp(right - left + 1);
  int i = left;
  int j = mid + 1;
  int k = 0;
  while (i <= mid && j <= right) {
    if (arr[i] <= arr[j]) {
      temp[k++] = arr[i++];
    } else {
      temp[k++] = arr[j++];
    }
  }
  while (i <= mid) {
    temp[k++] = arr[i++];
  }
  while (j <= right) {
    temp[k++] = arr[j++];
  }
  for (int l = 0; l < static_cast<int>(temp.size()); l++) {
    arr[left + l] = temp[l];
  }
}

class MergeSort {
private:
  void mergeSort(std::vector<int> &arr, int left, int right) {
    if (left >= right) {
      return;
    }
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }

public:
  void sort(std::vector<int> &arr) {
    if (arr.empty()) {
      return;
    }
    mergeSort(arr, 0, static_cast<int>(arr.size()) - 1);
  }
};

class MergeInsertionSort {
private:
  int threshold;
  void mergeInsertionSort(std::vector<int> &arr, int left, int right) {
    if (right - left + 1 <= threshold) {
      insertionSort(arr, left, right);
      return;
    }
    int mid = left + (right - left) / 2;
    mergeInsertionSort(arr, left, mid);
    mergeInsertionSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }

public:
  explicit MergeInsertionSort(int th) : threshold(th) {}
  void MISort(std::vector<int> &arr) {
    if (arr.empty()) {
      return;
    }
    mergeInsertionSort(arr, 0, static_cast<int>(arr.size()) - 1);
  }
};

class SortTester {
public:
  static long long measureTimeMISort(std::vector<int> arr, MergeInsertionSort &sorter) {
    auto start = std::chrono::high_resolution_clock::now();
    sorter.MISort(arr);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long ms = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    return ms;
  }
  static long long measureTimeMergeSort(std::vector<int> arr, MergeSort &sorter) {
    auto start = std::chrono::high_resolution_clock::now();
    sorter.sort(arr);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long ms = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    return ms;
  }
};
int main() {
  std::vector<int> sizes;
  for (int i = 500; i <= 100000; i += 100) {
    sizes.push_back(i);
  }
  // --- Экспериментальные замеры для обычного MergeSort ---
  // тест 1 (случайные массивы)
  std::vector<int> base = ArrayGenerator::genRandom(100000);
  std::ofstream outRandom("merge_sort_random.csv");
  outRandom << "size;time_run1_ms;time_run2_ms;time_run3_ms;average_ms\n";
  for (int size: sizes) {
    std::vector<long long> times;
    for (int i = 0; i < 3; i++) {
      std::vector<int> work_mas = ArrayGenerator::getSubArray(base, size);
      MergeSort sorter;
      long long ms = SortTester::measureTimeMergeSort(work_mas, sorter);
      times.push_back(ms);
    }
    long long avg = (times[0] + times[1] + times[2]) / 3;
    outRandom << size << ";" << times[0] << ";" << times[1] << ";" << times[2] << ";" << avg << "\n";
  }
  outRandom.close();
  std::cout<<"MergeSort random done!\n";

  // тест 2 (отсортированные в обратном порядке массивы)
  std::vector<int> base2 = ArrayGenerator::genReverseSorted(100000);
  std::ofstream outReverseSorted("merge_sort_reversed.csv");
  outReverseSorted << "size;time_run1_ms;time_run2_ms;time_run3_ms;average_ms\n";
  for (int size: sizes) {
    std::vector<long long> times;
    for (int i = 0; i < 3; i++) {
      std::vector<int> work_mas = ArrayGenerator::getSubArray(base2, size);
      MergeSort sorter;
      long long ms = SortTester::measureTimeMergeSort(work_mas, sorter);
      times.push_back(ms);
    }
    long long avg = (times[0] + times[1] + times[2]) / 3;
    outReverseSorted << size << ";" << times[0] << ";" << times[1] << ";" << times[2] << ";" << avg << "\n";
  }
  outReverseSorted.close();
  std::cout<<"MergeSort reverse done!\n";

  // тест 3 ("почти" отсортированные массивы)
  std::vector<int> base3 = ArrayGenerator::genNearlySorted(100000);
  std::ofstream outNearlySorted("merge_sort_nearly_sorted.csv");
  outNearlySorted << "size;time_run1_ms;time_run2_ms;time_run3_ms;average_ms\n";
  for (int size: sizes) {
    std::vector<long long> times;
    for (int i = 0; i < 3; i++) {
      std::vector<int> work_mas = ArrayGenerator::getSubArray(base3, size);
      MergeSort sorter;
      long long ms = SortTester::measureTimeMergeSort(work_mas, sorter);
      times.push_back(ms);
    }
    long long avg = (times[0] + times[1] + times[2]) / 3;
    outNearlySorted << size << ";" << times[0] << ";" << times[1] << ";" << times[2] << ";" << avg << "\n";
  }
  outNearlySorted.close();
  std::cout<<"MergeSort nearly sorted done!\n";

  // --- Экспериментальные замеры для MergeInsertionSort ---
  std::vector thresholds = {5, 10, 20, 30, 50};

  // тест 1 (случайные массивы)
  std::vector<int> base4 = ArrayGenerator::genRandom(100000);
  std::ofstream outRandomMI("merge_insertion_sort_random.csv");
  outRandomMI << "size;threshold;time_run1_ms;time_run2_ms;time_run3_ms;average_ms\n";
  for (int size: sizes) {
    for (int threshold: thresholds) {
      std::vector<long long> times;
      for (int i = 0; i < 3; i++) {
        std::vector<int> work_mas = ArrayGenerator::getSubArray(base4, size);
        MergeInsertionSort sorter(threshold);
        long long ms = SortTester::measureTimeMISort(work_mas, sorter);
        times.push_back(ms);
      }
      long long avg = (times[0] + times[1] + times[2]) / 3;
      outRandomMI << size << ";" << threshold <<";"<< times[0] << ";" << times[1] << ";" << times[2] << ";" << avg << "\n";
    }
  }
  outRandomMI.close();
  std::cout<<"MergeInsertionSort random done!\n";

  // тест 2 (отсортированные в обратном порядке массивы)
  std::vector<int> base5 = ArrayGenerator::genReverseSorted(100000);
  std::ofstream outReverseSortedMI("merge_insertion_sort_reversed.csv");
  outReverseSortedMI << "size;threshold;time_run1_ms;time_run2_ms;time_run3_ms;average_ms\n";
  for (int size: sizes) {
    for (int threshold: thresholds) {
      std::vector<long long> times;
      for (int i = 0; i < 3; i++) {
        std::vector<int> work_mas = ArrayGenerator::getSubArray(base5, size);
        MergeInsertionSort sorter(threshold);
        long long ms = SortTester::measureTimeMISort(work_mas, sorter);
        times.push_back(ms);
      }
      long long avg = (times[0] + times[1] + times[2]) / 3;
      outReverseSortedMI << size << ";" << threshold <<";"<< times[0] << ";" << times[1] << ";" << times[2] << ";" << avg
                         << "\n";
    }
  }
  outReverseSortedMI.close();
  std::cout<<"MergeInsertionSort reverse done!\n";

  // тест 3 ("почти" отсортированные массивы)
  std::vector<int> base6 = ArrayGenerator::genNearlySorted(100000);
  std::ofstream outNearlySortedMI("merge_insertion_sort_nearly_sorted.csv");
  outNearlySortedMI << "size;threshold;time_run1_ms;time_run2_ms;time_run3_ms;average_ms\n";
  for (int size: sizes) {
    for (int threshold: thresholds) {
      std::vector<long long> times;
      for (int i = 0; i < 3; i++) {
        std::vector<int> work_mas = ArrayGenerator::getSubArray(base6, size);
        MergeInsertionSort sorter(threshold);
        long long ms = SortTester::measureTimeMISort(work_mas, sorter);
        times.push_back(ms);
      }
      long long avg = (times[0] + times[1] + times[2]) / 3;
      outNearlySortedMI << size << ";" << threshold <<";"<< times[0] << ";" << times[1] << ";" << times[2] << ";" << avg
                        << "\n";
    }
  }
  outNearlySortedMI.close();
  std::cout<<"MergeInsertionSort nearly sorted done!\n";

  return 0;
}
