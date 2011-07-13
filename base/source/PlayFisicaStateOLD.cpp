/*
 *  PlayFisicaState.cpp
 *  Normal "play" state
 *
 *  Created by Isabel Manssour and Márcio Pinho on 05/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#include "CGame.h"
#include "PlayFisicaState.h"
#include "PauseState.h"

PlayFisicaState PlayFisicaState::m_PlayState;

using namespace std;


void PlayFisicaState::CarregaTiles() {
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

	////

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

void PlayFisicaState::CarregaSprites() {
	string nomeArq = BASE_DIR + "data/img/char9.png";
	spriteCao = new CSprite();
	spriteCao->loadSprite(nomeArq.c_str(), 108, 62, 20, 66, 0, 72, 4, 4, 6);
	spriteCao->setScale(1);
	//spriteCao->setPosition(0, (mapFundo->getHeightTileMap()*mapFundo->getHeight())-85);
	spriteCao->setPosition(200, 10);
	spriteCao->setRotation(30);
	spriteCao->setAnimRate(8); // taxa de animação em frames por segundo(troca dos frames dele)

//	nomeArq = BASE_DIR + "data/img/char2.png";
	spriteCacador = new CSprite();
//	spriteCacador->loadSprite(nomeArq.c_str(), 128, 82, 0, 46, 0, 53, 4, 2, 7);
	spriteCacador->loadSprite(nomeArq.c_str(), 108, 62, 20, 66, 0, 72, 4, 4, 6);
	spriteCacador->setScale(1);
	spriteCacador->setMirror(false);
	
//	spriteCacador->setPosition(200, spriteCao->getY()+spriteCao->getHeight()*1.5);
	spriteCacador->setPosition(200, 300);
	spriteCacador->setAnimRate(2); // taxa de animação em frames por segundo(troca dos frames dele)
}

void PlayFisicaState::CarregaHuds() {
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

void PlayFisicaState::MontaLayer() {
	cout << "Monta Layers....";
	layers = new CLayerHandler(5);
	layers->add(mapFundo,0);
	layers->add(mapColisao,1);
	layers->add(spriteCao,1);
	layers->add(spriteCacador,1);
	layers->add(spriteVida,2);
	layers->add(fonte,2);
}

void PlayFisicaState::AtualizaTiros() {
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

void PlayFisicaState::VerificaColisaoTiros() {
	for(int i=0; i<vetTiros.size(); i++) {
		float x = vetTiros[i].getX();
		float y = vetTiros[i].getY();
		if (spriteCao->getX()<x && x<(spriteCao->getX()+spriteCao->getWidth()))
			if (spriteCao->getY()<y && y<(spriteCao->getY()+spriteCao->getHeight()))
				layers->remove(spriteCao);
	}
}

void PlayFisicaState::InitFisica() {



	// inicializa a classe de física e a Box2D
	Fisica = CPhysics::instance();
	b2Vec2 g(0,10);
	Fisica->setGravity(g);
	Fisica->setConvFactor(10);

	CSprite *s;
	s = spriteCao;
	fisicaCao = Fisica->newBoxImage(CAO_ID,    //int id,
	                                s,				// CImage* sprite,
	                                1,				// float density,
	                                1.0,			// float friction,
	                                0.0,			// float restitution
	                                false);		// bool staticObj=false
	
	fisicaCao= Fisica->newBox(WALL_ID,
		               s->getX(), s->getY(), // posicao
		               s->getWidth(), s->getHeight(), // tamanho X e Y
					   s->getRotation(),
		               1,   //densidade
		               0.5,    // float friction,
		               0.1,    // float restitution
		               false);  // bool staticObj=false

	//b2Vec2 pos(20,0);
	//Fisica->setPosition(fisicaCao, pos);
	//fisicaCao->SetFixedRotation(true);
	s = spriteCacador;
	fisicaPiso = Fisica->newBoxImage(WALL_ID,    //int id,
	                    s,				// CImage* sprite,
	                    1,						// float density,
	                    0,					// float friction,
	                    0.5,					// float restitution
	                    true);					// bool staticObj=false


	//Fisica->setAngle(fisicaCao, 45);
	//Fisica->setAngle(fisicaPiso, 45);
	//b2Vec2 pos(spriteCao->getX()/10, 100/10);
	//Fisica->setPosition(fisicaPiso, pos);

	//spriteCao->xOffset(spriteCao->getX()/2);

	Fisica->setDrawOffset(spriteCao->getWidth()/2, spriteCao->getHeight()/2);

}

void PlayFisicaState::init() {
#ifdef __APPLE__
	BASE_DIR = "../../../bin/"; // Codelite
#endif
	//BASE_DIR = "../../../../bin/"; // Visual Studio

	CarregaTiles();
	CarregaSprites();
	CarregaHuds();
	MontaLayer();

	InitFisica();

	// SDL_GetTicks() tells how many milliseconds have past since an arbitrary point in the past.
	lastTimeTiro = SDL_GetTicks();
	tickIntervalTiro = 50;//1000;
	keystate = SDL_GetKeyState(NULL); // get array of key states

	currentFrame = 0;


	cout << "PlayFisicaState Init Successful" << endl;
}

void PlayFisicaState::cleanup() {
	delete spriteCao;
	delete spriteCacador;
	delete spriteVida;
	cout << "PlayFisicaState Clean Successful" << endl;
}

void PlayFisicaState::pause() {
	cout << "PlayFisicaState Paused" << endl;
}

void PlayFisicaState::resume() {
	cout << "PlayFisicaState Resumed" << endl;
}

bool PlayFisicaState::TemColisaoSpriteTile(CSprite *sprite, CTilesMap *map) {
	return false;
}

void PlayFisicaState::handleEvents(CGame* game) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		// check for messages
		switch (event.type) {
			// exit if the window is closed
		case SDL_QUIT:
			game->quit();
			break;
			// check for keypresses
		case SDL_KEYDOWN: 								// ApplyAngularImpulse/ApplyTorque
			if  (event.key.keysym.sym == SDLK_t) {
				//fisicaCao->ApplyAngularImpulse(-1000);
				fisicaCao->ApplyTorque(-1000);
				break;
			}
			if  (event.key.keysym.sym == SDLK_y) {     // ApplyLinearImpulse
				b2Vec2 impulso;
				b2Vec2 pos;
				impulso.x = 0; impulso.y = -1000;
				//impulso = fisicaCao->GetWorldVector(impulso);
				pos = fisicaCao->GetWorldCenter(); 
				//pos.x +=1;
				fisicaCao->ApplyLinearImpulse(impulso, pos);
				//fisicaCao->ApplyForce(impulso, pos);
				break;
			}			
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

void PlayFisicaState::update(CGame* game) {
	if (SDL_GetTicks()-lastTimeTiro>tickIntervalTiro) {
		//cout << "Diferenca de Tempo: " << SDL_GetTicks()-lastTime << " milisegundos." << endl;
		lastTimeTiro = SDL_GetTicks();
		AtualizaTiros();
		VerificaColisaoTiros();
	}

	spriteVida->update(game->getUpdateInterval());
	spriteCao->update(game->getUpdateInterval());
	spriteCacador->update(game->getUpdateInterval());

	Fisica->step();

	//b2Vec2 pos;
	//pos = fisicaCao->GetPosition();
	//cout << "X = "<< pos.x << " Y = " << pos.y << endl;


}

void PlayFisicaState::draw(CGame* game) {
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

	// Desenha um box ao redor da sprite do cao
	// TO DO : Criar um método DrawEnvelop na cImage ou na cSprite.

	glColor3f(0,0,0);

	/*	glBegin(GL_LINE_LOOP);
		{
			glVertex2f(spriteCacador->getX(), spriteCacador->getY());
			glVertex2f(spriteCacador->getX()+spriteCacador->getWidth(),spriteCacador->getY());
			glVertex2f(spriteCacador->getX()+spriteCacador->getWidth(),spriteCacador->getY()+spriteCacador->getHeight());
			glVertex2f(spriteCacador->getX(),spriteCacador->getY()+spriteCacador->getHeight());
		}
		glEnd();
		glColor3f(1,1,1);
	*/
	Fisica->debugDraw();

	SDL_GL_SwapBuffers();
}
