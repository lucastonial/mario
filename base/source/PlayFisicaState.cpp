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
	string nomeArq = BASE_DIR + "data/maps/Mario.txt";

	mapFundo = new CTilesMap();
	ret = mapFundo->loadMap(nomeArq.c_str());
	if (!ret) {
		cout << "Arquivo de mapa ("<< nomeArq << ") não exsite." ;
		return;
	}

	nomeArq = BASE_DIR + "data/maps/TilesMario.png";
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

	nomeArq = BASE_DIR + "data/maps/CenarioMarioColisao.txt";

	mapColisao = new CTilesMap();
	ret = mapColisao->loadMap(nomeArq.c_str());
	if (!ret) {
		cout << "Arquivo de mapa ("<< nomeArq << ") não existe." ;
		return;
	}

	nomeArq = BASE_DIR + "data/maps/TilesMario2.png";
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
	string nomeArq = BASE_DIR + "data/img/Mario.png";
	spriteMario = new CSprite();
	spriteMario->loadSprite(nomeArq.c_str(), 18, 32, 0, 0, 0, 0, 3, 1, 3);
	spriteMario->setScale(1);
	spriteMario->setFrameRange(0,0);

	spriteMario->setPosition(200,440);

	spriteMario->setAnimRate(4); // taxa de animação em frames por segundo(troca dos frames dele)
	spriteMario->setScale(1.0);

	nomeArq = BASE_DIR + "data/img/char2.png";
	spriteCobra = new CSprite();
	spriteCobra->loadSprite(nomeArq.c_str(), 128, 82, 0, 46, 0, 53, 4, 2, 7);
	spriteCobra->setScale(1);
	spriteCobra->setMirror(false);

	spriteCobra->setPosition(300, 250);
	spriteCobra->setAnimRate(2); // taxa de animação em frames por segundo(troca dos frames dele)

	//Chama o método para carregar os inimigos
	CarregaInimigos("data/img/QuestionBlocks.png", 600, 450);
	CarregaInimigos("data/img/QuestionBlocks.png", 1000, 450);
	CarregaInimigos("data/img/QuestionBlocks.png", 1500, 450);
	CarregaInimigos("data/img/QuestionBlocks.png", 2000, 450);
	CarregaInimigos("data/img/QuestionBlocks.png", 2500, 450);
	
	
	
	//Chama o método para carregar os Question Blocks - Ordem de aparecimento pelo X (coluna)
	CarregaQuestionBlocks("data/img/QuestionBlocks.png", 768, 352); //Coluna 24, Linha 11
	CarregaQuestionBlocks("data/img/QuestionBlocks.png", 928, 352); //Coluna 29, Linha 11
	CarregaQuestionBlocks("data/img/QuestionBlocks.png", 960, 224); //Coluna 30, Linha 7
	CarregaQuestionBlocks("data/img/QuestionBlocks.png", 992, 352); //Coluna 31, Linha 11
	CarregaQuestionBlocks("data/img/QuestionBlocks.png", 3104, 352); //Coluna 97, Linha 11
	CarregaQuestionBlocks("data/img/QuestionBlocks.png", 3712, 224); //Coluna 116, Linha 7
	CarregaQuestionBlocks("data/img/QuestionBlocks.png", 4128, 352); //Coluna 129, Linha 11
	CarregaQuestionBlocks("data/img/QuestionBlocks.png", 4224, 224); //Coluna 132, Linha 7
	CarregaQuestionBlocks("data/img/QuestionBlocks.png", 4224, 352); //Coluna 132, Linha 11
	CarregaQuestionBlocks("data/img/QuestionBlocks.png", 4320, 352); //Coluna 135, Linha 11
	CarregaQuestionBlocks("data/img/QuestionBlocks.png", 4960, 224); //Coluna 155, Linha 7
	CarregaQuestionBlocks("data/img/QuestionBlocks.png", 4992, 224); //Coluna 156, Linha 7
	CarregaQuestionBlocks("data/img/QuestionBlocks.png", 6336, 352); //Coluna 198, Linha 11

	CarregaItens("data/img/Mashroom.png", 928, 320); //Coluna 29, Linha 10

}

