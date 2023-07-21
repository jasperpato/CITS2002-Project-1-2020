PROJECT = schedule

C11 = cc -std=c11
CFLAGS = -Wall -Werror

ODIR=obj

DEPS = $(PROJECT).h

ODIR=obj
_OBJ = globals.o util.o read_file.o queue.o blocked.o schedule.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(C11) -c -o $@ $< $(CFLAGS)

$(PROJECT): $(OBJ)
	$(C11) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(PROJECT)