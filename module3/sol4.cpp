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
int ITER_NUMBER = 10000;

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

    bool TestCase1() {
        if (field[0] == 1 && 
            field[1] == 2 && 
            field[2] == 3 && 
            field[7] == 4 && 
            field[6] == 0) {
            cout << "DEBUG: TEST CASE1";
            return true;
        }
    }
    // GameState FillTest1() {
    //     MoveRight();
    //     MoveDown();
    //     MoveLeft();
    //     MoveLeft();
    //     MoveUp();
    //     MoveRight();
    //     MoveDown();
    //     MoveRight();
    //     MoveUp();
    // }
    
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

    size_t PosFirstSecondSevenInMoreEight() {
        for (int i = 8; i < FieldSize; ++i) {
            if (field[i] < 8) {
                return i;
            }
        }
    }

    size_t CalcDistByAbs() const {
        size_t calcDist = getInvCount();
        size_t first = FindFirstWrong();
        if (first <= 4) {
            calcDist += (1 << 30);
            for (int i = 0; i < FieldSize - 1; i++) {
                if (field[i] == 1 || field[i] == 2 || field[i] == 3) {
                    // cout << calcDist << endl;
                    calcDist += (1 << (FieldSize - i)) * FindManhatDist(i, field[i] - 1);
                    calcDist += 10 * FindManhatDist(i, emptyPos);
                }
                if (field[i] == 4) {
                    calcDist += (1 << (FieldSize - i)) * FindManhatDist(i, 7);
                    calcDist += 10 * FindManhatDist(i, emptyPos);
                }
                if (!field[i]) {
                    calcDist += (1 << (FieldSize - i)) * FindManhatDist(i, 6);
                }
            }
            return calcDist;
        } else {
            cout << "first " << first << endl;
            cout << "YES!!" << endl;
            cout << *this << endl;
            exit(0);
        }
        if (COUNTER == ITER_NUMBER) {
            cout << *this << endl;
            cout << "first" << first << endl;
        }




        ++COUNTER;
        if (first == 1) {
            // cout << 1 << " ";
            cout << first << endl;
            size_t firstWrongPos = GetPosOfElem(first);
            cout << firstWrongPos << endl;
            calcDist += (1 << 25) * FindManhatDist(firstWrongPos, 0);
            calcDist += (1 << 20) * FindManhatDist(firstWrongPos, emptyPos);
            return calcDist;
        } else if (first == 2) {
            // cout << 2 << " ";
            size_t firstWrongPos = GetPosOfElem(first);
            // cout << firstWrongPos << " ";
            calcDist += (1 << 24) * FindManhatDist(firstWrongPos, 1);
            calcDist += (1 << 20) * FindManhatDist(firstWrongPos, emptyPos);
            return calcDist;
        } else if (first == 3) {
            // cout << 3 << " ";
            size_t firstWrongPos = GetPosOfElem(first);
            calcDist += (1 << 23) * FindManhatDist(firstWrongPos, 2);
            calcDist += (1 << 20) * FindManhatDist(firstWrongPos, emptyPos);
        } else if (first == 4) {
            // cout << 4 << " ";
            size_t firstWrongPos = GetPosOfElem(first);
            calcDist += (1 << 22) * FindManhatDist(firstWrongPos, 7);
            calcDist += (1 << 20) * FindManhatDist(firstWrongPos, emptyPos);
        } else if (first == 5) {
            // size_t firstWrongPos = GetPosOfElem(first);
            calcDist += (1 << 21) * FindManhatDist(emptyPos, 6);
        }

            // for (int i = 0; i < 8; ++i) {
            //     if (field[i] > 7) calcDist += (1 << 17);
            // }
            // ++COUNTER;
            // if (COUNTER > 10000 && COUNTER < 10010) {
            //     cout << "res: " << endl;
            //     cout << *this << endl;
            //     cout << "empty: " << (int)emptyPos << endl;
            // }
            // if (emptyPos < 8) calcDist += (1 << 15);
            // for (int i = 8; i < FieldSize; ++i) {
            //     if (field[i] < 8) {
            //         calcDist += (1 << 19) * (i / sideSize - 1);
            //     }
            // }
            // for (int i = 8; i < FieldSize; ++i) {
            //     if (field[i] < 8) {
            //         calcDist += (1 << 16) * FindManhatDist(i, emptyPos);
            //         break;
            //     }
            // }
        else {
            exit(0);
        }
        // if (COUNTER == ITER_NUMBER) cout << "CalcDist: ";
        // if (COUNTER == ITER_NUMBER) cout << first << " ";
        if (!first) return 0;
        size_t firstWrongPos = GetPosOfElem(first);
        if (COUNTER == ITER_NUMBER) cout << firstWrongPos << " ";
        if (COUNTER == ITER_NUMBER) cout << emptyPos << " ";
        
        if (first <= 8) {
            calcDist += (1 << 26);
        }
        if (COUNTER == ITER_NUMBER) cout << FindManhatDist(firstWrongPos, emptyPos) << " ";
        if (COUNTER == ITER_NUMBER) cout << "here is state: " << GetEmptyPos() << "\n " << *this << endl;
        size_t row = first / 4;
        calcDist += FindManhatDist(firstWrongPos, emptyPos) * (1 << (FieldSize - row));
        ++COUNTER;
        // for (int i = 0; i < FieldSize - 1; i++) {
        //     if (field[i]) {
        //         // cout << calcDist << endl;
        //         // calcDist += (1 << (FieldSize - i)) * abs(field[i] - (i + 1));
        //         calcDist += abs(field[i] - (i + 1));
        //     }
        // }
        size_t calcAbsDist = 0;
        for (int i = 0; i < FieldSize - 1; i++) {
            if (field[i]) {
                // cout << calcDist << endl;
                calcAbsDist += (1 << (FieldSize - i)) * FindManhatDist(i, field[i] - 1);
            }
        }
        calcDist += calcAbsDist;

        return calcDist;
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
    // cout << "invCount: " << invCount << endl;
    unsigned int numberOfStr = emptyPos / sideSize + 1;
    // cout << "numberOfStr: " << numberOfStr << endl;
    return ((invCount + numberOfStr) % 2 == 0) ? true : false;
}