void PlayFisicaState::CarregaQuestionBlocks(string path, float positionX, float positionY)
{
	string nomeArq = BASE_DIR + path;

	insereQuestionBlocks = new CSprite();
	insereQuestionBlocks->loadSprite(nomeArq.c_str(), 31, 32, 0, 0, 0, 0, 2, 1, 2);
	insereQuestionBlocks->setScale(1);
	insereQuestionBlocks->setPosition(positionX, positionY);

	insereQuestionBlocks->setAnimRate(20); // taxa de animação em frames por segundo(troca dos frames dele)
	QuestionBlocks.push_back(insereQuestionBlocks);


}

void PlayFisicaState::CarregaInimigos(string path, float positionX, float positionY)
{
	string nomeArq = BASE_DIR + path;

	spriteInimigos = new CSprite();
	spriteInimigos->loadSprite(nomeArq.c_str(), 31, 32, 0, 0, 0, 0, 2, 1, 2);
	spriteInimigos->setScale(1);
	spriteInimigos->setPosition(positionX, positionY);

	spriteInimigos->setAnimRate(2); // taxa de animação em frames por segundo(troca dos frames dele)
	VetInimigos.push_back(spriteInimigos);

}

void PlayFisicaState::CarregaItens(string path, float positionX, float positionY)
{
	string nomeArq = BASE_DIR + path;

	insereItem = new CSprite();
	insereItem->loadSprite(nomeArq.c_str(), 32, 32, 0, 0, 0, 0, 7, 1, 7);
	insereItem->setScale(1);
	insereItem->setPosition(positionX, positionY);

	insereItem->setAnimRate(2); // taxa de animação em frames por segundo(troca dos frames dele)
	Itens.push_back(insereItem);

}

//verifica colisao do mario
void PlayFisicaState::VerificaColisao(CSprite *inimigo, int id){
	if(!VetInimigosMortos[id]){ //aqui testa se o inimigo que está sendo testado não esta morto (quando remover do vetor de inimigos podemos tirar isso, por enquanto está ai pra testes)
		VarTipoColisao = NADA; //começa com o tipo de colisao = nada, pois como estamos permitindo o mario entrar no inimigo devemos testar primeiro se ele colidiu de lado
		//colisao lado
		if(inimigo->getX()<(spriteMario->getX()+20) && spriteMario->getX()<(inimigo->getX()+inimigo->getWidth()+10)){
			if(inimigo->getY()<(spriteMario->getY()+20) && spriteMario->getY()<(inimigo->getY()+inimigo->getHeight())){
				cout << "****LADO****" << endl;
				VarTipoColisao = LADO;
			}
		}
		if(VarTipoColisao != LADO){ // se não colidiu de lado permitimos o teste para colisao de cima, ai não tem perigo de morrer e depois matar
			//colisao cima
			if(inimigo->getX()<(spriteMario->getX()+10) && spriteMario->getX()<(inimigo->getX()+inimigo->getWidth()+5)){
				if((inimigo->getY())<(spriteMario->getY()+40) && spriteMario->getY()<(inimigo->getY()+inimigo->getHeight())){
					cout << "****CIMA****" << endl;
					layers->remove(inimigo);
					VetInimigosMortos[id] = true; //quando o inimigo morre é adicionado no vetor de inimigos mortos
					VarTipoColisao = CIMA;
				}
			}
		}
	}	
	
}

void PlayFisicaState::VerificaColisaoQuestionBlocks(CSprite *questionBlock){	
	//colisao baixo
	if(questionBlock->getX()<(spriteMario->getX()+20) && spriteMario->getX()<(questionBlock->getX()+questionBlock->getWidth()+10)){
		if(questionBlock->getY()<(spriteMario->getY()) && spriteMario->getY()<(questionBlock->getY()+questionBlock->getHeight()+2)){
			cout << "****BAIXO****" << endl;
			layers->remove(questionBlock);
			VarTipoColisao = BAIXO;
		}
	}
}

