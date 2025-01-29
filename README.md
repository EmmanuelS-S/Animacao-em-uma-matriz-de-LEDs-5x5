# Controle de Animações em Matriz de LEDs 5x5 com Raspberry Pi Pico W

## 📋 Sobre
Este projeto implementa a geração de animações em uma matriz de LEDs RGB (5x5 WS2812) utilizando o microcontrolador Raspberry Pi Pico W. O controle das animações é realizado por meio de um teclado matricial 4x4, permitindo a seleção de diferentes padrões visuais e luminosidades.

## 🚀 Funcionalidades

### 🎨 Controle de Animações na Matriz de LEDs:
- 7 animações diferentes associadas às teclas `0-9`.
- Cada animação conta com no mínimo 5 frames para garantir fluidez.
- O FPS das animações é ajustável pela equipe de desenvolvimento.

### 🌈 Controle Manual de Iluminação:
- `Tecla A`: Desliga todos os LEDs.
- `Tecla B`: Liga todos os LEDs na cor azul (100% de intensidade).
- `Tecla C`: Liga todos os LEDs na cor vermelha (80% de intensidade).
- `Tecla D`: Liga todos os LEDs na cor verde (50% de intensidade).
- `Tecla #`: Liga todos os LEDs na cor branca (20% de intensidade).

### 🔊 Sinal Sonoro Opcional:
- Possibilidade de ativar um sinal sonoro utilizando um buzzer passivo eletromagnético MLT-8530 SMD 3.6V.

### 🔄 Reinicialização do Sistema:
- `Tecla *`: Sai do modo de execução e habilita o modo de gravação via software (reboot do Raspberry Pi Pico W).

## 🛠️ Instruções de Uso

### 1. Conexões de Hardware

| Componente             | GPIO |
|------------------------|------|
| Matriz de LEDs (5x5)  | GPIO 14 |
| Teclado Matricial 4x4 | GPIO 10-13 |
| Buzzer (opcional)     | GPIO 21 |
| Terra                 | Todos os componentes |

### 2. Ambiente de Desenvolvimento
- Utilizamos **VS Code** com o **Kit de Desenvolvimento de Software Pico SDK**.
- Executamos a simulação no **Wokwi**.
- Implementamos em **linguagem C**.

### 3. Versionamento e Colaboração
- Criamos um repositório no **GitHub** para registro e submissão da atividade.
- Cada desenvolvedor criou uma branch específica para sua tarefa.
- Realizamos commits regulares com mensagens descritivas.
- O líder da equipe revisou e aprovou os pull requests.
- Implementamos testes para validar as funcionalidades do sistema.

## 🏆 Resultados Obtidos

O código foi testado e validado com sucesso no simulador **Wokwi**, demonstrando a geração de animações na matriz de LEDs RGB e o controle via teclado matricial. O projeto reforça conceitos de manipulação de **GPIOs**, **animação LED** e **uso de microcontroladores**.

### 🎥 Solução Implementada

[🔗 Link para o vídeo do ensaio (duração máx. de 3 minutos)](https://drive.google.com/file/d/1zuRzge1zHnsWBxX6Bay-etaF1Xp1IqG-/view?usp=sharing)

## 👨‍💻 Contribuidores
- Adriano Trajano
- Eduardo Lino
- Emanuel Souza
- Gabriel Marcone
- Kauan Teixeira
- Matheus Amorim

