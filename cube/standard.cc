#include "include/standard.h"

void GenRandomVector(int N, std::vector<int> *seq, int MAX, int MIN) {
  std::random_device rd; //dunno was needed
  std::mt19937 gen(rd()); //random num generator
  
  //for a number between min and max
  std::uniform_int_distribution<> dis(MIN, MAX);

  if (N > 0) {
    //gen  first item
    int last_num = dis(gen);
    seq->push_back(last_num);

    //gen the remaining items
    //with no same moves twice behind each other
    for (int i = 1; i < N; ++i) {
      int current_num;
      do {
        current_num = dis(gen);
      } while (current_num == last_num); //while same
      seq->push_back(current_num);
      last_num = current_num;
    } //for
  }
} //randomVec