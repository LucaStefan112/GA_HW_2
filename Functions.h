#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cmath>
#include <cstdlib>
#include <time.h>
#include <vector>

class Functions{
  public:
    enum FunctionTypes { Dejong, Schwefel, Rastrigin, Michalewicz };

    // Select function:
    void selectFunction(FunctionTypes functionType, int dimension);

    // Generate Random Function Input:
    std::vector<int> generateRandom();

    // Function result:
    float getResult(std::vector<int> x);

  // private:
    int bitSpace, dimension;
    float a, b, minConstant;
    FunctionTypes functionType;
    
    static const int decimalPrecision = 100000;

    // Functions:
    static float dejong1(std::vector<float> x);
    static float schwefel7(std::vector<float> x);
    static float rastrigin6(std::vector<float> x);
    static float michalewicz12(std::vector<float> x);
};

#endif //FUNCTIONS_H