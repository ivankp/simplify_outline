all: simplify_outline

%: %.cpp
	g++ -std=c++20 -Wall -Wextra -O3 $< -o $@
