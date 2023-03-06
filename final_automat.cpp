#include <iostream>

using namespace std;

class Sender {
    void CMD_Rotate_and_Move(int a, int d) {
        cout << "������� �� " << a << "��������" << endl;
        cout << "�������� �� " << d << "��" << endl;
    }
    void CMD_Get_drink() {
        cout << "������� �������" << endl;
    }
    void CMD_Give_drink() {
        cout << "������� ���������" << endl;
    }
    void CMD_Start_point() {
        cout << "����� � ��������� ���������" << endl;
    }
};

enum Rob_State {
    Start,
    To_Dispenser,
    Rotate_and_Move,
    Delivery,
    Go_to_start
}

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
        if (!to_dispenser()) {
            state = To_Dispenser;
            break;
        }
        if (!delivery()) {
            state = Delivery;
            break;
        }
        if (!go_to_start()) {
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