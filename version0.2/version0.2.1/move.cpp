#include "move.h"
#include "position.h"
#include "search.h"

int KING_HIT [256][10];
int ADVISOR_HIT [256][10];
int BISHOP_HIT [256][10];
int BISHOP_PIN [256][10];
int KNIGHT_HIT [256][10];
int KNIGHT_PIN [256][10];
int PAWN_HIT [256][2][10];

void InitMove ( void ) {
	int p = STA_POS;
	int t;

	// 1. 初始化
	for ( int i = 0; i < 256; i ++ ) {
		for ( int j = 0; j < 10; j ++ ) {
			 KING_HIT [i][j] = 0;
			 ADVISOR_HIT [i][j] = 0;
			 BISHOP_HIT [i][j] = 0;
			 BISHOP_PIN [i][j] = 0;
			 KNIGHT_HIT [i][j] = 0;
			 KNIGHT_PIN [i][j] = 0;
			 PAWN_HIT [i][0][j] = 0;
			 PAWN_HIT [i][1][j] = 0;
		}
	}

	// 2. 求结果
	while ( p != 0 ) {
		// 1. 生成将的HIT
		t = 0;
		for ( int i = 0; i < 4 ; i ++ ) {
			int x = p + KING_DIR[i];
			if ( LegalPosition[x] & PositionMask[ KING_TYPE ] ) {
				KING_HIT[p][t] = x;
				t ++;
			}
		}

		// 2. 生成士的HIT
		t = 0;
		for ( int i = 0; i < 4 ; i ++ ) {
			int x = p + ADVISOR_DIR[i];
			if ( LegalPosition[x] & PositionMask[ ADVISOR_TYPE ] ) {
				ADVISOR_HIT[p][t] = x;
				t ++;
			}
		}

		// 3. 生成象的HIT及PIN
		t = 0;
		for ( int i = 0; i < 4 ; i ++ ) {
			int x = p + BISHOP_DIR[i];
			if ( LegalPosition[x] & PositionMask[ BISHOP_TYPE ] ) {
				BISHOP_HIT[p][t] = x;
				BISHOP_PIN[p][t] = p + BISHOP_PIN_DIR[i];
				t ++;
			}
		}

		// 4. 生成马的HIT及PIN
		t = 0;
		for ( int i = 0; i < 8 ; i ++ ) {
			int x = p + KNIGHT_DIR[i];
			if ( LegalPosition[x] & PositionMask[ KNIGHT_TYPE ] ) {
				KNIGHT_HIT[p][t] = x;
				KNIGHT_PIN[p][t] = p + KNIGHT_PIN_DIR[i];
				t ++;
			}
		}

		// 5. 生成红兵的HIT
		t = 0;
		for ( int i = 0; i < 3; i ++ ) {
			int x = p + RED_PAWN_DIR[i];
			if ( LegalPosition[x] & PositionMask[ RED_PAWN_TYPE ] ) {
				PAWN_HIT[p][0][t] = x;
				t ++;
			}
		}

		// 6. 生成黑兵的HIT
		t = 0;
		for ( int i = 0; i < 3; i ++ ) {
			int x = p + BLACK_PAWN_DIR[i];
			if ( LegalPosition[x] & PositionMask[ BLACK_PAWN_TYPE ] ) {
				PAWN_HIT[p][1][t] = x;
				t ++;
			}
		}

		p = NEXTSQ (p);
	}
}

