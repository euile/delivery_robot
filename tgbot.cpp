#include "tgbot.h"

mosq = mosquitto_new(NULL, true, NULL);

flag = false;
password_flag = false;
password;

first = false;

// ОТПРАВКА КОМАНДЫ РОБОТУ

void message(double angle, double dist) {
    angle = round(angle * 10);
    dist = round(dist * 10);
    string str_angle = to_string(angle);
    string str_dist = to_string(dist);

    string cmd;

    if (angle >= 0) {
        cmd = "{\"cmd\":\"right\",\"val\":" + str_angle + ", \"spd\":0.6}";
        mosquitto_publish(mosq, NULL, MQTT_TOPIC.c_str(), cmd.length(), cmd.c_str(), 0, false);
        cmd = "{\"cmd\":\"forward\",\"val\":" + str_dist + ", \"spd\":0.6}";
        mosquitto_publish(mosq, NULL, MQTT_TOPIC.c_str(), cmd.length(), cmd.c_str(), 0, false);
    }

    if (angle < 0) {
        cmd = "{\"cmd\":\"left\",\"val\":" + str_angle + ", \"spd\":0.6}";
        mosquitto_publish(mosq, NULL, MQTT_TOPIC.c_str(), cmd.length(), cmd.c_str(), 0, false);
        cmd = "{\"cmd\":\"forward\",\"val\":" + str_dist + ", \"spd\":0.6}";
        mosquitto_publish(mosq, NULL, MQTT_TOPIC.c_str(), cmd.length(), cmd.c_str(), 0, false);
    }
}


enum class Rob_State {
    Start = 0,
    To_Dispenser = 1,
    To_Student = 2,
};

state = Rob_State::Start;

void ProcessFiniteAutomat(TgBot::Bot& bot)
{
    switch (state)
    {
    case Rob_State::Start:
    {
        vector <vector<double>> current_data = coords();
        while (abs(current_data[1][0]) > 0.1 || current_data[1][1] > 0.1)  // угол и расст
        {
            current_data = coords();
            message(current_data[1][0], current_data[1][1]);
            bot.getApi().sendMessage(CHAT_ID, "I'm going to the dispenser");

        }
        bot.getApi().sendMessage(CHAT_ID, "Arrived at the dispenser");
        state = Rob_State::To_Dispenser;
    }
    case Rob_State::To_Dispenser:
    {
        bot.getApi().sendMessage(CHAT_ID, "Waiting for the preparation of the drink! This will take 20 seconds");
        Sleep(5000);
        vector <vector<double>> current_data = coords();
        while (abs(current_data[2][0]) > 0.1 || current_data[2][1] > 0.1)
        {
            current_data = coords();
            message(current_data[2][0], current_data[2][1]);
            bot.getApi().sendMessage(CHAT_ID, "Going to the student");
        }
        bot.getApi().sendMessage(CHAT_ID, "Got to the student");
        state = Rob_State::To_Student;
    }
    case Rob_State::To_Student:
    {
        bot.getApi().sendMessage(CHAT_ID, "Waiting 20 seconds for you to pick up the drink");
        Sleep(5000);
        vector <vector<double>> current_data = coords();
        while (abs(current_data[0][0]) > 0.1 || current_data[0][1] > 0.1)
        {
            current_data = coords();
            message(current_data[0][0], current_data[0][1]);
            bot.getApi().sendMessage(CHAT_ID, "Going to the start point");

        }
        bot.getApi().sendMessage(CHAT_ID, "Returned to the starting point");
        state = Rob_State::Start;
    }
    }
}


void robot_launch()
{
    setlocale(LC_ALL, "Rus");

    TgBot::Bot bot(BOT_TOKEN);

    mosquitto_lib_init();
    mosquitto_connect(mosq, MQTT_SERVER_ADDRESS.c_str(), MQTT_SERVER_PORT, 0);
    mosquitto_subscribe(mosq, NULL, MQTT_TOPIC.c_str(), 0);

    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {


        if (!first) {
            CHAT_ID = message->chat->id;
            bot.getApi().sendMessage(CHAT_ID, "Hi! I'm robot Petya, delivering beer and troubles. Write password");
        }

        if (password_flag) {
            if (message->text == "start") {
                flag = true;
            }
            else {
                bot.getApi().sendMessage(CHAT_ID, "You've written " + message->text + ", but I don't care");

            }
        }

        if (message->text == password) {
            password_flag = true;
            bot.getApi().sendMessage(CHAT_ID, "Password is correct");
        }


        if (first && !password_flag) {
            bot.getApi().sendMessage(CHAT_ID, "Password is wrong!");
        }
        first = true;

        });

    while (true) {
        try {
            TgBot::TgLongPoll longPoll(bot);
            while (!flag) {
                longPoll.start();

            }
            ProcessFiniteAutomat(bot);
        }
        catch (TgBot::TgException& e) {
            printf("error: %s\n", e.what());
        }
    }
    bot.getApi().deleteWebhook();

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    
}
