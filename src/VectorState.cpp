#include "../header/VectorState.h"
void printBoolVector(std::vector<bool> arr) {
    for (int i = 0; i < arr.size(); i++) {
        std::cout << i << ":" << arr[i] << " ";
    }
    std::cout << std::endl;
}
std::vector<bool> getZeroBoolVector() {
    std::vector<bool> result;
    for (int i = 0; i < 4; i++) {
        result.push_back(false);
    }
    return result;
}
std::string printState(state_holder& state) {
    std::string result = "";
    std::string delim = "";
    for (auto item : state) {
        result += delim + std::to_string(item);
        delim = ", ";
    }
    return result;
}
int getZeroIndex(state_holder &state) {
    return std::find(state.begin(), state.end(), 0) - state.begin();
}
int getPuzzlePieceIndex(state_holder &state, smallInt target) {
    return std::find(state.begin(), state.end(), target) - state.begin();
}

void swap(state_holder &ary, int index1, int index2) {
    int temp = ary[index1];
    ary[index1] = ary[index2];
    ary[index2] = temp;
}