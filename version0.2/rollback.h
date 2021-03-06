#ifndef ROLLBACK_H_
#define ROLLBACK_H_

#include "base.h"

// 重复局面形态
const int REP_WIN = 1;
const int REP_LOSE = -1;
const int REP_DRAW = 0;
const int REP_NONE = -99;
const int RBL_MAXN = 128;

// 回滚哈希
const int rbHashNum = 1 << 20;
const int rbHashMask = rbHashNum - 1;
extern int RollBackHash [ rbHashNum ];

// 回滚结构体
struct RollBackListStruct {
	int move[ RBL_MAXN ]; // 着法
	int dstPiece[ RBL_MAXN ]; // 终点棋子
	bool check[ RBL_MAXN ]; // 将军态
	bool checked[ RBL_MAXN ]; // 被将军态
	bool chased[ RBL_MAXN ]; // 被捉态
	// ... // 捉子局面
	std::pair<ULL, ULL> zobrist [ RBL_MAXN ]; // zobrist值
	int nRollNum; // 回滚表长度

	void Init ( void ); // 初始化
	int LastMove ( void ) const { // 上一步着法
		return nRollNum == 0 ? 0 : move[ nRollNum - 1 ];;
	}
	int LastDstPiece ( void ) const { // 上一步终点棋子
		return nRollNum == 0 ? 0 : dstPiece[ nRollNum - 1 ];
	}
	bool LastCheck ( void ) const { // 上一步将军态
		return nRollNum == 0 ? false : check[ nRollNum - 1 ];
	}
	bool LastChecked ( void ) const { // 上一步被将军态
		return nRollNum == 0 ? false : checked[ nRollNum - 1 ];
	}
	bool LastChased ( void ) const { // 上一步被捉态
		return nRollNum == 0 ? false : chased[ nRollNum - 1 ];
	}
	std::pair<ULL, ULL> LastZobrist ( void ) const { // 上一步zobrist值
		return zobrist [ nRollNum - 1 ];
	}

	int RepStatus ( void ) const; // 重复局面形态
	int RepValue ( const int vRep ) const; // 重复局面打分
};


#endif /* ROLLBACK_H_ */
