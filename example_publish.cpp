

#include "amqpcpp.h"
#include <iostream>

int main (int argc, char** argv) {



	try {
//		AMQP amqp;
//		AMQP amqp(AMQPDEBUG);
	
		AMQP amqp("123123:akalend@localhost:5673/private");		// all connect string

		AMQPExchange * ex = amqp.createExchange("e");
		ex->Declare("e", "fanout");

		AMQPQueue * qu2 = amqp.createQueue("q2");
		qu2->Declare();
		qu2->Bind( "e", "");		

		string ss = "***********************";
		/*
		ss = ss+ss+ss+ss+ss+ss+ss;
		ss += ss+ss+ss+ss+ss+ss+ss;
		ss += ss+ss+ss+ss+ss+ss+ss;
		ss += ss+ss+ss+ss+ss+ss+ss;
		ss += ss+ss+ss+ss+ss+ss+ss;
*/

		ex->Publish(  ss , ""); // publish very long message
		
		ex->Publish(  "---------------------------" , "");
		ex->Publish(  "123#################################123" , "");
		
		
		if (argc==2) {
			AMQPQueue * qu = amqp.createQueue();			
			qu->Cancel(   amqp_cstring_bytes(argv[1]) );
		}												
						
	} catch (AMQPException e) {
		std::cout << e.getMessage() << std::endl;
	}

	return 0;					

}