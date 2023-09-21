#include <ncurses.h>
#include <unistd.h>

#define WIDTH 25
#define LENGHT 80
#define STEP_DELAY 5000
#define MAX_DELAY 200000
#define MIN_DELAY 5000
#define START_DELAY 100000

int init();
int input(int **matrix);
void setting();
int input_from_file(int **matrix);
int input_interactive(int **matrix);
void game(int **old);
int rules_check(int **old, int i, int j);
int summa(int **old, int i, int j);
int check(int **old, int **new);
void draw_field();
void draw_stat(int step);
void draw_endgame();
void end_game();

//Главная функция
int main() {
    init();
    return 0;
}

//Старт игры
int init() {
    //Инициализация поля
    int data_field[WIDTH][LENGHT];
    int *field[WIDTH];
    for (int i = 0; i < WIDTH; i++) *(field + i) = (int *)data_field + LENGHT * i;
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < LENGHT; j++) {
            field[i][j] = 0;
        }
    }
    //Ввод с файла
    int flag = 0;
    if ((fseek(stdin, 0, SEEK_END), ftell(stdin)) > 0) {
        rewind(stdin);
        input_from_file(field);
        if (freopen("/dev/tty", "r", stdin) == NULL) flag = 1;
    }
    if (flag) {
        printf("n/a");
    } else {
        //Настройка
        setting();
        //Интерактивный ввод
        if (!input_interactive(field)) {
            curs_set(0);
            //Запуск игры
            game(field);
        }
        //Вывод окончания игры
        end_game();
    }
    return 0;
}

//Ввод с файла
int input_from_file(int **matrix) {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < LENGHT; j++) {
            scanf("%d ", &matrix[i][j]);
        }
    }
    return 0;
}

//Настройка
void setting() {
    newterm(NULL, stdout, stdin);
    noecho();
    cbreak();
    nodelay(stdscr, true);
    keypad(stdscr, true);
}

//Интерактивный ввод
int input_interactive(int **field) {
    clear();
    int curs_x = 1, curs_y = 2;
    draw_field(field);
    move(curs_y, curs_x);
    fseek(stdin, 0, SEEK_END);
    refresh();
    for (int ch = getch(); ch != 10; ch = getch()) {
        if (ch == KEY_UP && curs_y > 2)
            curs_y--;
        else if (ch == KEY_DOWN && curs_y < 26)
            curs_y++;
        else if (ch == KEY_LEFT && curs_x > 1)
            curs_x--;
        else if (ch == KEY_RIGHT && curs_x < 80)
            curs_x++;
        else if (ch == 'q')
            return -1;
        else if (ch == KEY_BACKSPACE || ch == KEY_DC || ch == 127) {
            field[curs_y - 2][curs_x - 1] = 0;
            clear();
            draw_field(field);
        } else if (ch == ' ') {
            field[curs_y - 2][curs_x - 1] = 1;
            clear();
            draw_field(field);
        }
        move(curs_y, curs_x);
        refresh();
    }
    return 0;
}

// Запуск игры
void game(int **old) {
    //Инициализация поля после шага
    int newfield[WIDTH][LENGHT];
    int *new[WIDTH];
    for (int i = 0; i < WIDTH; i++) {
        *(new + i) = (int *)newfield + LENGHT * i;
    }
    //Основной цикл
    int delay = START_DELAY;
    int step = 0;
    int end = 0;
    while (!end) {
        int ch = getch();
        if (ch == 'q') break;
        if (ch == KEY_DOWN && delay < MAX_DELAY) delay += STEP_DELAY;
        if (ch == KEY_UP && delay > MIN_DELAY) delay -= STEP_DELAY;
        flushinp();
        usleep(delay);
        //Вывод игрового поля на экран
        draw_field(old);
        //Вывод статистики на экран
        draw_stat(step);
        //Обработка шага
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < LENGHT; j++) {
                //Проверка по правилам игры
                if (rules_check(old, i, j)) {
                    new[i][j] = 1;
                } else {
                    new[i][j] = 0;
                }
            }
        }
        //Проверка на окончание игры
        end = check(old, new);
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < LENGHT; j++) {
                old[i][j] = new[i][j];
            }
        }
        step++;
    }
}

//Проверка по правилам игры
int rules_check(int **old, int i, int j) {
    //Подсчёт соседей
    int sum = summa(old, i, j);
    int flag;
    if (old[i][j] && (sum == 2 || sum == 3)) {
        flag = 1;
    } else if (!old[i][j] && sum == 3) {
        flag = 1;
    } else {
        flag = 0;
    }
    return flag;
}

//Подсчёт соседей
int summa(int **old, int i, int j) {
    int nx, ny, neighbours = 0;
    for (int n = -1; n <= 1; n++) {
        for (int m = -1; m <= 1; m++) {
            if (n == 0 && m == 0) continue;
            nx = (j + n + LENGHT) % LENGHT;
            ny = (i + m + WIDTH) % WIDTH;
            neighbours += old[ny][nx];
        }
    }
    return neighbours;
}

//Вывод поля на экран
void draw_field(int **field) {
    mvprintw(0, 31, "--- GAME OF LIFE ---");
    mvprintw(1, 0, "X");
    for (int i = 1; i < 81; i++) mvprintw(1, i, "%c", '-');
    mvprintw(1, LENGHT + 1, "X");
    int n = 1;
    while (n < 26) {
        mvprintw(n + 1, 0, "%c", '|');
        mvprintw(n + 1, 81, "%c", '|');
        n++;
    }
    mvprintw(WIDTH + 2, 0, "X");
    for (int i = 1; i < 81; i++) mvprintw(WIDTH + 2, i, "%c", '-');
    mvprintw(WIDTH + 2, LENGHT + 1, "X");
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < LENGHT; j++) {
            mvprintw(i + 2, j + 1, "%c", (field[i][j]) ? '*' : ' ');
        }
    }
    refresh();
}

//Вывод статистики на экран
void draw_stat(int step) { mvprintw(WIDTH + 3, 0, "Generation: %d", step); }

//Проверка на окончание игры
int check(int **old, int **new) {
    int flag = 0, status = 1;
    for (int i = 0; i < WIDTH && !flag; i++) {
        for (int j = 0; j < LENGHT; j++) {
            if (old[i][j] != new[i][j]) {
                flag = 1;
            }
        }
    }
    if (flag) status = 0;
    return status;
}

//Настройка
void end_game() {
    curs_set(0);
    nodelay(stdscr, false);
    draw_endgame();
    getch();
    endwin();
    setvbuf(stdout, NULL, _IOLBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

//Вывод окончания игры
void draw_endgame() {
    mvprintw(11, 30, "***********************");
    mvprintw(12, 30, "*                     *");
    mvprintw(13, 30, "*      GAME OVER      *");
    mvprintw(14, 30, "*                     *");
    mvprintw(15, 30, "***********************");
}
