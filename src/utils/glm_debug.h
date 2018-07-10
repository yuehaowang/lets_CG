#ifndef _GLM_DEBUG_H_
#define _GLM_DEBUG_H_

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace glm_debug {
    void OutputMat4(glm::mat4 mat)
    {
        for (int i = 0; i < 16; i++) {
            if (i > 0 && i % 4 == 0) {
                std::cout << std::endl;
            }

            int col = i % 4;
            int row = (i - col) / 4;

            std::cout << mat[col][row] << " ";
        }

        std::cout << std::endl;
    }
}


#endif