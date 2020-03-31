#include <array>
#include <vector>
#include <iostream>
#include <cmath>
#include <memory>
#include <algorithm>

using vec2_t = std::array<int, 2>;

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

  void add_obstacle(vec2_t p)
  {
    if(p[0] >= 0 && p[0] < w_max_ && p[1] >= 0 && p[1] < h_max_)
    {
      grids_[p[0]][p[1]] = OBSTACLE_CHAR;
    }
  }

  void add_obstacle_wall(vec2_t p1, vec2_t p2)
  {
    if(p1[0] > w_max_ || p2[0] > w_max_ || p1[1] > h_max_ || p2[1] > h_max_
       || p1[0] < 0 || p1[1] < 0 || p2[0] < 0 || p2[1] < 0)
    {
      return;
    }

    size_t y_max = std::max(p1[1], p2[1]);
    size_t y_min = std::min(p1[1], p2[1]);

    if(p1[0] == p2[0])
    {
      for (int i = 0; i <= y_max - y_min; i++)
      {
        grids_[p1[0]][i + y_min] = OBSTACLE_CHAR;
      }
      return;
    }

    size_t x_max = std::max(p1[0], p2[0]);
    size_t x_min = std::min(p1[0], p2[0]);

    if(p1[1] == p2[1])
    {
      for (int i = 0; i <= x_max - x_min; i++)
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

  std::vector<vec2_t> get_neighbors(vec2_t p) const
  {
    std::vector<vec2_t> neighbors;
    if(p[0] > 0)
    {
      vec2_t p_neighbor{p[0] - 1, p[1]};
      if(grids_[p_neighbor[0]][p_neighbor[1]] != OBSTACLE_CHAR)
      {
        neighbors.push_back(p_neighbor);
      }
    }
    if(p[0] < w_max_ - 1)
    {
      vec2_t p_neighbor{p[0] + 1, p[1]};
      if(grids_[p_neighbor[0]][p_neighbor[1]] != OBSTACLE_CHAR)
      {
        neighbors.push_back(p_neighbor);
      }
    }
    if(p[1] > 0)
    {
      vec2_t p_neighbor{p[0], p[1] - 1};
      if(grids_[p_neighbor[0]][p_neighbor[1]] != OBSTACLE_CHAR)
      {
        neighbors.push_back(p_neighbor);
      }
    }
    if(p[1] < h_max_ - 1)
    {
      vec2_t p_neighbor{p[0], p[1] + 1};
      if(grids_[p_neighbor[0]][p_neighbor[1]] != OBSTACLE_CHAR)
      {
        neighbors.push_back(p_neighbor);
      }
    }
    return neighbors;
  }

  void add_path(const std::vector<vec2_t> &path)
  {
    for(const auto &p : path)
    {
      if(p[0] >= 0 && p[0] < w_max_ && p[1] >= 0 && p[1] < h_max_)
      {
        grids_[p[0]][p[1]] = PATH_CHAR;
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

inline double manhattan_distance(vec2_t a, vec2_t b)
{
  return std::abs(a[0] - b[0]) + std::abs(a[1] - b[1]);
}

class State
{
public:
  State(vec2_t start, vec2_t target)
    : target_{target}
  {
    path_.push_back(start);
  }

  std::vector<State> expand(std::shared_ptr<Map> map)
  {
    std::vector<State> result;
    auto neighbor_grids = find_neighbor_grids(map);
    for(auto grid : neighbor_grids)
    {
      auto temp = *this;
      temp.path_.push_back(grid);
      temp.path_length_ += 1;
      result.push_back(std::move(temp));
    }
    return result;
  }

  double score() const
  {
    return path_length_ + manhattan_distance(path_.back(), target_);
  }
  bool is_terminal() const
  {
    return path_.back() == target_;
  }
  const std::vector<vec2_t>& path() const
  {
    return path_;
  }
private:
  std::vector<vec2_t> find_neighbor_grids(std::shared_ptr<Map> map) const
  {
    auto potential_neighbors = map->get_neighbors(path_.back());
    std::vector<vec2_t> neighbors;
    for(auto nb : potential_neighbors)
    {
      bool been_there = false;
      for(auto it = path_.rbegin(); it != path_.rend(); it++)
      {
        if((*it) == nb)
        {
          been_there = true;
        }
      }
      if(!been_there)
      {
        neighbors.push_back(nb);
      }
    }
    return neighbors;
  }
  std::vector<vec2_t> path_;
  vec2_t target_;
  double path_length_;
};

class AstarPath
{
public:
  AstarPath(std::shared_ptr<Map> map, vec2_t start, vec2_t target)
    : map_{map}, start_{start}, target_{target}
  {
    frontier_states_.emplace_back(start, target);
    a_star_search();
  }
  const std::vector<vec2_t>& path() const
  {
    return best_path_;
  }

private:
  void a_star_search()
  {
    int c = 0;
    while(!frontier_states_.empty())
    {
      auto current_state = pop_most_promising_state();
      std::cout << c++ << ", " << current_state.score() << "\r";
      // auto temp_map = *map_;
      // temp_map.add_path(current_state.path());
      // temp_map.print();
      if(current_state.is_terminal())
      {
        best_path_ = current_state.path();
        break;
      }
      auto new_states = current_state.expand(map_);
      frontier_states_.insert(
        frontier_states_.end(), new_states.begin(), new_states.end());
    }
    std::cout << "\n";

    if(best_path_.empty())
    {
      std::cout << "didn't find any path!\n";
    }
  }

  State pop_most_promising_state()
  {
    std::sort(frontier_states_.begin(), frontier_states_.end(),
              [](const auto &s1, const auto &s2){
                return s1.score() > s2.score();});
    auto result = frontier_states_.back();
    frontier_states_.pop_back();
    return result;
  }

  std::shared_ptr<Map> map_;
  vec2_t start_;
  vec2_t target_;
  std::vector<State> frontier_states_;
  std::vector<vec2_t> best_path_;
};
