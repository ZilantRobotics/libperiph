COMMON_INCLUDES=-I ../sensors/
COMMON_LIBS=-lpthread -pthread -lgtest
FLAGS=-Wall -std=c++11
GXX_COMPILE=g++ $(COMMON_INCLUDES) $(FLAGS)  -o shell 

SRC_UBLOX_TEST=test_ublox.cpp                   ../sensors/ublox.c
SRC_ESC_FLAME_TEST=test_esc_flame.cpp           ../sensors/esc_flame.c


all: esc_flame_test ublox_test

esc_flame_test:
	$(GXX_COMPILE) $(SRC_ESC_FLAME_TEST) $(COMMON_LIBS)
	./shell

ublox_test:
	$(GXX_COMPILE) $(SRC_UBLOX_TEST) $(COMMON_LIBS)
	./shell
