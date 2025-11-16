# Compilateur
CXX = g++
CXXFLAGS = -Wall -Wextra -g -std=c++17

# Nom de l'exécutable
EXEC = main

# Fichiers sources
SRCS = main.cpp \
       outils.cpp \
       chargesauve.cpp

# Fichiers objets générés automatiquement
OBJS = $(SRCS:.cpp=.o)

# Règle principale
all: $(EXEC)

# Edition de liens
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation des .cpp en .o
%.o: %.cpp outils.h image.h chargesauve.h
	$(CXX) $(CXXFLAGS) -c $<

# Nettoyage
clean:
	rm -f *.o $(EXEC)

# Nettoyage complet
mrproper: clean
	rm -f $(EXEC)

.PHONY: all clean mrproper