//controla os estados do mario
void PlayFisicaState::EstadosMario(){
	switch (VarEstadosMario) {
		case INICIAL:
			cout << "inicial";
			if(VarTipoColisao == LADO)
				VarEstadosMario = MORTE;
			//bool teste = spriteMario->bboxCollision(spriteCobra); //colisao exemplo
			//se colidir com inimigo: VarEstadosMario = MORTE;
			//se colidir com cogumelo: VarEstadosMario = COGUMELO e modifica sprite para maior e seta tipo do item dos blocos para flor;
			break;
		case COGUMELO:
			cout << "cogumelo";
			if(VarTipoColisao == LADO)
				VarEstadosMario = INICIAL;
			//se colidir com inimigo: VarEstadosMario = INICIAL e modifica sprite para menor e seta o tipo do item dos blocos para cogumelo;
			//se colidir com flor: VarEstadosMario = FLOR e modifica sprite para outra cor;
			break;
		case FLOR:
			cout << "flor";
			if(VarTipoColisao == LADO)
				VarEstadosMario = INICIAL;
			//permissao de atirar
			//se colidir com inimigo: VarEstadosMario = INICIAL e modifica sprite para menor e seta o tipo do item dos blocos para cogumelo;
			//se colidir com flor: VarEstado = FLOR e vida++;
			break;
		case MORTE:
			cout << "morte";
			//finaliza o jogo
			break;
	}
}
/*
//aqui foi feito teste dos estados do mario no events
//aqui comeca os testes do estado do mario
			}else if(event.key.keysym.sym == SDLK_g) {
        VarEstadosMario = COGUMELO;  
			}else if(event.key.keysym.sym == SDLK_m) {
				if(VarEstadosMario == COGUMELO || VarEstadosMario == FLOR)
					VarEstadosMario = INICIAL;
				else if(VarEstadosMario == INICIAL)
					VarEstadosMario = MORTE;
			}else if(event.key.keysym.sym == SDLK_f) {
        VarEstadosMario = FLOR;  
			}else if(event.key.keysym.sym == SDLK_l) {
        if(VarEstadosMario == FLOR)
					cout << " ==== ATIRA ===== " << endl;
				else
					cout << " ******* NAO PODE ATIRAR ****** " << endl;
			}
			//aqui termina os testes do estado do mario

*/
void PlayFisicaState::MontaLayer() {
	cout << "Monta Layers....";
	layers = new CLayerHandler(5);
	layers->add(mapFundo,0);
	layers->add(mapColisao,1);
	layers->add(spriteMario,1);
	layers->add(spriteCobra,1);
	for(int nCount = 0; nCount < (int)QuestionBlocks.size(); nCount++)
	{
		layers->add(QuestionBlocks[nCount],1);
	}
	
	for(int nCount = 0; nCount < (int)VetInimigos.size(); nCount++){
		layers->add(VetInimigos[nCount],1);
	}
	
	for(int nCount = 0; nCount < (int)Itens.size(); nCount++)
	{
		layers->add(Itens[nCount],1);
	}
	
}

