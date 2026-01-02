CC = clang-17
CFLAGS = -Wall -Wextra -g
CPPFLAGS = -Iinclude
LDFLAGS = -Llib -lraylib -lm

BIN = bin

.PHONY: clean app ray wasm 3d all

all: $(BIN)/app $(BIN)/ray $(BIN)/app.wasm $(BIN)/3d.wasm
app: $(BIN)/app
ray: $(BIN)/ray
wasm: $(BIN)/app.wasm
3d: $(BIN)/3d.wasm

$(BIN):
	mkdir -p $(BIN)

$(BIN)/app: example.c | $(BIN)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^
	mkdir -p pics

$(BIN)/app.wasm: wasm.c | $(BIN)
	clang-17 $(CFLAGS) --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export=render -Wl,--export-memory -Wl,--allow-undefined -o $@ $^

$(BIN)/3d.wasm: 3d.c | $(BIN)
	clang-17 $(CFLAGS) --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export=render -Wl,--export-memory -Wl,--allow-undefined -o $@ $^

$(BIN)/ray: ray.c | $(BIN)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< $(LDFLAGS) -o $@

clean:
	rm -rf $(BIN) pics
