#include <iostream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <mosquitto.h>
#include <json-c/json.h>
#include <pthread.h>

using namespace std;



//---------------- методы mosquitto_sub ----------------//
void on_connect(struct mosquitto* mosq, void* obj, int rc) {
    printf("ID: %d\n", *(int*)obj);
    if (rc) {
        printf("Error with result code: %d\n", rc);
        exit(-1);
    }
    mosquitto_subscribe(mosq, NULL, "test/t1", 0);
}

void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg) {
    printf("New message with topic %s: %s\n", msg->topic, (char*)msg->payload);
}
//---------------- состояния робота ----------------//

enum class Rob_State {
    Start = 0,
    To_Dispenser = 1,
    Rotate_and_Move = 2,
    Delivery = 3,
    Go_to_start = 4
};

Rob_State state = Rob_State::Start;

//---------------- класс команд, отправляемых роботу ----------------//

class Sender {
public:
    Sender() {

    }
    
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
        cout << "Был ли робот у дозатора?\n";
        bool temp;
        cin >> temp;
        return temp;
    }

    bool delivery()
    {
        cout << "Был ли робот у студента?\n";
        bool temp;
        cin >> temp;
        return temp;
    }

    bool go_to_start()
    {
        cout << "Вернулся ли робот в стартовое положение?\n";
        bool temp;
        cin >> temp;
        return temp;
    }
    ~Sender() {

    }
};

//---------------- класс выводимых сообщений ----------------//

class Messages {
public:
    Messages() {

    }
    void CMD_Rotate_and_Move(int a, int d) {
        cout << "Поворот на " << a << " градусов" << endl;
        cout << "Движение на " << d << " см" << endl;
    }
    void CMD_Get_drink() {
        cout << "Получение напитка" << endl;
    }
    void CMD_Give_drink() {
        cout << "Доставка напитка" << endl;
    }
    void CMD_Start_point() {
        cout << "Робот в стартовом состоянии" << endl;
    }
    ~Messages() {

    }
};

void ProcessFiniteAutomat(Messages & message, Sender & send)
{
    
    switch (state)
    {

    case Rob_State::Start:
    {
        cout << "Start (если хотите выйти - нажмите клавишу '0', продолжить - любую цифру)\n";
        int put; cin >> put;
        switch (put) {
        case 0:
            exit(0);
        default:
            if (send.command_tg())
            {
                state = Rob_State::To_Dispenser;
                break;
            }
            else {
                state = Rob_State::Start;
                break;
            }
        }
        
    }

    case Rob_State::Delivery:
    {
        cout << "Delivery\n";
        message.CMD_Give_drink();
        if (send.get_angle() > 0.1 || send.get_dist() > 0.1)
        {
            cout << "Условие движения\n";
            state = Rob_State::Rotate_and_Move;
            break;
        }
        cout << "Отдает напиток\n";
        if (!send.state_drink()) {
            cout << "Условие к старту\n";
            state = Rob_State::Go_to_start;
            break;
        }
        else {
            state = Rob_State::Delivery;
            break;
        }
    }

    case Rob_State::To_Dispenser:
    {
        cout << "To_Dispencer\n";
        message.CMD_Get_drink();
        if (send.get_angle() > 0.1 || send.get_dist() > 0.1)
        {
            cout << "Условие движения\n";
            state = Rob_State::Rotate_and_Move;
            break;
        }
        cout << "Берет напиток\n";
        if (send.state_drink()) {
            cout << "Условие доставки\n";
            state = Rob_State::Delivery;
            break;
        }
        else {
            state = Rob_State::To_Dispenser;
            break;
        }
    }

    case Rob_State::Rotate_and_Move:
    {
        cout << "Rotate_and_Move\n";
        message.CMD_Rotate_and_Move(send.get_angle(), send.get_dist());
        send.angle_and_dist(send.get_angle(), send.get_dist());
        if (!send.to_dispenser()) {  // флаг был ли робот у дозатора
            cout << "Условие к диспенсеру\n";
            state = Rob_State::To_Dispenser;
            break;
        }
       if (!send.delivery()) {  // флаг доехал ли робот до студента
            cout << "Условие доставки\n";
            state = Rob_State::Delivery;
            break;
        }
        if (!send.go_to_start()) {  // флаг вернулся ли робот в стартовое состояние
            cout << "Условие к старту\n";
            state = Rob_State::Go_to_start;
            break;
        }
    }

    case Rob_State::Go_to_start:
    {
        cout << "Go_to_start\n";
        message.CMD_Start_point();
        if (send.get_angle() > 0.1 || send.get_dist() > 0.1) {
            cout << "Условие движения\n";
            state = Rob_State::Rotate_and_Move;
            break;
        } 
        state = Rob_State::Start;
        break;
    }
    }
   
}

int main()
{
    setlocale(LC_ALL, "Rus");
    Messages message;
    Sender send;
    while(1)ProcessFiniteAutomat(message, send);

//---------------- работа mosquitto_sub  ----------------//
    mosquitto_lib_init();
    
    return 0;

}
