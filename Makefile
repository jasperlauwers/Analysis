SHELL = /bin/bash
VPATH = src/ include/ obj/
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(notdir $(CPP_FILES:.cpp=.o))

# CC = g++ -std=c++11 -g -Wall
# Optimized compilation
CC = g++ -std=c++11 -O3 -Wall
CFLAGS = $(shell root-config --cflags) -I./include -I./src
CFLAGS_libconfig = -I. -I/home/jlauwers/VBS/Run2/Analysis/libconfig-1.5/lib
LIBS   = $(shell root-config --libs) -lPhysics -lThread -lMinuit -lHtml -lVMC -lEG -lGeom 
LIBS_libconfig =  -L/home/jlauwers/VBS/Run2/Analysis/libconfig-1.5/lib/.libs/ -lconfig++

all: testAnalysis

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(CFLAGS_libconfig)

%.o: %.C %.h
	$(CC) -c -o $@ $< $(CFLAGS)

TreeReader.o: TreeReader.cpp TreeReader.hpp latino.h

Particle.o: Particle.cpp Particle.hpp

Jet.o: Jet.cpp Jet.hpp Particle.hpp

Lepton.o: Lepton.cpp Lepton.hpp Particle.hpp

EventContainer.o: EventContainer.cpp EventContainer.hpp Lepton.hpp Jet.hpp Particle.hpp

EventReader.o: EventReader.cpp EventReader.hpp TreeReader.hpp latino.h EventContainer.hpp Lepton.hpp Jet.hpp Particle.hpp ConfigContainer.cpp VariableContainer.cpp SampleContainer.cpp CutContainer.cpp

ConfigReader.o: ConfigReader.cpp ConfigReader.hpp

VariableReader.o: VariableReader.cpp VariableReader.hpp VariableContainer.cpp ConfigReader.hpp

SampleReader.o: SampleReader.cpp SampleReader.hpp SampleContainer.cpp ConfigReader.hpp

CutReader.o: CutReader.cpp CutReader.hpp CutContainer.cpp ConfigReader.hpp

EventCleaner.o: EventCleaner.cpp EventCleaner.hpp VariableContainer.cpp EventContainer.hpp Lepton.hpp Jet.hpp Particle.hpp

ConfigHandler.o: ConfigHandler.cpp ConfigHandler.hpp VariableReader.hpp SampleReader.hpp CutReader.hpp ConfigReader.hpp VariableContainer.cpp SampleContainer.cpp CutContainer.cpp

EventSelecter.o: EventSelecter.cpp EventSelecter.hpp EventContainer.hpp Lepton.hpp Jet.hpp Particle.hpp CutContainer.cpp

EventPlotter.o: EventPlotter.cpp EventPlotter.hpp EventContainer.hpp Lepton.hpp Jet.hpp Particle.hpp ConfigContainer.cpp VariableContainer.cpp SampleContainer.cpp CutContainer.cpp

testAnalysis.o: testAnalysis.cpp EventReader.hpp TreeReader.hpp latino.h EventContainer.hpp Lepton.hpp Jet.hpp Particle.hpp VariableReader.hpp VariableContainer.cpp ConfigReader.hpp SampleReader.hpp SampleContainer.cpp CutReader.hpp CutContainer.cpp CutReader.hpp EventCleaner.hpp ConfigHandler.hpp  EventSelecter.hpp ConfigContainer.cpp EventPlotter.hpp

testAnalysis : $(OBJ_FILES) latino.o
	$(CC) -o $@ $^ $(CFLAGS) $(CFLAGS_libconfig) $(LIBS) $(LIBS_libconfig)

clean:
	-rm -f obj/*.o testAnalysis