// 执棋方将军
bool PositionStruct::Check ( void ) const { // 可用行列位压数组优化
	int ST = SIDE_TYPE (player);
	int OppSideKingPos = piece [ KING_FROM + OPP_SIDE_TYPE(player) ];
	int k;

	// 1. 判断马将军
	for ( int i = KNIGHT_FROM; i <= KNIGHT_TO; i ++ ) {
		if ( piece[i+ST] ) {
			k = 0;
			while ( KNIGHT_HIT[ piece[i+ST] ][k] != 0 ) {
				if ( KNIGHT_HIT[ piece[i+ST] ][k] == OppSideKingPos ) {
					int pin = KNIGHT_PIN[ piece[i+ST] ][k]; // 马脚
					if ( square[pin] == 0 ) {
						return true;
					}
				}
				k ++;
			}
		}
	}

	// 2. 判断车将军
	for ( int i = ROOK_FROM; i <= ROOK_TO; i ++ ) {
		if ( piece[i+ST] ) {
			for ( int j = 0; j < 4; j ++ ) {
				int p = piece[i+ST];
				int meetTime = 0;
				while ( true ) {
					p = p + DIR[j];
					if ( ! IN_BOARD(p) ) {
						break;
					}
					if ( square[p] != 0 ) {
						meetTime ++;
					}
					if ( meetTime == 1 ) {
						if ( p == OppSideKingPos ) {
							return true;
						}
						break;
					}
				}
			}
		}
	}

	// 3. 判断炮将军
	for ( int i = CANNON_FROM; i <= CANNON_TO; i ++ ) {
		if ( piece[i+ST] ) {
			for ( int j = 0; j < 4; j ++ ) {
				int p = piece[i+ST];
				int meetTime = 0;
				while ( true ) {
					p = p + DIR[j];
					if ( ! IN_BOARD(p) ) {
						break;
					}
					if ( square[p] != 0 ) {
						meetTime ++;
					}
					if ( meetTime == 2 ) {
						if ( p == OppSideKingPos ) {
							return true;
						}
						break;
					}
				}
			}
		}
	}

	// 4. 判断兵将军
	for ( int i = PAWN_FROM; i <= PAWN_TO; i ++ ) {
		if ( piece[i+ST] ) {
			k = 0;
			while ( PAWN_HIT[piece[i+ST]][player][k] != 0 ) {
				if ( PAWN_HIT[piece[i+ST]][player][k] == OppSideKingPos ) {
					return true;
				}
				k ++;
			}
		}
	}

	return false;
}

// 执棋方被将军
bool PositionStruct::Checked ( void ) const { // 可用行列位压数组优化
	int ST = OPP_SIDE_TYPE ( player );
	int ThisSideKingPos = piece [ KING_FROM + SIDE_TYPE(player) ];
	int k;

	// 1. 判断被马将军
	for ( int i = KNIGHT_FROM; i <= KNIGHT_TO; i ++ ) {
		if ( piece[i+ST] ) {
			k = 0;
			while ( KNIGHT_HIT[ piece[i+ST] ][k] != 0 ) {
				if ( KNIGHT_HIT[ piece[i+ST] ][k] == ThisSideKingPos ) {
					int pin = KNIGHT_PIN[ piece[i+ST] ][k];
					if ( square[pin] == 0 ) {
						return true;
					}
				}
				k ++;
			}
		}
	}

	// 2. 判断被车将军
	for ( int i = ROOK_FROM; i <= ROOK_TO; i ++ ) {
		if ( piece[i+ST] ) {
			for ( int j = 0; j < 4; j ++ ) {
				int p = piece[i+ST];
				int meetTime = 0;
				while ( true ) {
					p = p + DIR[j];
					if ( ! IN_BOARD(p) ) {
						break;
					}
					if ( square[p] != 0 ) {
						meetTime ++;
					}
					if ( meetTime == 1 ) {
						if ( p == ThisSideKingPos ) {
							return true;
						}
						break;
					}
				}
			}
		}
	}

	// 3. 判断被炮将军
	for ( int i = CANNON_FROM; i <= CANNON_TO; i ++ ) {
		if ( piece[i+ST] ) {
			for ( int j = 0; j < 4; j ++ ) {
				int p = piece[i+ST];
				int meetTime = 0;
				while ( true ) {
					p = p + DIR[j];
					if ( ! IN_BOARD(p) ) {
						break;
					}
					if ( square[p] != 0 ) {
						meetTime ++;
					}
					if ( meetTime == 2 ) {
						if ( p == ThisSideKingPos ) {
							return true;
						}
						break;
					}
				}
			}
		}
	}

	// 4. 判断被兵将军
	for ( int i = PAWN_FROM; i <= PAWN_TO; i ++ ) {
		if ( piece[i+ST] ) {
			k = 0;
			while ( PAWN_HIT[piece[i+ST]][1-player][k] != 0 ) {
				if ( PAWN_HIT[piece[i+ST]][1-player][k] == ThisSideKingPos ) {
					return true;
				}
				k ++;
			}
		}
	}

	return false;
}

