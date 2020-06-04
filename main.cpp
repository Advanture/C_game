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

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    SetConsoleTextAttribute(hConsole, 4);
    cout << "\n\t Welcome to the Quiz Game! \n\n";
    SetConsoleTextAttribute(hConsole, 0);
    menu();
    return 0;
}

void menu(){
    SetConsoleTextAttribute(hConsole, 255);
    cout << "-------------------[MENU]--------------------\n To start the game type \"/start\"!            \n To show the leaderboard type \"/leaderboard\"!\n To exit the game type \"/exit\"!              \n Good Luck!                                  \n-------------------[MENU]--------------------\n";
    SetConsoleTextAttribute(hConsole, 0);
    string input;
    while (cin >> input) {
        if (input == "/start") {
            game();
        } else if (input == "/leaderboard") {
            leaderboard();
        } else if (input == "/exit") {
            cout << "Bye!";
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

    int scores = 0;
    string answer;
    string nickname;
    nickAgain:
    cout << "Enter your nick-name:";
    cin >> nickname;
    for (auto  user : leaderboard){
        if(user["nick"] == nickname){
            SetConsoleTextAttribute(hConsole, 4);
            cout << "This name was already used" << endl;
            SetConsoleTextAttribute(hConsole, 0);
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
        SetConsoleTextAttribute(hConsole, 255);
        cout << question << endl;
        SetConsoleTextAttribute(hConsole, 0);
        for (size_t i = 0; i < 4; ++i)
        {
            string answers = questions[unused_questions[curr_question]]["answers"][i];
            cout << "\t" << i + 1 << ". " << answers << endl;
        }
        cout << "Enter the number of your answer:";
        cin >> answer;
        if (answer == questions[unused_questions[curr_question]]["correct"])
        {
            SetConsoleTextAttribute(hConsole, 2);
            cout << "Correct!" << endl;
            SetConsoleTextAttribute(hConsole, 0);
            scores++;
        }
        else
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Incorrect!" << endl;
            SetConsoleTextAttribute(hConsole, 0);
        }
        unused_questions.erase(unused_questions.begin() + curr_question);
    }
    SetConsoleTextAttribute(hConsole, 32);
    cout << "Nice! You earned " << scores << " points!" << endl;
    SetConsoleTextAttribute(hConsole, 0);

    ofstream lead("users.json");
    json user;
    user["nick"] = nickname;
    user["score"] = scores;
    leaderboard.push_back(user);
    lead << leaderboard;
    q.close();
    lead.close();

    SetConsoleTextAttribute(hConsole, 255);
    cout << "\nTo comeback to menu type \"/back\"!\n";
    SetConsoleTextAttribute(hConsole, 0);
    back_input:
    string input;
    cin >> input;
    if(input == "/back"){
        menu();
    } else {
        goto back_input;
    }
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
    SetConsoleTextAttribute(hConsole, 255);
    cout << "\nTo comeback to menu type \"/back\"!\n";
    SetConsoleTextAttribute(hConsole, 0);
    back_input:
    string input;
    cin >> input;
    if(input == "/back"){
        menu();
    } else {
        goto back_input;
    }
}