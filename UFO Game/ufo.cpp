#include <iostream>
#include "ufo_functions.hpp"

int main() {
  std::string codeword = "codeacademy";
  std::string answer = "___________";
  int misses = 0;

  greet();

  while (answer != codeword && misses < 7) {

    std::vector<char> incorrect;
    bool guess = false;

    display_misses(misses);
    std::cout << "Incorrect Guesses:\n";

    for (int i = 0; i < incorrect.size(); i++) {
      std::cout << incorrect[i] << ' ';
    }

    std::cout << "\nCodeword:\n";
 
    for (int i = 0; i < answer.length(); i++) {
    std::cout << answer[i] << ' ';
  }

    misses++;
  }

  end_game(answer, codeword);

}
