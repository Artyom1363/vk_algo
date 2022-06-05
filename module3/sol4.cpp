/*
Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду:
[ 1  2  3  4 ]
[ 5  6  7  8 ]
[ 9  10 11 12] 
[ 13 14 15 0 ]
где 0 задает пустую ячейку.
Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.
Формат входных данных
Начальная расстановка.
Формат выходных данных
Если решение существует, то в первой строке выходного файла выведите минимальное число перемещений костяшек, которое нужно сделать, чтобы достичь выигрышной конфигурации, а во второй строке выведите соответствующую последовательность ходов: L означает, что костяшка сдвинулась влево, R – вправо, U – вверх, D – вниз. Если таких последовательностей несколько, то выведите любую из них. Если же выигрышная конфигурация недостижима, то выведите в выходной файл одно число −1.

*/


#include <iostream>
#include <array>
#include <cassert>
#include <unordered_map>
#include <queue>
#include <cstring>
#include <algorithm>
#include <set>


using std::cin;
using std::cout;
using std::endl;
using std::set;
using std::pair;
using std::string;
using std::array;

const unsigned int sideSize = 4;
const char FieldSize = sideSize * sideSize;
const std::array<char, FieldSize> finishField = {1, 2, 3, 4, 
                                                 5, 6, 7, 8, 
                                                 9, 10, 11, 12,
                                                 13, 14, 15, 0};

int COUNTER = 0;
int ITER_NUMBER = 1000000;

class GameState {
public:
    GameState(const std::array<char, FieldSize> &field);
    
    GameState(const GameState &other);
    bool IsSolvable() const;
    bool IsComplete() const;
    bool CanMoveLeft() const;
    bool CanMoveRight() const;
    bool CanMoveUp() const;
    bool CanMoveDown() const;

    GameState MoveLeft() const;
    GameState MoveRight() const;
    GameState MoveUp() const;
    GameState MoveDown() const;
    
    size_t CalcDist() const;

    size_t GetEmptyPos() const { return emptyPos; } 

    bool operator==(const GameState &other) const;

    size_t SumManhatDist() const {
        size_t dist = 0;
        for (int i = 0; i < FieldSize; ++i) {
            if (field[i] && field[i] != i + 1) {
                dist += FindManhatDist(i, field[i] - 1);
            }
        }
        return dist;
    }
    
private:
    size_t getInvCount() const {
        size_t invCount = 0;
        for (int i = 0; i < FieldSize - 1; i++) {
            for (int j = i + 1; j < FieldSize; j++) {
                if (field[i] && field[j] && field[i] > field[j])
                    invCount++;
            }
        }
        return invCount;
    }

    size_t FindFirstWrong() const {
        for (int i = 0; i < FieldSize; ++i) {
            if (field[i] != i + 1) {
                return i + 1;
            }
        }
        return 0;
    }

    size_t GetPosOfElem(size_t elem) const {
        for (int i = 0; i < FieldSize; ++i) {
            if (field[i] == elem) {
                return i;
            }
        }
        return 0;
    }

    size_t FindManhatDist(size_t first, size_t second) const {
        size_t x1 = first / sideSize, y1 = first % sideSize;
        size_t x2 = second / sideSize, y2 = second % sideSize;
        return abs(x1 - x2) + abs(y1 - y2);
    }


        
    std::array<char, FieldSize> field;
    char emptyPos;
    
    friend struct GameStateHasher;
    friend std::ostream& operator<<(std::ostream &out, const GameState &state);
};

std::ostream& operator<<(std::ostream &out, const std::array<char, FieldSize> &arr) {
    for (int i = 0; i < sideSize; i++) {
        for (int j = 0; j < sideSize; j++) {
            out << static_cast<int>(arr[i * sideSize + j]) << " ";
        }
        out << std::endl;
    }
    return out;
}

GameState::GameState(const std::array<char, FieldSize> &field) : field(field){
    emptyPos = -1;
    for (int i = 0; i < FieldSize; i++) {
        if (field[i] == 0)
            emptyPos = i;
    }
    assert(emptyPos != -1);
}

GameState::GameState(const GameState &other) : 
    field(other.field), emptyPos(other.emptyPos) {}

bool GameState::IsSolvable() const {
    size_t invCount = getInvCount();
    unsigned int numberOfStr = emptyPos / sideSize + 1;
    return ((invCount + numberOfStr) % 2 == 0) ? true : false;
}

bool GameState::IsComplete() const {
    return field == finishField;
}

bool GameState::CanMoveLeft() const {
    return emptyPos % sideSize != 3;
}

bool GameState::CanMoveRight() const {
    return emptyPos % sideSize != 0;
}

bool GameState::CanMoveUp() const {
    return emptyPos < 12;
}

bool GameState::CanMoveDown() const {
    return emptyPos > 3;
}

