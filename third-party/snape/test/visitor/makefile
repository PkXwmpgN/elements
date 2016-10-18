OBJDIR_BUILD = build
OBJ = $(OBJDIR_BUILD)/test_visitor.o

all: directory test

test: $(OBJ)
	g++ -o $(OBJDIR_BUILD)/test $<

directory $(OBJDIR_BUILD):
	mkdir -p $(OBJDIR_BUILD)

$(OBJ): $(OBJDIR_BUILD)/%.o: %.cpp
	g++ -I../../include -std=c++14 -Wall -Wextra -Werror -c $< -o $@

clean:
	 rm -r $(OBJDIR_BUILD)
