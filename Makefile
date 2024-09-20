NAME        = libftpp.a
CFLAGS      = -std=c++11 -pedantic -g -Wall -Wextra -Werror #-fsanitize=address
CC			= c++
RM          = rm -f
SRCS_PATH	= src/
POOL_PATH	= Pool/

OBJS_PATH	= obj/
DEPS_PATH	= deps/

MAKE_OBJ_DIR	=	$(OBJS_PATH) $(addprefix $(OBJS_PATH), $(POOL_PATH))  \
								\
					$(addprefix $(DEPS_PATH), $(POOL_PATH))  
INCS        = -I./includes


#Colors


DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m
LIGHT_GREEN = \033[1;92m

###

POOL			=	pool.cpp

POOL_FILES		=	$(addprefix $(POOL_PATH), $(POOL))


SRC				+=	$(POOL_FILES) 
				
DEPS			= 	$(addprefix $(DEPS_PATH), $(SRC:.cpp=.d))

OBJS 			=	$(addprefix $(OBJS_PATH), $(SRC:.cpp=.o))

all: $(NAME)


$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "$(LIGHT_GREEN)Created $(NAME) library$(DEF_COLOR)"
	
$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp | $(MAKE_OBJ_DIR) $(DEPS_PATH)
			@echo "$(CYAN)Compiling $< $(DEF_COLOR)"
			@$(CC) $(CFLAGS) $(INCS) -MMD -MP -c $< -o $@
			@mv $(basename $@).d $(patsubst $(SRCS_PATH)%,$(DEPS_PATH)%,$(dir $<))

$(MAKE_OBJ_DIR):
	@echo "$(GREEN)Creating $(NAME) Obj Dir $(DEF_COLOR)"
	@mkdir -p $(MAKE_OBJ_DIR)


$(DEPS_PATH):
	@echo "$(GREEN)Creating $(NAME) Deps Dir $(DEF_COLOR)"
	@mkdir -p $(DEPS_PATH)

-include $(DEPS)


clean: clean_objects

fclean:  clean_objects
	@$(RM) $(NAME)
	@echo "$(GREEN)$(NAME) executable cleaned!$(DEF_COLOR)"

clean_objects:
	@echo "$(GREEN)$(NAME) Objects and Dependencies cleaned!$(DEF_COLOR)"
	@$(RM) -r $(OBJS_PATH) $(DEPS_PATH)


re: fclean all 

.PHONY: all fclean clean re 