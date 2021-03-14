#include <iostream>
#include "ufo_functions.hpp"

int main() {
  std::string codeword = "codechallenge";
  std::string answer = "___________";
  int misses = 0;

  greet();

  char letter;

  while (answer != codeword && misses < 7) {

    std::vector<char> incorrect;
    bool guess = false;

    display_misses(misses);
    display_status(incorrect, answer);

    std::cout << "Please enter your guess: ";
    std::cin >> letter;

    for (int i = 0; i < codeword.length(); i++) {
      if ( letter == codeword[i]) {

        answer[i] = letter;
        guess = true;

      }
    }

    if (guess) {
      std::cout << "Correct!\n";
    }
    else {
      std::cout << "Incorrect! The tractor beam pulls the person in further.\n";
      incorrect.push_back(letter);

      misses++;
    }

      guess = false;
    
  }
  end_game(answer, codeword);

}
