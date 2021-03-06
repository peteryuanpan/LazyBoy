#include "time.h"
#include "base.h"
#include <sys/time.h>

double TimeLimit [100];
double startT [100];

// 设置时间限制
void SetTimeLimit ( const int x, const double limit ) {
	TimeLimit[x] = limit;
}

// 现在的时间
double ThisTime ( void ) {
	struct timeval tp;
	gettimeofday ( &tp, NULL );
	return tp.tv_sec + tp.tv_usec / 1000000.0;
}

// 初始化开始时间
void InitBeginTime ( const int x ) {
	startT[x] = ThisTime ();
}

// 已超时
bool TimeOut ( const int x ) {
	return ThisTime() - startT[x] >= TimeLimit[x];
}

// 计算用时
double TimeCost ( const int x ) {
	return ThisTime() - startT[x];
}
