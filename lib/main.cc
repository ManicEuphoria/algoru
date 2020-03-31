#include <memory>
#include "a_star.hh"

int main()
{
  auto map = std::make_shared<Map>(50, 20);
  map->add_obstacle_wall({10, 4}, {13, 11});
  AstarPath a_star_path{map, {3, 10}, {20, 10}};
  map->add_path(a_star_path.path());
  map->print();
  return 0;
}
