#ifndef _CARDS_H_
#define _CARDS_H_

// Flag para os cliques nas cartas
typedef struct {
		// evento que se espera acontecer
		int ev_esperado;
		// status do evento
		int ev_status;
		// posição da carta a ser desenhada
		int card_pos;
		// contador do número de cartas já desenhadas
		int card_num;
} clk_flag;

typedef struct {
	// nível de programação
	int prog;
	// nível de condição
	int cond;
	// nível de ação
	int act;
	// vetor de "sequência correta"
	int v[15];
} challenger_rule; 

typedef struct {
    int v[15];
    int f;
} card_pile;

// Index das cartas.
typedef enum { DIRECT=0, IF=1, WHILE=2 } prog_card_t;
typedef enum { COND=1000, IF_ELSE=1001, WHILE_BREAK=1002, ARRAY=1003 } cond_card_t;
typedef enum { ATK=2000, DEF=2001, ATK_DOUBLE=2002, ARRAY_GRAB=2003, ARRAY_USE=2004 } act_card_t;

void draw_undo_card();
void draw_help_card();
void draw_memory_card();
void draw_compile_card();

void draw_prog_cards(challenger_rule cr);
void draw_conditional_cards(challenger_rule cr);
void draw_action_cards(challenger_rule cr);

void draw_selected_card(int index, int count);
void redraw_selected_cards(card_pile cp);

int check_bounds(ALLEGRO_EVENT ev, int left, int top, int right, int bottom);
int check_cards_bounds(ALLEGRO_EVENT ev, int left, int top, int right, int bottom, clk_flag *flags, int level);
int detect_click_pos(ALLEGRO_EVENT ev, clk_flag *flags, challenger_rule cr);

int validate_card_click(clk_flag *flags, int level, int card);
int validate_selection(int v1[], int v2[]);

void inicializa_vetor(int v[]);
void transferir_desafio(challenger_rule *cr, int v[]);
void transferir_condicao(char *dest, char *origin);
void cr_init(challenger_rule *cr);

void card_pile_init(card_pile *cp);
int card_pull(card_pile *cp, int n);
int card_pop(card_pile *cp);
int card_stack_count(card_pile cp);
void card_pile_to_array(card_pile cp, int v[]);

#endif