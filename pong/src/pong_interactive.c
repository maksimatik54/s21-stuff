#include <ncurses.h>
#include <unistd.h>

void drawField(int fieldX, int fieldY, int fieldWidth, int fieldHeight);
void drawBall(int fieldX, int fieldY, int ballX, int ballY);
void drawRackets(int fieldX, int fieldY, int fieldWidth, int leftRacket, int rigthRacket);
void drawScore(int fieldX, int fieldY, int player1Score, int player2Score);

int main() {
    // Field parameters

    int fieldX = 0, fieldY = 0;
    int fieldWidth = 80, fieldHeight = 25;

    // Ball parameters

    int ballX = 38, ballY = 12;
    int speedX = 1, speedY = 0;

    // Rackets

    int leftRacket = 12;
    int rightRacket = 12;

    // Players scores

    int player1Score = 0;
    int player2Score = 0;

    initscr();  // init of ncurses and its tools

    noecho();  // block keyboard output to screen

    nodelay(stdscr, TRUE);  // non-blocking getch() means if there is no input
                            // getch() will return ERR and doesn't wait for input

    curs_set(0);  // hide cursor

    // Game loop

    while (player1Score < 21 && player2Score < 21) {
        clear();  // clears screen

        // Controls

        int c = getch();
        if (c == 'a' && leftRacket > 2) {
            leftRacket--;
        } else if (c == 'z' && leftRacket < 22) {
            leftRacket++;
        } else if (c == 'k' && rightRacket > 2) {
            rightRacket--;
        } else if (c == 'm' && rightRacket < 22) {
            rightRacket++;
        } else if (c == 'q') {
            break;
        }

        // Score check

        if (ballX == 1 || ballX == 78) {
            if (ballX == 1) {
                player2Score += 1;
                speedX = -1;
            }

            if (ballX == 78) {
                player1Score += 1;
                speedX = -1;
            }

            // Returning to initial position

            ballX = 38;
            ballY = 12;
            speedY = 0;
            leftRacket = 12;
            rightRacket = 12;
        }

        // Racket collisions

        if ((ballX == 3 && ballY - leftRacket >= -2 && ballY - leftRacket <= 2) ||
            (ballX == 76 && ballY - rightRacket >= -2 && ballY - rightRacket <= 2)) {
            // Right racket collisions

            if (ballX == 76) {
                if (ballY == rightRacket - 1 || (ballY == rightRacket - 2 && speedY == 1)) {
                    speedX = -speedX;
                    speedY = -1;
                } else if (ballY == rightRacket) {
                    speedX = -speedX;
                    speedY = 0;
                } else if (ballY == rightRacket + 1 || (ballY == rightRacket + 2 && speedY == -1)) {
                    speedX = -speedX;
                    speedY = 1;
                }
            }

            // Left racket collisions

            if (ballX == 3) {
                if (ballY == leftRacket - 1 || (ballY == leftRacket - 2 && speedY == 1)) {
                    speedX = -speedX;
                    speedY = -1;
                } else if (ballY == leftRacket) {
                    speedX = -speedX;
                    speedY = 0;
                } else if (ballY == leftRacket + 1 || (ballY == leftRacket + 2 && speedY == -1)) {
                    speedX = -speedX;
                    speedY = 1;
                }
            }
        }

        // Top and bottom borders collisions

        if ((ballY == 1 && speedY < 0) || (ballY == 23 && speedY > 0)) {
            speedY = -speedY;
        }

        // Ball movements

        ballX += speedX;
        ballY += speedY;

        // Graphics is implemented via separate functions

        drawField(fieldX, fieldY, fieldWidth, fieldHeight);
        drawBall(fieldX, fieldY, ballX, ballY);
        drawRackets(fieldX, fieldY, fieldWidth, leftRacket, rightRacket);
        drawScore(fieldX, fieldY, player1Score, player2Score);

        refresh();      // outputs all mvprintws
        flushinp();     // flushes input buffer
        usleep(75000);  // game tick
    }

    endwin();  // end ncurses routines and clears its structures

    // Congratulations

    if (player1Score == 21) printf("Player 1 won! Congratulations!");
    if (player2Score == 21) printf("Player 2 won! Congratulations!");

    return 0;
}

void drawField(int fieldX, int fieldY, int fieldWidth, int fieldHeight) {
    char* string = "------------------------------------------------------------------------------";

    mvprintw(fieldY, fieldX + 1, "%s", string);
    int n = 0;
    while (n < fieldHeight - 2) {
        mvprintw(fieldY + 1 + n, fieldX, "%c", '|');
        mvprintw(fieldY + 1 + n, fieldX + fieldWidth - 1, "%c", '|');
        n++;
    }
    mvprintw(fieldY + fieldHeight - 1, fieldX + 1, "%s", string);
}

void drawBall(int fieldX, int fieldY, int ballX, int ballY) {
    mvprintw(fieldY + ballY, fieldX + ballX, "%c", '@');
}

void drawRackets(int fieldX, int fieldY, int fieldWidth, int leftRacket, int rightRacket) {
    mvprintw(fieldY + leftRacket, fieldX + 2, "%s", "H");
    mvprintw(fieldY + leftRacket + 1, fieldX + 2, "%s", "H");
    mvprintw(fieldY + leftRacket - 1, fieldX + 2, "%s", "H");

    mvprintw(fieldY + rightRacket, fieldX + fieldWidth - 3, "%s", "H");
    mvprintw(fieldY + rightRacket + 1, fieldX + fieldWidth - 3, "%s", "H");
    mvprintw(fieldY + rightRacket - 1, fieldX + fieldWidth - 3, "%s", "H");
}

void drawScore(int fieldX, int fieldY, int player1Score, int player2Score) {
    char* string = "------------------------------------------------------------------------------";
    mvprintw(fieldY, fieldX + 1, "%s", string);
    mvprintw(fieldY, fieldX + 35, " %02i : %02i ", player1Score, player2Score);
}
