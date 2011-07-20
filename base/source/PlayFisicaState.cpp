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
	string nomeArq = BASE_DIR + "data/img/SpritesMario.png";
	spriteMario = new CSprite();
	spriteMario->loadSprite(nomeArq.c_str(), 18, 31, 0, 0, 0, 0, 4, 4, 13);
	spriteMario->setScale(1.0);
	spriteMario->setFrameRange(0,0);

	spriteMario->setPosition(200,400);

	spriteMario->setAnimRate(4); // taxa de animação em frames por segundo(troca dos frames dele)

	
	
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

	//Chama o método para carregar os Goombas
	CarregaGoomba("data/img/Goomba.png", 600, 448);
	CarregaGoomba("data/img/Goomba.png", 1312, 448); //Coluna 41, Linha 14
	//CarregaGoomba("data/img/Goomba.png", 1500, 450);
	//CarregaGoomba("data/img/Goomba.png", 2000, 450);
	//CarregaGoomba("data/img/Goomba.png", 2500, 450);


    //Chama o método para carregar o único Koopa Troopa da fase
	CarregaKoopaTroopas("data/img/KoopaTroopa.png", 400, 450);


}

void PlayFisicaState::CarregaQuestionBlocks(string path, float positionX, float positionY)
{
	string nomeArq = BASE_DIR + path;

	insereQuestionBlocks = new CSprite();
	insereQuestionBlocks->loadSprite(nomeArq.c_str(), 31, 32, 0, 0, 0, 0, 2, 1, 2);
	insereQuestionBlocks->setScale(1);
	insereQuestionBlocks->setPosition(positionX, positionY);

	insereQuestionBlocks->setAnimRate(15); // taxa de animação em frames por segundo(troca dos frames dele)
	QuestionBlocks.push_back(insereQuestionBlocks);


}

void PlayFisicaState::CarregaGoomba(string path, float positionX, float positionY)
{
	string nomeArq = BASE_DIR + path;

	spriteGoomba = new CSprite();
	spriteGoomba->loadSprite(nomeArq.c_str(), 23, 32, 0, 0, 0, 0, 3, 1, 3);
	spriteGoomba->setScale(1.3);
	spriteGoomba->setPosition(positionX, positionY);

	spriteGoomba->setAnimRate(4); // taxa de animação em frames por segundo(troca dos frames dele)
	VetGoomba.push_back(spriteGoomba);

}


void PlayFisicaState::CarregaKoopaTroopas(string path, float positionX, float positionY)
{
	string nomeArq = BASE_DIR + path;
	
	spriteKoopaTroopa = new CSprite();
	spriteKoopaTroopa->loadSprite(nomeArq.c_str(), 24, 32, 0, 0, 0, 0, 6, 1, 6);
	spriteKoopaTroopa->setScale(1.5);
	spriteKoopaTroopa->setMirror(false);

	spriteKoopaTroopa->setPosition(300, 250);
	spriteKoopaTroopa->setAnimRate(2); // taxa de animação em frames por segundo(troca dos frames dele)

}

void PlayFisicaState::CarregaMushroons(string path, float positionX, float positionY) // booleano para assegurar que a função exute apenas uma vez
{
		string nomeArq = BASE_DIR + path;

		insereItem = new CSprite();
		insereItem->loadSprite(nomeArq.c_str(), 32, 32, 0, 0, 0, 0, 7, 1, 7);
		insereItem->setScale(1);
		insereItem->setPosition(positionX, positionY);

		insereItem->setAnimRate(5); // taxa de animação em frames por segundo(troca dos frames dele)
		Mushroons.push_back(insereItem);


}

void PlayFisicaState::InicializaFisicaMushroons()
{

#define MUSHROOM
#ifdef MUSHROOM

	//INICIALIZAÇÃO (CRIA CAIXAS DE FÍSICA PARA OS MUSHROONS)
	for (int nCount = 0; nCount < Mushroons.size(); nCount++){
		auxiliar = Mushroons[nCount];
		fisicaMushroom = Fisica->newBoxImage(MUSHROOM_ID,    //int id,
			auxiliar,                // CImage* sprite,
			0.0,                // float density,
			0.3,            // float friction,
			0.3,            // float restitution
			1.0,			 // float linearDamping
			1.0,			 // float angularDamping
			false);        // bool staticObj=false
		vectorFisicaMushroom.push_back(fisicaMushroom);	
		
	}
#endif
	
}

