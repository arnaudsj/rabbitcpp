RABBITCPP_SRC_PATH = ./src
RABBITCPP_EXAMPLE_PATH = ./examples

all: libcpp example_publish example_consume example_get

rabbitmq_clib:
	cd deps/rabbitmq-c; autoreconf -i; ./configure --prefix=`cd ../..;pwd`; make; make install

libcpp:	
	gcc -c -fPIC -I./include -L./lib $(RABBITCPP_SRC_PATH)/AMQPBase.cpp $(RABBITCPP_SRC_PATH)/AMQPException.cpp $(RABBITCPP_SRC_PATH)/AMQPMessage.cpp $(RABBITCPP_SRC_PATH)/AMQPConnection.cpp $(RABBITCPP_SRC_PATH)/AMQPExchange.cpp $(RABBITCPP_SRC_PATH)/AMQPQueue.cpp
	ar rcs libamqpcpp.a *o
	mv libamqpcpp.a lib/

example_publish:
	g++ -o ./bin/example_publish  -lamqpcpp -lrabbitmq    -Iamqpcpp -I./include -I./src -L./lib  -L.  $(RABBITCPP_EXAMPLE_PATH)/example_publish.cpp

example_consume:
	g++ -o ./bin/example_consume  -lamqpcpp -lrabbitmq    -Iamqpcpp -I./include -I./src -L./lib  -L.  $(RABBITCPP_EXAMPLE_PATH)/example_consume.cpp

example_get:
	g++ -o ./bin/example_get  -lamqpcpp -lrabbitmq    -Iamqpcpp -I./include -I./src -L./lib  -L.  $(RABBITCPP_EXAMPLE_PATH)/example_get.cpp

clean:
	rm build/*o
	rm lib/*a
	cd bin; rm example_publish example_consume example_get
