/*
 *  AMQPMessage.cpp
 *  rabbitmq++
 *
 *  Created by Alexandre Kalendarev on 15.04.10.
 *
 */

#include <iostream>
#include <string>


using namespace std;

#include "amqpcpp.h"

#ifndef __AMQPMESSAGE
#define __AMQPMESSAGE



AMQPMessage::AMQPMessage( AMQPQueue * queue ) 
{
	this->queue=queue;
	 message_count=-1;
	 data=NULL;
//		 cout <<  "ctor AMQPMessage\n";
}
		
AMQPMessage::~AMQPMessage() 
{
//			cout <<  "dtor AMQPMessage";
	if (data) {
		free(data);
//				cout <<  " free(data)";
	}
			//cout<<endl;
}
	
void AMQPMessage::setMessage( char * data)	
{
	if (!data)
		return;
	
	if (this->data)
		free(this->data);
			
	this->data = strdup(data);
}

char * AMQPMessage::getMessage()
{			
	if (this->data)
		return this->data;
		
	return '\0';			
}	

string AMQPMessage::getConsumerTag() 
{
	return this->consumer_tag;
}

void AMQPMessage::setConsumerTag( amqp_bytes_t consumer_tag) 
{
	this->consumer_tag.assign( (char*)consumer_tag.bytes, consumer_tag.len );	
}

void AMQPMessage::setConsumerTag( string consumer_tag) 
{
	this->consumer_tag=consumer_tag;	
}
		
void AMQPMessage::setDeliveryTag(uint32_t delivery_tag) 
{			
	this->delivery_tag=delivery_tag;
}
	
uint32_t AMQPMessage::getDeliveryTag() 
{			
	return this->delivery_tag;
}
		
void AMQPMessage::setMessageCount( int count) {
	this->message_count = count;
}

int AMQPMessage::getMessageCount()	{
	return message_count;
}

void AMQPMessage::setExchange(amqp_bytes_t exchange) 
{
	if (exchange.len)
		this->exchange.assign( (char*)exchange.bytes, exchange.len );	
}

void AMQPMessage::setExchange(char * exchange) 
{
	this->exchange = exchange;
}
		
string AMQPMessage::getExchange()
{
	return exchange;		
}	

void AMQPMessage::setRoutingKey(amqp_bytes_t routing_key) 
{
	if (routing_key.len)
		this->routing_key.assign( (char*)routing_key.bytes, routing_key.len );	
}	
		
void AMQPMessage::setRoutingKey(char * routing_key)	
{
	this->routing_key=routing_key;		
}	
		
string AMQPMessage::getRoutingKey()	
{
	return routing_key;		
}	


AMQPQueue * AMQPMessage::getQueue() {
	return queue;
}
		
#endif
