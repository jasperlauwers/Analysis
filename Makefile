SHELL = /bin/bash
VPATH = src/ include/ test/
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
CC = g++ -std=c++11 -O3 -Wall
CFLAGS = $(shell root-config --cflags) -I./include
CFLAGS_libconfig = -I. -I/home/jlauwers/VBS/Run2/Analysis/libconfig-1.5/lib
LIBS   = $(shell root-config --libs) -lPhysics -lThread -lMinuit -lHtml -lVMC -lEG -lGeom 
LIBS_libconfig =  -L/home/jlauwers/VBS/Run2/Analysis/libconfig-1.5/lib/.libs/ -lconfig++

all: testAnalysis

obj/%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(CFLAGS_libconfig)

obj/%.o: %.C %.h
	$(CC) -c -o $@ $< $(CFLAGS)

obj/TreeReader.o: TreeReader.cpp TreeReader.hpp latino.h

obj/Particle.o: Particle.cpp Particle.hpp

obj/Jet.o: Jet.cpp Jet.hpp Particle.hpp

obj/Lepton.o: Lepton.cpp Lepton.hpp Particle.hpp

obj/EventContainer.o: EventContainer.cpp EventContainer.hpp Lepton.hpp Jet.hpp Particle.hpp

obj/EventReader.o: EventReader.cpp EventReader.hpp TreeReader.hpp latino.h EventContainer.hpp Lepton.hpp Jet.hpp Particle.hpp ConfigContainer.hpp VariableContainer.hpp SampleContainer.hpp CutContainer.hpp

obj/ConfigReader.o: ConfigReader.cpp ConfigReader.hpp

obj/VariableReader.o: VariableReader.cpp VariableReader.hpp VariableContainer.hpp ConfigReader.hpp

obj/SampleReader.o: SampleReader.cpp SampleReader.hpp SampleContainer.hpp ConfigReader.hpp

obj/CutReader.o: CutReader.cpp CutReader.hpp CutContainer.hpp ConfigReader.hpp

obj/EventCleaner.o: EventCleaner.cpp EventCleaner.hpp VariableContainer.hpp EventContainer.hpp Lepton.hpp Jet.hpp Particle.hpp

obj/ConfigHandler.o: ConfigHandler.cpp ConfigHandler.hpp VariableReader.hpp SampleReader.hpp CutReader.hpp ConfigReader.hpp VariableContainer.hpp SampleContainer.hpp CutContainer.hpp

obj/VariableDictionary.o: VariableDictionary.cpp VariableDictionary.hpp EventContainer.hpp Lepton.hpp Jet.hpp Particle.hpp

obj/EventSelecter.o: EventSelecter.cpp EventSelecter.hpp VariableDictionary.hpp EventContainer.hpp Lepton.hpp Jet.hpp Particle.hpp CutContainer.hpp

obj/EventPlotter.o: EventPlotter.cpp EventPlotter.hpp VariableDictionary.hpp EventContainer.hpp Lepton.hpp Jet.hpp Particle.hpp ConfigContainer.hpp VariableContainer.hpp SampleContainer.hpp CutContainer.hpp

obj/testAnalysis.o: testAnalysis.cpp EventReader.hpp TreeReader.hpp latino.h EventContainer.hpp Lepton.hpp Jet.hpp Particle.hpp VariableReader.hpp VariableContainer.hpp ConfigReader.hpp SampleReader.hpp SampleContainer.hpp CutReader.hpp CutContainer.hpp CutReader.hpp EventCleaner.hpp ConfigHandler.hpp  EventSelecter.hpp VariableDictionary.hpp ConfigContainer.hpp EventPlotter.hpp

testAnalysis : $(OBJ_FILES) obj/testAnalysis.o obj/latino.o
	$(CC) -o $@ $^ $(CFLAGS) $(CFLAGS_libconfig) $(LIBS) $(LIBS_libconfig)

clean:
	-rm -f obj/*.o testAnalysis