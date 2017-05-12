#ifndef SEARCH_H_
#define SEARCH_H_

#include "base.h"
#include "position.h"
#include "rollback.h"

const int SEARCH_MAX_DEPTH = 64;

extern PositionStruct pos; // 当前搜索局面
extern RollBackListStruct roll; // 回滚着法表

struct SearchStruct {
	int bmv; // 最佳着法
	int bvl; // 最优得分
	int nNode; // 总节点数
	int nBeta; // beta点个数
	int maxDistance;
};
extern SearchStruct Search;

int SearchMain ( void );

#endif /* SEARCH_H_ */