void PlayFisicaState::InitFisica() {
	// inicializa a classe de física e a Box2D
	Fisica = CPhysics::instance();
	b2Vec2 g(0,60);
	Fisica->setGravity(g);
	Fisica->setConvFactor(10);

	CSprite *s;
#define MARIO
#ifdef MARIO
	s = spriteMario;
	fisicaMario = Fisica->newBoxImage(MARIO_ID,    //int id,
	                                s,                // CImage* sprite,
	                                1,                // float density,
	                                0.5,            // float friction,
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
	                                  0.2,            // float restitution
	                                  0.5,			 // float linearDamping
	                                  0.5,			 // float angularDamping
	                                  false);        // bool staticObj=false
#endif
	//Fisica->newBox(OBSTACULO1,    //int id,
	//               s->getX() + s->getWidth()*1.4, //pos x
	//               s->getY() ,            // pos y
	//               s->getWidth()/2,                // width
	//               s->getHeight(),            // height
	//               0,                // rotation
	//               1,              // float density,
	//               0.2,            // float friction,
	//               0.5,            // float restitution
	//               0.5,                // float linearDamping
	//               0.5,                // float angularDamping
	//               false);          // bool staticObj=false

	//Fisica->newCircle(BOLA,
	//                  s->getX() + s->getWidth(),
	//                  s->getY() + s->getHeight()+ 30,
	//                  15, 			   // float radius
	//                  1,              // float density,
	//                  1.0,            // float friction,
	//                  0.5,            // float restitution
	//                  0.5,            // float linearDamping
	//                  0.5,            // float angularDamping
	//                  false);         // bool staticObj=false

	CriaMapDeColisao();
	

	b2RevoluteJointDef jointDef;
	jointDef.Initialize(fisicaMario, fisicaCobra, fisicaMario->GetWorldCenter());

	fisicaMario->SetFixedRotation(true);
	

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

	keystate = SDL_GetKeyState(NULL); // get array of key states

	currentFrame = 0;;

	cout << "PlayFisicaState Init Successful" << endl;
	
	//inicializa estado do mario
	VarEstadosMario = INICIAL;
	//inicializa tipo do item = cogumelo
	tipoItem = 1;

	tempoEsperaPulo = 0; ////inicia com 0 para entrar no if do pulo
	estaColidindo = true; //inicia como true para dizer que o personagem inicia colidindo com o chão podendo pular
	colisao = false;//inicia como false a variável auxiliar que controla a colisão entre os sprites
	
	for(int nCount = 0; nCount < (int)VetInimigos.size(); nCount++){
			VetInimigosMortos[nCount] = false;
	}
	
}

void PlayFisicaState::cleanup() {
	delete spriteMario;
	cout << "PlayFisicaState Clean Successful" << endl;
}

void PlayFisicaState::pause() {
	cout << "PlayFisicaState Paused" << endl;
}

void PlayFisicaState::resume() {
	cout << "PlayFisicaState Resumed" << endl;
}

bool PlayFisicaState::TemColisaoSpriteTile(CSprite *sprite, CTilesMap *map) {
	return true;
}

