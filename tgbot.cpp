#include <iostream>
#include <cstring>
#include <tgbot/tgbot.h>
#include <mosquitto.h>

using namespace std;

const string BOT_TOKEN = "6092622281:AAEzaSMdLB4LaKGzP23sEmAdfTYgL6IqeKc";
const string MQTT_TOPIC = "tgbot_topic";
const long long CHAT_ID = 1372293216;
const string MQTT_SERVER_ADDRESS = "localhost";
const int MQTT_SERVER_PORT = 1883;

struct mosquitto* mosq = mosquitto_new(NULL, true, NULL);

void on_message(struct mosquitto* mosq, void* userdata, const struct mosquitto_message* message) {
    cout << (char*)message->payload << endl;
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

class My_Sender {
public:
    My_Sender() {

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
    ~My_Sender() {

    }
};

//---------------- класс выводимых сообщений ----------------//

class My_Messages {
public:
    My_Messages() {

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
    ~My_Messages() {

    }
};

void ProcessFiniteAutomat(My_Messages& message, My_Sender& send)
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
            
                state = Rob_State::To_Dispenser;
                break;
            
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

My_Messages my_message;
My_Sender my_send;

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Rus");
    
    

    TgBot::Bot bot(BOT_TOKEN);
    mosquitto_lib_init();
    mosquitto_connect(mosq, MQTT_SERVER_ADDRESS.c_str(), MQTT_SERVER_PORT, 0);
    mosquitto_subscribe(mosq, NULL, MQTT_TOPIC.c_str(), 0);
   
    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        
        if (message->text == "start") {
            bot.getApi().sendMessage(message->chat->id, "Hi! I'm Petya, delivering beer and troubles. I'm coming to you with a drink");
            while(true)ProcessFiniteAutomat(my_message, my_send);
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "You've written " + message->text + ", but I don't care");
     
        mosquitto_publish(mosq, NULL, MQTT_TOPIC.c_str(), message->text.length(), message->text.c_str(), 0, false);
       
        });
    
    try {
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            longPoll.start();
        }
    }
    catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    
    // Запускаем телеграм-бота
    bot.getApi().deleteWebhook();
   
    // Отключаемся от брокера и освобождаем ресурсы
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;



}
