# A simple example of makefile.
  # Type "make" to compile and link your program.
  # Type "make clean" to delete objects.
  # Type "make [PACKAGE]" to compile and link the specified package only.
  # Type "make all" to compile and link all your packages and programs.

#--------------------------PART 1: about the compiler-------------------------------------------
# Specify the compiler. Usually it is g++, if you are using gcc.
CXX = g++
# Here you can pass different options to the compiler
CXXFLAGS = -g -O2 -W -Wall -fPIC
# How you create your libs
LDFLAGS = -shared -fPIC

#--------------------------PART 2: about packages you would use-------------------------------------------
# If you want to use external files, or third party packages, you need to specify two kinds of paths and a list of shared library files.
  # INCDIR tells the compiler where are thoses headers contained.
  # LIBDIR shows directories which contain library files coresponding to those headers. 
  # LIBS is the list of shared library files you would use.They can be found in LIBDIR. ( Names changing rule: libXXX.so to -lXXX )

# Take root as an example. ( Root is a widely used package developed by CERN for analysing. )
INCDIR = -I$(shell $(ROOTSYS)/bin/root-config --incdir) -I$(MYGEANT4ROOT)/include/Geant4 -I$(MYCLHEPROOT)/include
LIBDIR = $(shell $(ROOTSYS)/bin/root-config --glibs) -L$(MYGEANT4ROOT)/lib64 $(shell $(MYCLHEPROOT)/bin/clhep-config --libs)
LIBS = -pthread -lm -ldl -rdynamic

#--------------------------PART 3: about your own packages and the main program-------------------------------------------
# Names of user defined packages

# Change this name to your program which contains the main function
TARGET = MyRootInterface
LIBOBJECT = ../libs/lib$(TARGET).so
OBJECT = ../libs/$(TARGET).o

# Object list for user defined packages
OBJ = 

$(OBJECT): $(TARGET).cxx $(TARGET).hxx
	$(CXX) $(CXXFLAGS) $(INCDIR) $(LIBDIR) $(LIBS) -c $(TARGET).cxx -o $(OBJECT)
	$(CXX) $(LDFLAGS) -o $(LIBOBJECT) $(OBJECT)

#--------------------------PART 4: about make options-------------------------------------------
# You can create new options to realise special functions
# Here is an example showing how to make clean and make a specified user defined package
.PHONY: clean
clean:
	$(RM) $(OBJECT)
	$(RM) $(LIBOBJECT)

.PHONY: all
all: $(OBJECT)
