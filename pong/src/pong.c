#include <stdio.h>

void drawField(int scoreL, int scoreR, int ballX, int ballY, int playerLY, int playerRY);
void clearField();
int RacketsMovement(int playerY, char input);

int main() {
    int playerLY = 11, playerRY = 11;
    int scoreL = 0, scoreR = 0;
    int ballX = 4, ballY = 15;
    int speedX = -1, speedY = -1;
    char c;
    while (scoreL <= 21 && scoreR <= 21) {
        clearField();
        drawField(scoreL, scoreR, ballX, ballY, playerLY, playerRY);

        if (scoreL == 21 || scoreR == 21) break;
        if (scoreL < 21 && scoreR < 21) {
            char inputL, inputR;

            do {
                fseek(stdin, 0, SEEK_END);
                scanf("%c%c%c", &inputL, &inputR, &c);
                if (c != '\n') inputL = 0;

            } while ((inputL != 32 && inputL != 97 && inputL != 122) ||
                     (inputR != 32 && inputR != 109 && inputR != 107));

            playerLY = RacketsMovement(playerLY, inputL);
            playerRY = RacketsMovement(playerRY, inputR);

            if (ballX == 1 || ballX == 78) {
                if (ballX == 1) {
                    scoreR += 1;
                    speedX = 1;
                }

                if (ballX == 78) {
                    scoreL += 1;
                    speedX = -1;
                }
                ballX = 40;
                ballY = 13;
                speedY = 0;
                playerLY = 13;
                playerRY = 13;
            }

            if ((ballX == 3 && ballY - playerLY >= -1 && ballY - playerLY <= 3) ||
                (ballX == 76 && ballY - playerRY >= -1 && ballY - playerRY <= 3)) {
                if (ballX == 76) {
                    if (ballY == playerRY || (ballY == playerRY - 1 && speedY == 1)) {
                        speedX = -speedX;
                        speedY = -1;
                    } else if (ballY == playerRY + 1) {
                        speedX = -speedX;
                        speedY = 0;
                    } else if (ballY == playerRY + 2 || (ballY == playerRY + 3 && speedY == -1)) {
                        speedX = -speedX;
                        speedY = 1;
                    }
                }
                if (ballX == 3) {
                    if (ballY == playerLY || (ballY == playerLY - 1 && speedY == 1)) {
                        speedX = -speedX;
                        speedY = -1;
                    } else if (ballY == playerLY + 1) {
                        speedX = -speedX;
                        speedY = 0;
                    } else if (ballY == playerLY + 2 || (ballY == playerLY + 3 && speedY == -1)) {
                        speedX = -speedX;
                        speedY = 1;
                    }
                }
            }

            if ((ballY == 1 && speedY < 0) || (ballY == 23 && speedY > 0)) {
                speedY = -speedY;
            }

            ballX += speedX;
            ballY += speedY;
        }
    }
    if (scoreL == 21) printf("Player 1 win!");
    if (scoreR == 21) printf("Player 2 win!");

    return 0;
}

int RacketsMovement(int playerY, char input) {
    if (input < 97) input += 32;

    if ((input == 'a') && playerY > 1) {
        playerY--;
    } else if ((input == 'z') && playerY < 21) {
        playerY++;
    }

    if ((input == 'k') && playerY > 1) {
        playerY--;
    } else if ((input == 'm') && playerY < 21) {
        playerY++;
    }

    return playerY;
}

void drawField(int scoreL, int scoreR, int ballX, int ballY, int playerLY, int playerRY) {
    int height = 25, width = 80, racketSize = 3;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == width / 2 && y == 0) {
                printf("%d:%d", scoreL, scoreR);
                if (scoreL > 9 && scoreR > 9)
                    x += 5;
                else if ((scoreL < 10 && scoreR > 9) || (scoreR < 10 && scoreL > 9))
                    x += 4;
                else if (scoreL < 10 && scoreR < 10)
                    x += 3;
            }
            if ((x == 0 || x == width - 1) && (y != 0 && y != height - 1)) {
                printf("|");
            } else if ((y == 0 || y == height - 1) && (x != 0 && x != width - 1)) {
                printf("-");
            } else if (x == ballX && y == ballY) {
                printf("@");
            } else if ((x == 2) && (y >= playerLY && y <= playerLY + racketSize - 1)) {
                printf("H");
            } else if ((x == width - 3) && (y >= playerRY && y <= playerRY + racketSize - 1)) {
                printf("H");
            } else
                printf(" ");
        }
        printf("\n");
    }
}

void clearField() {
    printf("\033[2J");
    printf("\033[H");
}
