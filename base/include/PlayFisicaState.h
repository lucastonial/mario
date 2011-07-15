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
			void CarregaInimigos(string path, float positionX, float positionY);
			void CarregaItens(string path, float positionX, float positionY);
			void EstadosMario();
			void AcionaItens();
			void VerificaColisaoQuestionBlocks();
			void VerificaColisao(CSprite *inimigo);
		
    private:
			static PlayFisicaState m_PlayState;
			CLayerHandler *layers;
			CTilesMap *mapFundo;
			CTilesMap *mapColisao;
			CSprite *spriteMario;
			CSprite *spriteCobra;
			CSprite *spriteInimigos;
			CSprite *insereQuestionBlocks;
			CSprite *insereItem;

      Uint8* keystate; // state of all keys (1 means key is pressed)

			int currentFrame;
			bool done;

			string BASE_DIR;
		
			CPhysics* Fisica;
			b2Body* fisicaMario;
			b2Body* fisicaPiso;
			b2Body* fisicaCobra;
		
			enum {
				MARIO_ID, COBRA_ID, OBSTACULO1, BOLA, 
			};

			//enum para verificar qual ação o Mario está fazendo
			enum ACOES_MARIO {
					PULANDO, CAMINHANDO, CORRENDO
			}; ACOES_MARIO AcaoMario;
			
			//enum que seta o tipo de colisao
			enum TIPO_COLISAO {
				CIMA, BAIXO, LADO
			}; TIPO_COLISAO VarTipoColisao;
			
			//enum que seta os estados do Mario
			enum ESTADOS_MARIO {
				INICIAL, COGUMELO, FLOR, MORTE 
			}; ESTADOS_MARIO VarEstadosMario;
			
			//seta o tipo do item que vai cair dos QuestionBlocks: se for 1 == cogumelo se 2 == flor
			int tipoItem;
			
			//Vetor de Question Blocks
			vector<CSprite *> QuestionBlocks;
			
			vector<CSprite *> VetInimigos;

			//Vetor de itens
			vector<CSprite *> Itens;

			//variável usada para ver se o Mario está colidingo com o mapa de tiles
			bool estaColidindo;

			//variável para controlar a colisão entre Sprites
			bool colisao;

			//contador que permite o Mario pular, toda vez que for 0 e for pressionado a tecla de espaço
			int tempoEsperaPulo;
		
			b2Vec2 Direcao;
			b2Vec2 PontoFinal;
			
			b2RevoluteJoint* joint; 

		
};

#endif
