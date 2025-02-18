#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include <hardware/adc.h>
#include "hardware/pwm.h"

#include "pico/bootrom.h"

#include "lib/ssd1306.h"

// I2C - OLED 128X64 -> x 0 - 127, ,y 0 - 63;
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
bool cor = true;

// Led RGB
const uint LED_B = 13;
const uint LED_R = 12;
const uint LED_G = 11;

//converte_joystic k
#define EIXO_X 26 //Eixo X do JoyStik ADC 0
#define EIXO_Y 27 //Eixo Y do JoyStik ADC 1

// Botoes
#define BUTTON_A 5
#define BUTTON_B 6
#define BTN_STICK 22

#define DEBOUNCE_TIME 200000 // 200ms em us de debounce
uint32_t last_time = 0;      // captura o tempo do ultimo acionamento do botão para o debounce

// PWM
#define PWM_FREQ   20000 // 20 kHz
#define PWM_WRAP   255   // 8 bits de wrap (256 valores)
const float DIVIDER_PWM = 125.0; // Valor encontrado em calculos na parte de anotações

int pulso_x;
int pulso_y;
uint16_t adc_value_x;
uint16_t adc_value_y; 
uint slice_blue;
uint slice_red;

// Variaveis
volatile bool led_B_ativado = 0; // Usado para saber qual led esta ligado
volatile bool led_G_ativado = 0; // Usado para saber qual led esta ligado
volatile bool led_R_ativado = 0; // Usado para saber qual led esta ligado
volatile bool BUTTON_A_pressionado = false;


// Funcoes
void callback_botao(uint gpio, uint32_t events);
void converte_joystic (int input);

int main(){
    // Inicializa entradas e saídas.
    stdio_init_all();

    // Inicia Leds
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);

    // Inicia botao A
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // Inicia botao B
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Inicia joystic 
    gpio_init(BTN_STICK);
    gpio_set_dir(BTN_STICK, GPIO_IN);
    gpio_pull_up(BTN_STICK);
    
    //função para iqr
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_RISE, true, &callback_botao);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_RISE, true, &callback_botao); //Bootloader
    gpio_set_irq_enabled_with_callback(BTN_STICK, GPIO_IRQ_EDGE_FALL, true, &callback_botao); 
    
    // Inicia ADC
    adc_init();
    adc_gpio_init(EIXO_X);
    adc_gpio_init(EIXO_Y);

    // Inicia PWM
    // Inicializa o PWM nos pinos desejados
    gpio_set_function(LED_B, GPIO_FUNC_PWM);
    gpio_set_function(LED_R, GPIO_FUNC_PWM);

    // Obtém os números dos canais PWM para os pinos
    slice_blue = pwm_gpio_to_slice_num(LED_B); //obter o canal (slice) PWM da GPIO
    slice_red = pwm_gpio_to_slice_num(LED_R); //obter o canal (slice) PWM da GPIO

    // Configuração da frequência PWM
    pwm_set_clkdiv(slice_red, (float)clock_get_hz(clk_sys) / PWM_FREQ / (PWM_WRAP + 1));
    pwm_set_clkdiv(slice_blue, (float)clock_get_hz(clk_sys) / PWM_FREQ / (PWM_WRAP + 1));

    // Configura o wrap do contador PWM para 8 bits (256)
    pwm_set_wrap(slice_blue, PWM_WRAP); //definir o valor de wrap – valor máximo do contador PWM
    pwm_set_wrap(slice_red, PWM_WRAP); //definir o valor de wrap – valor máximo do contador PWM
    
    // Habilita o PWM
    pwm_set_enabled(slice_red, 1); //habilitar o pwm no slice correspondente
    pwm_set_enabled(slice_blue, 1); //habilitar o pwm no slice correspondente
    led_R_ativado = 1;
    led_B_ativado = 1;

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                                        // Pull up the data line
    gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
    
    ssd1306_t ssd;
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd);    
    ssd1306_send_data(&ssd);   

    // Limpa o display. O display inicia com todos os pixels desligados.
    ssd1306_fill(&ssd, !cor);
    ssd1306_rect(&ssd, 31, 63, 8, 8, cor, !cor); // Desenha um retângulo    
    ssd1306_send_data(&ssd);

    int x = 63, y = 31; // centro do ssd1306

    while (true){
        converte_joystic(0);
        converte_joystic(1);
        
        pwm_set_gpio_level(LED_R, pulso_x);  
        pwm_set_gpio_level(LED_B, pulso_y); 
        
        ssd1306_fill(&ssd, !cor); // Limpa o display
        
        if(led_G_ativado)
            ssd1306_rect(&ssd, 1, 1, 126, 62, cor, !cor); // Desenha um retângulo borda

        // Eixo X
        if(adc_value_y == 2048){ // Se estiver no meio 
            x = 63 ;
        }
        else if (adc_value_y > 2048){ // Se aumentar os valores
            x = ((adc_value_y - 2048) / 32) + 54;
        }
        else{ // Se diminuir os valores
            x = (((adc_value_y - 2048)) / 32) + 67;
        }
        
        // Eixo Y
        if(adc_value_x == 2048)
            y = 31;
        else if(adc_value_x > 2048){
            y =  67 - (adc_value_x / 64);
            // y =  ((adc_value_x - 2048) / 64) + 24; PARA VALOR INVERTIDO
        }
        else{
            y = 54 - ((adc_value_x) / 64);
            // y =  ((adc_value_x - 2048) / 64) + 35; //PARA VALOR INVERTIDO
        }
        
        ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor); // Desenha um retângulo
        ssd1306_rect(&ssd, y, x, 8, 8, cor, cor); // Desenha um retângulo        
        ssd1306_send_data(&ssd); // Atualiza o display

        sleep_ms(100);
    } // Fim While True
} // Fim main



// Le e converte valores negativos oriundos do joystick
void converte_joystic (int input){ 
    // Seleciona 0 ou 1
    adc_select_input(input);
    sleep_us(2);
    if(input == 0){
        adc_value_x = adc_read(); 
        pulso_x = ((adc_value_x -2048)*255)/2048;
        if (pulso_x < 0){
            pulso_x = pulso_x *(-1);
        }
    }
    else if (input == 1){        
        adc_value_y = adc_read();
        pulso_y = ((adc_value_y -2048)*255)/2048;
        if (pulso_y < 0){
            pulso_y = pulso_y *(-1);
        }
    }
    else{
        printf("Valor Invalido!\n");
    }
}

//função para indicar que botão está sendo apertado
void callback_botao(uint gpio, uint32_t events){
    uint32_t current_time = to_us_since_boot(get_absolute_time()); // captura o momento do acionamento e converte para microsegundos
    if(current_time - last_time > DEBOUNCE_TIME){
        if (gpio == BUTTON_A) {
            // Habilita o PWM
            pwm_set_enabled(slice_red, led_R_ativado);  // habilitar/desabilita o pwm no slice correspondente
            pwm_set_enabled(slice_blue, led_B_ativado); // habilitar/desabilita o pwm no slice correspondente
            led_R_ativado = !led_R_ativado;
            led_B_ativado = !led_B_ativado;
        }
        else if(gpio == BUTTON_B){
            reset_usb_boot(0, 0); //func para entrar no modo bootsel 
        }        
        else if(gpio == BTN_STICK){
            led_G_ativado = !led_G_ativado;
            gpio_put(LED_G, led_G_ativado);
        }
        last_time = current_time; // Atualiza o tempo para o debounce
    } // debounce
}