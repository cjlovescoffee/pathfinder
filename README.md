# Pathfinder

## Introduction

This is a console application written in c++ that implements the A* search algorithm to find the shortest path between two points on a 2D gridded map. It reads maps stored in text files and prints the shortest path as a series of directional instructions, with N representing North, E for East, S for South and W for West. In this document, and in code, I refer to any space represented by the characters `A`, `B`, `.` or `x` as a _node_. `A` represents a start point, `B` represents the goal, `.` is a traversable node and `x` is an untraversable node.

## How It Works

I assume that the nodes in a map are spaced apart equally and that their positions on a 2D plane can be represented accurately by `x` and `y` coordinates. This assumption allows me to use Pythagorean theorum to calculate A*'s heuristic value for each node.

For example, in the following map, if `B` is at coordinate `(0, 4)` and the `!` is at coordinate `(3, 2)`, I can calculate the heuristic value as the square root of `(-3 * -3) + (2 * 2)` which is approximately `3.6`. 

```
xxxxBx
x....x
x..xxx
x.!..x
xxxxAx
```

Any space in a map that is not represented by `A`, `B`, `.` or `x` defaults to `x`. For this reason, my program handles irregularly formed maps. For example:

```
x
x...B
x.
x.
x...A
```

This map will get treated as though it were:

```
xxxxx
x...B
x.xxx
x.xxx
x...A
```

> The solution to this map according to my program is `WWWNNNEEE`

## A* Search Algorithm

The A* search algorithm works by iterating through nodes in an order that is determined by selecting from a pool of elligible nodes whichever returns the lowest value for the function `f(n) = g(n) + h(n)`, where `n` is the node to iterate over next, `g` represents the cost of arriving at `n` from the start node, and `h` represents the estimated cost of reaching the goal node from `n`.

Starting at the start node, the algorithm evaluates a node's _neighbouring_ nodes to determine _their_ lowest cost back to the start and their estimated cost to reach the goal. This estimated cost is referred to as the _heuristic_ value, and its inclusion is the key difference between the A* search algorithm and Dijkstra's algorithm.

> This program can perform Dijkstra's algorithm by returning a constant value in place of the heuristic value for each node

If a neighbour has not yet been evaluated, it is added to the pool of elligible nodes for the next iteration of the algorithm to choose from (according to the function described above).

Every time a node is evaluated as a _neighbour_, it is updated to reflect the definitive shortest path to the start (represented in this program by a `parent` pointer) and its heuristic value is calculated.

The algorithm iterates over nodes until it has reached the goal node.

When the algorithm has reached the goal, we can work backwards from the goal node by looking at the node that represents the shortest path back to the start - the `parent` pointer, as described above.

This program determines the actual solution by iterating through these `parent` pointers and establishing the direction of the path by comparing `x` and `y` coordinates. For example:

```
xxxxxxx
xA...Bx
xxxxxxx
```

The A* search algorithm will determine that `B`'s parent is the `.` immediately to the left. `B` can be found at `(5, 1)` and it's parent at `(4, 1)`. A difference of 1 in the x-axis represents a movement eastward or westward - in this case, because the program works backwards from the goal, a negative difference represents an eastward movement, so this movement can be described as `E`.

> In this example, the heuristic value for A is 4. The heuristic value for it's eastern neighbour is 3.

## Build

There is a `Makefile` included with this repository, which is written to use `g++`. If you have `make` installed, run:

```
make
```

If not, this command should build the program:

```
g++ -std=c++17 -ggdb -Iinclude src/FileParser.cpp src/MapData.cpp src/MapSolver.cpp Main.cpp -o bin/main 
```

These commands build an executable into the `/bin` directory.

## Run

### Arguments

#### Filename (required)

This program requires a path to a valid text file that represents a map to solve:

```
bin/main ./maps/example.txt
```

#### Use Dijkstra's Algorithm (optional)

Optionally use Dijkstra's algorithm instead of A* by passing an optional second parameter:

```
bin/main ./maps/example.txt dijkstra
```

## Example Map

```
xBxxxxxxxxxxxxxxxxxxxx
x......xx............x
xxxxxx.xx..xxx.xxxxx.x
xxx.....xx.xx..xx....x
xxx.xxxxxx.x..xxx.xxxx
xxx........xxxxxx....x
xxxxxxxxxxxxxxxxxxxx.x
xxx..................x
xxxAxxxxxxxxxxxxxxxxxx
```

> Solution: `NEEEEEEEEEEEEEEEEENNWWWNNEEENNWWWWWWWWWWSSSSWWWWWWWNNEEENNWWWWWN`

## TODO

It was trivial to add the necessary scaffolding to allow for ordinal (NE, SE, SW, NW) movement in addition to cardinal movement. The only missing implementation is to add additional neighbours to each node to allow for 8 possible paths instead of 4.

That said, I think a far more interesting addition to this program would be movement alterations that represent unusual terrain. For example:

```
xxxBxxx
x1....x
x.xxx2x
x1xxx.x
x..A..x
```

Where `1` and `2` represent movement impediments or alterations (some obvious video game suggestions: mud, snow, ice, an enemy).

## Notes

This program was developed and tested on macOS (Big Sur) 11.0.1. The compiler is `Apple clang version 12.0.0 (clang-1200.0.32.27)`.

## Author

[Chris West](mailto:cjlovescoffee@gmail.com)
