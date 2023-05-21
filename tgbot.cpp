#include <iostream>	
#include <unistd.h>
#include <vector>

using namespace std;


//---------------- состояния робота ----------------//
enum class Rob_State {
    Start = 0,
    To_Dispenser = 1,
    To_Student = 2,
};

//---------------- класс команд, отправляемых роботу ----------------//

class My_Sender {
public:
    My_Sender() {}

    ~My_Sender() {}

    vector<vector<double>> angles_dists()
    {
        vector<vector<double>> all_data;
        cout << "Введите угол поворота и расстояние до места старта\n";
        double a, b; cin >> a >> b;
        vector<double> temp ={a, b};
        all_data.push_back(temp);
        cout << "Введите угол поворота и расстояние до дозатора\n";
        cin >> a >> b;
        temp ={a, b};
        all_data.push_back(temp);
        cout << "Введите угол поворота и расстояние до студента\n";
        cin >> a >> b;
        temp ={a, b};
        all_data.push_back(temp);
        return all_data;
    }

    bool command_tg()
    {
        bool temp;
        cout << "\nЗапущена ли доставка в тг-боте?\n";
        cin >> temp;
        return temp;
    }
};

int main()
{
    Rob_State state = Rob_State::Start;
    My_Sender sender;
    while (sender.command_tg())
    {
        switch (state)
        {
        case Rob_State::Start:
        {
            vector <vector<double>> current_data = sender.angles_dists();
            while (abs(current_data[1][0]) > 0.1 || current_data[1][1] > 0.1)  // угол и расст
            {
                current_data = sender.angles_dists();
                cout << "Робот едет к дозатору!\n";
            }
            cout << "Робот доехал до дозатора!\n";
            state = Rob_State::To_Dispenser;
        }
        case Rob_State::To_Dispenser:
        {
            cout << "Робот ждет приготовление напитка! Это займет 20 секунд.\n";
            sleep(5);
            vector <vector<double>> current_data = sender.angles_dists();
            while (abs(current_data[2][0]) > 0.1 || current_data[2][1] > 0.1)
            {
                current_data = sender.angles_dists();
                cout << "Робот едет к студенту!\n";
            }
            cout << "Робот доехал до студента!\n";
            state = Rob_State::To_Student;
        }
        case Rob_State::To_Student:
        {
            cout << "Робот ждет 20 секунд, пока вы заберете напиток!\n";
            sleep(5);
            vector <vector<double>> current_data = sender.angles_dists();
            while (abs(current_data[0][0]) > 0.1 || current_data[0][1] > 0.1)
            {
                current_data = sender.angles_dists();
                cout << "Робот едет к месту старта!\n";
            }
            cout << "Робот вернулся в место старта!\n";
            state = Rob_State::Start;
        }
        }
    }
    return 0;
}