// 将对将局面
bool PositionStruct::KingFaceKing ( void ) const { // 可用行列位压数组优化
	int RED_KING_POS = piece [ RED_TYPE + KING_FROM ];
	int BLACK_KING_POS = piece [ BLACK_TYPE + KING_FROM ];

	if ( RED_KING_POS == 0 || BLACK_KING_POS == 0 ) { // 如果将万一不存在，则无解
		return false;
	}
	if ( COL(RED_KING_POS) != COL(BLACK_KING_POS) ) { // 两将必须同列
		return false;
	}

	// 判断两将之间是否有棋子挡住
	const int dir[] = {+16, -16};
	int p;
	for ( int i = 0; i < 2 ; i ++ ) {
		p = RED_KING_POS;
		int meetTime = 0;
		while ( true ) {
			p = p + dir[i];
			if ( ! IN_BOARD(p) ) {
				break;
			}
			if ( square[p] != 0 ) {
				meetTime ++;
			}
			if ( meetTime == 1 ) {
				if ( p == BLACK_KING_POS ) {
					return true;
				}
				break;
			}
		}
	}

	return false;
}

// 生成吃子着法
void PositionStruct::GenCapMove ( int *move, int &nMoveNum ) const {
	int ST = SIDE_TYPE ( player );
	int k;

	// 1. 生成将的吃子着法
	for ( int i = KING_FROM; i <= KING_TO; i ++ ) {
		if ( piece[i+ST] ) {
			k = 0;
			while ( KING_HIT[ piece[i+ST] ][k] != 0 ) {
				int hit = KING_HIT[piece[i+ST]][k];
				if ( square[hit] != 0 && COLOR_TYPE(i+ST) != COLOR_TYPE(square[hit]) ) {
					move[nMoveNum++] = MOVE ( piece[i+ST], hit );
				}
				k ++;
			}
		}
	}

	// 2. 生成士的吃子着法
	for ( int i = ADVISOR_FROM; i <= ADVISOR_TO; i ++ ) {
		if ( piece[i+ST] ) {
			k = 0;
			while ( ADVISOR_HIT[ piece[i+ST] ][k] != 0 ) {
				int hit = ADVISOR_HIT[piece[i+ST]][k];
				if ( square[hit] != 0 && COLOR_TYPE(i+ST) != COLOR_TYPE(square[hit]) ) {
					move[nMoveNum++] = MOVE ( piece[i+ST], hit );
				}
				k ++;
			}
		}
	}

	// 3. 生成象的吃子着法
	for ( int i = BISHOP_FROM; i <= BISHOP_TO; i ++ ) {
		if ( piece[i+ST] ) {
			k = 0;
			while ( BISHOP_HIT[ piece[i+ST] ][k] != 0 ) {
				int hit = BISHOP_HIT[piece[i+ST]][k];
				if ( square[hit] != 0 && COLOR_TYPE(i+ST) != COLOR_TYPE(square[hit]) ) {
					int pin = BISHOP_PIN[piece[i+ST]][k];
					if ( square[pin] == 0 ) { // 象脚
						move[nMoveNum++] = MOVE ( piece[i+ST], hit );
					}
				}
				k ++;
			}
		}
	}

	// 4. 生成马的吃子着法
	for ( int i = KNIGHT_FROM; i <= KNIGHT_TO; i ++ ) {
		if ( piece[i+ST] ) {
			k = 0;
			while ( KNIGHT_HIT[ piece[i+ST] ][k] != 0 ) {
				int hit = KNIGHT_HIT[piece[i+ST]][k];
				if ( square[hit] != 0 && COLOR_TYPE(i+ST) != COLOR_TYPE(square[hit]) ) {
					int pin = KNIGHT_PIN[piece[i+ST]][k];
					if ( square[pin] == 0 ) { // 马脚
						move[nMoveNum++] = MOVE ( piece[i+ST], hit );
					}
				}
				k ++;
			}
		}
	}

	// 5. 生成车的吃子着法
	for ( int i = ROOK_FROM; i <= ROOK_TO; i ++ ) { // 可用行列位压数组优化
		if ( piece[i+ST] ) {
			for ( int j = 0; j < 4; j ++ ) {
				int hit = piece[i+ST];
				int meetTime = 0;
				while ( true ) {
					hit = hit + DIR[j];
					if ( ! IN_BOARD(hit) ) {
						break;
					}
					if ( square[hit] != 0 ) {
						meetTime ++;
					}
					if ( meetTime == 1 ) {
						if ( COLOR_TYPE(i+ST) != COLOR_TYPE(square[hit]) ) {
							move[nMoveNum++] = MOVE ( piece[i+ST], hit );
						}
						break;
					}
				}
			}
		}
	}

	// 6. 生成炮的吃子着法
	for ( int i = CANNON_FROM; i <= CANNON_TO; i ++ ) { // 可用行列位压数组优化
		if ( piece[i+ST] ) {
			for ( int j = 0; j < 4; j ++ ) {
				int hit = piece[i+ST];
				int meetTime = 0;
				while ( true ) {
					hit = hit + DIR[j];
					if ( ! IN_BOARD(hit) ) {
						break;
					}
					if ( square[hit] != 0 ) {
						meetTime ++;
					}
					if ( meetTime == 2 ) {
						if ( COLOR_TYPE(i+ST) != COLOR_TYPE(square[hit]) ) {
							move[nMoveNum++] = MOVE ( piece[i+ST], hit );
						}
						break;
					}
				}
			}
		}
	}

	// 7. 生成兵的吃子着法
	for ( int i = PAWN_FROM; i <= PAWN_TO; i ++ ) {
		if ( piece[i+ST] ) {
			k = 0;
			while ( PAWN_HIT[piece[i+ST]][player][k] != 0 ) {
				int hit = PAWN_HIT[piece[i+ST]][player][k];
				if ( square[hit] != 0 && COLOR_TYPE(i+ST) != COLOR_TYPE(square[hit]) ) {
					move[nMoveNum++] = MOVE ( piece[i+ST], hit );
				}
				k ++;
			}
		}
	}
}

