.PHONY: clean 

CXX      = g++
FILTER   = *
INCLUDE  = ../include
SOURCES  = $(wildcard $(FILTER).cpp)
EXECS    = $(notdir $(SOURCES:.cpp=))
FLAGS    = -I $(INCLUDE) -std=c++11 -Wall -Wextra -Wpedantic -g -O0

.PHONY: all clean

all: $(EXECS)

clean:
	find -type f -not -name "*.cpp" -not -name "Makefile" -not -name "runall" | xargs rm

% : %.cpp $(INCLUDE)/clipp.h
	$(CXX) $(FLAGS) -o $@ $<
