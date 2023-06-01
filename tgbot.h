#pragma once
#include <cstring>
#include <tgbot/tgbot.h>
#include <mosquitto.h>
#include <Windows.h>
#include "route.h"

const string BOT_TOKEN;
const string MQTT_TOPIC;
long long CHAT_ID;
const string MQTT_SERVER_ADDRESS;
const int MQTT_SERVER_PORT;

struct mosquitto* mosq;

bool flag;
bool password_flag;
string password;
bool first;
void message(double angle, double dist);
enum class Rob_State;
Rob_State state;
void ProcessFiniteAutomat(TgBot::Bot& bot);
void robot_launch();