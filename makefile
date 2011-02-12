CSC = csc
.SUFFIXES: .cpp

tgrep : main.cpp
	$(CSC) $(.ALLSRC) -o $(.TARGET)
