#include <array>
#include <vector>
#include <iostream>
#include <cmath>

using point_t = std::array<int, 2>;

class Map
{
public:
  Map(size_t w_max, size_t h_max)
    : w_max_{w_max}, h_max_{h_max}
  {
    for (size_t x = 0; x < w_max_; x++)
    {
      std::vector<char> column;
      for (size_t y = 0; y < h_max; y++)
      {
        column.push_back(AIR_CHAR);
      }
      grids_.push_back(std::move(column));
    }
  }

  void add_obstacle(point_t p)
  {
    if(p[0] < w_max_ && p[1] < h_max_)
    {
      grids_[p[0]][p[1]] = OBSTACLE_CHAR;
    }
  }

  void add_obstacle_wall(point_t p1, point_t p2)
  {
    if(p1[0] > w_max_ || p2[0] > w_max_ || p1[1] > h_max_ || p2[1] > h_max_)
    {
      return;
    }

    size_t y_max = std::max(p1[1], p2[1]);
    size_t y_min = std::min(p1[1], p2[1]);

    if(p1[0] == p2[0])
    {
      for (int i = 0; i < y_max - y_min; i++)
      {
        grids_[p1[0]][i + y_min] = OBSTACLE_CHAR;
      }
      return;
    }

    size_t x_max = std::max(p1[0], p2[0]);
    size_t x_min = std::min(p1[0], p2[0]);

    if(p1[1] == p2[1])
    {
      for (int i = 0; i < x_max - x_min; i++)
      {
        grids_[i + x_min][p1[1]] = OBSTACLE_CHAR;
      }
      return;
    }

    auto delta_y = p2[1] - p1[1];
    auto delta_x = p2[0] - p1[0];
    auto slope = static_cast<double>(delta_y) / (delta_x);

    if(std::abs(slope) <= 1)
    {
      auto pleft = p1[0] < p2[0] ? p1 : p2;
      auto pright = p1[0] >= p2[0] ? p1 : p2;
      for (int dx = 0; dx <= pright[0] - pleft[0]; dx++)
      {
        auto x = pleft[0] + dx;
        auto y = static_cast<size_t>(std::round(slope * dx + pleft[1]));
        grids_[x][y] = OBSTACLE_CHAR;
      }
    }
    else
    {
      auto plower = p1[1] < p2[1] ? p1 : p2;
      auto pupper = p1[1] >= p2[1] ? p1 : p2;
      auto slope_r = 1.0 / slope;
      for (int dy = 0; dy <= pupper[1] - plower[1]; dy++)
      {
        auto y = y_min + dy;
        auto x = static_cast<size_t>(std::round(slope_r * dy + plower[0]));
        grids_[x][y] = OBSTACLE_CHAR;
      }
    }

  }

  void print() const
  {
    std::cout << '+';
    for (size_t w = 0; w < w_max_; w++)
    {
      std::cout << '-';
    }
    std::cout << "+\n";

    for (size_t h = 0; h < h_max_; h++)
    {
      std::cout << "|";
      for (size_t w = 0; w < w_max_; w++)
      {
        std::cout << grids_[w][h];
      }
      std::cout << "|\n";
    }

    std::cout << '+';
    for (size_t w = 0; w < w_max_; w++)
    {
      std::cout << '-';
    }
    std::cout << "+\n";
  }

  const std::vector<std::vector<char>>& grids() const
  {
    return grids_;
  }
private:
  std::vector<std::vector<char>> grids_;
  size_t w_max_;
  size_t h_max_;
  static constexpr char AIR_CHAR = ' ';
  static constexpr char OBSTACLE_CHAR = '#';
  static constexpr char PATH_CHAR = 'o';
};

std::vector<point_t> a_star(const Map &map, point_t start, point_t target)
{
  return {};
}
