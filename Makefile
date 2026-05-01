NAME			= lemipc

DIR_SRCS		= ./srcs


IPC_SRCS		= 	${DIR_SRCS}/ipc/init.c \
					${DIR_SRCS}/ipc/clean.c \
					${DIR_SRCS}/ipc/semaphore.c \
					${DIR_SRCS}/ipc/player_count.c \

PARSING_SRCS	= 	${DIR_SRCS}/parsing/handler.c \

SIGNAL_SRCS		= 	${DIR_SRCS}/signal/handler.c \


CC				= cc

SRCS			= 	${IPC_SRCS} \
					${PARSING_SRCS} \
					${SIGNAL_SRCS} \
					${DIR_SRCS}/main.c

OBJS_DIR		= .objs

OBJS			=	${SRCS:%.c=$(OBJS_DIR)/%.o}

DEPS			= 	${OBJS:.o=.d}

CFLAGS			= -Wall -Wextra -Werror -MMD -MP -I ./inc

$(OBJS_DIR)/%.o: %.c Makefile
			@mkdir -p $(dir $@)
			${CC} ${CFLAGS} -c $< -o $@

${NAME}:	${OBJS}
			$(CC)  $(OBJS) -o $(NAME)


-include ${DEPS}

all:		${NAME}

clean:
			rm -rf ${OBJS_DIR}

fclean:		clean
			rm -f ${NAME}

re:			fclean all

.PHONY:		all clean fclean re