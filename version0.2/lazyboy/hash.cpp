/*
 * hash.cpp
 *
 *  Created on: 2017年3月25日
 *      Author: peteryuanpan
 */

#include "base.h"

ULL ZobristSP [256][14];
ULL ZobristPlayer;

ULL rand64 ( void ) {
	return ((ULL)rand()) ^ ((ULL)rand() << 15) ^ ((ULL)rand() << 30) ^ ((ULL)rand() << 45);
}

void InitZobrist () {
	srand (time(NULL));
	for ( int i = 0; i < 256; i ++ ) {
		for ( int j = 0; j < 14; j ++ ) {
			ZobristSP[i][j] = rand64 ();
		}
	}
	ZobristPlayer = rand64 ();
}

// 生成局面的zobrist值
void PositionStruct::GenZobrist ( void ) {

}

// 判断 在到根路径上重复
bool PositionStruct::Duplicated ( void ) const {
	return false;
}
