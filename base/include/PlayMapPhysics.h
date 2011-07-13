/*
 *  PlayMapPhysics.h
 *  Normal "play" state - tiled map viewing
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#ifndef PLAY_MAPPHYS_H_
#define PLAY_MAPPHYS_H_

#include <SDL.h>
#include "CGameState.h"
#include "CSprite.h"
#include "TMXLoader.h"
#include "CPhysics.h"
#include "ShaderManager.h"

class PlayMapPhysics : public CGameState
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
    static PlayMapPhysics* instance()
    {
        return &m_PlayMapPhysics;
    }

    protected:

    PlayMapPhysics() {}

    private:

    static PlayMapPhysics m_PlayMapPhysics;

    CPhysics* phys;
    b2Body* playerPhys, *npcPhys;

    int x, y;
    float cameraSpeed;
    float xvel, yvel;
    float zvel;
    
    enum {
        PLAYER_ID, NPC_ID, GROUND_ID, TOTEM_ID, BALL_ID
    };
    bool onGround; // true if player is on the ground
    CImage* ball;
    CSprite* player;
    CSprite* npc;
    CMultiImage* totem;
    float playerX, playerY;
    TMXLoader map;
    Uint8* keystate; // state of all keys (1 means key is pressed)
    bool firstTime;
    float playerAnimRate;

    void centerPlayerOnMap(CGame* game);

    // Audio
    irrklang::ISoundSource* walkSoundSource;
    irrklang::ISound* walkSound;
    irrklang::ISoundSource* jumpSoundSource;
    irrklang::ISoundSource* superJumpSoundSource;
    irrklang::ISoundSource* smurf3DSoundSource;
    irrklang::ISound* loopedSound;

    // Shaders
    ShaderManager sm;
};

#endif
