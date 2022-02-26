#pragma once

#include "GameEngine.h"

#include <SDL/SDL.h>
#include <IMAGE/SDL_image.h>
#include <TTF/SDL_ttf.h>

#include <string>

//Texture wrapper class
class LTexture
{
public:
    //Initializes variables
    LTexture();

    //Deallocates memory
    ~LTexture();

    //Loads image at specified path
    bool loadFromFile(std::string path);

    //Creates image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

    //Deallocates texture
    void free();

    //Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //Set blending
    void setBlendMode(SDL_BlendMode blending);

    //Set alpha modulation
    void setAlpha(Uint8 alpha);

    //Renders texture at given point
    void render(int x0, int y0, int width = 0, int height = 0, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    //Image dimensions
    int tWidth;
    int tHeight;

    int getTextLength() { return textLength; }

    //Makes sure the context is initialized, called by the GameEngine.init()
    //static void setSharedContext(Context* sharedContext) { mSharedContext = sharedContext; };

protected:
    //static Context* mSharedContext;

private:
    int textLength = 0;
    //The actual hardware texture
    SDL_Texture* mTexture;
};

