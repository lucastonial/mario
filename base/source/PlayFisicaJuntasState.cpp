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

void PlayFisicaState::DesenhaLinha(float x1, float y1, float x2, float y2) {
	glColor3f(0,0,0);
	glBegin(GL_LINES);
	{
		glVertex2f(x1,y1);
		glVertex2f(x2,y2);
	}
	glEnd();

	glColor3f(1,1,1);

}
void PlayFisicaState::DesenhaLinha(b2Vec2 pontoInicial, b2Vec2 pontoFinal) {
	DesenhaLinha(pontoInicial.x, pontoInicial.y, pontoFinal.x, pontoFinal.y);
}


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

	spriteCao->setPosition(300,20);

	spriteCao->setAnimRate(8); // taxa de animação em frames por segundo(troca dos frames dele)
	spriteCao->setScale(1.0);

	nomeArq = BASE_DIR + "data/img/char2.png";
	spriteCobra = new CSprite();
	spriteCobra->loadSprite(nomeArq.c_str(), 128, 82, 0, 46, 0, 53, 4, 2, 7);
	spriteCobra->setScale(1);
	spriteCobra->setMirror(false);

	spriteCobra->setPosition(spriteCao->getX()+ spriteCao->getWidth()/2, spriteCao->getY()+ spriteCao->getHeight()/2 );
	spriteCobra->setAnimRate(2); // taxa de animação em frames por segundo(troca dos frames dele)

}

void PlayFisicaState::MontaLayer() {
	cout << "Monta Layers....";
	layers = new CLayerHandler(5);
	layers->add(mapFundo,0);
	layers->add(mapColisao,1);
	layers->add(spriteCao,1);
	layers->add(spriteCobra,1);


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
#define CAO
#ifdef CAO
	s = spriteCao;
	fisicaCao = Fisica->newBoxImage(CAO_ID,    //int id,
	                                s,                // CImage* sprite,
	                                1,                // float density,
	                                0.2,            // float friction,
	                                0.0,            // float restitution
	                                0.5,			 // float linearDamping
	                                0.5,			 // float angularDamping
	                                false);        // bool staticObj=false
#endif

#define COBRA
#ifdef COBRA
	s = spriteCobra;
	fisicaCobra = Fisica->newBoxImage(COBRA_ID,    //int id,
	                                  s,                // CImage* sprite,
	                                  1,                // float density,
	                                  0.3,            // float friction,
	                                  0.0,            // float restitution
	                                  0.5,			 // float linearDamping
	                                  0.5,			 // float angularDamping
	                                  false);        // bool staticObj=false
#endif
	Fisica->newBox(OBSTACULO1,    //int id,
	               s->getX() + s->getWidth()*1.4, //pos x
	               s->getY() ,            // pos y
	               s->getWidth()/2,                // width
	               s->getHeight(),            // height
	               0,                // rotation
	               1,              // float density,
	               0.2,            // float friction,
	               0.5,            // float restitution
	               0.5,                // float linearDamping
	               0.5,                // float angularDamping
	               false);          // bool staticObj=false

	Fisica->newCircle(BOLA,
	                  s->getX() + s->getWidth(),
	                  s->getY() + s->getHeight()+ 100,
	                  15, 			   // float radius
	                  1,              // float density,
	                  1.0,            // float friction,
	                  0.5,            // float restitution
	                  0.5,            // float linearDamping
	                  0.5,            // float angularDamping
	                  false);         // bool staticObj=false

	CriaMapDeColisao();
	

	b2RevoluteJointDef jointDef;	

	jointDef.Initialize(fisicaCobra, fisicaCao, fisicaCao->GetWorldCenter());
	Fisica->world->CreateJoint(&jointDef);

}

