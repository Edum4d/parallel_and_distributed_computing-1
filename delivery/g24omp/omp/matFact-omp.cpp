#include <vector>
#include <omp.h>
#include "src/readInput.h"
#include "src/initialLR.h"
#include "src/updateLR.h"
#include "src/filterFinalMatrix.h"
#include "src/verifyResult.h"

int main(int argc, char *argv[]) {

    int numberOfIterations;
    int numberOfFeatures;
    double convergenceCoefficient;

    int numberOfUsers;
    int numberOfItems;
    int numberOfNonZeroElements;

    std::vector<std::vector<double>> A;
    std::vector<std::vector<int>> nonZeroElementIndexes;

    std::string inputFileName = argv[1];

    readInput(inputFileName, A, nonZeroElementIndexes,
              numberOfIterations, numberOfFeatures, convergenceCoefficient,
              numberOfUsers, numberOfItems,
              numberOfNonZeroElements);

    std::vector<std::vector<double>> L(numberOfUsers, std::vector<double>(numberOfFeatures));
    std::vector<std::vector<double>> R(numberOfFeatures, std::vector<double>(numberOfItems));

    initialLR(L, R, numberOfUsers, numberOfItems, numberOfFeatures);

    std::vector<std::vector<double>> B(numberOfUsers, std::vector<double>(numberOfItems,0));

    std::vector<std::vector<double>> StoreL;
    std::vector<std::vector<double>> StoreR;

    for (int iteration = 0; iteration < numberOfIterations; iteration++) {
        StoreR = R;
        StoreL = L;

        updateLR(A, nonZeroElementIndexes,
                 L, R, StoreL, StoreR,
                 numberOfUsers, numberOfItems, numberOfFeatures,
                 numberOfNonZeroElements,
                 convergenceCoefficient);
    }

    std::vector<int> BV(numberOfUsers);
    filterFinalMatrix(A, B, nonZeroElementIndexes,
                      L, R,
                      numberOfUsers, numberOfItems, numberOfFeatures,
                      numberOfNonZeroElements,
                      BV);

    return 0;
}