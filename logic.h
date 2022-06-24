#ifndef _logic_h
#define _logic_h

#define HEIGHT 20         // All boxes are 20px tall
#define SPEED 10          // 10px/s
#define BOX_WIDTH 150     // Box is 100px long
#define X_OFFSET 20       // 20 pixels from side
#define Y_OFFSET 20       // 20 pixels off bottom
#define CIRCLE_RADIUS 10 // 10 pixels
#define MAX_WIDTH 800    // Game won't be larger than 1024 pixels
#define BLOCKS_PER_ROW 10 // 10 Blocks Per Row (BPR) for a total of 50 blocks
#define CIRCLE_SPEED 10  // Speed of the Circle

int detectCornerCollison(int circleX, int circleY, int width, int height);
int detectCollision(int *objects, int circleX, int circleY, int width, int *direction);
int detectPaddleCollision(int circleX, int circleY, int boxPosition, int height, int *direction);
int getSpeed(int width, int boxPosition, int *boxDirection);
void doNothing();

#endif