#include <iostream>
#include <deque>
#include <array>
#include <utility>
#include <vector>
#include <string>

#define Y_MIN 0
#define Y_MAX 1

#define X 0
#define Y 1

using Coord = std::array<int, 2>;
using VertMinMax = std::array<Coord, 2>;
using Area = std::deque<VertMinMax>;


void update_time_area(Area& area, int t) {
  int y_min_left = area.front()[Y_MIN][Y];
  int y_max_left = area.front()[Y_MAX][Y];
  int y_min_right = area.back()[Y_MIN][Y];
  int y_max_right = area.back()[Y_MAX][Y];

  for(auto& pair: area) {
    pair[Y_MIN][Y] -= t;
    pair[Y_MAX][Y] += t;
  }

  int x_min = area.front()[Y_MIN][X];
  for(int x = x_min - 1; x >= x_min - t; --x) {
    int dy = t - (x_min - x);
    area.push_front({{{{x, y_min_left - dy}},
                      {{x, y_max_left + dy}}}});
  }

  int x_max = area.back()[Y_MIN][X];
  for(int x = x_max + 1; x <= x_max + t; ++x) {
    int dy = t - (x - x_max);
    area.push_back({{{{x, y_min_right - dy}},
                     {{x, y_max_right + dy}}}});
  }
}


void generate_gps_area(const Coord& coord, int d, Area& area) {
  for(int x = coord[X] - d; x <= coord[X] + d; ++x) {
    int dy = d - std::abs(coord[X] - x);
    area.push_back({{{{x, coord[Y] - dy}}, {{x, coord[Y] + dy}}}});
  }
}

void intersection_area(const Area& area1, const Area& area2, Area& res) {
  auto it1 = area1.begin();
  auto it2 = area2.begin();
  while(it1 != area1.end() && it2 != area2.end()) {
    if((*it1)[Y_MIN][X] < (*it2)[Y_MIN][X]) {
      ++it1;
    }
    else if((*it1)[Y_MIN][X] > (*it2)[Y_MIN][X]) {
      ++it2;
    }
    else {
      int x = (*it1)[Y_MIN][X];
      int y_min = std::max((*it1)[Y_MIN][Y], (*it2)[Y_MIN][Y]);
      int y_max = std::min((*it1)[Y_MAX][Y], (*it2)[Y_MAX][Y]);
      if(y_min <= y_max) {
        res.push_back({{{{x, y_min}}, {{x, y_max}}}});
      }
      ++it1;
      ++it2;
    }
  }
}

int main() {
	int t, d, n;
  std::cin >> t >> d >> n;
  Area area;
  area.push_back({{{{0, 0}}, {{0, 0}}}});
  for(int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    Area gps_area, tmp_area;
    update_time_area(area, t);
    generate_gps_area({{x, y}}, d, gps_area);
    intersection_area(area, gps_area, tmp_area);
    area = std::move(tmp_area);
  }

  std::vector<std::string> results;
  for(auto& pair: area) {
    int x = pair[Y_MIN][X];
    for(int y = pair[Y_MIN][Y]; y <= pair[Y_MAX][Y]; ++y) {
      results.push_back(std::to_string(x) +  " " + std::to_string(y));
    }
  }

  std::cout << results.size() << std::endl;
  for(auto& res: results) {
    std::cout << res << std::endl;
  }

	return 0;
}
