/*
 *  PlayState.cpp
 *  Normal "play" state
 *
 *  Created by Isabel Manssour and Márcio Pinho on 05/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#include "CGame.h"
#include "PlayState.h"
#include "PauseState.h"

PlayState PlayState::m_PlayState;

using namespace std;


void PlayState::CarregaTiles() {
	bool ret;
	string nomeArq = BASE_DIR + "data/maps/fundo.txt";

	mapFundo = new CTilesMap();
	ret = mapFundo->loadMap(nomeArq.c_str());
	if (!ret) {
		cout << "Arquivo de mapa ("<< nomeArq << ") não exsite." ;
		return;
	}

	nomeArq = BASE_DIR + "data/maps/blocks1.png";
	ret = mapFundo->loadTileMap(nomeArq.c_str(),
	                            32, 32,		// int w, int h,
	                            2, 2, 		// int hSpace, int vSpace,
	                            2, 2, 		// int xIni, int yIni,
	                            18, 11, 	// int column, int row,
	                            196);		// int total

	if (!ret) {
		cout << "Arquivo de tiles ("<< nomeArq << ") não existe." ;
		return;
	}

	nomeArq = BASE_DIR + "data/maps/blocoColisao.txt";

	mapColisao = new CTilesMap();
	ret = mapColisao->loadMap(nomeArq.c_str());
	if (!ret) {
		cout << "Arquivo de mapa ("<< nomeArq << ") não existe." ;
		return;
	}

	nomeArq = BASE_DIR + "data/maps/blocks1.png";
	ret = mapColisao->loadTileMap(nomeArq.c_str(),
	                              32, 32,		// int w, int h,
	                              2, 2, 		// int hSpace, int vSpace,
	                              2, 2, 		// int xIni, int yIni,
	                              18, 11, 	// int column, int row,
	                              196);		// int total

	if (!ret) {
		cout << "Arquivo de tiles ("<< nomeArq << ") não existe." ;
		return;
	}
}

void PlayState::CarregaSprites() {
	string nomeArq = BASE_DIR + "data/img/char9.png";
	spriteCao = new CSprite();
	spriteCao->loadSprite(nomeArq.c_str(), 108, 62, 20, 66, 0, 72, 4, 4, 6);
	spriteCao->setScale(1);
	//spriteCao->setPosition(0, (mapFundo->getHeightTileMap()*mapFundo->getHeight())-85);
	spriteCao->setPosition(0, 0);
	spriteCao->setAnimRate(8); // taxa de animação em frames por segundo(troca dos frames dele)

	nomeArq = BASE_DIR + "data/img/char2.png";
	spriteCacador = new CSprite();
	spriteCacador->loadSprite(nomeArq.c_str(), 128, 82, 0, 46, 0, 53, 4, 2, 7);
	spriteCacador->setScale(1);
	spriteCacador->setMirror(false);
	spriteCacador->setPosition(250, (mapFundo->getHeightTileMap()*mapFundo->getHeight())-210);
	spriteCacador->setAnimRate(2); // taxa de animação em frames por segundo(troca dos frames dele)
}

void PlayState::CarregaHuds() {
	string nomeArq = BASE_DIR + "data/fonts/font.png"; //biggraybugs.png"; //lucida12.png";
	fonte = new CFont();
	fonte->loadFont(nomeArq.c_str(), 512, 512); // 1024, 1024); //112, 208);
	fonte->setPosition(30,30);
	fonte->setText("Vidas:");
	// fonte->setType(CFont::CFONT_LEFT);

	nomeArq = BASE_DIR + "data/img/1945.png";
	spriteVida = new CSprite();
	spriteVida->loadSprite(nomeArq.c_str(), 32, 32, 1, 1, 268, 103, 1, 3, 3);
	spriteVida->setScale(1);
	spriteVida->setMirror(false);
	spriteVida->setPosition(208, 33);
	spriteVida->setAnimRate(5);
}

void PlayState::MontaLayer() {
	cout << "Monta Layers....";
	layers = new CLayerHandler(5);
	layers->add(mapFundo,0);
	layers->add(mapColisao,1);
	layers->add(spriteCao,1);
	layers->add(spriteCacador,1);
	layers->add(spriteVida,2);
	layers->add(fonte,2);
}

void PlayState::AtualizaTiros() {
	vector<CTiro>::iterator j;
	for (j = vetTiros.begin(); j != vetTiros.end(); j++) {
		j->setX(j->getX()-4);
		// Remove do vetor se o tiro saiu da janela
		if (j->getX() <= 0) {
			vetTiros.erase(j);
			return;
		}
	}
}

void PlayState::VerificaColisaoTiros() {
	for(int i=0; i<vetTiros.size(); i++) {
		float x = vetTiros[i].getX();
		float y = vetTiros[i].getY();
		if (spriteCao->getX()<x && x<(spriteCao->getX()+spriteCao->getWidth()))
			if (spriteCao->getY()<y && y<(spriteCao->getY()+spriteCao->getHeight()))
				layers->remove(spriteCao);
	}
}

void PlayState::init() {
#ifdef __APPLE__
	BASE_DIR = "../../../bin/"; // Codelite
#endif
	//BASE_DIR = "../../../../bin/"; // Visual Studio

	CarregaTiles();
	CarregaSprites();
	CarregaHuds();
	MontaLayer();
	colisao = false;

	// SDL_GetTicks() tells how many milliseconds have past since an arbitrary point in the past.
	lastTimeTiro = SDL_GetTicks();
	tickIntervalTiro = 50;//1000;
	keystate = SDL_GetKeyState(NULL); // get array of key states

	currentFrame = 0;
	pulando = false;
	subindo = false;
	deltaY = -20;

	cout << "PlayState Init Successful" << endl;
}

void PlayState::cleanup() {
	delete spriteCao;
	delete spriteCacador;
	delete spriteVida;
	cout << "PlayState Clean Successful" << endl;
}

void PlayState::pause() {
	cout << "PlayState Paused" << endl;
}

void PlayState::resume() {
	cout << "PlayState Resumed" << endl;
}
// método que testa se há colisão entre uma sprite e os elementos do tile
bool PlayState::TemColisaoSpriteTile(CSprite *sprite, CTilesMap *map) {

	return false;
}

void PlayState::handleEvents(CGame* game) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		// check for messages
		switch (event.type) {
			// exit if the window is closed
		case SDL_QUIT:
			game->quit();
			break;
			// check for keypresses
		case SDL_KEYDOWN:
			// exit if ESCAPE is pressed
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				done = true;
				game->quit();
				break;
			} else if (event.key.keysym.sym == SDLK_SPACE) {
				// Dispara um tiro
				CTiro tiro(spriteCacador->getX(),spriteCacador->getY()+50);
				vetTiros.push_back(tiro);
			} else if (event.key.keysym.sym == SDLK_p)
				game->pushState(PauseState::instance());
			else if (event.key.keysym.sym == SDLK_a) {
				if (!pulando) {
					cout << "Inicia pulo... " << endl;
					pulando = true;
					subindo = true;
					cont = 20;
				} else cout << "Ja estou pulando...." << endl;
			}

			break;
			// SDL_APPACTIVE: When the application is either minimized/iconified
			// (gain=0) or restored ('gain'=1) this type of activation event occurs
		case SDL_APPACTIVE:
			if (event.active.gain==0) {
			}
			break;
		case SDL_VIDEORESIZE:
			game->resize(event.resize.w, event.resize.h);
			break;
		} // end switch
	}// end of message processing


}

void PlayState::update(CGame* game) {
	if (SDL_GetTicks()-lastTimeTiro>tickIntervalTiro) {
		//cout << "Diferenca de Tempo: " << SDL_GetTicks()-lastTime << " milisegundos." << endl;
		lastTimeTiro = SDL_GetTicks();
		AtualizaTiros();
		VerificaColisaoTiros();
	}

	spriteVida->update(game->getUpdateInterval());
	spriteCao->update(game->getUpdateInterval());
	spriteCacador->update(game->getUpdateInterval());

}

void PlayState::draw(CGame* game) {
	glClearColor(0.59, 0.84, 0.91, 0);

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

	layers->draw();

	glDisable(GL_TEXTURE_2D); // isto é necessário quando se deseja desenhar SEM texturas
	glColor3f(0,0,0);
	glPointSize(5);
	for(int i=0; i<vetTiros.size(); i++) {
		glBegin(GL_POINTS);
		glVertex2i(vetTiros[i].getX(),vetTiros[i].getY());
		glEnd();
	}
	glColor3f(1,1,1);


	SDL_GL_SwapBuffers();
}