void PlayFisicaState::InicializaFisicaGoombas()
{

#define GOOMBA
#ifdef GOOMBA
	for(int nCount = 0; nCount < VetGoomba.size(); nCount++)
	{
		auxiliar = VetGoomba[nCount];
		fisicaGoomba = Fisica->newBoxImage(GOOMBA_ID,    //int id,
			auxiliar,                // CImage* sprite,
			0.0,                // float density,
			0.2,            // float friction,
			0.2,            // float restitution
			1.5,			 // float linearDamping
			0.0,			 // float angularDamping
			false);        // bool staticObj=false
		vectorFisicaGoombas.push_back(fisicaGoomba);
	}
#endif

}

void PlayFisicaState::InicializaFisicaKoopaTroopa()
{

#define KOOPATROOPA
#ifdef KOOPATROOPA

		auxiliar = spriteKoopaTroopa;
		fisicaKoopaTroopa = Fisica->newBoxImage(KOOPATROOPA_ID,    //int id,
			auxiliar,                // CImage* sprite,
			0.0,                // float density,
			0.3,            // float friction,
			0.2,            // float restitution
			0.5,			 // float linearDamping
			0.5,			 // float angularDamping
			false);        // bool staticObj=false

#endif

}

//verifica colisao do mario
void PlayFisicaState::VerificaColisao(CSprite *inimigo){
	//colisao lado
	if(inimigo->getX()<(spriteMario->getX()+25) && spriteMario->getX()<(inimigo->getX()+inimigo->getWidth()+10)){
		if(inimigo->getY()<(spriteMario->getY()+20) && spriteMario->getY()<(inimigo->getY()+inimigo->getHeight())){
			cout << "****LADO****" << endl;
			VarTipoColisao = LADO;
		}else
			VarTipoColisao = NADA; //se nao for Lado seta para Nada para permitir que teste se esta em cima, pois se não fizer isso ele sempre vai entrar no teste de cima
	}
		
	if(VarTipoColisao != LADO){ // se não colidiu de lado permitimos o teste para colisao de cima, ai não tem perigo de morrer e depois matar
		//colisao cima
		//REFAZER ESSA CONDIÇÃO DE COLISÃO
		if(inimigo->getX()<(spriteMario->getX()+15) && spriteMario->getX()<(inimigo->getX()+inimigo->getWidth()+5)){
			if((inimigo->getY())<(spriteMario->getY()+45) && spriteMario->getY()<(inimigo->getY()+inimigo->getHeight())){
				cout << "****CIMA****" << endl;
				VarTipoColisao = CIMA;
				colisao = true;
			}
		}
	}
}

