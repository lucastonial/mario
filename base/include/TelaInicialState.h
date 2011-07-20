/*
 *  TelaInicialState.h
 *  Example "pause" state
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#ifndef _TELAINICIAL_STATE_H_
#define _TELAINICIAL_STATE_H_

#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#endif

#include "CGameState.h"
#include "CImage.h"
#include "CSprite.h"
#include "CLayerHandler.h"
#include "CTilesMap.h"
#include "CFont.h"

#include <vector>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sstream>

#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#endif

using namespace std;


class TelaInicialState : public CGameState
{
    public:
        void init();
        void cleanup();

        void pause();
        void resume();

        void handleEvents(CGame* game);
        void update(CGame* game);
        void draw(CGame* game);

        // Implement Singleton Pattern
        static TelaInicialState* instance()
        {
            return &m_TelaInicialState;
        }

    protected:
        TelaInicialState() {}

    private:
        static TelaInicialState m_TelaInicialState;
        CImage* telaInicialSprite;
        CFont*  telaInicialFont;
        string BASE_DIR;
		
};

#endif
