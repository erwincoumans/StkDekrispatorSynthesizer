# GNU Make solution makefile autogenerated by Premake
# Type "make help" for usage help

ifndef config
  config=release32
endif
export config

PROJECTS := App_Dekrispator stk

.PHONY: all clean help $(PROJECTS)

all: $(PROJECTS)

App_Dekrispator: stk
	@echo "==== Building App_Dekrispator ($(config)) ===="
	@${MAKE} --no-print-directory -C . -f App_Dekrispator.make


stk: 
	@echo "==== Building stk ($(config)) ===="
	@${MAKE} --no-print-directory -C . -f stk.make

clean:
	@${MAKE} --no-print-directory -C . -f App_Dekrispator.make clean
	@${MAKE} --no-print-directory -C . -f stk.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "   release32"
	@echo "   debug32"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   App_Dekrispator"
	@echo "   stk"
	@echo ""
	@echo "For more information, see http://industriousone.com/premake/quick-start"