void PlayFisicaState::VerificaColisaoQuestionBlocks(CSprite *questionBlock, int identificador){	
	//colisao baixo
	if(questionBlock->getX()<(spriteMario->getX()) && spriteMario->getX()<(questionBlock->getX()+questionBlock->getWidth())){
		if(questionBlock->getY()<(spriteMario->getY()) && spriteMario->getY()<(questionBlock->getY()+questionBlock->getHeight()+2)){
			cout << "****BAIXO****" << endl;
			
			layers->remove(questionBlock);
			//QuestionBlocks.erase(QuestionBlocks.begin()+identificador);
			VarTipoColisao = BAIXO;
			cout << identificador << endl;

			//Se colide com o questionblock[1] carrega esse cogumelo
			if (identificador == 1 && !carregouMushroom1) {
			    CarregaMushroons("data/img/Mushroom.png", 928, 320); //Coluna 29, Linha 10
				carregouMushroom1 = true;
				impedeCrashMoveMushroom = true;
				tempoParaMover = 30;
				podeCriarFisicaMushroom = true;

			}
			else if(identificador == 4 && !carregouMushroom2) {//Se colide com o questionblock[4] carrega esse cogumelo
				CarregaMushroons("data/img/Mushroom.png", 3104, 320); //Coluna 97, Linha 10
				carregouMushroom2 = true;
				impedeCrashMoveMushroom = true;
				tempoParaMover = 30;
				podeCriarFisicaMushroom = true;
			}
			else if (identificador == 8 && !carregouMushroom3) {//Se colide com o questionblock[8] carrega esse cogumelo
				CarregaMushroons("data/img/Mushroom.png", 4224, 320); //Coluna 132, Linha 6
				carregouMushroom3 = true;
				impedeCrashMoveMushroom = true;
				tempoParaMover = 30;
				podeCriarFisicaMushroom = true;
			}

			//ADICIONA OS SPRITES DOS MUSHROONS NAS CAMADAS
			for(int nCount = 0; nCount < (int)Mushroons.size(); nCount++)
			{
				layers->add(Mushroons[nCount],1);
			}

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
			//bool teste = spriteMario->bboxCollision(spriteKoopaTroopa); //colisao exemplo
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
void PlayFisicaState::MoveMushroom()
{
	
	for (int nCount = 0; nCount < Mushroons.size(); nCount++)
	{

		int xMax = (Mushroons[nCount]->getX()+Mushroons[nCount]->getWidth()*2)/32;
		int xMin = (Mushroons[nCount]->getX()-Mushroons[nCount]->getWidth())/32;
		int tileMax = mapColisao->getTileNumber(xMax, Mushroons[nCount]->getY()/32);
		int tileMin = mapColisao->getTileNumber(xMin, Mushroons[nCount]->getY()/32);

		vectorFisicaMushroom[nCount]->ApplyLinearImpulse(DirecaoMushroons,PontoFinalMushroons);
	
		if (tileMax != 192) {

			cout << "O NUMERO DO TILE EH: " << tileMax << endl;
			DirecaoMushroons = b2Vec2(-1.3,0);

		}	

		if (tileMin != 192){

			DirecaoMushroons = b2Vec2(1.3,0);

		}



	}

}

void PlayFisicaState::MoveGoombas()
{
	
	for (int nCount = 0; nCount < VetGoomba.size(); nCount++)
	{

		int xMax = (VetGoomba[nCount]->getX()+VetGoomba[nCount]->getWidth()*2)/32;
		int xMin = (VetGoomba[nCount]->getX()-VetGoomba[nCount]->getWidth())/32;
		int tileMax = mapColisao->getTileNumber(xMax, VetGoomba[nCount]->getY()/32);
		int tileMin = mapColisao->getTileNumber(xMin, VetGoomba[nCount]->getY()/32);

		vectorFisicaGoombas[nCount]->ApplyLinearImpulse(DirecaoGoombas,PontoFinalGoombas);
	
		if (tileMax != 192) {

			cout << "O NUMERO DO TILE EH: " << tileMax << endl;
			DirecaoGoombas = b2Vec2(-1.4,0);

		}	

		if (tileMin != 192){

			DirecaoGoombas = b2Vec2(1.4,0);

		}



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
	layers->add(spriteKoopaTroopa,1);
	for(int nCount = 0; nCount < (int)QuestionBlocks.size(); nCount++)
	{
		layers->add(QuestionBlocks[nCount],1);
	}
	
	for(int nCount = 0; nCount < (int)VetGoomba.size(); nCount++){
		layers->add(VetGoomba[nCount],1);
	}
	
	//for(int nCount = 0; nCount < (int)Mushroons.size(); nCount++)
	//{
	//	layers->add(Mushroons[nCount],1);
	//}
	
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

	
	//INICIALIZAÇÃO (CRIA CAIXAS DE FÍSICA PARA OS GOOMBAS)
	InicializaFisicaKoopaTroopa();

	//INICIALIZAÇÃO (CRIA CAIXAS DE FÍSICA PARA OS GOOMBAS)
	InicializaFisicaGoombas();

	CriaMapDeColisao();
	
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(fisicaMario,fisicaGoomba, fisicaMario->GetWorldCenter());

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
    
	//inicializa a variável de colisão do Mario (define o lado que ele colide com os inimigos
	VarTipoColisao = NADA;

	//inicializa tipo do item = cogumelo
	tipoItem = 1;

	//inicializa variáveis para controlar se os mushroons estão carregados
	carregouMushroom1 = false;
	carregouMushroom2 = false;
	carregouMushroom3 = false;

    impedeCrashMoveMushroom = false;

	podeCriarFisicaMushroom = false;


	//inicializa o vetor de direção dos goombas
	DirecaoGoombas = b2Vec2(1.4,0);

	//inicializa o vetor de direção dos mushroons
	DirecaoMushroons = b2Vec2(1.3,0);

	tempoEsperaPulo = 0; ////inicia com 0 para entrar no if do pulo
	colisao = false;//inicia como false a variável auxiliar que controla a colisão entre os sprites
	
	AcaoMario = PARADO;
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

	//ESSA FUNÇÃO PRECISA ESTAR AQUI PARA CARREGAR APENAS 1 COGUMELO A CADA COLISÃO. 
	//SE FICAR NO UPDATE SÃO CARREGADOS 3 COGUMELOS POR VEZ (O QUE É ERRADO)
	for(int nCount = 0; nCount < (int)QuestionBlocks.size(); nCount++){

		VerificaColisaoQuestionBlocks(QuestionBlocks[nCount], nCount);		
	}

	if(VarEstadosMario == INICIAL)
		spriteMario->setScale(1);

	if (AcaoMario != PULANDO){
		switch(VarEstadosMario)
		{
		case INICIAL:
			spriteMario->setFrameRange(0,0);
			break;

		case COGUMELO:
			spriteMario->setFrameRange(5,5);
			break;
		case FLOR:
			spriteMario->setFrameRange(9,9);
		}
	}

	
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
			else if (event.key.keysym.sym == SDLK_b){
				fisicaKoopaTroopa->SetActive(false);
				
			}
			
			if  (event.key.keysym.sym == SDLK_SPACE) {

						
				AcaoMario = PULANDO;

				if(VarEstadosMario == INICIAL)
				{
					spriteMario->setFrameRange(3,3);

				}
				else if(VarEstadosMario == COGUMELO)
				{
					spriteMario->setFrameRange(8,8);
				}
				else if (VarEstadosMario == FLOR)
				{
					spriteMario->setFrameRange(12,12);
				}

				b2Vec2 impulso;
				b2Vec2 pos;
				impulso.x = 0;
				impulso.y = -200;
				pos = fisicaMario->GetWorldCenter();

				while(noChao){
					noChao = false;
					cout << "*****CHAO*****" << endl;
					fisicaMario->ApplyLinearImpulse(impulso, pos);

				}


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
	
	if (keystate[SDLK_SPACE]==1) {
	
		AcaoMario = PULANDO;

		if(VarEstadosMario == INICIAL)
		{
			spriteMario->setFrameRange(3,3);

		}
		else if(VarEstadosMario == COGUMELO)
		{
			spriteMario->setFrameRange(8,8);
		}
		else if (VarEstadosMario == FLOR)
		{
			spriteMario->setFrameRange(12,12);
		}

	//	AcaoMario = PULANDO;

	//	b2Vec2 impulso;
	//	b2Vec2 pos;
	//	impulso.x = 0;
	//	impulso.y = -200;
	//	pos = fisicaMario->GetWorldCenter();

	//	if (tempoEsperaPulo == 0){
	//		while(noChao){
	//			noChao = false;
	//			cout << "*****CHAO*****" << endl;
	//			fisicaMario->ApplyLinearImpulse(impulso, pos);
	//		}
	//		tempoEsperaPulo = 10;
	//}

	}


	if(keystate[SDLK_RIGHT]==1) {
		
		PontoFinal = fisicaMario->GetWorldCenter();
		Direcao = b2Vec2(55,0);	
		
		fisicaMario->ApplyLinearImpulse(Direcao, PontoFinal);
		
		spriteMario->setMirror(false);
		
		if(AcaoMario == PULANDO){
			if(VarEstadosMario == INICIAL)
			{
				spriteMario->setFrameRange(3,3);

			}
			else if(VarEstadosMario == COGUMELO)
			{
				spriteMario->setFrameRange(8,8);
			}
			else if (VarEstadosMario == FLOR)
			{
				spriteMario->setFrameRange(12,12);
			}
		}

		AcaoMario = CAMINHANDO;
		if(VarEstadosMario == INICIAL)
		{
			spriteMario->setFrameRange(0,2);
		}
		else if(VarEstadosMario == COGUMELO)
		{
			spriteMario->setFrameRange(5,7);
		}
		else if (VarEstadosMario == FLOR)
		{
			spriteMario->setFrameRange(9,11);
		}



	}
	
	if(keystate[SDLK_LEFT]==1) {

		PontoFinal = fisicaMario->GetWorldCenter();
		Direcao = b2Vec2(55,0);

		fisicaMario->ApplyLinearImpulse(-Direcao, PontoFinal);
		spriteMario->setMirror(true);
		
		if(AcaoMario == PULANDO){
			if(VarEstadosMario == INICIAL)
			{
				spriteMario->setFrameRange(3,3);
				spriteMario->setScale(1);

			}
			else if(VarEstadosMario == COGUMELO)
			{
				spriteMario->setFrameRange(8,8);
			}
			else if (VarEstadosMario == FLOR)
			{
				spriteMario->setFrameRange(12,12);
				spriteMario->setScale(2);
			}
		}

		if(VarEstadosMario == INICIAL)
		{
			spriteMario->setFrameRange(0,2);
			spriteMario->setScale(1);
		}
		else if(VarEstadosMario == COGUMELO)
		{
			spriteMario->setFrameRange(5,7);
		}
		else if (VarEstadosMario == FLOR)
		{
			spriteMario->setFrameRange(9,11);
		}

		AcaoMario = CAMINHANDO;


		

	}
	
}

void PlayFisicaState::update(CGame* game) {

	spriteMario->update(game->getUpdateInterval());
	//Percorre todo o vetor de blocks para poder animar
	for(int i=0; i < (int)QuestionBlocks.size(); i ++){ 
		//Usado para animar os sprites
		QuestionBlocks[i]->update(game->getUpdateInterval()); 
	}

	for(int i=0; i < (int)Mushroons.size(); i ++){ 
		//Usado para animar os sprites
		Mushroons[i]->update(game->getUpdateInterval()); 
		Mushroons[i]->setFrameRange(6,6);
	}


	spriteKoopaTroopa->update(game->getUpdateInterval());
	spriteKoopaTroopa->setFrameRange(0,1);


	Fisica->step();

	if (Fisica->haveContact(KOOPATROOPA_ID+1, MARIO_ID)) {
		cout << "Contato !!!"<< endl;
	}
	game->setXpan(spriteMario->getX()-150);
	game->updateCamera();

	b2Vec2 pos;
	pos = fisicaMario->GetPosition();
	//cout << "X = "<< pos.x << " Y = " << pos.y << endl;

	PontoFinalGoombas = fisicaGoomba->GetWorldCenter();
	MoveGoombas();

	if (podeCriarFisicaMushroom)
	{
		//CHAMA A FUNÇÃO PARA CRIAR AS CAIXAS DE FÍSICA
		InicializaFisicaMushroons();
		podeCriarFisicaMushroom = false;
	}

	if (impedeCrashMoveMushroom) 
	{
		PontoFinalMushroons = fisicaMushroom->GetWorldCenter();
		if (tempoParaMover > 0){
			tempoParaMover--;
		}
		else
		{
			MoveMushroom();
		}
	}
    
	

	// Controle do pulo do Mario	
	int y = (spriteMario->getY()+spriteMario->getHeight()*2)/32;

	if (mapColisao->getTileNumber(spriteMario->getX()/32, y) != 192){

		if(mapColisao->getTileNumber(spriteMario->getX()/32, y) == 192)
			cout << "O NUMERO DO TILE EH: " << mapColisao->getTileNumber(spriteMario->getX()/32, y) << endl;

		noChao = true;
	}

	EstadosMario();	



		//Testa a colisão entre o Mario e o item
		for(int nCount = 0; nCount < Mushroons.size();){
			colisao = spriteMario->bboxCollision(Mushroons[nCount]);
			if(colisao){
				//remove o item do vetor e da camada
				layers->remove(Mushroons[nCount]);
				Mushroons.erase(Mushroons.begin()+ nCount);
				vectorFisicaMushroom[nCount]->SetActive(false);
				vectorFisicaMushroom.erase(vectorFisicaMushroom.begin()+nCount);
				impedeCrashMoveMushroom = false;
				VarEstadosMario = COGUMELO;
				spriteMario->setCurrentFrame(5);


			}
			else
			{
				nCount ++;
			} 
		}
		

		for(int nCount = 0; nCount < VetGoomba.size();)
		{
	
			VerificaColisao(VetGoomba[nCount]);
			if (colisao == true && VarTipoColisao == CIMA)
			{

				vectorFisicaGoombas[nCount]->SetActive(false);
				layers->remove(VetGoomba[nCount]);
				VetGoomba.erase(VetGoomba.begin()+nCount);
				vectorFisicaGoombas.erase(vectorFisicaGoombas.begin()+nCount);
				VarTipoColisao = NADA;
			}
			else
			{
				VerificaColisao(VetGoomba[nCount]);

				VetGoomba[nCount]->update(game->getUpdateInterval()); 
				VetGoomba[nCount]->setFrameRange(0,1);
				nCount++;
			}
			
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
	int identificador = GOOMBA_ID + 5; // identificador para cada tile
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
