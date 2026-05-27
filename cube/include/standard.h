#include <random>
#include <vector>

//create a vector with N random integers between MAX inclusive and MIN inclusive
//where a number k in the sequence is never equal to a number (k-1)
void GenRandomVector(int N, std::vector<int> *seq, int MAX, int MIN);