target = hello
skin   = native

CFLAGS := $(shell xeno-config --skin=$(skin) --cflags)
LDFLAGS := $(shell xeno-config --skin=$(skin) --ldflags)
LDFLAGS+=-lxenomai
$(target):$(target).c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	@rm $(target)

