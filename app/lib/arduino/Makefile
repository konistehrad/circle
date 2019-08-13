#
# Makefile
#

CIRCLEHOME = ../../..

OBJS	=  Print.o twi.o Wire.o WString.o itoa.o

libarduino.a: $(OBJS)
	@echo "  AR    $@"
	@rm -f $@
	@$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
