// BFS maze solver

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "matrix.h"
#include "Maze.h"
#include "Queue.h"

bool visitAndMarkPath(Maze m, struct cell current,
struct cell **predecessor, bool **visited, struct cell start);

bool solve(Maze m) {
    // starting vertex src
    struct cell start = MazeGetStart(m);
    
    // create visited and predecessor arrays
    int rows = MazeHeight(m);
    int cols = MazeWidth(m);
    bool **visited = createBoolMatrix(rows, cols);
    struct cell **predecessor = createCellMatrix(rows, cols);

    // create queue and enqueue src
    Queue q = QueueNew();
    QueueEnqueue(q, start);

    // mark src as visited
    visited[start.row][start.col] = true;
    int move[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // while the queue is not empty:
    while (!QueueIsEmpty(q)) {
        // dequeue v (vertex)
        struct cell current = QueueDequeue(q);
        // if v has been visited:
        if (visitAndMarkPath(m, current, predecessor, visited, start)) {
            freeBoolMatrix(visited);
            freeCellMatrix(predecessor);
            QueueFree(q);
            return true;
        }

        // for all edges (v, w) where w has not been visited:
        // where v is the current cell and w is the neighbor cell
        for (int i = 0; i < 4; i++) {
            int newRow = current.row + move[i][0];
            int newCol = current.col + move[i][1];
            struct cell neighbor = {newRow, newCol};
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols && 
                !MazeIsWall(m, neighbor) && !visited[newRow][newCol]) {
                // mark w as visited
                visited[newRow][newCol] = true;
                // set predecessor of w to v
                predecessor[newRow][newCol] = current;
                // enqueue w
                QueueEnqueue(q, neighbor);
            }
        }
    }

    freeBoolMatrix(visited);
    freeCellMatrix(predecessor);
    QueueFree(q);
    return false;
}

bool visitAndMarkPath(Maze m, struct cell current, struct cell **predecessor, bool **visited, struct cell start) {
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