void PlayFisicaState::init() {

#ifdef __APPLE__
	BASE_DIR = "../../../bin/"; // Codelite
#endif

#ifdef _MSC_VER
	BASE_DIR = "../../../../bin/"; // Visual Studio
#endif

	CarregaTiles();
	CarregaSprites();
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
		case SDL_KEYDOWN:
			// exit if ESCAPE is pressed
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				done = true;
				game->quit();
				break;
			} else if (event.key.keysym.sym == SDLK_p)
				game->pushState(PauseState::instance());
			else if (event.key.keysym.sym == SDLK_a) {

			}
			if  (event.key.keysym.sym == SDLK_l) {
				fisicaCao->ApplyLinearImpulse(Direcao, PontoFinal);
				cout << "aplicando impulso DIR...."<< endl ;
				break;
			}
			if  (event.key.keysym.sym == SDLK_k) {
				fisicaCao->ApplyLinearImpulse(-Direcao, PontoFinal);
				cout << "aplicando impulso ESQ...."<< endl ;
				break;
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
	spriteCao->update(game->getUpdateInterval());

	Fisica->step();

	if (Fisica->haveContact(OBSTACULO1+1, CAO_ID)) {
		cout << "Contato !!!"<< endl;
	}

	b2Vec2 pos;
	pos = fisicaCao->GetPosition();
	//cout << "X = "<< pos.x << " Y = " << pos.y << endl;

	PontoFinal = fisicaCao->GetWorldCenter();
	Direcao = b2Vec2(0,100);

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

	Fisica->debugDraw();

	b2Vec2 fim;
	// converte de coordenadas da física para coordenadas de tela
	fim.x = PontoFinal.x * 10;
	fim.x -=35;
	fim.y = PontoFinal.y * 10;

	// Traca vetor de impulso
	b2Vec2 inicio;
	inicio.x = fim.x - Direcao.x;
	inicio.y = fim.y - Direcao.y;
	DesenhaLinha( inicio,  fim);

	//cout << "Ponto Final -> X = "<< PontoFinal.x << " Y = " << PontoFinal.y << endl;
	//cout << "Direcao -> X = "<< Direcao.x << " Y = " << Direcao.y << endl;

	DesenhaLinha(300, 0, 300,1000);

	SDL_GL_SwapBuffers();
}

// Traça uma borda preta ao redor de uma imagem
void PlayFisicaState::DesenhaBordaImagem(CImage *i) {
	glPushMatrix();
	{
		glRotatef(i->getRotation(), 0, 0, 1);
		glBegin(GL_LINE_LOOP);
		{
			glVertex2f(i->getX(), i->getY());
			glVertex2f(i->getX()+i->getWidth(),i->getY());
			glVertex2f(i->getX()+i->getWidth(),i->getY()+i->getHeight());
			glVertex2f(i->getX(),i->getY()+i->getHeight());
		}
		glEnd();
	}
	glPopMatrix();
	glColor3f(1,1,1);
}

// Cria uma caixa de colisão
void PlayFisicaState::CriaCaixa(int id, float x, float y, float w, float h) {
	Fisica->newBox(id, x,y,w,h,
	               0,                // rotation
	               1,              // float density,
	               1.0,            // float friction,
	               0.0,            // float restitution
	               0.5,                // float linearDamping
	               0.5,                // float angularDamping
	               true);          // bool staticObj=false

}
// Cria objetos de colisão a partir de um 'mapa de tiles'
void PlayFisicaState::CriaMapDeColisao() {
	float x,y; // usadas para calcular a posição de cada "tile"
	x = 0;
	y = 0;
	int identificador = BOLA + 5; // identificador para cada tile
	float width = mapColisao->getWidth();
	float height = mapColisao->getHeight();
	float difX = width/2;
	float difY = height/2;

	for(int lin=0; lin < mapColisao->getHeightTileMap(); lin++) {
		for (int col=0; col< mapColisao->getWidthTileMap(); col++) {
			//cout <<  "   "  << mapColisao->getTileNumber(col,lin);
			if (mapColisao->getTileNumber(col,lin) != 192) { // se não é um "tile" do fundo
				// faz manualmente o ajuste entre o (0,0) do "tile" e o (0,0) da Box2D
				CriaCaixa(identificador, x+difX,y+difY, width, height);
				identificador ++;
				cout << "["<< col << "," << lin << "]  --  X = "<< x << " Y = " << y << endl;
			}
			//else cout << "["<< col << "," << lin << "] --" << endl;
			x += mapColisao->getWidth();
		}
		//cout << endl;
		x = 0;
		y += mapColisao->getHeight();
	}

}
