
#include "opencv2/core/core.hpp"
#include "opencv2/core/core_c.h"

namespace MatrixDebugger {
    void printMatrix(const char* matrixName, cv::Mat matrix) {
        std::cout << matrixName << std::endl;
        std::cout << matrix << std::endl;
        std::cout << "\n\n";  
    }    
}