// DFS maze solver

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "matrix.h"
#include "Maze.h"
#include "Stack.h"

bool visitAndMarkPath(Maze m, struct cell current,
struct cell **predecessor, bool **visited, struct cell start);
static bool isWithinBounds(Maze m, struct cell c);

bool solve(Maze m) {
    // starting vertex src
    struct cell start = MazeGetStart(m);

    // create visited and predecessor arrays
    int height = MazeHeight(m);
    int width = MazeWidth(m);
    bool **visited = createBoolMatrix(height, width);
    struct cell **predecessor = createCellMatrix(height, width);

    // create stack and push src
    Stack stack = StackNew();
    StackPush(stack, start);

    // while the stack is not empty:
    while (!StackIsEmpty(stack)) {
        // pop v
        struct cell current = StackPop(stack);

        // if v has been visited:
		// continue (i.e., return to beginning of loop)
        if (visited[current.row][current.col]) continue;

        // mark v as visited
        visited[current.row][current.col] = true;

        if (visitAndMarkPath(m, current, predecessor, visited, start)) {
            freeBoolMatrix(visited);
            freeCellMatrix(predecessor);
            StackFree(stack);
            return true;
        }

        struct cell neighbors[4] = {
            {current.row - 1, current.col},
            {current.row, current.col + 1},
            {current.row, current.col - 1},
            {current.row + 1, current.col}
        };

        // for all edges (v, w) where w has not been visited:
        for (int i = 0; i < 4; i++) {
            struct cell neighbor = neighbors[i];
            if (isWithinBounds(m, neighbor) && !visited[neighbor.row][neighbor.col] 
            && !MazeIsWall(m, neighbor)) {
                // set predecessor of w to v
                predecessor[neighbor.row][neighbor.col] = current;
                // push w
                StackPush(stack, neighbor);
            }
        }
    }
    
    freeBoolMatrix(visited);
    freeCellMatrix(predecessor);
    StackFree(stack);
    return false;
}

static bool isWithinBounds(Maze m, struct cell c) {
    int height = MazeHeight(m);
    int width = MazeWidth(m);
    return c.row >= 0 && c.row < height && c.col >= 0 && c.col < width;
}

bool visitAndMarkPath(Maze m, struct cell current, struct cell **predecessor, 
bool **visited, struct cell start) {
    if (MazeVisit(m, current)) {
        struct cell pathCell = current;
        while (!(pathCell.row == start.row && pathCell.col == start.col)) {
            MazeMarkPath(m, pathCell);
            pathCell = predecessor[pathCell.row][pathCell.col];
        }
        MazeMarkPath(m, start);
        return true;
    }
    return false;
}