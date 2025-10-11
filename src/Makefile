# le nom de l'éxécutable
TARGET = ppmviewer.exe

# compilateur
GCC = gcc

# Options de compilations
CFLAGS = -Wall -g -Wextra -Wno-unused-variable

# Fichiers sources et objets
SRCS = main.c ppm_app.c
OBJS = $(patsubst %.c, %.o, $(SRCS))

# Règle principale
$(TARGET): $(OBJS)
	$(GCC) $(OBJS) -o $(TARGET)


# Règle pour les .o
%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

# Nétoyage
clean: 
	rm -f $(OBJS) $(TARGET)
