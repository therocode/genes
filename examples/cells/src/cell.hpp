#pragma once
#include <cstdint>
#include <fea/render2d.hpp>
#include "texturemap.hpp"

class Cell
{
    public:
        Cell(uint32_t size, const glm::vec2& position);
        void render(fea::Renderer2D& renderer);
        void setupTextures(const TextureMap& textures);
        void setRotation(float angle);
        void rotate(float angle);
        void setSpin(float angle);
        void update();
    private:
        float mRotation;
        float mSpin;
        fea::Quad mFieldQuad;
        fea::Quad mBallQuad;
        fea::Quad mShadowQuad;
        fea::Quad mPatternQuad;
};
