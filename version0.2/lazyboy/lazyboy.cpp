#include "base.h"
#include "ucci.h"

int main() {
	CommEnum Order;
	printf("Type 'ucci' to begin.\n");
	if( BootLine() == e_CommUcci ) {
		OrDeal.PreparePrint(); //引导状态

		while(true){ //空闲状态+思考状态
			Order = IdleLine(Command, 0);
			if(Order == e_CommQuit) break; //quit
			if(Order == e_CommIsReady) OrDeal.ReadyOK();
			else if(Order == e_CommSetOption){
				if(Command.Option.Type == e_NewGame) {
					OrDeal.NewGame();
				}
			}
			else if(Order == e_CommPosition) OrDeal.PositionFen();
			else if(Order == e_CommGo || Order == e_CommGoPonder) OrDeal.GoPrint();
		}

        OrDeal.Bye();
	}
	return 0;
}