bool GameState::IsComplete() const {
    // cout << "field: " << field << endl;
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
    return CalcDistByAbs();
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
    // int maxIters = 10000;
    int cnt = 0;
    while (tmpDists.size()) {
        pair <size_t, GameState> curVertex = *(tmpDists.begin());
        GameState state = curVertex.second;
        tmpDists.erase(tmpDists.begin());
        ++cnt;
        // cout << state << endl;
        // cout << "dist: " << state.CalcDist() << endl;
        if (cnt == 1000) cout << state << endl;
        if (cnt > 1000) break;
        
        if (state.IsComplete())
            break;
        
        if (state.TestCase1()) {
            cout << state << endl;
            exit(0);
            // state = state.fillTest1();
            // size_t distToNewState = newState.CalcDist();
            // if (!visited.count(state)) {
            //     visited[newState] = 'L';
            //     tmpDists.insert({distToNewState, newState});
            // }
        }

        if (state.CanMoveLeft()) {
            // cout << "CanMoveLeft" << endl;
            GameState newState = state.MoveLeft();
            size_t distToNewState = newState.CalcDist();
            // cout << distToNewState << endl;
            if (!visited.count(newState)) {
                visited[newState] = 'L';
                tmpDists.insert({distToNewState, newState});
            }
        }
        if (state.CanMoveRight()) {
            // cout << "CanMoveRight" << endl;
            GameState newState = state.MoveRight();
            size_t distToNewState = newState.CalcDist();
            // cout << distToNewState << endl;
            if (!visited.count(newState)) {
                visited[newState] = 'R';
                tmpDists.insert({distToNewState, newState});
            }
        }
        if (state.CanMoveUp()) {
            // cout << "CanMoveUp" << endl;
            GameState newState = state.MoveUp();
            size_t distToNewState = newState.CalcDist();
            // cout << distToNewState << endl;
            if (!visited.count(newState)) {
                visited[newState] = 'U';
                tmpDists.insert({distToNewState, newState});
            }
        }
        if (state.CanMoveDown()) {
            // cout << "Can Move Down" << endl;
            GameState newState = state.MoveDown();
            size_t distToNewState = newState.CalcDist();
            // cout << distToNewState << endl;
            if (!visited.count(newState)) {
                visited[newState] = 'D';
                tmpDists.insert({distToNewState, newState});
            }
        }
        // cout << "tmpDists in the end " << tmpDists.size() << endl;
    }
    // cout << "cnt: " << cnt << endl;
    
    std::string path;
    GameState state(finishField);
    
    // std::cout << state << std::endl;
    // cnt = 0;
    // cout << "CAME TO REVERSE PATH" << endl;;
    while (visited[state] != 'S') {
        // ++cnt;
        // if (cnt > maxIters) break;
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
        
        // std::cout << state << std::endl;
    }
    
    std::reverse(path.begin(), path.end());
    return path;
}

int main(int argc, const char * argv[]) {
    size_t a = -1;
    cout << a << endl;
    // GameState test(finishField);
    // cout << test.CalcDistByAbs() << endl;
    // cout << test.IsSolvable();
    // array<char, FieldSize> field = {2, 3, 1, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
    // array<char, FieldSize> field = {0, 1, 10, 13, 
    //                                 15, 7, 11, 8,
    //                                 9, 14, 6, 12,
    //                                 3, 2, 5, 4};

    // array<char, FieldSize> field = {1, 0, 10, 13, 
    //                                 15, 7, 11, 8,
    //                                 9, 14, 6, 12,
    //                                 3, 2, 5, 4};
    array<char, FieldSize> field = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 0, 3, 2, 4, 1};
    string path = GetSolution(field);
    cout << path.size() << endl << path << endl;
    // calcDist += (1 << (FieldSize - i)) * FindManhatDist(i, 7);
    // GameState gs(field);
    // cout << gs.CalcDist() << endl;
    return 0;
}
