CXXFLAGS = -std=c++17 -municode -mwindows
OBJ := main.o Object.o Game.o

juego.exe: $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

test: juego.exe
	juego.exe

clean:
	@del /Q *.o 2>nul
	
.PHONY: clean test