# Makefile

# Executable file name
EXEC_FILE = ush

# Library path and name
LIB_DIR = libmx
LIB_FILE = libmx.a

# Compiler settings
CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

# Serial list of tasks
PIPELINE = uninstall lib build link


SRC_FILES = $(wildcard src/*.c)
OBJ_FILES = $(addprefix obj/, $(notdir $(SRC_FILES:%.c=%.o)))
INC_FILES = $(wildcard inc/*.h)

BLACK	= \033[30;1m
GREEN 	= \033[32;1m
RED 	= \033[31;1m
YELLOW 	= \033[33;1m
BLUE	= \033[34;1m
VIOLET	= \033[35;1m
CYAN	= \033[36;1m
GRAY	= \033[37;1m

BOLD	= \033[1m
OPACITY	= \033[2m
UNDER	= \033[4m
BLINK	= \033[5m

UNSET 	= \033[0m

PRMPT = [$(BLUE)$(EXEC_FILE)$(UNSET)]


all: $(PIPELINE)


lib:
	@printf "$(PRMPT) Starting makefile for $(UNDER)$(LIB_FILE)$(UNSET)...\n"
	@make -s -C $(LIB_DIR)


link:
	@printf "$(PRMPT) Linking executable file\t"
	@$(CC) $(CFLAGS) obj/* -I inc/ $(LIB_DIR)/$(LIB_FILE) -o $(EXEC_FILE)
	@printf "$(GREEN)linked$(UNSET)\n"


archive:
	@printf "$(PRMPT) Archiving library\t\t"
	@ar -rc $(EXEC_FILE) obj/*.o
	@printf "$(GREEN)archived$(UNSET)\n"


index:
	@printf "$(PRMPT) Indexing library\t\t"
	@ranlib $(EXEC_FILE)
	@printf "$(GREEN)indexed$(UNSET)\n"


build: $(OBJ_FILES)
	@printf "\r$(PRMPT) Compiling                                                                                                            "
	@printf "\r$(PRMPT) Compiling object files\t$(GREEN)compiled$(UNSET)\n"


$(OBJ_FILES): obj


obj:
	@mkdir -p $@


obj/%.o: src/%.c $(INC_FILES)
	@printf "\r$(PRMPT) Compiling                                                                                                            "
	@printf "\r$(PRMPT) Compiling $(<:src/%.c=%).o"
	@$(CC) $(CFLAGS) -c $< -o $@ -I inc


clean:
	@printf "$(PRMPT) Deleting obj/ directory\t"
	@rm -rf obj $@
	@rm -rf $(OBJ_FILES) $@
	@printf "$(RED)deleted$(UNSET)\n"

fullclean:
	@make clean -sC $(LIB_DIR)
	@printf "$(PRMPT) Deleting obj/ directory\t"
	@rm -rf obj $@
	@rm -rf $(OBJ_FILES) $@
	@printf "$(RED)deleted$(UNSET)\n"


uninstall:
	@printf "$(PRMPT) Deleting obj/ directory\t"
	@rm -rf obj $@
	@rm -rf $(OBJ_FILES) $@
	@printf "$(RED)deleted$(UNSET)\n"
	@printf "$(PRMPT) Deleting executable file\t"
	@rm -rf $(EXEC_FILE)
	@printf "$(RED)deleted$(UNSET)\n"

fulluninstall:
	@make uninstall -sC $(LIB_DIR)
	@printf "$(PRMPT) Deleting obj/ directory\t"
	@rm -rf obj $@
	@rm -rf $(OBJ_FILES) $@
	@printf "$(RED)deleted$(UNSET)\n"
	@printf "$(PRMPT) Deleting executable file\t"
	@rm -rf $(EXEC_FILE)
	@printf "$(RED)deleted$(UNSET)\n"


reinstall: uninstall all


.PHONY: all lib link archive index build uninstall clean fullclean fulluninstall reinstall
