#include "anchor_generator.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace anchors {
    AnchorGenerator::AnchorGenerator(const AnchorsConfig& config)
    : numberLayers_(config.numberLayers), featureMapsSizes_(config.featureMapsSizes) {
        scales_ = GenerateScales(config.minScale, config.maxScale, config.featureMapsSizes);

        ratios_ = config.ratios;
        if (scales_.size() != ratios_.size()) {
            ratios_.push_back(1.f);
        }

        const float minDim = std::min(config.imgSize.second, config.imgSize.first);
        anchorScales_ = {
            (minDim / config.imgSize.first),
            (minDim / config.imgSize.second)
        };
    }

    std::vector<std::vector<float>> AnchorGenerator::GenerateAnchors() {
        std::vector<std::vector<float>> priors;
        for (size_t idx = 0; idx < numberLayers_; ++idx) {
            const auto squareScale = std::sqrt(scales_[idx] * scales_[idx + 1]);
            std::vector<float> layerScales(scales_.size(), scales_[idx]);
            layerScales[numberLayers_ - 1] = squareScale;
            assert(layerScales.size() == ratios_.size());
            const auto anchorsPerLayer = GeneratePerLayerAnchors(
                featureMapsSizes_[idx].first,
                featureMapsSizes_[idx].second,
                layerScales,
                1.f / featureMapsSizes_[idx].second,
                1.f / featureMapsSizes_[idx].first
            );
            priors.push_back(anchorsPerLayer);
        }
        return priors;
    }

    // private
    std::vector<float> AnchorGenerator::GenerateScales(
            const float minScale,   
            const float maxScale, 
            const std::vector<std::pair<float, float>>& featureMapsSizes) {
        if (featureMapsSizes.empty()) {
            throw std::runtime_error("Error: Input feature map sizes are empty!");
        }

        std::vector<float> scales(featureMapsSizes.size() + 1, 0.f);
        for (size_t idx = 0; idx < featureMapsSizes.size(); ++idx) {
            scales[idx] = minScale + (maxScale - minScale) * (idx) / (featureMapsSizes.size() - 1);
        }
        scales[featureMapsSizes.size()] = 1.f;
        return scales;
    }

    std::vector<float> AnchorGenerator::GeneratePerLayerAnchors(
            const size_t gridHeight,
            const size_t gridWidth,
            const std::vector<float>& perLayerScales,
            const float stepX,
            const float stepY) {
        assert(perLayerScales.size() == ratios_.size());

        const auto numberOfRatios = ratios_.size();
        std::vector<float> anchorHalfWidths(numberOfRatios, 0.f);
        std::vector<float> anchorHalfHeights(numberOfRatios, 0.f);
        for (size_t idx = 0; idx < numberOfRatios; ++idx) {
            anchorHalfWidths[idx] = 0.5f * perLayerScales[idx] * anchorScales_.second * std::sqrt(ratios_[idx]);
            anchorHalfHeights[idx] = 0.5f * perLayerScales[idx] * anchorScales_.first / std::sqrt(ratios_[idx]);
        }

        const auto gridStride = gridHeight * gridWidth;
        const auto offsetX = 0.5f, offsetY = 0.5f;
        const size_t numParams = 4;
        const size_t totNumParams = numParams * numberOfRatios;
        const auto anchorsSize = gridStride * totNumParams;
        std::vector<float> perLayerAnchors(anchorsSize, 0.f);
        for (size_t idx = 0; idx < gridStride; ++idx) {
            const auto cx = stepX * ((idx % gridWidth) + offsetX);
            const auto cy = stepY * ((idx / gridWidth) + offsetY);
            for (size_t scaleIdx = 0; scaleIdx < numberOfRatios; ++scaleIdx) {
                const size_t paramStride = scaleIdx * numParams;
                perLayerAnchors[idx * totNumParams + paramStride] = cy - anchorHalfHeights[scaleIdx];
                perLayerAnchors[idx * totNumParams + 1 + paramStride] = cx - anchorHalfWidths[scaleIdx];
                perLayerAnchors[idx * totNumParams + 2 + paramStride] = cy + anchorHalfHeights[scaleIdx];
                perLayerAnchors[idx * totNumParams + 3 + paramStride] = cx + anchorHalfWidths[scaleIdx];
            }
        }
        return perLayerAnchors;
    }
}  // namespace anchors
