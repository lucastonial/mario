/*
 *  TelaInicialState.cpp
 *  Example "Congratulations" state
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#include <iostream>
#include <SDL.h>
#include <SDL_opengl.h>
#include "CGame.h"
#include "TelaInicialState.h"
#include "PlayFisicaState.h"


TelaInicialState TelaInicialState::m_TelaInicialState;
//PauseState PauseState::m_PauseState;

using namespace std;


void TelaInicialState::init()
{
	BASE_DIR = "../../../../bin/";
    bool ret;
	telaInicialSprite = new CImage();
	string nomeArq = BASE_DIR + "data/img/TelaInicial.png";
	telaInicialSprite->loadImage(nomeArq.c_str());
    telaInicialSprite->setPosition(140,30);
   // telaInicialFont = new CFont();
   //nomeArq = BASE_DIR + "data/fonts/lucida12.png";
   // telaInicialFont->loadFont(nomeArq.c_str(), 112, 208);
    cout << "TelaInicialState Init Successful" << endl;
}

void TelaInicialState::cleanup()
{
    delete telaInicialSprite;
	cout << "TelaInicialState Clean Successful" << endl;
}

void TelaInicialState::resume() {}

void TelaInicialState::pause() {}

void TelaInicialState::handleEvents(CGame* game)
{
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				game->quit();
				break;

			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
			case SDLK_SPACE:
				game->changeState(PlayFisicaState::instance());
				break;
			case SDLK_ESCAPE:
				game->quit();
				break;
				}
		}
	}
}

void TelaInicialState::update(CGame* game)
{
}

void TelaInicialState::draw(CGame* game)
{
    glClearColor(0,0,0,0); // black
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    telaInicialSprite->setScale(1);
    telaInicialSprite->draw();
    glLoadIdentity();
    //telaInicialFont->draw(300,300,"Pressione ESC para sair do jogo");
    SDL_GL_SwapBuffers();
}

