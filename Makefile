CC = clang-17
CFLAGS = -Wall -Wextra -g
CPPFLAGS = -Iinclude
LDFLAGS = -Llib -lraylib -lm

.PHONY: clean

app: example.c olive.c utils.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $<

wasm: wasm.c
	clang-17 $(CFLAGS) --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export=render -Wl,--export-memory -Wl,--allow-undefined -o app.wasm $^

3d: 3d.c
	clang-17 $(CFLAGS) --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export=render -Wl,--export-memory -Wl,--allow-undefined -o 3d.wasm $^

ray: ray.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< $(LDFLAGS) -o $@

clean:
	rm -f app *.ppm *.png wasm 3d ray
