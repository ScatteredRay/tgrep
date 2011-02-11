CSC = csc
.SUFFIXES: .o .cpp

.cpp.o:
	$(CSC) -c $(.IMPSRC)

tgrep : main.cpp
	$(CSC) $(.ALLSRC) -o $(.TARGET)
