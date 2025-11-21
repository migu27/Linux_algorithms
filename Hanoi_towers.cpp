#include <iostream>
#include <vector>
#include "stdlib.h"

using namespace std;

const int NUM_RODS = 3;
int num_discs = 4;
int num_moves = 0;
vector<vector<int>>rods;

void naive_print() {
  for (int i = 0; i < NUM_RODS; i++){
    cout << "===";
    for (int j = 0; j < rods[i].size(); j++) {
      cout << rods[i][j] << " ";
    }
    cout << endl;
  }
  cout << "------" << endl;
}

void move(int from, int to) {
  int from_rod = from-1;
  int to_rod = to-1;
  int moved_disc = rods[from_rod].back();
  rods[from_rod].pop_back();
  rods[to_rod].push_back(moved_disc);
  naive_print();
  num_moves++;
}

void hanoi(int n, int from, int helper, int to) {
  if (n > 0) {
    hanoi(n-1, from, to, helper);
    move(from, to); // Largest disc at the bottom of the first rod
    hanoi(n-1, helper, from, to);
  }
}

// Expects integer argument indicating the number of discs
int main (int argc, char *argv[]) {
  if (argc == 2)
    num_discs = atoi(argv[1]);
  else {
    cout << "Exiting, wrong number of parameters. It should be: hanoi <n>, <n> number of discs\n";
    return -1;
  }

  rods.resize(NUM_RODS, vector<int>(num_discs));
  for (int i = 0; i < num_discs; i++) {
    rods[0][i] = num_discs - i;
    rods[1].pop_back();
    rods[2].pop_back();
  }
	
  naive_print();
  hanoi(num_discs, 1, 2, 3);
  cout << endl << "Number of moves: " << num_moves << endl;
  return 0;
}
