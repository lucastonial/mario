/*
 *  MenuState.cpp
 *  Example "menu" state
 *
 *  Created by Marcelo Cohen on 04/11.
 *  Updated by Isabel Manssour and Márcio Pinho on 05/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */


#include <iostream>
#include <SDL.h>
#include <SDL_opengl.h>
#include "CGame.h"
#include "MenuState.h"
#include "PlayFisicaState.h"


MenuState MenuState::m_MenuState;

using namespace std;

void MenuState::init()
{

#ifdef __APPLE__
	BASE_DIR = "../../../bin/"; // Codelite
#endif
#ifdef _MSC_VER
	BASE_DIR = "../../../../bin/"; // Visual Studio
#endif

    bool ret;
	string nomeArq = BASE_DIR + "data/maps/menuEntrada.txt";

	mapaFundo = new CTilesMap();
	ret = mapaFundo->loadMap(nomeArq.c_str());
	if (!ret) {
		cout << "Arquivo de mapa ("<< nomeArq << ") não existe." ;
		return;
	}
	else cout << "Leitura OK !" << endl;

	nomeArq = BASE_DIR + "data/maps/blocks1.png";
	ret = mapaFundo->loadTileMap(nomeArq.c_str(),
	                       32, 32,		// int w, int h,
	                       2, 2, 		// int hSpace, int vSpace,
	                       2, 2, 		// int xIni, int yIni,
	                       18, 11, 	// int column, int row,
	                       196);		// int total

	if (!ret) {
		cout << "Arquivo de tiles ("<< nomeArq << ") não existe." ;
		return;
	}

    nomeArq = BASE_DIR + "data/fonts/lucida12.png";
    fonte = new CFont();
    fonte->loadFont(nomeArq.c_str(), 112, 208);
    fonte->setPosition(300,250);
    fonte->setText("Pressione espaco para comecar");

	cout << "MenuState Init Successful" << endl;
}

void MenuState::cleanup()
{
    delete mapaFundo;
    delete fonte;
	cout << "MenuState Cleanup Successful" << endl;
}

void MenuState::pause()
{
	cout << "MenuState Paused" << endl;
}

void MenuState::resume()
{
	cout << "MenuState Resumed" << endl;
}

void MenuState::handleEvents(CGame* game)
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

void MenuState::update(CGame* game)
{
}

void MenuState::draw(CGame* game)
{
 	glClearColor(0.3, 0.3, 0.3, 0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, game->getWidth(), game->getHeight(), 0, 1, -1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(0, 0, game->getWidth(), game->getHeight());

	glEnable(GL_TEXTURE_2D); // isto é necessário quando se deseja desenhar com texturas

	// Enable transparency through blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mapaFundo->draw();
    fonte->draw();

    SDL_GL_SwapBuffers();
}

