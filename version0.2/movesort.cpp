#include "movesort.h"
#include "debug.h"
#include "search.h"
#include "hash.h"

// 历史表
LL HistoryTable [ 256 * 256 ];

// 清空历史表
void ClearHistoryTable ( void ) {
	memset ( HistoryTable, 0, sizeof HistoryTable );
}

// 更新历史表
void InsertHistoryTable ( const int mv, const int depth ) {
	if ( mv != 0 && !pos.GoodCapMove(mv) ) { // !
		HistoryTable[mv] += depth * depth;
	}
}

// 生成着法
int MoveSortStruct::InitMove ( void ) {
	// 1. 初始化
	memset ( move, 0, sizeof move );
	nMoveIndex = 0;
	nMoveNum = 0;

	// 2. 生成所有着法
	pos.GenAllMove ( move, nMoveNum );
	pos.DelMeaningLessMove ( move, nMoveNum );

	// 3. 获得置换表着法
	int bstmv = QueryMoveInHashTableWithoutLimit ();

	// 4. 给着法分类赋值
	int type[128];
	for ( int i = 0; i < nMoveNum; i ++ ) {
		type[i] = SORT_TYPE_OTHER;
		if ( bstmv == move[i] ) {
			type[i] = SORT_TYPE_BEST_MOVE;
		}
		else if ( pos.square[DST(move[i])] != 0 ) {
			const int v = pos.MvvLva ( SRC(move[i]), DST(move[i]) );
			if ( v > 0 ) {
				type[i] = SORT_TYPE_GOOD_CAP;
			}
			else {
				type[i] = SORT_TYPE_OTHER;
			}
		}
		else {
			type[i] = SORT_TYPE_OTHER;
		}
	}

	// 5. 按赋值降序排序
	int t = 0;
	for ( int k = MAX_SORT_TYPE; k >= 0; k -- ) {
		for ( int i = t; i < nMoveNum; i ++ ) {
			if ( type[i] == k ) {
				SWAP ( move[i], move[t] );
				SWAP ( type[i], type[t] );
				t ++;
			}
		}
	}

	// 6. 对GoodCap类按照得分降序排序
	for ( int i = 0; i < nMoveNum; i ++ ) {
		if ( type[i] == SORT_TYPE_GOOD_CAP ) {
			for ( int j = i + 1; j < nMoveNum; j ++ ) {
				if ( type[j] == SORT_TYPE_GOOD_CAP ) {
					const int vi = pos.MvvLva ( SRC(move[i]), DST(move[i]) );
					const int vj = pos.MvvLva ( SRC(move[j]), DST(move[j]) );
					if ( vi < vj ) {
						SWAP ( move[i], move[j] );
					}
				}
			}
		}
	}

	// 7. 对Other类按历史表降序排序
	for ( int i = 0; i < nMoveNum; i ++ ) {
		if ( type[i] == SORT_TYPE_OTHER ) {
			for ( int j = i + 1; j < nMoveNum; j ++ ) {
				if ( type[j] == SORT_TYPE_OTHER ) {
					if ( HistoryTable[move[i]] < HistoryTable[move[j]] ) {
						SWAP ( move[i], move[j] );
					}
				}
			}
		}
	}

	return nMoveNum;
}

// 生成好的吃子着法
int MoveSortStruct::InitGoodCapMove ( void ) {
	// 1. 初始化
	memset ( move, 0, sizeof move );
	nMoveIndex = 0;
	nMoveNum = 0;

	// 2. 生成所有吃子着法
	pos.GenCapMove ( move, nMoveNum );
	pos.DelMeaningLessMove ( move, nMoveNum );

	// 3. 获得每个着法的得分
	int vl[128];
	for ( int i = 0; i < nMoveNum; i ++ ) {
		vl[i] = pos.MvvLva ( SRC(move[i]), DST(move[i]) );
	}

	// 4. 将着法按照得分降序排序
	for ( int i = 0; i < nMoveNum; i ++ ) {
		for ( int j = i + 1; j < nMoveNum; j ++ ) {
			if ( vl[i] < vl[j] ) {
				SWAP ( move[i], move[j] );
				SWAP ( vl[i], vl[j] );
			}
		}
	}

	// 5. 去除得分非正的吃子着法
	for ( int i = nMoveNum - 1; i >= 0; i -- ) {
		if ( vl[i] <= 0 ) {
			nMoveNum --;
		}
	}

	return nMoveNum;
}

// 将5, 士1, 象1, 马3, 车4, 炮3, 兵2
const int SIMPLE_VALUE[7] = {5, 1, 1, 3, 4, 3, 2};

// 吃子着法估分
int PositionStruct::MvvLva ( const int src, const int dst ) const {
	const int valSrc = Protected2( 1-player, src, dst ) ? SIMPLE_VALUE[PIECE_TYPE[square[src]]] : 0;
	const int valDst = SIMPLE_VALUE[PIECE_TYPE[square[dst]]];
	if ( valDst >= valSrc ) {
		return valDst - valSrc + 1;
	}
	else if ( valDst >= 3 || IN_THIS_SIDE_BOARD(SIDE_TAG(player), dst) ) {
		return 1;
	}
	else {
		return 0;
	}
}

// 判断是否是好的吃子着法
bool PositionStruct::GoodCapMove ( const int mv ) const {
	const int xDst = square[DST(mv)];
	if ( xDst == 0 ) {
		return false;
	}
	if ( !Protected2(1-player, SRC(mv), DST(mv)) ) {
		return true;
	}
	const int typeSrc = PIECE_TYPE [ square[SRC(mv)] ];
	const int typeDst = PIECE_TYPE [ square[DST(mv)] ];
	return SIMPLE_VALUE[typeDst] > SIMPLE_VALUE[typeSrc];
}
