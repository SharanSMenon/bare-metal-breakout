#include "logic.h"

/**
 * @brief Checks whether the ball hit the corners.
 *
 * @param circleX
 * @param circleY
 * @param width
 * @param height
 * @return int
 */
int detectCornerCollison(int circleX, int circleY, int width, int height)
{
    if (circleY - CIRCLE_RADIUS <= 0)
    { // top collison
        return 0;
    }
    if (circleX - CIRCLE_RADIUS <= X_OFFSET)
    { // left collison
        return 1;
    }
    if (circleX + CIRCLE_RADIUS >= width)
    { // Right Collison
        return 2;
    }
    if (circleY + CIRCLE_RADIUS >= height - Y_OFFSET / 2)
    { // bottom collison
        return 3;
    }
    return -1;
}

/**
 * @brief Checks whether the ball hit the paddle
 *
 * @param circleX
 * @param circleY
 * @param boxPosition
 * @param height
 * @param direction
 * @return int
 */
int detectPaddleCollision(int circleX, int circleY, int boxPosition, int height, int *direction)
{
    int x1 = X_OFFSET + boxPosition;
    int x2 = X_OFFSET + boxPosition + BOX_WIDTH;
    int y1 = height - Y_OFFSET;
    // int y2 = height;
    int midpoint = (x1 + x2) / 2;
    if ((circleY + CIRCLE_RADIUS >= y1) && (circleX >= x1 && circleX <= x2))
    {
        if (circleX <= midpoint)
        {
            *direction = 0; // go left
        }
        else
        {
            *direction = 1; // go right
        }
        return 1;
    }
    return -1;
}

/**
 * @brief Checks whether the ball hit the blocks, and returns the block number.
 *
 * @param objects
 * @param circleX
 * @param circleY
 * @param width
 * @param direction
 * @return int
 */
int detectCollision(int *objects, int circleX, int circleY, int width, int *direction)
{
    int blockWidth = width / BLOCKS_PER_ROW;
    for (int i = BLOCKS_PER_ROW * 5 - 1; i >= 0; --i)
    { //  we check from bottom to top.
        int row = i / 10;
        int col = i % 10;
        int x1 = X_OFFSET + blockWidth * col;       // (x1, y1) = top left corner
        int x2 = X_OFFSET + blockWidth * (col + 1); // (x2, y2) = bottom right corner
        int y1 = Y_OFFSET + HEIGHT * row;
        int y2 = Y_OFFSET + HEIGHT * (row + 1);
        // Check for collision
        if (objects[i] == 0)
        { // block needs to exist (0 = exist, 1 = gone)
          // check from right collison
            if ((circleX - CIRCLE_RADIUS <= x2 && circleX + CIRCLE_RADIUS >= x1) && (circleY >= y1 && circleY <= y2))
            {
                *direction = 0; // right
                return i;
            }
            else if ((circleX + CIRCLE_RADIUS >= x1 && circleX + CIRCLE_RADIUS <= x2) && (circleY >= y1 && circleY <= y2))
            {                   // from left collison
                *direction = 1; // left
                return i;
            }
            if ((circleY + CIRCLE_RADIUS >= y1 && circleY + CIRCLE_RADIUS <= y2) && (circleX >= x1 && circleX <= x2))
            {                   // from top collison
                *direction = 3; // top
                return i;
            }
            else if ((circleY - CIRCLE_RADIUS <= y2 && circleY + CIRCLE_RADIUS >= y1) && (circleX >= x1 && circleX <= x2))
            {                   // from bottom collison
                *direction = 2; // bottom
                return i;
            }
        }
    }
    return -1;
}

/**
 * @brief Get the Speed of the box
 *
 * @param width
 * @param boxPosition
 * @param boxDirection
 * @return int
 */
int getSpeed(int width, int boxPosition, int *boxDirection)
{
    if (width - BOX_WIDTH == boxPosition)
    {                      // reaches right corner
        *boxDirection = 1; // move left
        return -SPEED;
    }
    else if (boxPosition == 0)
    {
        *boxDirection = 0; // move right
        return SPEED;
    }
    return (*boxDirection == 0 ? SPEED : -SPEED);
}

/**
 * @brief A function that does absolutely nothing. Used mainly for development purposes.
 *
 */
void doNothing()
{
    asm volatile("nop");
}