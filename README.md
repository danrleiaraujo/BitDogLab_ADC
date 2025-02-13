<h1 align="center">
  <br>
    <img width="400px" src="https://github.com/danrleiaraujo/Comunicacao_Serial_RP2040/tree/main/Comunicacao_Serial_RP2040/src/logo.png"> 
  <br>
  Unidade 4 - Capítulo 6 - Introdução a interfaces de comunicação Serial.
  <br>
</h1>
<div align="center">

</div>

<div align="center"> 
  
Esta atividade tem como objetivo usar o Raspberry Pi Pico W para compreender o funcionamento e aplicação de interrupções em microcontroladores, comunicação i2c para acendimento da matriz oled ssd1306 e o uso da matriz de led. O projeto envolve programação em C, integração com o SDK do Raspberry Pi Pico e o simulador Wokwi. 
</div>

<details open="open">
<summary>Sumário</summary>
  
- [Requisitos](#requisitos)
- [Componentes Utilizados](#componentes-utilizados)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [Funcionalidades](#funcionalidades)
- [Como Executar](#como-executar)
  - [Pré-requisitos](#pré-requisitos)
  - [Passos para Execução](#passos-para-execução)
- [Vídeo Demonstração](#vídeo-demonstração)
- [Autores](#autores-do-subgrupo-3)

</details>

## Requisitos

O projeto deve cumprir os seguintes requisitos:

1. **Uso de interrupções**: Todas as funcionalidades relacionadas aos botões devem ser implementadas utilizando rotinas de interrupção (IRQ).
2. **Debouncing**:  É obrigatório implementar o tratamento do bouncing dos botões via software.
3. **Controle de LEDs**: O projeto deve incluir o uso de LEDs comuns e LEDs WS2812, demonstrando o domínio de diferentes tipos de controle.
4. **Utilização do Display 128 x 64**: A utilização de fontes maiúsculas e minúsculas demonstrará o domínio do uso de bibliotecas e a utilização do protocolo i2c.
5. **Organização do código**: O código deve estar bem estruturado e comentado para facilitar o entendimento.

## Componentes Utilizados

1. **Raspberry Pi Pico W**: Microcontrolador para controle do sistema.
2. **Botões**: Interface para entrada de comandos.
3. **Matriz de LEDs WS2812 (5x5)**: Exibição de Numeros.
4. **Display 128 x 64:** Exibição de Numeros e caracteres.
5. **VS Code**: Ambiente de desenvolvimento para código em linguagem C.
6. **Simulador Wokwi**: Para simulação do circuito.
  <div align="center">
      <img width="800px" src="https://github.com/danrleiaraujo/Comunicacao_Serial_RP2040/tree/main/Comunicacao_Serial_RP2040/src/image.png" />
      <p>Fig 1. Projeto no simulador .</p>
   </div>
   
## Estrutura do Projeto

- **ws2812b.c**: Código responsável pela configuração e controle da matriz de LEDs.
- **PIO.c**: Biblioteca para controle do PIO (Programmable Input/Output) do Raspberry Pi Pico W.
- **desenho.c**: Funções para definição das animações a serem exibidas na matriz de LEDs.
- **funcoes.c**: Funções auxiliares para controle do sistema.
- **ssd1306.h**: Funções e configurações do display.
- **ssd1306.c**: Funções e inicialização do display.
- **font.h**: Acrescentação de caracteres.


## Funcionalidades 
- **Entrada de caracteres via PC**: Manda caracteres via serial do pc para a placa bitDogLab e é exibido no display SSD1306.
- **Entrada de Numeros via PC**: Caso o caractere seja número, será enviada também para a matriz de led ws2812.
- **Interação botão A**: Pressionar o botão A deve alternar o estado do LED RGB Verde (ligado/desligado) e exibir uma mensagem indicativa no display.
- **Interação botão B**: Pressionar o botão B deve alternar o estado do LED RGB Azul (ligado/desligado) e exibir uma mensagem indicativa no display.


## Como Executar

### Pré-requisitos

1. **Git**: Certifique-se de ter o Git instalado no seu sistema. 
2. **VS Code**: Instale o Visual Studio Code, um editor de código recomendado para desenvolvimento com o Raspberry Pi Pico.
3. **Pico SDK**: Baixe e configure o SDK do Raspberry Pi Pico, conforme as instruções da documentação oficial.
4. **Simulador Wokwi**: Utilize o simulador de eletrônica Wokwi para simular o projeto, caso não tenha acesso à placa física.
5. **Conexão USB**: Certifique-se de que o Raspberry Pi Pico W esteja conectado via USB quando for testar o código.

### Passos para Execução

1. **Clonar o repositório**: Clone o repositório utilizando o comando Git no terminal:
   
   ```bash
   git clone https://github.com/danrleiaraujo/BitDogLabInterrupcao.git
   ```
2. Abrir no VS Code: Abra a pasta clonada no VS Code e no terminal do VS Code, vá até a pasta do projeto.
3. Compilar o código.


   
## Vídeo Demonstração
  https://drive.google.com/file/d/1sythUCN-Bra0ZbPk1jXvzwqktAxcZ2Pb/view?usp=sharing

  
## Autor

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/danrleiaraujo" target="_blank">
        <img src="https://avatars.githubusercontent.com/u/44043273?v=4" width="100px;" alt=""/>
      </a>
      <br /><sub><b> Danrlei Araujo</b></sub>
    </td>
  </tr>
</table>

