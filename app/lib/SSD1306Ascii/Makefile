#
# Makefile
#

CIRCLEHOME = ../../..

OBJS	=  SSD1306Ascii.o 

LIBS	= $(CIRCLEHOME)/app/lib/arduino/libarduino.a

libssd1306ascii.a: $(OBJS)
	@echo "  AR    $@"
	@rm -f $@
	@$(AR) cr $@ $(OBJS)

include $(CIRCLEHOME)/Rules.mk
