/*
 *  PlayPhysics.cpp
 *  Testbed for Box2D (physics) experiments
 *
 *  Created by Marcelo Cohen on 05/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#include <iostream>
#include <SDL.h>
#include "Graphics.h"
#include <cmath>
#include "CGame.h"
#include "PauseState.h"
#include "PlayPhysics.h"

PlayPhysics PlayPhysics::m_PlayPhysics;

using namespace std;

void PlayPhysics::init()
{
    map.loadMap("data/maps/dungeon.tmx");
	cout << "Collision objects: " << map.totalObjects() << endl;
    playerK.vel.X = playerK.vel.Y = 0;   // current player speed
    cameraSpeed = 8;   // speed to use
    zvel = 0;

    keystate = SDL_GetKeyState(NULL);

    player = new CSprite();
    player->loadSprite("data/img/Char19s.png", 32, 32, 0, 0, 0, 0, 1, 1, 1);
//    map.getCenter(2,2,playerK.pos.X,playerK.pos.Y);
    map.getCenter(35,5,playerK.pos.X,playerK.pos.Y);

    enemy = new CSprite();
    enemy->loadSprite("data/img/Char14s.png", 32, 32, 0, 0, 0, 0, 1, 1, 1);
    float ex, ey;
//    map.getCenter(12,15,ex,ey);
    map.getCenter(45,4,ex,ey);
    enemyK.pos.X = ex;
    enemyK.pos.Y = ey;
    enemyK.maxSpeed = 4;

    steerMode = CHASE_BEHAVIOR; // default: chase player

    firstTime = true; // to set map position at first update

    // Init array of blocking ids
    for(int i=0; i<256; i++)
        blocks[i] = 0; // default: not blocking

    // Set indices of blocking ids
    blocks[0] = blocks[1] = blocks[2] = 1;
    blocks[4] = 1;
    for(int i=8; i<=14; i++)
        blocks[i] = 1;

    phys = CPhysics::instance();
    phys->setGravity(0);
    phys->setConvFactor(10);
    float bx, by;
    map.getCenter(33,17,bx,by);
    b2Body* ground2= phys->newBox(bx  ,   by , 48, 16,1000,0.5,0.5,true);
						 //newBox(id,     x,   y, width, float height, float density, float friction, float restitution, bool staticObj=false);
	
    b2Body* ground = phys->newBox(1200,500,200,10,1000,0.5,0.5,true); // ground
    phys->setAngle(ground, 10); //15);
    player->setPosition(playerK.pos.X, playerK.pos.Y);
    playerPhys = phys->newBoxImage(PLAYER_ID, player,1,0.5,0);

    enemy->setPosition(enemyK.pos.X, enemyK.pos.Y);
    //enemyPhys = phys->newBoxImage(enemy,1,0.5,0.1);

    playerPhys->SetFixedRotation(true);
    //enemyPhys ->SetFixedRotation(true);
	for(int o=0; o<map.totalObjects(); o++)
	{
		Object& obj = map.getObject(o);
		phys->newBox(WALL_ID, obj.x,obj.y, obj.width,obj.height, 1000,      0.5,      0.1,              true);
	            //newBox(id,     x,    y, width,         height, density,  friction,  restitution,   staticObj=false);
	}

    // Set offset to add to translation when drawing debug shapes
    // in physics
    phys->setDrawOffset(map.getTileWidth()/2, map.getTileHeight()/2);
    cout << "PlayPhysics Init Successful" << endl;
}

void PlayPhysics::cleanup()
{
    delete player;
    //  delete playSprite2;
	cout << "PlayPhysics Clean Successful" << endl;
}

void PlayPhysics::pause()
{
	cout << "PlayPhysics Paused" << endl;
}

void PlayPhysics::resume()
{
	cout << "PlayPhysics Resumed" << endl;
}

void PlayPhysics::handleEvents(CGame* game)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				game->quit();
				break;

            case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
                    case SDLK_p:
                        game->pushState(PauseState::instance());
                        break;
                    case SDLK_a:
                        steerMode++;
                        if(steerMode > EVADE_BEHAVIOR)
                            steerMode = CHASE_BEHAVIOR;
                        break;
                    case SDLK_ESCAPE:
                        game->quit();
                        break;
                    case SDLK_t:
                        //playerPhys->ApplyTorque(-100);
                        playerPhys->ApplyAngularImpulse(-100);
                        break;
                    case SDLK_f:
                        playerPhys->ApplyLinearImpulse(b2Vec2(0,-300), playerPhys->GetWorldCenter());
                        break;
                }
                break;
            case SDL_VIDEORESIZE:
                game->resize(event.resize.w, event.resize.h);
                //map.cleanup();
                //map.loadMap("data/maps/desert.tmx");
		}
	}
    playerK.vel.X = -keystate[SDLK_LEFT]*cameraSpeed + keystate[SDLK_RIGHT]*cameraSpeed;
    playerK.vel.Y = -keystate[SDLK_UP]*cameraSpeed + keystate[SDLK_DOWN]*cameraSpeed;

    playerPhys->ApplyLinearImpulse(b2Vec2(playerK.vel.X,playerK.vel.Y), playerPhys->GetWorldCenter());
    zvel = -5*keystate[SDLK_LSHIFT] + 5*keystate[SDLK_RSHIFT];
//    playerK.heading = playerK.vel / playerK.vel.getLength();
}

// Steering Behavior: chase target
irrklang::vec3df PlayPhysics::chase(Kinematic& vehicle, irrklang::vec3df& target)
{
    irrklang::vec3df desiredVel = target - vehicle.pos;
    desiredVel.normalize();
    desiredVel *= vehicle.maxSpeed;
    return desiredVel - vehicle.vel;
}

// Steering Behavior: arrive at target
irrklang::vec3df PlayPhysics::arrive(Kinematic& vehicle, irrklang::vec3df& target, float decel)
{
    irrklang::vec3df toTarget = target - vehicle.pos;
    float d = toTarget.getLength();
    if(d > 0)
    {
        // Calculate the speed required to reach the target given the desired
        // deceleration
        float speed = d / decel;

        // Make sure the velocity does not exceed the max
        speed = min(speed, vehicle.maxSpeed);

        // From here proceed just like chase, except we don't need to normalize
        // the toTarget vector because we have already gone to the trouble
        // of calculating its length: d
        irrklang::vec3df desiredVel = toTarget * speed / d;
        return desiredVel - vehicle.vel;
    }
    return irrklang::vec3df(0,0,0);
}

// Steering Behavior: flee from target
irrklang::vec3df PlayPhysics::flee(Kinematic& vehicle, irrklang::vec3df& target, float panicDistance)
{
    float panicDistance2 = panicDistance * panicDistance;
    if(enemyK.pos.getDistanceFromSQ(target) > panicDistance2)
        return irrklang::vec3df(0,0,0);
    irrklang::vec3df desiredVel = vehicle.pos - target;
    desiredVel.normalize();
    desiredVel *= vehicle.maxSpeed;
    return desiredVel - vehicle.vel;
}

// Steering Behavior: pursuit target
irrklang::vec3df PlayPhysics::pursuit(Kinematic& vehicle, Kinematic& target)
{
    irrklang::vec3df toEvader = target.pos - vehicle.pos;
    double relativeHeading = vehicle.heading.dotProduct(target.heading);
    // If target is facing us, go chase it
    if(toEvader.dotProduct(vehicle.heading) > 0 && relativeHeading < -0.95) // acos(0.95) = 18 graus
        return chase(vehicle, target.pos);

    // Not facing, so let's predict where the target will be

    // The look-ahead time is proportional to the distance between the target
    // and the enemy, and is inversely proportional to the sum of the
    // agents' velocities

    float vel = target.vel.getLength();
    double lookAheadTime = toEvader.getLength() / (vehicle.maxSpeed + vel);

    // Now chase to the predicted future position of the target

    irrklang::vec3df predicted(target.pos + target.vel * lookAheadTime);
    return arrive(vehicle, predicted, 1);
}

// Steering Behavior: evade target
irrklang::vec3df PlayPhysics::evade(Kinematic& vehicle, Kinematic& target)
{
    irrklang::vec3df toPursuer = target.pos - vehicle.pos;

    // The look-ahead time is proportional to the distance between the pursuer
    // and the vehicle, and is inversely proportional to the sum of the
    // agents' velocities

    float vel = target.vel.getLength();
    double lookAheadTime = toPursuer.getLength() / (vehicle.maxSpeed + vel);

    // Now chase to the predicted future position of the target

    irrklang::vec3df predicted(target.pos + target.vel * lookAheadTime);
    return flee(vehicle, predicted);
}

void PlayPhysics::update(CGame* game)
{
    // First time in update, set initial camera pos
    if(firstTime) {
        game->setYpan(6*32);
        game->setXpan(-10*32);
        game->updateCamera();
        firstTime = false;
    }

    // Player motion ?
    if(zvel != 0) {
        game->setZoom(game->getZoom()+zvel);
        game->updateCamera();
    }

    //checkCollision(game, playerK);
    phys->step();
    playerK.pos.X = player->getX();
    playerK.pos.Y = player->getY();
//    enemyK.pos.X = enemy->getX();
//    enemyK.pos.Y = enemy->getY();
    centerPlayerOnMap(game);

#define STEERING
#ifdef STEERING
    // Apply steering behavior(s)

    //irrklang::vec3df steeringForce = flee(enemyK, playerK,100);
    //irrklang::vec3df steeringForce = pursuit(enemyK, playerK);
    irrklang::vec3df steeringForce;

    switch(steerMode) {
        case CHASE_BEHAVIOR:
            steeringForce = chase(enemyK, playerK.pos);
            break;
        case ARRIVE_BEHAVIOR:
            steeringForce = arrive(enemyK, playerK.pos, 1); // 0.3 - lento ... 1 - rápido
            break;
        case PURSUIT_BEHAVIOR:
            steeringForce = pursuit(enemyK, playerK);
            break;
        case FLEE_BEHAVIOR:
            steeringForce = flee(enemyK, playerK.pos, 100);
            break;
        case EVADE_BEHAVIOR:
            steeringForce = evade(enemyK, playerK);
    }
    irrklang::vec3df accel = steeringForce/1; // mass;

    b2Vec2 thrust = b2Vec2(accel.X*50,accel.Y*50);

    //cout << "Thrust: " << thrust.x << " " << thrust.y << endl;
    //enemyPhys->ApplyLinearImpulse(thrust,enemyPhys->GetWorldCenter());
    enemyK.vel += accel; // * deltaTime

    // Can't exceed max speed
    if(enemyK.vel.getLengthSQ() > enemyK.maxSpeed*enemyK.maxSpeed)
        enemyK.vel = enemyK.vel.normalize() * enemyK.maxSpeed;

    // Only update heading if speed is above minimum threshold
    if(enemyK.vel.getLengthSQ() > 0.00000001) {
        enemyK.heading = enemyK.vel / enemyK.vel.getLength();
    }
#else
    // Basic chase

    enemyK.vel.set(0,0,0);
    if(enemyK.pos.X < playerK.pos.X)
        enemyK.vel.X = 2;
    else if(enemyK.pos.X > playerK.pos.X)
        enemyK.vel.X = -2;
    if(enemyK.pos.Y < playerK.pos.Y)
        enemyK.vel.Y = 2;
    else if(enemyK.pos.Y > playerK.pos.Y)
        enemyK.vel.Y = -2;
#endif
    checkCollision(game, enemyK);
}

// Collision detection and centering based on code from
// http://www.parallelrealities.co.uk/tutorials/intermediate/tutorial14.php

void PlayPhysics::centerPlayerOnMap(CGame* game)
{
    float maxMapX = map.getNumMapColumns() * map.getTileWidth();
    float maxMapY = map.getNumMapRows() * map.getTileHeight();

    float gameWidth  = game->getWidth();
    float gameHeight = game->getHeight();

    float panX = playerK.pos.X - (gameWidth/2);

    if(panX < 0)
        panX = 0;

    else if(panX + gameWidth >= maxMapX)
        panX = maxMapX - gameWidth;

    float panY = playerK.pos.Y - (gameHeight/2);

    if(panY < 0)
        panY = 0;

    else if(panY + gameHeight >= maxMapY)
        panY = maxMapY - gameHeight;

    game->setXpan(panX);
    game->setYpan(panY);
    game->updateCamera();
}

void PlayPhysics::checkCollision(CGame* game, Kinematic& obj)
{
    int i, x1, x2, y1, y2;

    // Get the limits of the map
    int maxMapX = map.getNumMapColumns();
    int maxMapY = map.getNumMapRows();

    // Get the width and height of a single tile
    int tileW = map.getTileWidth();
    int tileH = map.getTileHeight();

    // Get the height and width of the object (in this case, 100% of a tile)
    int playerH = tileH;
    int playerW = tileW;

    // Test the horizontal movement first
    i = playerH > tileH ? tileH : playerH;

    for (;;)
    {
        x1 = (obj.pos.X + obj.vel.X) / tileW;
        x2 = (obj.pos.X + obj.vel.X + playerW - 1) / tileW;

        y1 = (obj.pos.Y) / tileH;
        y2 = (obj.pos.Y + i - 1) / tileH;

        if (x1 >= 0 && x2 < maxMapX && y1 >= 0 && y2 < maxMapY)
        {
            if (obj.vel.X > 0)
            {
                // Trying to move right

                int upRight   = map.getCell(x2,y1);
                int downRight = map.getCell(x2,y2);
                if (blocks[upRight] == 1 || blocks[downRight] == 1)
                {
                    // Place the player as close to the solid tile as possible
                    obj.pos.X = x2 * tileW;
                    obj.pos.X -= playerW + 1;
                    obj.vel.X = 0;
                }
            }

            else if (obj.vel.X < 0)
            {
                // Trying to move left

                int upLeft   = map.getCell(x1,y1);
                int downLeft = map.getCell(x1,y2);
                if (blocks[upLeft] == 1 || blocks[downLeft] == 1)
                {
                    // Place the player as close to the solid tile as possible
                    obj.pos.X = (x1+1) * tileW;
                    obj.vel.X = 0;
                }
            }
        }

        if (i == playerH) // Checked player height with all tiles ?
        {
            break;
        }

        i += tileH; // done, check next tile upwards

        if (i > playerH)
        {
            i = playerH;
        }
    }

    // Now test the vertical movement

    i = playerW > tileW ? tileW : playerW;

    for (;;)
    {
        x1 = (obj.pos.X / tileW);
        x2 = ((obj.pos.X + i) / tileW);

        y1 = ((obj.pos.Y + obj.vel.Y) / tileH);
        y2 = ((obj.pos.Y + obj.vel.Y + playerH) / tileH);

        if (x1 >= 0 && x2 < maxMapX && y1 >= 0 && y2 < maxMapY)
        {
            if (obj.vel.Y > 0)
            {
                // Trying to move down
                int downLeft  = map.getCell(x1,y2);
                int downRight = map.getCell(x2,y2);
                if (blocks[downLeft] == 1 || blocks[downRight] == 1)
                {
                    // Place the player as close to the solid tile as possible
                    obj.pos.Y = y2 * tileH;
                    obj.pos.Y -= playerH;
                    obj.vel.Y = 0;
                }
            }

            else if (obj.vel.Y < 0)
            {
                // Trying to move up

                int upLeft  = map.getCell(x1,y1);
                int upRight = map.getCell(x2,y1);
                if (blocks[upLeft] == 1 || blocks[upRight] == 1)
                {
                    // Place the player as close to the solid tile as possible
                    obj.pos.Y = (y1 + 1) * tileH;
                    obj.vel.Y = 0;
                }
            }
        }

        if (i == playerW)
        {
            break;
        }

        i += tileW; // done, check next tile to the right

        if (i > playerW)
        {
            i = playerW;
        }
    }

    // Now apply the movement

    obj.pos += obj.vel;

    if (obj.pos.X < 0)
        obj.pos.X = 0;
    else if (obj.pos.X + playerW >= maxMapX * tileW)
        obj.pos.X = maxMapX*tileW - playerW - 1;

    if(obj.pos.Y < 0)
        obj.pos.Y = 0;
    else if(obj.pos.Y + playerH >= maxMapY * tileH)
        obj.pos.Y = maxMapY*tileH - playerH - 1;
}


void PlayPhysics::draw(CGame* game)
{
    glClearColor(0.8,0.8,0.8,1); // light gray
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    map.draw();

    //player->setPosition(playerK.pos.X,playerK.pos.Y);
    player->draw();

    enemy->setPosition(enemyK.pos.X, enemyK.pos.Y);
    enemy->draw();

    phys->debugDraw();

    SDL_GL_SwapBuffers();
}
