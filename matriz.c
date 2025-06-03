#include "matriz.h"

// Número de LEDs na matriz (5x5)
#define NUM_PIXELS 25
#define OUT_PIN 7

// Luz vermelha (parte superior do semáforo)
static double desenho_alerta1[25] = {0.0, 0.0, 1.0, 0.0, 0.0,
                                    0.0, 0.0, 1.0, 0.0, 0.0, 
                                    0.0, 0.0, 1.0, 0.0, 0.0,
                                    0.0, 0.0, 1.0, 0.0, 0.0,
                                    0.0, 0.0, 1.0, 0.0, 0.0};

static double desenho_alerta2[25] = {0.0, 0.0, 0.0, 0.0, 1.0,
                                    0.0, 0.0, 0.0, 1.0, 0.0, 
                                    0.0, 0.0, 1.0, 0.0, 0.0,
                                    0.0, 1.0, 0.0, 0.0, 0.0,
                                    1.0, 0.0, 0.0, 0.0, 0.0};

static double desenho_alerta3[25] = {0.0, 0.0, 0.0, 0.0, 0.0,
                                    0.0, 0.0, 0.0, 0.0, 0.0, 
                                    1.0, 1.0, 1.0, 1.0, 1.0,
                                    0.0, 0.0, 0.0, 0.0, 0.0,
                                    0.0, 0.0, 0.0, 0.0, 0.0};


static double desenho_alerta4[25] = {1.0, 0.0, 0.0, 0.0, 0.0,
                                    0.0, 1.0, 0.0, 0.0, 0.0, 
                                    0.0, 0.0, 1.0, 0.0, 0.0,
                                    0.0, 0.0, 0.0, 1.0, 0.0,
                                    0.0, 0.0, 0.0, 0.0, 1.0};

// Estrutura do semáforo completo (para visualização)
static double apagar[25] = {0.0, 0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0, 0.0, 
                              0.0, 0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0, 0.0};

// --- Frames para a Animação da Ventoinha ---
static double ventoinha_frame_1[NUM_PIXELS] = { // "|"
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0
};

static double ventoinha_frame_2[NUM_PIXELS] = { // "/"
    0.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 0.0, 0.0
};

static double ventoinha_frame_3[NUM_PIXELS] = { // "-"
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
};

static double ventoinha_frame_4[NUM_PIXELS] = { // "\"
    1.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 1.0
};

// Vetor de ponteiros para os frames da animação (esta é a variável "global" que você queria)
// "static" a torna global apenas dentro de matriz.c
static double* animacao_ventoinha_frames[] = {
    ventoinha_frame_1,
    ventoinha_frame_2,
    ventoinha_frame_3,
    ventoinha_frame_4
};
static const int VENTOINHA_NUM_TOTAL_FRAMES = sizeof(animacao_ventoinha_frames) / sizeof(animacao_ventoinha_frames[0]);
static int ventoinha_frame_atual_idx = 0; // Índice para controlar qual frame exibir

// Função para desenhar o próximo frame da animação da ventoinha
void animacao_ventoinha_desenhar_proximo_frame(PIO pio, uint sm, double r, double g, double b) {
    uint32_t valor_led_dummy = 0; // O valor real é definido dentro de desenho_pio

    // Pega o ponteiro para o padrão do frame atual
    double* padrao_frame_atual = animacao_ventoinha_frames[ventoinha_frame_atual_idx];

    // Usa sua função existente para desenhar o padrão com a cor especificada
    desenho_pio(padrao_frame_atual, valor_led_dummy, pio, sm, r, g, b);

    // Avança para o próximo frame, voltando ao início se necessário
    ventoinha_frame_atual_idx++;
    if (ventoinha_frame_atual_idx >= VENTOINHA_NUM_TOTAL_FRAMES) {
        ventoinha_frame_atual_idx = 0;
    }
}

// Rotina para definir a intensidade das cores do LED (RGB)
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255; // Converte a intensidade de vermelho para 8 bits
  G = g * 255; // Converte a intensidade de verde para 8 bits
  B = b * 255; // Converte a intensidade de azul para 8 bits
  return (G << 24) | (R << 16) | (B << 8); // Retorna o valor RGB no formato 32 bits
}

void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        // Define a cor do LED com base no vetor de desenho
        if (desenho[24-i]>0.000) 
        {
            valor_led = matrix_rgb(b, r, g);
        }
        else
        {
            valor_led = matrix_rgb(0.0, 0.0, 0.0);
        }
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para a matriz de LEDs via PIO
    }
}

// Função para exibir o semáforo na matriz de LEDs
void apagar_matriz(PIO pio, uint sm)
{
    uint32_t valor_led;

    desenho_pio(apagar, valor_led, pio, sm, 0.0, 0.0, 0.0); // Vermelho (r=1.0)
}


uint pio_init(PIO pio)
{
    // Ajusta o clock do RP2040 para 128 MHz (128000 kHz)
    set_sys_clock_khz(128000, false);

    // Carrega o programa PIO na memória do PIO e retorna o offset onde ele foi colocado
    uint offset = pio_add_program(pio, &pio_matrix_program);

    // Reserva um state machine livre (bloqueante) e retorna seu índice
    uint sm = pio_claim_unused_sm(pio, true);

    // Inicializa o state machine com o programa carregado, definindo pino de saída
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    return sm;
}

