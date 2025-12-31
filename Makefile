CC = clang-17
CFLAGS = -Wall -Wextra -g
CPPFLAGS =
LDFLAGS =

.PHONY: clean

app: example.c olive.c utils.c
	$(CC) $(CFLAGS) -o $@ $<

wasm: wasm.c
	clang-17 $(CFLAGS) --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export=render -Wl,--export-memory -Wl,--allow-undefined -o app.wasm $^

clean:
	rm -f app *.ppm
