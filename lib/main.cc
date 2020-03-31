#include "a_star.hh"

int main()
{
  Map map{50, 20};
  map.add_obstacle_wall({15, 2}, {35, 2});
  map.add_obstacle_wall({15, 2}, {16, 6});
  map.add_obstacle_wall({35, 2}, {34, 6});
  map.add_obstacle_wall({16, 7}, {34, 7});
  map.add_obstacle_wall({14, 11}, {37, 11});
  map.add_obstacle_wall({14, 12}, {15, 15});
  map.add_obstacle_wall({36, 12}, {35, 15});
  map.add_obstacle_wall({15, 16}, {36, 16});

  map.print();
  return 0;
}
