/*
 *  MenuState.h
 *  Example "menu" state
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Updated by Isabel Manssour and Márcio Pinho on 05/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#endif

#include <cstdlib>
#include <iostream>
#include <cstring>
#include "CGameState.h"
#include "CTilesMap.h"
#include "CFont.h"

using namespace std;


class MenuState : public CGameState
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
        static MenuState* instance()
        {
            return &m_MenuState;
        }

    protected:
        MenuState() {}

    private:
        static MenuState m_MenuState;
        CTilesMap* mapaFundo;
        CFont *fonte;
        string BASE_DIR;

};

#endif

