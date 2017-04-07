#include "base.h"
#include "position.h"
#include "hash.h"
#include "move.h"
#include "search.h"
#include "ucci.h"

void PreparePrint ();

inline void print( const std::string s ) {
	std::cout << s << std::endl;
	fflush(stdout);
}

int main() {
	//freopen ("/Users/peteryuanpan/Desktop/data.txt", "r", stdin);

	CommEnum Order;
	bool First = true;

	printf("Type 'ucci' to begin.\n");

	if( BootLine() == e_CommUcci ) {
		PreparePrint (); // 引导状态

		while ( true ) { // 空闲状态+思考状态
			Order = IdleLine ( Command, 0 );
			if ( Order == e_CommQuit ) { // quit
				break;
			}
			if( Order == e_CommIsReady ) { // 
				print("readyok");
			}
			else if ( Order == e_CommSetOption ) {
				if( Command.Option.Type == e_NewGame ) { // setoption newgame
					First = true;
					Search.roll.Init ();
					InitZobrist ();
					InitMove ();
				}
			}
			else if ( Order == e_CommPosition ) { // position [ startpos | fen ] moves ...
				Search.pos.Init ( Command.Position.FenStr, Command.Position.MoveStr, Command.Position.MoveNum );
			}
			else if( Order == e_CommGo || Order == e_CommGoPonder ) { // go nodes 10077696
				if ( First ) { // 若是第一次，则进行一次完全搜索，将结果存入结果表中
					First = false;
					MainSearch ();
				}
				print( BestMove() );
			}
		}

		print("bye");
	}
	return 0;
}

void PreparePrint () {
	// 显示引擎的名称、版本、作者和使用者
	print("id name lazyboy");
    print("id version 0.2");
	print("id author peterpan");
    
	//设置参数
	print("option batch type check default false");
	print("option debug type check default false");
	print("option bookfiles type string default ");
	print("option egtbpaths type string default null");
	print("option hashsize type spin default 0 MB");
	print("option threads type spin default 0");
	print("option drawmoves type spin default 0");
	print("option pruning type check 0");
	print("option knowledge type check 0");
	print("option selectivity type spin min 0 max 3 default 0");
	print("option style type combo var solid var normal var risky default normal"); 
	print("copyprotection ok");
	print("ucciok");
}
