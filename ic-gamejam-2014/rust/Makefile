RUSTC := rustc

RUSTC_LIB_DIRS := -L /vol/bitbucket/da913/public/CSFML/lib/ -L /vol/bitbucket/da913/public/rust-sfml/lib/
RUSTC_FLAGS :=

CRATES := src/main.rs

all: game

game: $(RS_FILES)
	mkdir -p bin
	$(RUSTC) $(RUSTC_FLAGS) $(RUSTC_LIB_DIRS) $(CRATES) -o ./bin/game

clean:
	$(RM) -rf ./bin/*

.PHONY: clean
