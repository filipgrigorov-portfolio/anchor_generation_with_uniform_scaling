#include "anchor_generator.hpp"

#include <iostream>
#include <string>
#include <vector>

int main(void) {
    anchors::AnchorsConfig config = {
        6,
        0.2f,
        0.95f,
        {0.2f, 1.f, 2.f, 0.5f, 3.f, 0.3333f},
        {{18, 18}, {12, 12}, {10, 10}, {5, 5}, {4, 4}, {2, 2}},
        {20, 20},
    };
    auto anchorGenerator = anchors::AnchorGenerator(config);
    const auto anchors = anchorGenerator.GenerateAnchors();
    for (size_t lidx = 0; lidx < anchors.size(); ++lidx) {
        for (size_t idx = 0; idx < anchors[lidx].size(); ++idx) {
            std::cout << anchors[lidx][idx] << " ";
        }
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
