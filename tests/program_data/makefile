all: test

# Source files for target test
SOURCES = \
	../../third-party/pugixml/src/pugixml.cpp \
	../../elements/assets/asset_xml.cpp \
	../../elements/assets/assets_storage.cpp \
	../../elements/rendering/utils/program_data.cpp \
	test_program_data.cpp

# Object files for target test
OBJECTS = \
	pugixml.o \
	asset_xml.o \
	assets_storage.o \
	program_data.o \
	test_program_data.o

test: test_objects
	g++ -o test $(OBJECTS)

test_objects:
	g++ -I../../elements -I../../third-party/pugixml/src -std=c++14 -Wall -Wextra -Werror -c $(SOURCES)

clean:
	 rm $(OBJECTS) test

