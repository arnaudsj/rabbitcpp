

#include "amqpcpp.h"
#include <iostream>
#include <memory>

void onMessage( AMQPMessage * message  ) {
	char * data = message->getMessage();
	if (data)
		  cout << data << endl;

};


int main () {



	try {
//		AMQP amqp("123123:akalend@localhost/private");
		AMQP amqp("123123:akalend@localhost:5673/private");		

		auto_ptr<AMQPQueue> qu2(amqp.createQueue("q2"));
		qu2->Declare();
		qu2->Bind( "e", "");		

		qu2->addEvent(AMQP_MESSAGE, (void *) onMessage );
		qu2->Consume(AMQP_NOACK);		
										
	} catch (AMQPException e) {
		std::cout << e.getMessage() << std::endl;
	}

	return 0;					

}