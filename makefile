dronemake: drone.cpp driver.cpp
	g++ drone.cpp driver.cpp -std=c++11 -lpthread -o drone
