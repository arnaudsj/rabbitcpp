/*
 *  AMQPExchange.cpp
 *  rabbitmq++
 *
 *  Created by Alexandre Kalendarev on 10.03.10.
 *
 */

#include <iostream>
#include <string>

#include "amqpcpp.h"

using namespace std;



AMQPExchange::AMQPExchange(amqp_connection_state_t * cnn, int channelNum) {
		this->cnn = cnn;
		this->channelNum = channelNum;

//	std::cout << "AMQPQueue()\n";;				
		openChannel();
}	

AMQPExchange::AMQPExchange(amqp_connection_state_t * cnn, int channelNum, string name) {
		this->cnn = cnn;
		this->channelNum = channelNum;
		this->name = name;
		
		openChannel();
}




//Declare

void AMQPExchange::Declare() {
	AMQPExchange::parms=0;
	AMQPExchange::sendDeclareCommand();

}

void AMQPExchange::Declare(const char * name) {
	AMQPExchange::name=name;
	AMQPExchange::parms=0;
	AMQPExchange::sendDeclareCommand();
}

void AMQPExchange::Declare(string name) {
	AMQPExchange::parms=0;	
	AMQPExchange::name=name;
	AMQPExchange::sendDeclareCommand();
}

void AMQPExchange::Declare(const char * name, const char * type) {
	AMQPExchange::parms=0;
	AMQPExchange::name=name;
	AMQPExchange::type=type;
	AMQPExchange::sendDeclareCommand();	
}

void AMQPExchange::Declare(string name, const char * type) {
	AMQPExchange::parms=0;
	AMQPExchange::name=name;
	AMQPExchange::type=type;
	AMQPExchange::sendDeclareCommand();
}

void AMQPExchange::Declare(const char * name, const char * type, short parms) {
	AMQPExchange::name=name;
	AMQPExchange::type=type;
	AMQPExchange::parms=parms;
	AMQPExchange::sendDeclareCommand();	
}

void AMQPExchange::Declare(string name, const char * type, short parms) {
	AMQPExchange::name=name;
	AMQPExchange::type=type;
	AMQPExchange::parms=parms;
	AMQPExchange::sendDeclareCommand();
}

void AMQPExchange::sendDeclareCommand() {
	AMQPExchange::checkType();

	amqp_bytes_t exchange = amqp_cstring_bytes(name.c_str());
	amqp_bytes_t exchangetype = amqp_cstring_bytes(type.c_str());
	
//	cout << "parms=" << parms << endl;
	amqp_table_t args;
	args.num_entries = 0;
	args.entries = NULL;

	amqp_boolean_t passive =	(parms & AMQP_PASSIVE)		? 1:0;
	amqp_boolean_t autodelete = (parms & AMQP_AUTODELETE)	? 1:0;
	amqp_boolean_t durable =	(parms & AMQP_DURABLE)		? 1:0;
		
	amqp_exchange_declare(*cnn, (amqp_channel_t) 1, exchange, exchangetype, 
		passive, durable, autodelete, args );
		 
	amqp_rpc_reply_t res =amqp_get_rpc_reply(*cnn);
	
	AMQPBase::checkReply(&res);		
}

void AMQPExchange::checkType() {
	
	short isErr = 1;
	if ( type == "direct" ) 
		isErr = 0;
		
	if ( type == "fanout" ) 
		isErr = 0;

	if ( type == "topic" ) 
		isErr = 0;
		
	if (isErr)
			throw AMQPException::AMQPException("the type of AMQPExchange must be direct | fanout | topic" );
}

// Delete

void AMQPExchange::Delete() {
	
	if (!name.size())
			throw AMQPException::AMQPException("the name of exchange not set");
	
	AMQPExchange::sendDeleteCommand();
}

void AMQPExchange::Delete(const char * name) {
	AMQPExchange::name=name;
	AMQPExchange::sendDeleteCommand();
}

void AMQPExchange::Delete(string name) {
	AMQPExchange::name=name;
	AMQPExchange::sendDeleteCommand();
}