// 生成非吃子着法
void PositionStruct::GenNonCapMove ( int *move, int &nMoveNum ) const {
	int ST = SIDE_TYPE ( player );
	int k;

	// 1. 生成将的非吃子着法
	for ( int i = KING_FROM; i <= KING_TO; i ++ ) {
		if ( piece[i+ST] ) {
			k = 0;
			while ( KING_HIT[ piece[i+ST] ][k] != 0 ) {
				int hit = KING_HIT[piece[i+ST]][k];
				if ( square[hit] == 0 ) {
					move[nMoveNum++] = MOVE ( piece[i+ST], hit );
				}
				k ++;
			}
		}
	}

	// 2. 生成士的非吃子着法
	for ( int i = ADVISOR_FROM; i <= ADVISOR_TO; i ++ ) {
		if ( piece[i+ST] ) {
			k = 0;
			while ( ADVISOR_HIT[ piece[i+ST] ][k] != 0 ) {
				int hit = ADVISOR_HIT[piece[i+ST]][k];
				if ( square[hit] == 0 ) {
					move[nMoveNum++] = MOVE ( piece[i+ST], hit );
				}
				k ++;
			}
		}
	}

	// 3. 生成象的非吃子着法
	for ( int i = BISHOP_FROM; i <= BISHOP_TO; i ++ ) {
		if ( piece[i+ST] ) {
			k = 0;
			while ( BISHOP_HIT[ piece[i+ST] ][k] != 0 ) {
				int hit = BISHOP_HIT[piece[i+ST]][k];
				if ( square[hit] == 0 ) {
					int pin = BISHOP_PIN[piece[i+ST]][k];
					if ( square[pin] == 0 ) { // 象脚
						move[nMoveNum++] = MOVE ( piece[i+ST], hit );
					}
				}
				k ++;
			}
		}
	}

	// 4. 生成马的非吃子着法
	for ( int i = KNIGHT_FROM; i <= KNIGHT_TO; i ++ ) {
		if ( piece[i+ST] ) {
			k = 0;
			while ( KNIGHT_HIT[ piece[i+ST] ][k] != 0 ) {
				int hit = KNIGHT_HIT[piece[i+ST]][k];
				if ( square[hit] == 0 ) {
					int pin = KNIGHT_PIN[piece[i+ST]][k];
					if ( square[pin] == 0 ) { // 马脚
						move[nMoveNum++] = MOVE ( piece[i+ST], hit );
					}
				}
				k ++;
			}
		}
	}

	// 5. 生成车的非吃子着法
	for ( int i = ROOK_FROM; i <= ROOK_TO; i ++ ) {
		if ( piece[i+ST] ) {
			for ( int j = 0; j < 4; j ++ ) {
				int hit = piece[i+ST];
				int meetTime = 0;
				while ( true ) {
					hit = hit + DIR[j];
					if ( ! IN_BOARD(hit) ) {
						break;
					}
					if ( square[hit] != 0 ) {
						meetTime ++;
					}
					if ( meetTime > 0 ) {
						break;
					}
					move[nMoveNum++] = MOVE ( piece[i+ST], hit );
				}
			}
		}
	}

	// 6. 生成炮的非吃子着法
	for ( int i = CANNON_FROM; i <= CANNON_TO; i ++ ) {
		if ( piece[i+ST] ) {
			for ( int j = 0; j < 4; j ++ ) {
				int hit = piece[i+ST];
				int meetTime = 0;
				while ( true ) {
					hit = hit + DIR[j];
					if ( ! IN_BOARD(hit) ) {
						break;
					}
					if ( square[hit] != 0 ) {
						meetTime ++;
					}
					if ( meetTime > 0 ) {
						break;
					}
					move[nMoveNum++] = MOVE ( piece[i+ST], hit );
				}
			}
		}
	}

	// 7. 生成兵的非吃子着法
	for ( int i = PAWN_FROM; i <= PAWN_TO; i ++ ) {
		if ( piece[i+ST] ) {
			k = 0;
			while ( PAWN_HIT[piece[i+ST]][player][k] != 0 ) {
				int hit = PAWN_HIT[piece[i+ST]][player][k];
				if ( square[hit] == 0 ) {
					move[nMoveNum++] = MOVE ( piece[i+ST], hit );
				}
				k ++;
			}
		}
	}
}

// 生成所有着法
void PositionStruct::GenAllMove ( int *move, int &nMoveNum ) const {
	GenCapMove ( move, nMoveNum );
	GenNonCapMove ( move, nMoveNum );
}

// 去除无意义着法，无意义着法包括：1.被将军的着法  2.将对将的着法
void PositionStruct::DelMeaningLessMove ( int *move, int &nMoveNum ) {
	for ( int i = 0; i < nMoveNum; i ++ ) {
		MakeMove ( move[i] );
		if ( check || KingFaceKing() ) {
			SWAP ( move[i], move[nMoveNum-1] );
			nMoveNum --;
			i --;
		}
		UndoMakeMove ();
	}
}
