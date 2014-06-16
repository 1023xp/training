CC = gcc

RDIR = Rels
MAIN_DIR = main/Rels
INIT_DIR = init/Rels

OBJ_FILES = $(MAIN_DIR)/main.o \
     $(INIT_DIR)/init1.o $(INIT_DIR)/init2.o $(INIT_DIR)/init3.o $(INIT_DIR)/init4.o

#build dest
$(RDIR)/dest: $(RDIR) $(MAIN_DIR) $(INIT_DIR) $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@

# make directry
$(MAIN_DIR):
	mkdir $(MAIN_DIR)

$(RDIR):
	mkdir $(RDIR)
   
$(INIT_DIR):
	mkdir $(INIT_DIR)

#compile object
$(MAIN_DIR)/main.o: main/main.c
	$(CC) -c -o $@ $^

$(INIT_DIR)/%.o: init/%.c
	$(CC) -c -o $@ $^

#clean all
clean:
	rm -r -f $(RDIR)
	rm -r -f $(INIT_DIR)
	rm -r -f $(MAIN_DIR)
