#include "cells.hpp"
#include "texturemaker.hpp"
#include <random>

Cells::Cells():
     mInputHandler(new fea::SDL2InputBackend()),
     mWindow(new fea::SDL2WindowBackend(), fea::VideoMode(1280, 768), "Cells"),
     mRenderer(fea::Viewport({1280, 768}, {0, 0}, fea::Camera({1280.0f / 2.0f, 768.0f / 2.0f})))
{
}

void Cells::setup(const std::vector<std::string>& args)
{
    mWindow.setFramerateLimit(60);
    mInputHandler.setKeyRepeatEnabled(false);

    //setupAudio();
    setupGraphics();

    setupCells();
}

void Cells::loop()
{
    handleInput();

    update();

    mRenderer.clear();
    render();

    mWindow.swapBuffers();
    //mAudioPlayer.update();
}

void Cells::handleInput()
{
    fea::Event event;

    while(mInputHandler.pollEvent(event))
    {
        if(event.type == fea::Event::CLOSED)
        {
            quit();
        }
        else if(event.type == fea::Event::KEYPRESSED)
        {
            if(event.key.code == fea::Keyboard::ESCAPE)
                quit();
            else if(event.key.code == fea::Keyboard::SPACE)
                setupCells();
        }
        else if(event.type == fea::Event::KEYRELEASED)
        {
        }
    }
}
        
void Cells::render()
{
    for(auto& cell : mCells)
        cell.render(mRenderer);

    mRenderer.render();
}

//void Cells::setupAudio()
//{
//    fea::AudioFile file;
//    file.open("data/sounds/pistol.ogg");
//    mPistolSample.loadSampleData(file);
//    mPistolAudio.setSample(mPistolSample);
//
//    file.open("data/sounds/shotgun.ogg");
//    mShotgunSample.loadSampleData(file);
//    mShotgunAudio.setSample(mShotgunSample);
//}

void Cells::setupGraphics()
{
    mTextures.emplace("ball", makeTexture("resources/textures/ball.png"));
    mTextures.emplace("spots", makeTexture("resources/textures/spots.png"));
    mTextures.emplace("net", makeTexture("resources/textures/net.png"));
    mTextures.emplace("leafy", makeTexture("resources/textures/leafy.png"));
    mTextures.emplace("shadow", makeTexture("resources/textures/shadow.png"));
    mTextures.emplace("field", makeTexture("resources/textures/field.png"));
}

void Cells::setupCells()
{
    std::random_device rd;
    std::uniform_int_distribution<> randomSize(16, 128);
    std::uniform_int_distribution<> randomX(65, 1200);
    std::uniform_int_distribution<> randomY(65, 700);
    std::uniform_int_distribution<> randomImpulse(-3000, 3000);
    mCells.clear();

    for(int32_t i = 0; i < 100; i++)
    {
        mCells.push_back({(uint32_t)randomSize(rd), glm::vec2(randomX(rd), randomY(rd))});
    }

    for(auto& cell : mCells)
    {
        cell.setSpin(2.0f/((rand() % 2000) - 1000.0f));
        cell.applyImpulse(glm::vec2(randomImpulse(rd), randomImpulse(rd)));
        cell.setupTextures(mTextures);
    }
}

void Cells::update()
{
    for(auto& cell : mCells)
        cell.update();
}
