// libs padrões do C
#include <stdio.h>
#include <stdlib.h>

// libs do allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

// libs implementadas
#include "fases/fases.h"
#include "geral/dialog.h"

// Atributos da tela
const int LARGURA_TELA = 800;
const int ALTURA_TELA = 600;
    
// Variável global representando a janela principal
ALLEGRO_DISPLAY *janela = NULL;

int main() {
    int fases_resultado;
    // Variável representando a posição de tela
    ALLEGRO_DISPLAY_MODE disp_data;
    // Variável representando as imagens (menu)
    ALLEGRO_BITMAP *logo = NULL, *jogar = NULL, *sair = NULL;
    // Variável representando as interações de eventos
    ALLEGRO_EVENT_QUEUE *interacao = NULL;
    // Variável representando eventos
    ALLEGRO_EVENT evento;
    // Variável representando cor da fonte
    ALLEGRO_COLOR font_color;
    // Variável representando as fontes utilizadas
    ALLEGRO_FONT *font;

    // Inicializa a Allegro
    if (!al_init()) {
        fprintf(stderr, "Falha ao iniciar o Allegro!\n");
    }

    // Inicializa o add-on para utilização de imagens
    if (!al_init_image_addon()) {
        show_dialog("Código de Honra", "Erro!", "Falha ao inicializar o add-on de imagens.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
    }

    // inicializa o add-on para fontes
    al_init_font_addon();
    // inicializa o add-on para fontes true-type
    al_init_ttf_addon();
    // Atribui em disp_data as configurações de tela    
    al_get_display_mode(0, &disp_data);

    // Configura o display
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela){
        fprintf(stderr, "Falha ao criar display!\n");
        return -1;
    }
    
    // Atribui onde o display será posicionado
    al_set_window_position(janela, (disp_data.width-LARGURA_TELA)/2, (disp_data.height-ALTURA_TELA)/2);
    // Configura o título do display
    al_set_window_title(janela, "CÓDIGO DE HONRA!");

    // Torna apto o uso de mouse na aplicação
    if (!al_install_mouse()){
        show_dialog("Código de Honra", "Erro!", "Falha ao inicializar o uso de mouse.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(janela);
        return -1;
    }

    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)) {        
        show_dialog("Código de Honra", "Erro!", "Falha ao atribuir um ponteiro ao mouse.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(janela);
        return -1;
    }

    // carrega a fonte a ser usada
    font_color = al_map_rgb(0, 0, 0);
    font = al_load_ttf_font("res/font/architectsdaughter.ttf", 60, 0);
    if (!font) {
        show_dialog("Código de Honra", "Erro!", "Falha ao inicializar a fonte dos textos.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(janela);
        return 0;
    }  

    // Carrega as imagens
    logo = al_load_bitmap("res/img/menu/logo.png");
    jogar = al_load_bitmap("res/img/menu/jogar.png");
    sair = al_load_bitmap("res/img/menu/sair.png");

    // Desenha as imagens na tela
    al_clear_to_color(al_map_rgb(255, 255, 255)); // desenha fundo branco
    al_draw_bitmap(logo, 100, 50, 0); // x, y, z(inverte imagem)
    al_draw_bitmap(jogar, 50, 350, 0); // x, y, z(inverte imagem)
    al_draw_bitmap(sair, 50, 450, 0); // x, y, z(inverte imagem)

    // Atualiza a tela
    al_flip_display();

    // Criando interações do mouse
    interacao = al_create_event_queue();
    if (!interacao) {
        show_dialog("Código de Honra", "Erro!", "Falha ao inicializar a fila de eventos.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(janela);
        return -1;
    }

    // Inicialização das interações do mouse
    al_register_event_source(interacao, al_get_mouse_event_source());
    al_register_event_source(interacao, al_get_display_event_source(janela));
    
    while (1) {
        // Espera até que um evento ocorra
        al_wait_for_event(interacao, &evento);
        
        // Se houver clique no [X] ele registra o evento e para a execução do jogo
        if (interacao && evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        // Se for um evento do tipo clique, vê a posição do clique.
        if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            // Botão sair.
            if (evento.mouse.x >= 50 && evento.mouse.x <= al_get_bitmap_width(sair) + 50 &&
                evento.mouse.y >= 450 && evento.mouse.y <= al_get_bitmap_height(sair) + 450) {
                printf("\nParando a execução...");
                break;
            } 
            // Botão jogar.
            else if (evento.mouse.x >= 50  && evento.mouse.x <= al_get_bitmap_width(jogar) + 50 &&
                     evento.mouse.y >= 350  && evento.mouse.y <= al_get_bitmap_height(jogar) + 350) {
                printf("\nCarregando a primeira fase...\n");

                // Desregistra os eventos de mouse (novos eventos serão criados)
                al_unregister_event_source(interacao, al_get_mouse_event_source());

                fases_resultado = fases_manager();
                printf("\nFim da execução das fases.");
                
                // Inicializa o módulo que lida com as fases do jogo.
                if (fases_resultado >= 0) {
                     // Reconstroi a tela inicial

                        al_clear_to_color(al_map_rgb(255, 255, 255)); 
                        al_draw_bitmap(logo, 100, 50, 0); 
                        al_draw_bitmap(jogar, 50, 350, 0); 
                        al_draw_bitmap(sair, 50, 450, 0);

                        al_register_event_source(interacao, al_get_mouse_event_source());

                        al_flip_display();
                } else {
                    show_error_dialog("Favor contactar os desenvolvedores do jogo.");
                    break;
                }
            }
        }
        // Atualiza a tela
        al_flip_display();            
    }

    // Segura a execução
    al_rest(0.5);
    printf("\nFim da execução!\n");
 
    // Finaliza a display
    al_destroy_display(janela);
 
    return 0;
}

