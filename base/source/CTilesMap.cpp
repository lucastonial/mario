/*
 *  CTilesMap.cpp
 *
 *  Created by Isabel Manssour / Marcio Pinho on 05/11.
 *  Copyright 2011 PUCRS. All rights reserved.
 *
 */

#include <fstream>
#include <iomanip>

#include "CTilesMap.h"

using namespace std;


CTilesMap::CTilesMap() {
}


// Método que recebe o nome do arquivo que contem a matriz de tiles
// que sera usada para fazer o background.
bool CTilesMap::loadMap(const char nomeArq[]) {
	char aux[40];
	int ind;

	ifstream arq;
	arq.open(nomeArq, ios::in);

	if ( !arq.is_open() ) {
		return false;
	} else {
		arq >> aux >> widthTileMap;
		arq >> aux >> heightTileMap;
		for(int lin=0; lin<heightTileMap; lin++) {
			for (int col=0; col<widthTileMap; col++) {
				arq >> ind;
				map.push_back(ind-1); // este -1 está aqui pois as sprites do TMX iniciam em 1
			}
		}
		arq.close();
	}
	return true;


}
// **********************************************************
//
//
// **********************************************************
bool CTilesMap::loadTileMap(const char nomeArq[], int w, int h, int hSpace, int vSpace, int xIni, int yIni,
                            int column, int row, int total) {
	if(!loadMultiImage(nomeArq,w,h,hSpace,vSpace,xIni,yIni,column,row,total))
		return false;
	return true;
}

// **********************************************************
// Funcao que, a partir da matriz de tiles, desenha o background.
// O valor armazenado em "map" eh um indice para o vetor de subbitmaps
// da classe cImage. Os tiles são desenhados a partir da posicao
// x=0 e y=0, sendo x e y incrementados de acordo com a largura e
// altura do tile.
// **********************************************************
void CTilesMap::draw() {

	//cout << "CTilesMap::draw - Desenhado o map de tiles..."<< flush;
	//cout << "Largura: "<< getWidth() << " Altura: " << getHeight() << endl;
	int cont = 0;
	int x = 0;
	int y = 0;
	cont = 0;

	for(int lin=0; lin<heightTileMap; lin++) {
		for (int col=0; col<widthTileMap; col++) {
			setPosition(x,y);
			drawFrame(map[cont]);
			cont++;
			x += getWidth();
		}
		x = 0;
		y += getHeight();
	}
	//cout << "FIM." << endl;
}

// **********************************************************
//
//
// **********************************************************
void CTilesMap::printMap() {

	cout << "PrintMap..." << endl;
	int cont =0;
	cout << "Print Map"<< endl;
	for(int lin=0; lin<heightTileMap; lin++) {
		cout << "Linha " << lin << ": ";
		for (int col=0; col<widthTileMap; col++) {
			cout << setw (4) << map[cont];
			cont ++;
		}
		cout << endl;
	}
	cout << "Print Map: END;"<< endl;
}

int CTilesMap::getTileNumber(int col, int lin) // x, y
{
	return map[lin*widthTileMap+col] + 1; // // este +1 está aqui pois as sprites do TMX iniciam em 1
}