GameState GameState::MoveLeft() const {
    assert(CanMoveLeft());
        
    GameState newState(*this);
    std::swap(newState.field[emptyPos], newState.field[emptyPos + 1]);
    newState.emptyPos++;
    return newState;
}

GameState GameState::MoveRight() const {
     assert(CanMoveRight());
        
    GameState newState(*this);
    std::swap(newState.field[emptyPos], newState.field[emptyPos - 1]);
    newState.emptyPos--;
    return newState;
}

GameState GameState::MoveUp() const {
    assert(CanMoveUp());
    
    GameState newState(*this);
    std::swap(newState.field[emptyPos], newState.field[emptyPos + 4]);
    newState.emptyPos += sideSize;
    return newState;
}

GameState GameState::MoveDown() const { 
    assert(CanMoveDown());
        
    GameState newState(*this);
    std::swap(newState.field[emptyPos], newState.field[emptyPos - sideSize]);
    newState.emptyPos -= sideSize;
    return newState;
}

bool GameState::operator==(const GameState &other) const {
    return field == other.field;
}

size_t GameState::CalcDist() const {
    return SumManhatDist();
}

bool operator<(const GameState& l, const GameState& r) {
    return l.CalcDist() < r.CalcDist();
}

struct GameStateHasher {
    size_t operator()(const GameState &state) const {
        size_t hash = 0;
        std::memcpy(&hash, state.field.data(), sizeof(hash));
        return hash;
    }
};

std::ostream& operator<<(std::ostream &out, const GameState &state){
    for (int i = 0; i < sideSize; i++) {
        for (int j = 0; j < sideSize; j++) {
            out << static_cast<int>(state.field[i * sideSize + j]) << " ";
        }
        out << std::endl;
    }
    return out;
}



std::string GetSolution(const std::array<char, FieldSize> &field) {
    GameState startState(field);
    
    if (!startState.IsSolvable()) {
        return "-1";
    }
    
    std::unordered_map<GameState, char, GameStateHasher> visited;
    visited[startState] = 'S';
    
    std::queue<GameState> queue;
    std::set<pair<size_t, GameState>> tmpDists;
    size_t distToStart = startState.CalcDist();
    tmpDists.insert({distToStart, startState});
    int cnt = 0;
    while (tmpDists.size()) {
        pair <size_t, GameState> curVertex = *(tmpDists.begin());
        GameState state = curVertex.second;
        tmpDists.erase(tmpDists.begin());
        ++cnt;
        
        if (state.IsComplete())
            break;

        if (state.CanMoveLeft()) {
            GameState newState = state.MoveLeft();
            size_t distToNewState = newState.CalcDist();
            if (!visited.count(newState)) {
                visited[newState] = 'L';
                tmpDists.insert({distToNewState, newState});
            }
        }
        if (state.CanMoveRight()) {
            GameState newState = state.MoveRight();
            size_t distToNewState = newState.CalcDist();
            if (!visited.count(newState)) {
                visited[newState] = 'R';
                tmpDists.insert({distToNewState, newState});
            }
        }
        if (state.CanMoveUp()) {
            GameState newState = state.MoveUp();
            size_t distToNewState = newState.CalcDist();
            if (!visited.count(newState)) {
                visited[newState] = 'U';
                tmpDists.insert({distToNewState, newState});
            }
        }
        if (state.CanMoveDown()) {
            GameState newState = state.MoveDown();
            size_t distToNewState = newState.CalcDist();
            if (!visited.count(newState)) {
                visited[newState] = 'D';
                tmpDists.insert({distToNewState, newState});
            }
        }
    }
    
    std::string path;
    GameState state(finishField);
    while (visited[state] != 'S') {
        char move = visited[state];
        switch (move) {
            case 'L': {
                state = state.MoveRight();
                path += 'L';
                break;
            }
            case 'R': {
                state = state.MoveLeft();
                path += 'R';
                break;
            }
            case 'U': {
                state = state.MoveDown();
                path += 'U';
                break;
            }
            case 'D': {
                state = state.MoveUp();
                path += 'D';
                break;
            }
        }
    }
    
    std::reverse(path.begin(), path.end());
    return path;
}

void run(std::istream &input, std::ostream &output) {
    array<char, FieldSize> field;
    for (int i = 0, val; i < FieldSize; ++i) {
        input >> val;
        field[i] = val;
    }
                                     
    string path = GetSolution(field);
    if (path == "-1") cout << -1;
    else output << path.size() << endl << path << endl;
}

void test() {
    {   
        array<char, FieldSize> field = {1, 2, 5, 4, 
                                        3, 6, 7, 8, 
                                        9, 10, 11, 12, 
                                        13, 14, 15, 0};
        GameState gs(field);
        assert(gs.SumManhatDist() == 6);
        cout << "test 1 finished" << endl;
    }
}


int main(int argc, const char * argv[]) {
    run(cin, cout);
    return 0;
}
