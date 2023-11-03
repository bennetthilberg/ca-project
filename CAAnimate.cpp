#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"
#include <cstdlib>
#include <ctime>

emp::web::Document doc{"target"};

class Squanimal {
    int x;
    int y;
    // x and y of Squanimal's top left part box (they're 2x2 squares)
    int direction; // 0 = up, 1 = right, 2 = down, 3 = left

  public:
    Squanimal() {
        this->x = 4;
        this->y = 4;
        this->direction = 0;
    }
    Squanimal(int x, int y) {
        this->x = x;
        this->y = y;
        this->direction = 0;
    }
    int decideWhereToGo() {

        int direction = rand() % 4;
        // 0 = up, 1 = right, 2 = down, 3 = left
        return direction;
    }
    int getX() {
        return x;
    }
    int getY() {
        return y;
    }
    int getDirection() {
        return direction;
    }
    void setX(int x) {
        this->x = x;
    }
    void setY(int y) {
        this->y = y;
    }
    void setDirection(int direction) {
        this->direction = direction;
    }
};

class CAAnimator : public emp::web::Animate {

    Squanimal dave;
    // grid width and height
    const int num_h_boxes = 10;
    const int num_w_boxes = 15;
    const double RECT_SIDE = 25;
    const double width{num_w_boxes * RECT_SIDE};
    const double height{num_h_boxes * RECT_SIDE};

    // some vectors to hold information about the CA
    std::vector<std::vector<int>> cells;

    // where we'll draw
    emp::web::Canvas canvas{width, height, "canvas"};

  public:
    CAAnimator() : dave(0, 0) {

        // shove canvas into the div
        // along with some control buttons
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");

        // fill the vectors with 0 to start
        cells.resize(num_w_boxes, std::vector<int>(num_h_boxes, 0));

        // showing how to set a cell to 'alive'
        // cells[0][0] = 1;
        // cells[1][4] = 1;
    }

    int frameCounter = 0;

    void DoFrame() override {
        canvas.Clear();

        for (int x = 0; x < num_w_boxes; x++) {
            for (int y = 0; y < num_h_boxes; y++) {

                if (cells[x][y] == 0) {
                    // Draw a rectangle on the canvas filled with white and
                    // outlined in black
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE,
                                RECT_SIDE, "white", "black");
                } else {
                    // Draw a rectangle filled with black
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE,
                                RECT_SIDE, "black", "black");
                }
            }
        }

        for (int x = 0; x < num_w_boxes; x++) {
            for (int y = 0; y < num_h_boxes; y++) {
                if (cells[x][y] == 1) {
                    cells[x][y] = 0;
                }
            }
        }

        // Reset cells to 0
        for (int x = 0; x < num_w_boxes; x++) {
            for (int y = 0; y < num_h_boxes; y++) {
                if (cells[x][y] == 1) {
                    cells[x][y] = 0;
                }
            }
        }
        // Draw dave
        cells[dave.getX()][dave.getY()] = 1;     // dave's top left square
        cells[dave.getX() + 1][dave.getY()] = 1; // dave's top right square
        cells[dave.getX()][dave.getY() + 1] = 1; // dave's bottom left square
        cells[dave.getX() + 1][dave.getY() + 1] =
            1; // dave's bottom right square

        // Move dave his decided direction
        if (frameCounter % 5 == 0) {
            if (dave.getDirection() == 0) {
                dave.setY(dave.getY() - 1);
            } else if (dave.getDirection() == 1) {
                dave.setX(dave.getX() + 1);
            } else if (dave.getDirection() == 2) {
                dave.setY(dave.getY() + 1);
            } else if (dave.getDirection() == 3) {
                dave.setX(dave.getX() - 1);
            }
        }

        // Decide dave's direction (every 10 frames)
        if (frameCounter % 12 == 0) {
            dave.setDirection(dave.decideWhereToGo());
        }

        frameCounter++;
    }
};

CAAnimator animator;

int main() {
    srand(time(0));
    // Have animator call DoFrame once to start
    animator.Step();
}