SHELL = /bin/bash
VPATH = src/ include/ test/ macros/
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
CC = g++ -std=c++11 -O3 -Wall
CFLAGS = $(shell root-config --cflags) -I./include -I./macros
CFLAGS_libconfig = -I. -I/home/jlauwers/VBS/Run2/Analysis/libconfig-1.5/lib
LIBS   = $(shell root-config --libs) -lPhysics -lThread -lMinuit -lHtml -lVMC -lEG -lGeom 
LIBS_libconfig =  -L/home/jlauwers/VBS/Run2/Analysis/libconfig-1.5/lib/.libs/ -lconfig++

all: testAnalysis Plot_cut_efficiency stackPlotter

obj/%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(CFLAGS_libconfig)

obj/%.o: %.C
	$(CC) -c -o $@ $< $(CFLAGS)

obj/latino.o: latino.C latino.h

obj/TreeReader.o: TreeReader.cpp TreeReader.hpp latino.h

obj/Particle.o: Particle.cpp Particle.hpp

obj/Jet.o: Jet.cpp Jet.hpp Particle.hpp

obj/Lepton.o: Lepton.cpp Lepton.hpp Particle.hpp

obj/Met.o: Met.cpp Met.hpp Particle.hpp

obj/EventContainer.o: EventContainer.cpp EventContainer.hpp Lepton.hpp Jet.hpp Met.hpp Particle.hpp 

obj/EventReader.o: EventReader.cpp EventReader.hpp TreeReader.hpp latino.h EventContainer.hpp Lepton.hpp Jet.hpp Met.hpp Particle.hpp ConfigContainer.hpp VariableContainer.hpp SampleContainer.hpp CutContainer.hpp

obj/ConfigReader.o: ConfigReader.cpp ConfigReader.hpp

obj/VariableReader.o: VariableReader.cpp VariableReader.hpp VariableContainer.hpp ConfigReader.hpp

obj/SampleReader.o: SampleReader.cpp SampleReader.hpp SampleContainer.hpp ConfigReader.hpp

obj/CutReader.o: CutReader.cpp CutReader.hpp CutContainer.hpp ConfigReader.hpp

obj/EventCleaner.o: EventCleaner.cpp EventCleaner.hpp VariableContainer.hpp EventContainer.hpp Lepton.hpp Jet.hpp Met.hpp Particle.hpp

obj/ConfigHandler.o: ConfigHandler.cpp ConfigHandler.hpp VariableReader.hpp SampleReader.hpp CutReader.hpp ConfigReader.hpp VariableContainer.hpp SampleContainer.hpp CutContainer.hpp

obj/VariableDictionary.o: VariableDictionary.cpp VariableDictionary.hpp EventContainer.hpp Lepton.hpp Jet.hpp Met.hpp Particle.hpp

obj/EventSelecter.o: EventSelecter.cpp EventSelecter.hpp VariableDictionary.hpp EventContainer.hpp Lepton.hpp Jet.hpp Met.hpp Particle.hpp CutContainer.hpp

obj/BasePlotter.o: BasePlotter.cpp BasePlotter.hpp HistogramContainer.hpp EventContainer.hpp Lepton.hpp Jet.hpp Met.hpp Particle.hpp ConfigContainer.hpp VariableContainer.hpp SampleContainer.hpp CutContainer.hpp LatinoStyle2.h

obj/EventPlotter.o: EventPlotter.cpp EventPlotter.hpp BasePlotter.hpp HistogramContainer.hpp VariableDictionary.hpp EventContainer.hpp Lepton.hpp Jet.hpp Met.hpp Particle.hpp ConfigContainer.hpp VariableContainer.hpp SampleContainer.hpp CutContainer.hpp LatinoStyle2.h

obj/CutPlotter.o: CutPlotter.cpp CutPlotter.hpp BasePlotter.hpp HistogramContainer.hpp EventContainer.hpp Lepton.hpp Jet.hpp Met.hpp Particle.hpp ConfigContainer.hpp VariableContainer.hpp SampleContainer.hpp CutContainer.hpp LatinoStyle2.h

obj/testAnalysis.o: testAnalysis.cpp EventReader.hpp TreeReader.hpp latino.h EventContainer.hpp Lepton.hpp Jet.hpp Met.hpp Particle.hpp VariableReader.hpp VariableContainer.hpp ConfigReader.hpp SampleReader.hpp SampleContainer.hpp CutReader.hpp CutContainer.hpp CutReader.hpp EventCleaner.hpp ConfigHandler.hpp  EventSelecter.hpp VariableDictionary.hpp ConfigContainer.hpp EventPlotter.hpp BasePlotter.hpp HistogramContainer.hpp  LatinoStyle2.h

testAnalysis : $(OBJ_FILES) obj/testAnalysis.o obj/latino.o obj/LatinoStyle2.o
	$(CC) -o $@ $^ $(CFLAGS) $(CFLAGS_libconfig) $(LIBS) $(LIBS_libconfig)

obj/stackPlotter.o: stackPlotter.cpp EventReader.hpp TreeReader.hpp latino.h EventContainer.hpp Lepton.hpp Jet.hpp Met.hpp Particle.hpp VariableReader.hpp VariableContainer.hpp ConfigReader.hpp SampleReader.hpp SampleContainer.hpp CutReader.hpp CutContainer.hpp CutReader.hpp EventCleaner.hpp ConfigHandler.hpp  EventSelecter.hpp VariableDictionary.hpp ConfigContainer.hpp EventPlotter.hpp BasePlotter.hpp HistogramContainer.hpp  LatinoStyle2.h

stackPlotter : $(OBJ_FILES) obj/stackPlotter.o obj/latino.o obj/LatinoStyle2.o
	$(CC) -o $@ $^ $(CFLAGS) $(CFLAGS_libconfig) $(LIBS) $(LIBS_libconfig)

# obj/parallelStackPlots.o: parallelStackPlots.cpp EventReader.hpp TreeReader.hpp latino.h EventContainer.hpp Lepton.hpp Jet.hpp Met.hpp Particle.hpp VariableReader.hpp VariableContainer.hpp ConfigReader.hpp SampleReader.hpp SampleContainer.hpp CutReader.hpp CutContainer.hpp CutReader.hpp EventCleaner.hpp ConfigHandler.hpp  EventSelecter.hpp VariableDictionary.hpp ConfigContainer.hpp EventPlotter.hpp BasePlotter.hpp HistogramContainer.hpp 

# parallelStackPlots : $(OBJ_FILES) obj/parallelStackPlots.o obj/latino.o
# 	$(CC) -o $@ $^ $(CFLAGS) $(CFLAGS_libconfig) $(LIBS) $(LIBS_libconfig)

obj/Plot_cut_efficiency.o: Plot_cut_efficiency.cpp EventReader.hpp TreeReader.hpp latino.h EventContainer.hpp Lepton.hpp Jet.hpp Met.hpp Particle.hpp VariableReader.hpp VariableContainer.hpp ConfigReader.hpp SampleReader.hpp SampleContainer.hpp CutReader.hpp CutContainer.hpp CutReader.hpp EventCleaner.hpp ConfigHandler.hpp  EventSelecter.hpp ConfigContainer.hpp CutPlotter.hpp LatinoStyle2.h

Plot_cut_efficiency : $(OBJ_FILES) obj/Plot_cut_efficiency.o obj/latino.o obj/LatinoStyle2.o
	$(CC) -o $@ $^ $(CFLAGS) $(CFLAGS_libconfig) $(LIBS) $(LIBS_libconfig)

clean:
	-rm -f obj/*.o testAnalysis