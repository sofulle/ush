# Makefile

EXEC_DIR = project
EXEC_FILE = ush

TESTS_DIR = tests

CHECK_DIR = project


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

UNSET 	= \033[0m

PRMPT = [$(UNDER)$(VIOLET)main$(UNSET)]


all: $(EXEC_FILE) copy


$(EXEC_FILE):
	@printf "$(PRMPT) Starting makefile for $(UNDER)$(EXEC_FILE)$(UNSET)...\n"
	@make -s -C $(EXEC_DIR)


tests:
	@printf "$(PRMPT) Starting tests for $(UNDER)$(EXEC_FILE)$(UNSET)...\n"
	@./tests.sh $(EXEC_FILE) $(EXEC_DIR) $(TESTS_DIR)


uniqueness:
# @printf "$(PRMPT) Starting makefile for $(UNDER)$(CHECK_DIR)/$(EXEC_FILE)$(UNSET)...\n"
# @make -s -C $(CHECK_DIR)
	@printf "$(PRMPT) Starting uniqueness check for $(UNDER)$(EXEC_FILE)$(UNSET)...\n"
	@./check.sh $(EXEC_FILE) $(EXEC_DIR) $(CHECK_DIR)


copy:
	@printf "${PRMPT} Copying executable file to /usr/bin\t"
	@sudo cp $(EXEC_DIR)/$(EXEC_FILE) /usr/bin
	@printf "${GREEN}copied${UNSET}\n"


.PHONY: all tests uniqueness copy
