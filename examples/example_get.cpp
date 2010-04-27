
#include "amqpcpp.h"
#include <iostream>
#include <memory>

int main () {



	try {
//		AMQP amqp("123123:akalend@localhost/private");
		AMQP amqp("123123:akalend@localhost:5673/private");		

		auto_ptr<AMQPQueue> qu2(amqp.createQueue("q2"));
		qu2->Declare();		
		
		
		while (  1 ) {
			qu2->Get(AMQP_NOACK);

			auto_ptr<AMQPMessage> m (qu2->getMessage());
			
			cout << "count: "<<  m->getMessageCount() << endl;											 
			if (m->getMessageCount() > -1) {
			
			cout << "message\n"<< m->getMessage() << "\nmessage key: "<<  m->getRoutingKey() << endl;
			cout << "exchange: "<<  m->getExchange() << endl;											
			} else 
				break;				
						
							
		}	
	} catch (AMQPException e) {
		std::cout << e.getMessage() << std::endl;
	}

	return 0;					

}