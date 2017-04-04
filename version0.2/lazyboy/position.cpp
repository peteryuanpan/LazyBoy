/*
 * position.cpp
 *
 *  Created on: 2017年3月25日
 *      Author: peteryuanpan
 */

#include "base.h"

// 执棋方将军
bool PositionStruct::Check ( void ) const {
	return false;
}

// 执棋方被将军
bool PositionStruct::Checked ( void ) const {
	return false;
}

// 走一步棋
void PositionStruct::MakeMove ( const int mv ) {

}

// 撤回走法
void PositionStruct::UndoMakeMove ( const int mv ) {

}

// 生成吃子着法
int PositionStruct::GenCapMove ( int *move ) const {
	return 0;
}

// 生成非吃子着法
int PositionStruct::GenNonCapMove ( int *move ) const {
	return 0;
}

// 生成所有着法
int PositionStruct::GenAllMove ( int *move ) const {
	int num = 0;
	num += GenCapMove ( move + num );
	num += GenNonCapMove ( move + num );
	return num;
}


