################################################################################
#
#
#  Universidade Federal Rural do Rio de Janeiro
#  Instituto Multidisciplinar - Departamento de Ciencia da Computacao
#  por: Marcelo Zamith - zamith.marcelo@gmail.com
#  para compilar com target release use o comando: make target=release + rápido
#
#  Versão sobre propação de informação online. Caso do whatsapp
#
################################################################################
EXEFILE      = IModelCA++
APPLICATION  = -D_APPLICATION=\"$(EXEFILE)\"
CPUCC       = g++
CPPFLAGS    =  -m64 -Wno-reorder #     -Wunused-variable #-Wno-conversion-null -Wdelete-non-virtual-dtor
DEFS        = $(APPLICATION)  $(VERSION) -DALIGN=64
INCLUDES    = -I.
LIBDIR      =
LIBS     =  -lm -ljsoncpp
LINK     =  $(LIBDIR) $(LIBS)
CPU_COMPILE = $(CPUCC) $(DEFS) $(INCLUDES) $(CPPFLAGS)
MKDIR_P     = mkdir -p
OUT_DIR     = obj

ifeq ($(target), release)
  DEFS      +=
  CPPFLAGS  +=      -O2
else
  DEFS     += -D_DEBUG
  CPPFLAGS += -g -Wall -O0
endif

all: directories exec 

exec:	Agent                \
        SocialNetwork        \
		main

	$(CPU_COMPILE)  obj/Agent.o             \
	                obj/SocialNetwork.o     \
			        obj/main.o             \
	$(LINK) -o $(EXEFILE)

main:
	$(CPU_COMPILE) -c main.cpp -o obj/main.o

SocialNetwork:
	$(CPU_COMPILE) -c SocialNetwork.cpp -o obj/SocialNetwork.o

Agent:
	$(CPU_COMPILE) -c Agent.cpp -o obj/Agent.o


directories:
	${MKDIR_P} ${OUT_DIR}
clean:
	rm obj/*.o $(EXEFILE)


	
