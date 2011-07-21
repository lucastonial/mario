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
      void MontaLayer();
			bool TemColisaoSpriteTile(CSprite *sprite, CTilesMap *map);
			void InitFisica();
			void DesenhaBordaImagem(CImage* i);
			void DesenhaLinha(b2Vec2 pontoInicial, b2Vec2 pontoFinal) ;
			void DesenhaLinha(float x1, float y1, float x2, float y2);
			void CriaMapDeColisao();
			void CriaCaixa(int id, float x, float y, float w, float h);
			void CarregaQuestionBlocks(string path, float positionX, float positionY);
			void CarregaGoomba(string path, float positionX, float positionY);
			void CarregaKoopaTroopas(string path, float positionX, float positionY);
			void CarregaMushroons(string path, float positionX, float positionY);
			void CarregaFireFlower(string path, float positionX, float positionY);
			void InicializaFisicaMushroons();
			void InicializaFisicaGoombas();
			void InicializaFisicaKoopaTroopa();
			void EstadosMario(CGame* game);
			void VerificaColisaoQuestionBlocks(CSprite *questionBlock, int identificador);
			void VerificaColisao(CSprite *inimigo); //passa sprite do inimigo + seu ID para poder eliminá-lo
			void MoveGoombas();
			void MoveMushroom();
			void CarregaTiros(); //instancia os tiros
			void RemoveGoombasPeloAndamentoCenario();
			void CarregaSons(CGame* game);
			void MoveKoopaTroopa();
		
    private:
			static PlayFisicaState m_PlayState;
			CLayerHandler *layers;
			CTilesMap *mapFundo;
			CTilesMap *mapColisao;
			CSprite *spriteMario;
			CSprite *spriteKoopaTroopa;
			CSprite *spriteGoomba;
			CSprite *insereQuestionBlocks;
			CSprite *insereMushroom;
			CSprite *insereFireFlower;
			CSprite *testeTiro;
			
			//USADO PARA CARREGAR AS CAIXAS DE FÍSICA DOS PERSONAGENS E DOS ITENS
			CSprite *auxiliar;

      Uint8* keystate; // state of all keys (1 means key is pressed)

			int currentFrame;
			bool done;

			string BASE_DIR;
		
			CPhysics* Fisica;
			b2Body* fisicaMario;
			b2Body* fisicaPiso;
			b2Body* fisicaMushroom;
			b2Body* fisicaGoomba;
			b2Body* fisicaKoopaTroopa;
			b2Body* fisicaTiros;

			vector<b2Body *> vectorFisicaMushroom;
			vector<b2Body *> vectorFisicaGoombas;
			vector<b2Body *> vectorFisicaTiros;

			
		
			enum {
				MARIO_ID, KOOPATROOPA_ID, GOOMBA_ID , MUSHROOM_ID 
			};

			//enum para verificar qual ação o Mario está fazendo
			enum ACOES_MARIO {
				PARADO, PULANDO, CAMINHANDO, CORRENDO
			}; ACOES_MARIO AcaoMario;
			
			//enum que seta o tipo de colisao
			enum TIPO_COLISAO {
				NADA, CIMA, BAIXO, LADO
			}; TIPO_COLISAO VarTipoColisao;
			
			//enum que seta os estados do Mario
			enum ESTADOS_MARIO {
				INICIAL, COGUMELO, FLOR, MORTE 
			}; ESTADOS_MARIO VarEstadosMario;
			
			//seta o tipo do item que vai cair dos QuestionBlocks: se for 1 == cogumelo se 2 == flor
			int tipoItem;
			
			//Vetor de Question Blocks
			vector<CSprite *> QuestionBlocks;
			
			vector<CSprite *> VetGoomba;

			//Vetor de Mushroons
			vector<CSprite *> Mushroons;

			//Vetor de Fire Flower
			vector<CSprite *> FireFlower;
			
			//Vetor de tiros
			vector<CSprite *> vetTiros;

			//variável usada para impedir que seja carregado mais de um Item no mesmo local
			bool carregoubloco1;
			bool carregoubloco2;
			bool carregoubloco3;

			//variável usada para impedir que aconteça um crash na memória ao definir o PontoFinalGoombas no update
			//para um mushroom que foi removido do vetor
			bool impedeCrashMoveMushroom;


			bool noChao;

			//variável para controlar a colisão entre Sprites
			bool colisao;

			int contadorGoombas;
		
			b2Vec2 Direcao;
			b2Vec2 PontoFinal;

			//variáveis que controlam os movimentos dos goombas
			b2Vec2 DirecaoGoombas;
			b2Vec2 PontoFinalGoombas;

			//variáveis que controlam os movimentos dos koopa troopa
			b2Vec2 DirecaoKoopaTroopa;
			b2Vec2 PontoFinalKoopaTroopa;

			//variáveis que controlam os movimentos dos mushroons
			b2Vec2 DirecaoMushroons;
			b2Vec2 PontoFinalMushroons;

			//variavel para começar a mover o mushroom só depois que zerar
			int tempoParaMover;

			bool podeCriarFisicaMushroom;
			
			//timer se nao estiver no estado inicial e encostar lateralmente em um inimigo (timer para invencibilidade)
			double timerEstados;
			//tempo atual de execucao do jogo
			double current_time;

			//Variável para controlar se o som já está carregado
			bool somCarregado;

			//Variáveis para controlar os sons do jogo
			irrklang::ISoundSource *somTema; 
			irrklang::ISoundSource *somSalto;
	

			
			b2RevoluteJoint* joint; 

		
};

#endif
