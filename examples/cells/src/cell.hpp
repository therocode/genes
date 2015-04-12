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
        void setPosition(const glm::vec2& position);
        void translate(const glm::vec2& amount);
        void setSpin(float angle);
        void applyImpulse(const glm::vec2& force);
        void update();
    private:
        float mRotation;
        float mSpin;
        float mMass;
        uint32_t mSize;
        glm::vec2 mPosition;
        glm::vec2 mVelocity;
        fea::Quad mFieldQuad;
        fea::Quad mBallQuad;
        fea::Quad mShadowQuad;
        fea::Quad mPatternQuad;
};
