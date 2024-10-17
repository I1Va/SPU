CC = g++

CC_FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal       \
	-Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch     \
	-Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions                   \
	-Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 \
	-Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor   \
	-Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing \
	-Wstrict-null-sentinel -Wtype-limits -Wwrite-strings #-Werror

CC_DEBUG_FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported \
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security \
-Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual\
 -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
 -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override\
  -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros\
   -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs \
   -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging\
    -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla\
	 -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,$\
	 integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,$\
	 shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

INCLUDE_FLAGS = -Iinc

DEFINE_FLAGS = -D_DEBUG

BUILD_DIR = build

SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
SRC_OUTPUTFILE = $(BUILD_DIR)/assembler.out

# LD_FLAGS =

.PHONY: build

all: launch

make_src_folder:
	@mkdir -p $(BUILD_DIR)

build: make_src_folder
	$(CC) $(SRC_FILES) -O0 -o $(SRC_OUTPUTFILE) $(INCLUDE_FLAGS) $(CC_FLAGS)

debug: make_src_folder
	$(CC) $(SRC_FILES) -o $(SRC_OUTPUTFILE) $(DEFINE_FLAGS) $(INCLUDE_FLAGS) $(CC_DEBUG_FLAGS)

launch: make_src_folder debug
	./$(SRC_OUTPUTFILE)

# clear

# навести порядок в makefile


