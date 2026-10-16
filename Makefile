NAME				=	lemipc

DIR_SRCS			=	./srcs

DIR_GAME_SRCS		=	${DIR_SRCS}/game

DIR_PLAYER_SRCS		=	${DIR_GAME_SRCS}/player




UTILS_SRCS			= 	${DIR_SRCS}/utils/print.c \
						${DIR_SRCS}/utils/semaphore.c \

DEBUG_SRCS			= 	${DIR_SRCS}/debug/display_map.c \

GAME_PLAYER_SRCS	= 	${DIR_PLAYER_SRCS}/mouvement.c \
						${DIR_PLAYER_SRCS}/spawn.c \
						${DIR_PLAYER_SRCS}/limit.c \

GAME_LOGIC_SRCS		= 	${DIR_GAME_SRCS}/logic.c \



IPC_SRCS			= 	${DIR_SRCS}/ipc/init.c \
						${DIR_SRCS}/ipc/clean.c \
						${DIR_SRCS}/ipc/player_count.c \

PARSING_SRCS		= 	${DIR_SRCS}/parsing/handler.c \

SIGNAL_SRCS			= 	${DIR_SRCS}/signal/handler.c \


CC					= 	cc

SRCS				=   ${DEBUG_SRCS} \
						${GAME_LOGIC_SRCS} \
						${GAME_PLAYER_SRCS} \
						${IPC_SRCS} \
						${PARSING_SRCS} \
						${SIGNAL_SRCS} \
						${UTILS_SRCS} \
						${DIR_SRCS}/main.c \

OBJS_DIR			=	.objs

OBJS				=	${SRCS:%.c=$(OBJS_DIR)/%.o}

DEPS				= 	${OBJS:.o=.d}

CFLAGS				=	-Wall -Wextra -Werror -MMD -MP -I ./inc

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