void PlayFisicaState::handleEvents(CGame* game) {
	SDL_Event event;

	spriteMario->setFrameRange(0,0);
	
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
			
			if  (event.key.keysym.sym == SDLK_SPACE) {
				AcaoMario = PULANDO;
				
				b2Vec2 impulso;
				b2Vec2 pos;
				impulso.x = 0;
				impulso.y = -280;
				pos = fisicaMario->GetWorldCenter();
				
				//Se o tempo for 0 e o personagem está colidindo com o mapa de colisão ele pode pular
				if (tempoEsperaPulo == 0)
				{
					cout << "Inicia pulo... " << endl;
					
				   if (estaColidindo)
						fisicaMario->ApplyLinearImpulse(impulso, pos); //Punção para fazer o personagem pular
					
					//Seta o tempo com 45 para ser decrementado no update
					tempoEsperaPulo = 45;
					
				}
				else cout << "Ja estou pulando...." << endl;

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

	if (keystate[SDLK_UP]==1) {

		game->setZoom(game->getZoom()+1);

		game->updateCamera();

	}

	if (keystate[SDLK_DOWN]==1) {

		game->setZoom(game->getZoom()-1);

		game->updateCamera();

	}


	if(keystate[SDLK_RIGHT]==1) {

		if(AcaoMario != PULANDO)
		{	
			fisicaMario->ApplyLinearImpulse(Direcao, PontoFinal);
		}

		spriteMario->setMirror(false);
		cout << "aplicando impulso DIR...."<< endl ;
		spriteMario->setFrameRange(0,1);
		//game->setXpan(game->getXpan()+1.5);
		//game->updateCamera();
		AcaoMario = CAMINHANDO;

		//Testa a colisão entre o Mario e o item
		for(int nCount = 0; nCount < Itens.size();){
			colisao = spriteMario->bboxCollision(Itens[nCount]);
			if(colisao){
				//remove o item do vetor e da camada
				layers->remove(Itens[nCount]);
				Itens.erase(Itens.begin()+ nCount);
			}
			else
			{
				nCount ++;
			}
		}

	}
	



	if(keystate[SDLK_LEFT]==1) {

//		AcaoMario = CAMINHANDO;

		fisicaMario->ApplyLinearImpulse(-Direcao, PontoFinal);
		spriteMario->setMirror(true);
		spriteMario->setFrameRange(0,1);
		cout << "aplicando impulso ESQ...."<< endl ;
		

	}
	
}

void PlayFisicaState::update(CGame* game) {

	spriteMario->update(game->getUpdateInterval());
	//Percorre todo o vetor de blocks para poder animar
	for(int i=0; i < (int)QuestionBlocks.size(); i ++){ 
		//Usado para animar os sprites
		QuestionBlocks[i]->update(game->getUpdateInterval()); 
	}

	for(int i=0; i < (int)Itens.size(); i ++){ 
		//Usado para animar os sprites
		Itens[i]->update(game->getUpdateInterval()); 
		Itens[i]->setFrameRange(6,6);
	}

	Fisica->step();

	if (Fisica->haveContact(OBSTACULO1+1, MARIO_ID)) {
		cout << "Contato !!!"<< endl;
	}
	game->setXpan(spriteMario->getX()-150);
	game->updateCamera();

	b2Vec2 pos;
	pos = fisicaMario->GetPosition();
	//cout << "X = "<< pos.x << " Y = " << pos.y << endl;

	PontoFinal = fisicaMario->GetWorldCenter();
	Direcao = b2Vec2(25,0);

	EstadosMario();

    // Controle do pulo do Mario		
	if (tempoEsperaPulo > 0)
	{
		//cout << "pulando..." << endl;
		estaColidindo = false;
		tempoEsperaPulo--;

		if (tempoEsperaPulo == 0)
		{
			estaColidindo = TemColisaoSpriteTile(spriteMario, mapColisao); //Se o tempo for 0 ele já via estar colidindo com alguma coisa do mapa de colisão, logo é possível atriibuir a função TemColisaoSpriteTile que vai retornar "true"
		}
	}
	
		for(int nCount = 0; nCount < (int)VetInimigos.size(); nCount++){
			VerificaColisao(VetInimigos[nCount], nCount);
		}
		
		for(int nCount = 0; nCount < (int)QuestionBlocks.size(); nCount++){
			VerificaColisaoQuestionBlocks(QuestionBlocks[nCount]);
		}
		
}

void PlayFisicaState::draw(CGame* game) {
	glClearColor(0.59, 0.84, 0.91, 0);

	glEnable(GL_TEXTURE_2D); // isto é necessário quando se deseja desenhar com texturas

	// Enable transparency through blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	layers->draw();

	glDisable(GL_TEXTURE_2D); // isto é necessário quando se deseja desenhar SEM texturas
	glColor3f(0,0,0);
	glPointSize(5);

	glColor3f(1,1,1);

	Fisica->debugDraw();

	b2Vec2 fim;
	// converte de coordenadas da física para coordenadas de tela
	fim.x = PontoFinal.x * 10;
	fim.y = PontoFinal.y * 10;

	// Traca vetor de impulso
	b2Vec2 inicio;
	inicio.x = fim.x - Direcao.x;
	inicio.y = fim.y - Direcao.y;
	//DesenhaLinha( inicio,  fim);

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
