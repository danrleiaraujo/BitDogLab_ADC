#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"

#include "pico/bootrom.h"

#include "features/ssd1306.h"

// I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
ssd1306_t ssd;
bool cor = true;

// Led RGB
const uint LED_B = 11;
const uint LED_R = 13;

//Joystick
#define EIXO_Y 26 //Eixo Y do JoyStik ADC 0
#define EIXO_X 27 //Eixo X do JoyStik ADC 1

// Botoes
#define BUTTON_A 5
#define BTN_STICK 22

#define DEBOUNCE_TIME 200000 // 200ms em us de debounce
uint32_t last_time = 0;      // captura o tempo do ultimo acionamento do botão para o debounce

// Variaveis
volatile bool led_B_ativado = 0; // Usado para saber qual led esta ligado
volatile bool led_R_ativado = 0; // Usado para saber qual led esta ligado
volatile bool BUTTON_A_pressionado = false;



// Funcoes
void callback_botao(uint gpio, uint32_t events);
void resposta_btns(int led);
void escreve_caractere(char c);

int main(){
    // Inicializa entradas e saídas.
    stdio_init_all();

    adc_init();
    adc_gpio_init(EIXO_Y);
    adc_select_input(0);
    
    printf("Funciona 0\n");
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                                        // Pull up the data line
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    
    ssd1306_config(&ssd);                                         // Configura o display
    
    ssd1306_send_data(&ssd);         

    // Limpa o display. O display inicia com todos os pixels LIGADOS.
    ssd1306_fill(&ssd, true);
    ssd1306_send_data(&ssd);
    
    // Inicia Leds
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);

    // Inicia botao A
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // Inicia joystick
    gpio_init(BTN_STICK);
    gpio_set_dir(BTN_STICK, GPIO_IN);
    gpio_pull_up(BTN_STICK);

    //função para iqr
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_RISE, true, &callback_botao);
    gpio_set_irq_enabled_with_callback(BTN_STICK, GPIO_IRQ_EDGE_FALL, true, &callback_botao); //Bootloader

    while (true){
    } // Fim While True
} // Fim main


void escreve_caractere(char c){ // atualiza o display com o caractere inserido pelo usuario
    ssd1306_fill(&ssd, cor);
    ssd1306_rect(&ssd, 3, 3, 122, 58, !cor, cor);
    ssd1306_draw_string(&ssd, "CARACTERE", 28, 10);
    ssd1306_draw_string(&ssd, "         ", 20, 30);
    ssd1306_draw_string(&ssd, &c, 63, 30);
    ssd1306_draw_string(&ssd, "PRESSIONADO", 20, 48);
    ssd1306_send_data(&ssd);
}

void resposta_btns(int led){ //trata o acionamento dos botoes, verificando estado e cor dos leds

}

//função para indicar que botão está sendo apertado
void callback_botao(uint gpio, uint32_t events){
    uint32_t current_time = to_us_since_boot(get_absolute_time()); // captura o momento do acionamento e converte para microsegundos
    if(current_time - last_time > DEBOUNCE_TIME){
        if (gpio == BUTTON_A) {

        }
        else if(gpio == BTN_STICK){
            reset_usb_boot(0, 0); //func para entrar no modo bootsel 
        }
        last_time = current_time; // Atualiza o tempo para o debounce
    } // debounce
}


/*
#include <stdio.h>
#include "pico/stdlib.h"
#include <hardware/adc.h>
#include <hardware/pwm.h>

#define EIXO_Y 26 //Eixo Y do JoyStik ADC 0
//#define EIXO_X 27 Eixo X do JoyStik ADC 1
#define LED_PIN 12 //Pino do Led Azul

int main()
{
    
    adc_init();
    adc_gpio_init(EIXO_Y);
    adc_select_input(0);

    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(LED_PIN);
    pwm_set_enabled(slice, true);
    pwm_set_gpio_level(LED_PIN, 0);

    uint16_t result = adc_read();

    while (true) {
        uint16_t result = adc_read();
        pwm_set_gpio_level(LED_PIN, 16*result);
        sleep_ms(50);
    }
}
*/