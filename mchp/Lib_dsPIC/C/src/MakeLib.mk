.SUFFIXES:
.SUFFIXES: .c .o .eo .s .a .epo .epeo .ep24o.ep24eo
CPU=30F6014
DACPU=24FJ256DA210
EPCPU=33EP512MU814
EP24CPU=24EP512GU810
CHCPU = GENERIC-16DSP-CH


GCC1    = xc16-gcc
AR1     = xc16-ar
AS1 	= xc16-as

CC   = "$(GCCPATH)/$(GCC1)"
CPP  = "$(GCCPATH)/$(GCC1)"
LD   = "$(GCCPATH)/$(GCC1)"
AR   = "$(GCCPATH)/$(AR1)" -r
AS   = "$(GCCPATH)/$(AS1)"

OPTIM_GCC 		=  -Wall -O3 $(C_OPTS) -mconst-in-data -fverbose-asm -msmall-code -msmall-data -mtext=.libfp 
ASFLAGS=-ahldi=$*.lst ${AS_OPTS}

IDIR =../include
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = $(wildcard $(IDIR)*.h)
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = $(patsubst %.s,%.o,$(wildcard *.s)) $(patsubst %.c,%.o,$(wildcard *.c))
OBJ_COFF = $(patsubst %,$(ODIR)/%,$(_OBJ))

# rule for c files
$(ODIR)/%.o : %.c $(DEPS)
	$(CC) -c -omf=coff $(OPTIM_GCC) -menable-fixed $< -o$@ -mcpu=$(CPU)

$(ODIR)/%.eo : %.c $(DEPS)
	$(CC) -c -omf=elf $(OPTIM_GCC) -menable-fixed $< -o$@ -mcpu=$(CPU)

$(ODIR)/%.epo : %.c $(DEPS)
	$(CC) -c -omf=coff $(OPTIM_GCC) -menable-fixed $< -o$@ -mcpu=$(EPCPU)

$(ODIR)/%.epeo : %.c $(DEPS)
	$(CC) -c -omf=elf $(OPTIM_GCC) -menable-fixed $< -o$@ -mcpu=$(EPCPU)


# rule for assembly files
$(ODIR)/%.o : %.s $(DEPS) 
	$(AS) -omf=coff -o $@ $(ASFLAGS) $< -p $(CPU)

$(ODIR)/%.eo : %.s $(DEPS) 
	$(AS) -omf=elf -o $@ $(ASFLAGS) $< -p $(CPU)	

$(ODIR)/%.epo : %.s $(DEPS) 
	$(AS) -omf=coff -o $@ $(ASFLAGS) $< -p $(EPCPU)

$(ODIR)/%.epeo : %.s $(DEPS) 
	$(AS) -omf=elf -o $@ $(ASFLAGS) $< -p $(EPCPU)	


OBJ_ELF 	= $(OBJ_COFF:.o=.eo)
OBJ_EP_COFF	= $(OBJ_COFF:.o=.epo)
OBJ_EP_ELF 	= $(OBJ_COFF:.o=.epeo)

OBJ_ALL_ELF = $(OBJ_ELF)  $(OBJ_EP_ELF)
OBJ_ALL_COFF = $(OBJ_COFF)  $(OBJ_EP_COFF)

# Build library
ifeq ($(OMF),elf)
$(LDIR)/$(OUTPUT)_elf.a: $(OBJ_ALL_ELF)
	$(AR) -omf=elf $@ $^
else
$(LDIR)/$(OUTPUT)_coff.a: $(OBJ_ALL_COFF)
	$(AR) -omf=coff $@ $^
endif


