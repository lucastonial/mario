/*
 *  PlayFisicaState.h
 *  Normal "play" state
 *
 *  Created by Isabel Manssour and Márcio Pinho on 05/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */


#ifndef PLAYFISICA_STATE_H_
#define PLAYFISICA_STATE_H_

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
#include "CTiro.h"

#include "CPhysics.h"

#include <vector>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;

class PlayFisicaState : public CGameState
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
        static PlayFisicaState* instance()
        {
            return &m_PlayState;
        }


    protected:
        PlayFisicaState() {}
        void CarregaTiles();
        void CarregaSprites();
        void CarregaHuds();
        void MontaLayer();
        void AtualizaTiros();
        void VerificaColisaoTiros();
		bool TemColisaoSpriteTile(CSprite *sprite, CTilesMap *map);
		void InitFisica();


    private:
        static PlayFisicaState m_PlayState;
        CLayerHandler *layers;
        CTilesMap *mapFundo;
        CTilesMap *mapColisao;
        CSprite *spriteCao;
        CSprite *spriteCacador;
        CSprite *spriteVida;
        CFont *fonte;

        vector<CTiro> vetTiros;
        Uint8* keystate; // state of all keys (1 means key is pressed)

        int currentFrame;
        bool done;

        double lastTimeTiro;
        double tickIntervalTiro;

        string BASE_DIR;
		
		CPhysics* Fisica;
		b2Body* fisicaCao;
		
		enum {
        CAO_ID, WALL_ID
		};

};

#endif
