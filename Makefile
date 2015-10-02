geo: geo.cpp
	$(CXX) -Wall -Wextra -Werror -O3 -std=c++11 -o geo geo.cpp

.PHONY: clean
clean:
	-rm geo
