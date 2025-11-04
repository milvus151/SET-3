#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
double monte_carlo(double minX, double maxX, double minY, double maxY,
                   double x1, double y1, double r1,
                   double x2, double y2, double r2,
                   double x3, double y3, double r3,
                   int N) {
  std::mt19937 gen(12345);
  std::uniform_real_distribution<> distX(minX, maxX);
  std::uniform_real_distribution<> distY(minY, maxY);
  int cnt = 0;
  for (int i = 0; i < N; i++) {
    double x = distX(gen);
    double y = distY(gen);
    double dist1_sq = (x - x1) * (x - x1) + (y - y1) * (y - y1);
    double dist2_sq = (x - x2) * (x - x2) + (y - y2) * (y - y2);
    double dist3_sq = (x - x3) * (x - x3) + (y - y3) * (y - y3);

    if (dist1_sq <= r1*r1 && dist2_sq <= r2*r2 && dist3_sq <= r3*r3) {
      cnt++;
    }
  }
  double S_rect = (maxX - minX)*(maxY-minY);
  double result = cnt / static_cast<double>(N) * S_rect;
  return result;
}
int main() {
  double Xs[3] = {1.0, 1.5, 2.0};
  double Ys[3] = {1.0, 2.0, 1.5};
  double Rs[3] = {1.0, std::sqrt(5)/2, std::sqrt(5)/2};
  double S_exact = 0.25 * std::numbers::pi + 1.25 * std::asin(0.8) - 1.0;
  // Две области
  double wide_minX = 0.0;
  double wide_maxX = 2.0+std::sqrt(5)/2;
  double wide_minY = 0.0;
  double wide_maxY = 2.0+std::sqrt(5)/2;

  double narrow_minX = 0.8;
  double narrow_maxX = 2.0;
  double narrow_minY = 0.8;
  double narrow_maxY = 2.0;

  std::ofstream out("results.csv");
  out.precision(15);
  out << "N;res_wide;res_narrow;error_wide;error_narrow\n";
  for (int i = 100; i <= 100000; i+=500) {
    double res_wide = monte_carlo(wide_minX, wide_maxX, wide_minY, wide_maxY,
      Xs[0], Ys[0], Rs[0], Xs[1], Ys[1], Rs[1], Xs[2], Ys[2], Rs[2], i);

    double res_narrow = monte_carlo(narrow_minX, narrow_maxX, narrow_minY, narrow_maxY,
  Xs[0], Ys[0], Rs[0], Xs[1], Ys[1], Rs[1], Xs[2], Ys[2], Rs[2], i);

    double error_wide = std::abs(res_wide - S_exact) / S_exact;
    double error_narrow = std::abs(res_narrow - S_exact) / S_exact;

    out << i << ";"
    << res_wide << ";"<< res_narrow << ";"
    << error_wide << ";"<< error_narrow << "\n";
  }
  out.close();
  std::cout << "Results are saved to results.csv" << std::endl;
  std::cout << "Exact result: " << S_exact << std::endl;
  return 0;
}
