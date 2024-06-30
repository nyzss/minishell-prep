# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okoca <okoca@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/30 19:32:31 by okoca             #+#    #+#              #
#    Updated: 2024/06/30 12:23:01 by okoca            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL=  /bin/bash

MAKEFLAGS += --no-print-directory

LIBFT = libft

LIBFT_TARGET = ${LIBFT}/libft.a

LDFLAGS = -L${LIBFT} -lft

Black           =   $(shell echo -e "\033[0;30m")
FBlack          =   $(shell echo -e "\033[1;30m")
Red             =   $(shell echo -e "\033[0;31m")
FRed            =   $(shell echo -e "\033[1;31m")
Green           =   $(shell echo -e "\033[0;32m")
FGreen          =   $(shell echo -e "\033[1;32m")
Brown/Orange    =   $(shell echo -e "\033[0;33m")
FBrown/Orange   =   $(shell echo -e "\033[1;33m")
FYellow         =   $(shell echo -e "\033[1;33m")
Yellow          =   $(shell echo -e "\033[0;33m")
Blue            =   $(shell echo -e "\033[0;34m")
FBlue           =   $(shell echo -e "\033[1;34m")
Purple          =   $(shell echo -e "\033[0;35m")
FPurple         =   $(shell echo -e "\033[1;35m")
Cyan            =   $(shell echo -e "\033[0;36m")
FCyan           =   $(shell echo -e "\033[1;36m")
FWhite          =   $(shell echo -e "\033[1;37m")
White           =   $(shell echo -e "\033[0;37m")
RESET           =   $(shell echo -e "\033[0m")
TICK            =   $(shell echo -e "\xE2\x9C\x94")

CC = cc

SRC_DIR = src/

INCLUDES_DIR = includes

DEBUG ?= 1

CFLAGS = -Wall -Werror -Wextra -I${INCLUDES_DIR} -I${LIBFT} -D DEBUG=${DEBUG} -g

SRC_FILES = main.c \
			utils.c \
			lexer.c \
			parser.c \
			exec.c \
			builtin.c \
			history.c

NAME = prep

SRC = ${addprefix ${SRC_DIR}, ${SRC_FILES}}

OBJS = ${SRC:.c=.o}

%.o: %.c
	@${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${LIBFT_TARGET} ${OBJS}
	@echo "$$BANNER3"
	@echo "##### DEBUG MODE: ${DEBUG}"
	@echo -e "${FRed}\n>>>Building ${FPurple}${NAME}${RESET}${Red}...${RESET}"
	@${CC} ${CFLAGS} ${OBJS} -lreadline ${LDFLAGS} -o ${NAME}
	@echo "${FGreen}   Done [${TICK}]"

${LIBFT_TARGET}:
	make -C ${LIBFT}

all: ${NAME}

debug:
	${MAKE} DEBUG=1 all

release:
	${MAKE} DEBUG=0 all

clean:
	@echo "$(FRed)Cleaning $(NAME)$(FGreen)              [$(TICK)]"
	@rm -f ${OBJS}

fclean: clean
	@echo "$(FRed)Full Cleaning $(NAME)$(FGreen)         [$(TICK)]"
	@rm -f ${NAME}
	@echo "$(FYellow)------------------------------------------------------"

define BANNER
${FRed}
              _       _      __         ____
   ____ ___  (_)___  (_)____/ /_  ___  / / /
  / __ `__ \/ / __ \/ / ___/ __ \/ _ \/ / /
 / / / / / / / / / / (__  ) / / /  __/ / /
/_/ /_/ /_/_/_/ /_/_/____/_/ /_/\___/_/_/
${RESET}
endef
export BANNER


define BANNER2
${FRed}
• ▌ ▄ ·. ▪   ▐ ▄ ▪  .▄▄ ·  ▄ .▄▄▄▄ .▄▄▌  ▄▄▌
·██ ▐███▪██ •█▌▐███ ▐█ ▀. ██▪▐█▀▄.▀·██•  ██•
▐█ ▌▐▌▐█·▐█·▐█▐▐▌▐█·▄▀▀▀█▄██▀▐█▐▀▀▪▄██▪  ██▪
██ ██▌▐█▌▐█▌██▐█▌▐█▌▐█▄▪▐███▌▐▀▐█▄▄▌▐█▌▐▌▐█▌▐▌
▀▀  █▪▀▀▀▀▀▀▀▀ █▪▀▀▀ ▀▀▀▀ ▀▀▀ · ▀▀▀ .▀▀▀ .▀▀▀
${RESET}
endef
export BANNER2

define BANNER3
${FRed}
█▀▄▀█ ▄█    ▄   ▄█    ▄▄▄▄▄    ▄  █ ▄███▄   █    █
█ █ █ ██     █  ██   █     ▀▄ █   █ █▀   ▀  █    █
█ ▄ █ ██ ██   █ ██ ▄  ▀▀▀▀▄   ██▀▀█ ██▄▄    █    █
█   █ ▐█ █ █  █ ▐█  ▀▄▄▄▄▀    █   █ █▄   ▄▀ ███▄ ███▄
   █   ▐ █  █ █  ▐               █  ▀███▀       ▀    ▀
  ▀      █   ██                 ▀

${RESET}
endef
export BANNER3


re: fclean all

.PHONY: all clean fclean re
