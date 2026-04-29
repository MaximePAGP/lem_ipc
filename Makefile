NAME			= lemipc


SIGNAL_SRCS		= 	./srcs/signal/handler.c \


CC				= cc

SRCS			= 	${SIGNAL_SRCS} \
					./srcs/main.c

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