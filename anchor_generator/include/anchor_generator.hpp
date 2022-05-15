#pragma once

#include <vector>

namespace anchors {
    struct AnchorsConfig {
        size_t numberLayers;

        float minScale = 0.2f;
        float maxScale = 0.95f;
        
        std::vector<float> ratios;

        // Note: (height, width)
        std::vector<std::pair<float, float>> featureMapsSizes;

        // Note: (height, width)
        std::pair<float, float> imgSize;
    };

    class AnchorGenerator {
     public:
        AnchorGenerator(const AnchorsConfig& config);
        ~AnchorGenerator() = default;

        std::vector<std::vector<float>> GenerateAnchors();

     private:
        std::vector<float> GenerateScales(
            const float minScale, 
            const float maxScale, 
            const std::vector<std::pair<float, float>>& featureMapsSizes);

        std::vector<float> GeneratePerLayerAnchors(
            const size_t gridHeight,
            const size_t gridWidth,
            const std::vector<float>& perLayerScales,
            const float stepX,
            const float stepY);

        int numberLayers_;
        std::vector<std::pair<float, float>> featureMapsSizes_;

        std::vector<float> scales_;
        std::vector<float> ratios_;

        // Note: (height, width)
        std::pair<float, float> anchorScales_;
    };
}  // namespace anchors
