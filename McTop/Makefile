# a simple makefile for McTopAnalysis using root library
# author: Zhao Xianghu <zhaoxh@stu.nju.edu.cn>

CXX = g++
INCDIR = -I$(ROOTSYS)/include
LIBDIR = -L$(ROOTSYS)/lib
LIBS = -lCore -lCint -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -pthread -lm -ldl -rdynamic
CXXFLAGS = -O2 -W -Wall

MCTOPANALYSIS = McTopAnalysis

# change this name to your program
PROGRAM = testmctop


$(PROGRAM): $(PROGRAM).cxx $(MCTOPANALYSIS).o
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) $(LIBS) $(PROGRAM).cxx $(MCTOPANALYSIS).o -o $(PROGRAM)


$(MCTOPANALYSIS).o: $(MCTOPANALYSIS).cxx $(MCTOPANALYSIS).h
	$(CXX) $(CXXFLAGS) $(LIBDIR) -c $(MCTOPANALYSIS).cxx


.PHONY: clean
clean:
	$(RM) $(MCTOPANALYSIS).o
	$(RM) $(PROGRAM)
