#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include <hardware/adc.h>
#include "hardware/pwm.h"


#include "pico/bootrom.h"

#include "features/ssd1306.h"

// I2C - OLED 128X64 -> x 0 - 127, ,y 0 - 63;
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
#define EIXO_X 26 //Eixo X do JoyStik ADC 0
#define EIXO_Y 27 //Eixo Y do JoyStik ADC 1

// Botoes
#define BUTTON_A 5
#define BTN_STICK 22

#define DEBOUNCE_TIME 200000 // 200ms em us de debounce
uint32_t last_time = 0;      // captura o tempo do ultimo acionamento do botão para o debounce

// PWM
#define MAX_PULSE 2048 
const float DIVIDER_PWM = 125.0; // Valor encontrado em calculos na parte de anotações
int pulso;


// Variaveis
volatile bool led_B_ativado = 0; // Usado para saber qual led esta ligado
volatile bool led_R_ativado = 0; // Usado para saber qual led esta ligado
volatile bool BUTTON_A_pressionado = false;



// Funcoes
void callback_botao(uint gpio, uint32_t events);
void resposta_btns(int led);

int main(){
    // Inicializa entradas e saídas.
    stdio_init_all();

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
    
    // Inicia ADC
    adc_init();
    adc_gpio_init(EIXO_X);
    adc_gpio_init(EIXO_Y);

    uint16_t adc_value_x;
    uint16_t adc_value_y; 

    // Inicia PWM
    gpio_set_function(LED_B, GPIO_FUNC_PWM);
    uint slice_blue = pwm_gpio_to_slice_num(LED_B); //obter o canal (slice) PWM da GPIO
    pwm_set_clkdiv(slice_blue, DIVIDER_PWM); //define o divisor de clock do PWM
    pwm_set_wrap(slice_blue, MAX_PULSE); //definir o valor de wrap – valor máximo do contador PWM
    pwm_set_enabled(slice_blue, 1); //habilitar o pwm no slice correspondente
    
    gpio_set_function(LED_R, GPIO_FUNC_PWM);
    uint slice_red = pwm_gpio_to_slice_num(LED_R); //obter o canal (slice) PWM da GPIO
    pwm_set_clkdiv(slice_red, DIVIDER_PWM); //define o divisor de clock do PWM
    pwm_set_wrap(slice_red, MAX_PULSE); //definir o valor de wrap – valor máximo do contador PWM
    pwm_set_enabled(slice_red, 1); //habilitar o pwm no slice correspondente


    // I2C Initialisation. Using it at 400Khz.
    /*
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
        
    ssd1306_rect(&ssd, 63, 31, 8, 8, !cor, cor);
    ssd1306_send_data(&ssd);
    */
    while (true){
        adc_select_input(0);
        adc_value_x = adc_read();
        printf("Valor x do analogico: %u\n",adc_value_x);
        sleep_ms(100);
        adc_select_input(1);
        adc_value_y = adc_read();
        printf("Valor y do analogico: %u\n\n",adc_value_y);
        sleep_ms(100);

    } // Fim While True
} // Fim main




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