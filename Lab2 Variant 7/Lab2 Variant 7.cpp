#include <iostream>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <locale>;
#include "windows.h";

using namespace std;

class FiniteAutomaton {
public:
    FiniteAutomaton() {}

    void ReadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Помилка відкриття файлу " << filename << endl;
            return;
        }

        string line;
        ReadAlphabet(file);
        ReadStates(file);
        ReadInitialState(file);
        ReadFinalStates(file);
        while (getline(file, line)) {
            ReadTransition(line);
        }

        file.close();
    }

    void AddTransition(int fromState, char inputSymbol, int toState) {
        transitions_[make_pair(fromState, inputSymbol)] = toState;
    }

    bool ProcessInput(const string& input) {
        currentState = initialState;

        for (char symbol : input) {
            if (transitions_.count(make_pair(currentState, symbol)) == 0) {
                return false;
            }
            currentState = transitions_[make_pair(currentState, symbol)];
        }

        return finalStates.count(currentState) > 0;
    }

    bool CheckWord(const string& w0) {
        for (int state : states) {
            currentState = state;
            for (char s1 : alphabet) {
                for (char s2 : alphabet) {
                    bool ok = true;
                    string w = s1 + w0 + s2;
                    for (char symbol : w) {
                        if (transitions_.count(make_pair(currentState, symbol)) == 0) {
                            ok = false;
                            break;
                        }
                        currentState = transitions_[make_pair(currentState, symbol)];
                    }
                    if (ok && finalStates.count(currentState) > 0) {
                        cout << "Автомат допускає слово w = w1 + w0 + w2 = " << w;
                        return true;
                    }
                }
            }
        }
        return false;
    }

private:
    set<char> alphabet;
    set<int> states;
    int initialState;
    set<int> finalStates;
    int currentState;
    map<pair<int, char>, int> transitions_;

    void ReadAlphabet(ifstream& file) {
        string line;
        getline(file, line);
        stringstream ss(line);
        char symbol;
        while (ss >> symbol) {
            if (symbol != ' ') {
                alphabet.insert(symbol);
            }
        }
    }

    void ReadStates(ifstream& file) {
        string line;
        getline(file, line);
        stringstream ss(line);
        int state;
        while (ss >> state) {
            states.insert(state);
        }
    }

    void ReadInitialState(ifstream& file) {
        file >> initialState;
    }

    void ReadFinalStates(ifstream& file) {
        int numFinalStates;
        file >> numFinalStates;
        for (int i = 0; i < numFinalStates; ++i) {
            int finalState;
            file >> finalState;
            finalStates.insert(finalState);
        }
        string line;
        getline(file, line);
    }

    void ReadTransition(const string& line) {
        int fromState, toState;
        char inputSymbol;
        stringstream ss(line);
        ss >> fromState >> inputSymbol >> toState;
        transitions_[make_pair(fromState, inputSymbol)] = toState;
    }
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_CTYPE, "ukr");

    FiniteAutomaton automaton;

    automaton.ReadFromFile("input.txt");

    string w0;
    cout << "Введіть слово w0: "; /// a
    cin >> w0;

    try {
        if (!automaton.CheckWord(w0)) {
            cout << "Автомат не допускає слово w = w1 + w0 + w2";
        }
    }
    catch (const char* exception)
    {
        std::cerr << "Error: " << exception << '\n';
    }
    cout << endl;
    system("pause");
    return 0;
}
