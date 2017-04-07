#ifndef SEARCH_H_
#define SEARCH_H_

#include "base.h"
#include "position.h"
#include "rollback.h"
#include <map>

const int LIMIT_DEPTH = 30;

struct SearchStruct {
	PositionStruct pos; // 当前搜索局面
	RollBackListStruct roll; // 回滚着法表
	std::map < std::pair<ULL, ULL>, int > bestmove; // 最佳着法表
};

extern SearchStruct Search;

void MainSearch ( void );

std::string BestMove ( void );

#endif /* SEARCH_H_ */
