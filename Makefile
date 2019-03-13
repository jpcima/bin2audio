PREFIX = /usr/local
CXX = g++ -std=c++11
CXXFLAGS = -O2 -g -Wall
LDFLAGS =

CXXFLAGS += `pkg-config sndfile --cflags`
LDFLAGS += `pkg-config sndfile --libs`

all: bin2audio

clean:
	rm -f *.o bin2audio

install: all
	install -D -m755 bin2audio $(DESTDIR)$(PREFIX)/bin/bin2audio

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

bin2audio: bin2audio.o
	$(CXX) -o $@ $^ $(LDFLAGS)
