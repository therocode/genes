//#include <fea/audio.hpp>
#include <fea/structure.hpp>
#include <fea/render2d.hpp>
#include <fea/userinterface.hpp>
#include <fea/ui/sdl2windowbackend.hpp>
#include <fea/ui/sdl2inputbackend.hpp>
#include "texturemap.hpp"
#include "cell.hpp"

class Cells : public fea::Application
{
    public:
        Cells();
    protected:
        virtual void setup(const std::vector<std::string>& args) override;
        virtual void destroy() override; 
        virtual void loop() override;
    private:
        void handleInput();
        void render();
//        void setupAudio();
        void setupGraphics();
        void setupCells();
        void update();
        fea::InputHandler mInputHandler;
        fea::Renderer2D mRenderer;
        fea::Window mWindow;

        TextureMap mTextures;
        std::vector<Cell> mCells;

        //fea::AudioPlayer mAudioPlayer;
        //fea::AudioSample mPistolSample;
        //fea::Audio mPistolAudio;
        //fea::AudioSample mShotgunSample;
        //fea::Audio mShotgunAudio;
};
