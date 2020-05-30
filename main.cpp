#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <windows.h>
#include "json.hpp"
#include <ctime>

using namespace std;
using json = nlohmann::json;

void menu();
void game();
void leaderboard();

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    menu();
    return 0;
}

void menu(){
    string input;
    while (cin >> input) {
        if (input == "/start") {
            game();
        } else if (input == "/leaderboard") {
            leaderboard();
        } else if (input == "/exit") {
            exit(1);
        }
    }
}

void game()
{
    ifstream q("questions.json");
    fstream l("users.json");
    json questions;
    json leaderboard;
    l >> leaderboard;
    q >> questions;
    l.close();

    int progress = 0;
    int scores = 0;
    string answer;
    string nickname;
    nickAgain:
    cout << "Введите ник-нейм:";
    cin >> nickname;
    for (auto  user : leaderboard){
        if(user["nick"] == nickname){
            cout << "Данный ник уже был использован" << endl;
            goto nickAgain;
        }
    }

    vector <int> unused_questions(questions.size());

    for(int i = 0; i < questions.size(); i++){
        unused_questions[i] = i;
    }

    while (!unused_questions.empty())
    {
        int curr_question = rand() % unused_questions.size();

        string question = questions[unused_questions[curr_question]]["question"];
        cout << question << endl;
        for (size_t i = 0; i < 4; ++i)
        {
            string answers = questions[unused_questions[curr_question]]["answers"][i];
            cout << "\t" << i + 1 << ". " << answers << endl;
        }
        cout << "Введите номер ответа:";
        cin >> answer;
        if (answer == questions[unused_questions[curr_question]]["correct"])
        {
            cout << "Правильно!" << endl;
            scores++;
        }
        else
        {
            cout << "Не правильно!" << endl;
        }
        unused_questions.erase(unused_questions.begin() + curr_question);
    }
    cout << "Отлично! Вы заработали " << scores << " очков!" << endl;

    ofstream lead("users.json");
    json user;
    user["nick"] = nickname;
    user["score"] = scores;
    leaderboard.push_back(user);
    lead << leaderboard;
    q.close();
    lead.close();
}

void leaderboard()
{
    fstream l("users.json");
    json leaderboard;
    l >> leaderboard;
    vector <pair<string, int>> lb;
    for (size_t i = 0; i < leaderboard.size(); ++i)
    {
        lb.push_back(make_pair(leaderboard[i]["nick"], leaderboard[i]["score"]));
    }
    sort(lb.begin(), lb.end(), [](auto &left, auto &right) {
        return left.second > right.second;
    });
    for (size_t i = 0; i < lb.size(); ++i)
    {
        cout << "#" << i + 1 << " - " << lb[i].first << " - " << lb[i].second << endl;
    }
    l.close();
}