#include "cells.hpp"
#include "texturemaker.hpp"

Cells::Cells():
     mInputHandler(new fea::SDL2InputBackend()),
     mRenderer(fea::Viewport({1280, 768}, {0, 0}, fea::Camera({1280.0f / 2.0f, 768.0f / 2.0f}))),
     mWindow(new fea::SDL2WindowBackend())
{
}

void Cells::setup(const std::vector<std::string>& args)
{
    mWindow.create(fea::VideoMode(1280, 768), "Cells");
    mWindow.setFramerateLimit(60);
    mInputHandler.setKeyRepeatEnabled(false);

    mRenderer.setup();

    //setupAudio();
    setupGraphics();

    setupCells();
}

void Cells::destroy()
{
    mWindow.close();
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
    mCells = { 
        {128, {100.0f, 100.0f}},
        {128, {500.0f, 500.0f}},
        {64, {600.0f, 550.0f}},
        {32, {400.0f, 350.0f}}
    };

    for(auto& cell : mCells)
        cell.setSpin(2.0f/((rand() % 2000) - 1000.0f));

    for(auto& cell : mCells)
        cell.setupTextures(mTextures);
}

void Cells::update()
{
    for(auto& cell : mCells)
        cell.update();
}