void AMQPExchange::sendDeleteCommand(){
	 
	amqp_bytes_t exchange = amqp_cstring_bytes(name.c_str());
	amqp_exchange_delete_t s;
		s.ticket = 0;
		s.exchange = exchange;
		s.if_unused = ( AMQP_IFUNUSED & parms ) ? 1:0;
		s.nowait = ( AMQP_NOWAIT & parms ) ? 1:0;

	amqp_method_number_t method_ok = AMQP_EXCHANGE_DELETE_OK_METHOD; 
			
	amqp_rpc_reply_t res = amqp_simple_rpc(*cnn, channelNum, 
			AMQP_EXCHANGE_DELETE_METHOD, 
			&method_ok , &s);
	
	AMQPBase::checkReply(&res);		
}


// Bind

void AMQPExchange::Bind(const char * queueName) {
	if (type != "fanout")
		throw AMQPException::AMQPException("key is NULL, this using only for the type fanout" );
		
	AMQPExchange::sendBindCommand(queueName, NULL );
}

void AMQPExchange::Bind(string name) {
	if (type != "fanout")
		throw AMQPException::AMQPException("key is NULL, this using only for the type fanout" );

	AMQPExchange::sendBindCommand(name.c_str(), NULL );
}

void AMQPExchange::Bind(const char * name, string key) {
	AMQPExchange::sendBindCommand(name, key.c_str() );
}

void AMQPExchange::Bind(const char * name, const char * key) {
	AMQPExchange::sendBindCommand(name, key);
}


void AMQPExchange::Bind(string name, const char * key) {
	AMQPExchange::sendBindCommand(name.c_str(), key);
}

void AMQPExchange::Bind(string name, string key) {
	AMQPExchange::sendBindCommand(name.c_str(), key.c_str());
}

void AMQPExchange::sendBindCommand(const char * queue, const char * key){
	 
	amqp_bytes_t queueByte = amqp_cstring_bytes(queue);
	amqp_bytes_t exchangeByte = amqp_cstring_bytes(name.c_str());
	amqp_bytes_t keyByte = amqp_cstring_bytes(key);
	
    amqp_queue_bind_t s;
		s.ticket = 0;
		s.queue = queueByte;
		s.exchange = exchangeByte;
		s.routing_key = keyByte;
		s.nowait = ( AMQP_NOWAIT & parms ) ? 1:0;

		s.arguments.num_entries = 0;
		s.arguments.entries = NULL;

	
	amqp_method_number_t method_ok = AMQP_QUEUE_BIND_OK_METHOD;
    amqp_rpc_reply_t res = amqp_simple_rpc( *cnn,
					channelNum,
					AMQP_QUEUE_BIND_METHOD,
					&method_ok, &s);

	AMQPBase::checkReply(&res);
	
}


// Publish

void AMQPExchange::Publish(const char * message, string key) {
	AMQPExchange::sendPublishCommand(message, key.c_str() );
}

void AMQPExchange::Publish(const char * message, const char * key) {
	AMQPExchange::sendPublishCommand(message, key);
}

void AMQPExchange::Publish(string message, const char * key) {
	AMQPExchange::sendPublishCommand(message.c_str(), key);
}

void AMQPExchange::Publish(string message, string key) {
	AMQPExchange::sendPublishCommand(message.c_str(), key.c_str());
}


void AMQPExchange::sendPublishCommand(const char * message, const char * key) {
	amqp_bytes_t exchangeByte = amqp_cstring_bytes(name.c_str());
	amqp_bytes_t keyrouteByte = amqp_cstring_bytes(key);
	amqp_bytes_t messageByte = amqp_cstring_bytes(message);
		
	amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_cstring_bytes("text/plain");
    props.delivery_mode = 2; // persistent delivery mode


    short mandatory = (parms & AMQP_MANDATORY) ? 1:0;
    short immediate = (parms & AMQP_IMMIDIATE) ? 1:0;
	
	amqp_basic_publish(*cnn,
				    1,
				    exchangeByte,
				    keyrouteByte,
				    mandatory, // mandatory
				    immediate, // immediate
				    &props,
				    messageByte);
}

