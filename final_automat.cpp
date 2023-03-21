#include <iostream>

using namespace std;

/*--------------------*/
int get_angle()
{
    int temp;
    cout << "Введи угол\n";
    cin >> temp;
    return temp;
}

int get_dist()
{
    int temp;
    cout << "Введи дистанцию\n";
    cin >> temp;
    return temp;
}

bool command_tg()
{
    bool temp;
    cout << "Есть ли команда от тг бота?\n";
    cin >> temp;
    return temp;
}

bool state_drink()
{
    bool temp;
    cout << "Напиток у робота?\n";
    cin >> temp;
    return temp;
}

void angle_and_dist(int angle, int distance)
{
    cout << "Выстраивание маршрута\n";
}

bool to_dispenser()
{
    
}
/*--------------------*/

class Sender {
    void CMD_Rotate_and_Move(int a, int d) {
        cout << "Поворот на " << a << " градусов" << endl;
        cout << "Движение на " << d << " см" << endl;
    }
    void CMD_Get_drink() {
        cout << "Получен напиток" << endl;
    }
    void CMD_Give_drink() {
        cout << "Доставлен напиток" << endl;
    }
    void CMD_Start_point() {
        cout << "Робот в стартовом состоянии" << endl;
    }
};

enum Rob_State {
    Start = 1,
    To_Dispenser = 2,
    Rotate_and_Move = 3,
    Delivery = 4,
    Go_to_start = 5
};

Rob_State state = Start;

void ProcessFiniteAutomat()
{
    switch (state)
    {
        int angle = get_angle();
        int distance = get_dist();

        case Start:
            if (command_tg())
            {
                state = To_Dispenser;
                break;
            }

        case To_Dispenser:
            CMD_Get_drink();
            if (angle > 0.1 || distance > 0.1)
            {
                state = Rotate_and_Move;
                break;
            }
            if (state_drink()) {
                state = Delivery;
                break;
            }


        case Rotate_and_Move:
        CMD_Rotate_and_Move(angle, distance);
        angle_and_dist(angle,distance);
        if (!to_dispenser()) {  // флаг был ли робот у дозатора
            state = To_Dispenser;
            break;
        }
        if (!delivery()) {  // флаг доехал ли робот до студента
            state = Delivery;
            break;
        }
        if (!go_to_start()) {  // флаг вернулся ли робот в стартовое состояние
            state = Go_to_start;
            break;
        }

        case Delivery:
        CMD_Give_drink();
        if (angle > 0.1 || distance > 0.1)
        {
            state = Rotate_and_Move;
            break;
        }
        if (!state_drink()) {
            state = Go_to_start;
            break;
        }

        case Go_to_start:
        CMD_Start_point();
        if (angle_and_dist()) {
            state = Rotate_and_Move;
        }
        state = Start;
        break;
    }
}

int main()
{

    
    return 0;
}
