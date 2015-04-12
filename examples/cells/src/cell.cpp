#include "cell.hpp"
#include "pi.hpp"
#include <random>

Cell::Cell(uint32_t size, const glm::vec2& position):
    mRotation(0.0f),
    mSpin(0.0f),
    mMass((size/2.0f) * (size/2.0f) * pi()),
    mSize(size),
    mBallQuad({(float)size, (float)size}),
    mPatternQuad({(float)size, (float)size}),
    mShadowQuad({(float)size, (float)size}),
    mFieldQuad({size * 1.25f, size * 1.25})
{

    setPosition(position);

    mBallQuad.setOrigin(mBallQuad.getSize() / 2.0f);
    mPatternQuad.setOrigin(mPatternQuad.getSize() / 2.0f);
    mShadowQuad.setOrigin(mShadowQuad.getSize() / 2.0f);
    mFieldQuad.setOrigin(mFieldQuad.getSize() / 2.0f);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> randomColor(0, 255);

    mBallQuad.setColor({randomColor(mt),randomColor(mt),randomColor(mt)});
    mFieldQuad.setColor(mBallQuad.getColor());
    mPatternQuad.setColor({randomColor(mt), randomColor(mt), randomColor(mt)});
}

void Cell::render(fea::Renderer2D& renderer)
{
    renderer.queue(mFieldQuad);
    renderer.queue(mBallQuad);
    renderer.queue(mPatternQuad);
    renderer.queue(mShadowQuad);
}

void Cell::setupTextures(const TextureMap& textures)
{
    mFieldQuad.setTexture(textures.at("field"));
    mBallQuad.setTexture(textures.at("ball"));
    int32_t r= rand() % 3;
    if(r== 0)
        mPatternQuad.setTexture(textures.at("spots"));
    else if(r== 1)
        mPatternQuad.setTexture(textures.at("net"));
    else
        mPatternQuad.setTexture(textures.at("leafy"));
    mShadowQuad.setTexture(textures.at("shadow"));
}

void Cell::setRotation(float angle)
{
    mRotation = angle;

    mFieldQuad.setRotation(angle);
    mBallQuad.setRotation(angle);
    mPatternQuad.setRotation(angle);
    mShadowQuad.setRotation(angle);
}

void Cell::rotate(float angle)
{
    setRotation(mRotation + angle);
}

void Cell::setPosition(const glm::vec2& position)
{
    mPosition = position;
    
    mFieldQuad.setPosition(position);
    mBallQuad.setPosition(position);
    mPatternQuad.setPosition(position);
    mShadowQuad.setPosition(position);
}

void Cell::translate(const glm::vec2& amount)
{
    setPosition(mPosition + amount);
}

void Cell::setSpin(float angle)
{
    mSpin = angle;
}

void Cell::applyImpulse(const glm::vec2& force)
{
    glm::vec2 velocity = force / mMass;
    mVelocity += velocity;
}

void Cell::update()
{
    rotate(mSpin);
    translate(mVelocity);

    float radius = mSize / 2.0f;

    if(mPosition.x - radius < 0.0f ||
       mPosition.x + radius > 1280.0f)
    {
        glm::vec2 reflector;
        reflector.x = mVelocity.x * 2.0f * mMass;

        applyImpulse(-reflector);
    }
    if(mPosition.y - radius < 0.0f ||
       mPosition.y + radius > 768.0f)
    {
        glm::vec2 reflector;
        reflector.y = mVelocity.y * 2.0f * mMass;

        applyImpulse(-reflector);
    }
}
