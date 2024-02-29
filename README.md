## 1. Objetivos

Este trabalho tem por objetivo a criação de um sistema distribuído para o controle e monitoramento de um grupo de cruzamentos de sinais de trânsito. O sistema deve ser desenvolvido para funcionar em um conjunto de placas Raspberry Pi com um ***servidor central*** responsável pelo controle e interface com o usuário controlador e ***servidores distribuídos*** para o controle local e monitoramento dos sinais do cruzamento junto aos respectivos sensores que monitoram as vias. Dentre os dispositivos envolvidos estão: o controle de temporizaçãio e acionamento dos sinais de trânsito, o acionmento de botões de passagens de pedestres, o monitoramento de sensores de passagem de carros bem como a velocidade da via e o avanço de sinal vermelho.

A Figura 1 ilustra cruzamentos de trânsito.

![Figura](https://img.freepik.com/free-vector/colored-isolated-city-isometric-composition-with-road-crosswalk-city-center-vector-illustration_1284-30528.jpg)
![Figura](https://upload.wikimedia.org/wikipedia/commons/7/75/Makati_intersection.jpg)

Cada cruzamento possui:
- 4 Sinais de Trânsito (Em pares);
- 2 botões de acionamento para pedestres (pedir passagem), uma para cada direção;
- 2 Sensores de velocidade/presença/passagem de carros (nas vias auxiliares, um em cada direção);
- 2 Sensores de velocidade/presença/passagem de carros (nas vias principais, um em cada direção);
- 1 Sinalização de áudio (buzzer) para sinalizar quando o sinal está mudando de estado (quando o cruzamento de pedestres irá ser fechado);

Cada cruzamento deverá ser controlado por um processo individual que esteja rodando em uma placa Raspberry Pi e cada controlador de cruzamento deve se comunicar via rede (TCP/IP) com o servidor central.

Na Figura 2 é possível ver a arquitetura do sistema.

![Figura](/figuras/arquitetura_trabalho_1.png)

## 2. Componentes do Sistema

Para simplificar a implementação e logística de testes do trabalho, a quantidade de cruzamentos será limitada a 2 sendo que devem ser implementados 2 serviços de controle dos cruzamentos e um servidor central. 

### O sistema do Servidor Central será composto por:
1. 01 Placa Raspberry Pi 3/4;

### Cada unidade dos Servidores Distribuídos será composto por:
1. 01 Placa Raspberry Pi 3/4;
2. 04 Saídas GPIO (LEDs) representando os semáforos;
3. 02 Entradas sendo os botões de pedestre;
4. 02 Entradas sendo os sensores de velocidade/presença/contagem de veículos das vias auxiliares (2 por cruzamento);
5. 02 Entradas sendo os sensores de velocidade/presença/contagem (4 por cruzamento);
6. Saída de áudio para efeito sonoro estado do sinal para deficientes auditivos;

## 3. Conexões entre os módulos do sistema

1. Os servidores distribuídos deverão se comunicar com o servidor central através do Protocolo TCP/IP (O formato das mensagens ficam à cargo do aluno. A sugestão é o uso do formato JSON);
2. Cada instância do servidor distribuído (uma por cruzamento) deve rodar em um processo paralelo em portas distintas, podendo ser executado em placas distintas; 
4. Cada entrada / saída está representada na Tabela abaixo. Cada servidor distribuído é responsável pelo controle de um cruzamento.

<center>
Tabela 1 - Pinout da GPIO da Raspberry Pi
</center>
<center> 


| Item                     | GPIO Cruzamento 1 | GPIO Cruzamento 2 | Direção |
|--------------------------|:----:|:----:|:-------:|
| Semáforo 1 - Pino 1      |  09  | 10 | Saída   |
| Semáforo 1 - Pino 2      |  11  | 08 | Saída   |
| Semáforo 2 - Pino 1      |  05  | 01 | Saída   |
| Semáforo 2 - Pino 2      |  06  | 18 | Saída   |
| Botão de Pedestre 1      |  13  | 23 | Entrada |
| Botão de Pedestre 2      |  19  | 25 | Entrada |
| Sensor Via Auxiliar 1    |  26  | 25 | Entrada |
| Sensor Via Auxiliar 2    |  22  | 12 | Entrada |
| Sensor Via Principal 1   |   0  | 16 | Entrada |
| Sensor Via Principal 1   |  27  | 20 | Entrada |
| Buzzer                   |  17  | 21 | Saída   |

</center> 

## 4. Requisitos

Os sistema de controle possui os seguintes requisitos:

### **Servidores Distribuídos**

O código do Servidor Distribuído deve ser desenvolvido em **Python**, **C** ou **C++**.

Os servidores distribuídos tem as seguintes responsabilidades:  
1. Controlar os **semáforos** (temporização) - cruzamento com 4 sinais: os semáforos da via principal tem temporização diferente dos das vias auxiliares conforme e tabela abaixo.
   
<center>
Tabela 2 - Temporização dos Semáforos
</center>
<center> 

| Estado                                            | Via Principal (s) | Via Auxiliar (s) | 
|---------------------------------------------------|:----:|:---:|
| Verde (mínimo)                                    |  10  |  5  |
| Verde (máximo)                                    |  20  | 10  |
| Amarelo                                           |   2  |  2  |
| Vermelho (mínimo)                                 |   5  | 10  |
| Vermelho (máximo)                                 |  10  | 20  |
<!-- | Vermelho Total (Vermelho em ambas as direções)    |   1  |  1  | -->

</center> 

2. Controlar o acionamento dos **botões de travessia** de pedestres (2 por cruzamento): ao acionar o botão, o sinal em questão deverá cumprir seu tempo mínimo (Ex: permanecer verde pelo tempo mínimo antes de fechar. Caso o tempo mínimo já tenha passado, o sinal irá mudar de estado imediatamente após o botão ser pressionado);
3. Controlar o acionamento dos **sensores de passagem de carros** nas vias auxiliares. Caso o sinal esteja fechado e um carro pare na via auxiliar, o comportamento será o mesmo que um pedestre pressionar o **botões de travessia**;
4. Contar a *passagem de carros* em cada direção e sentido do cruzamento (4 valores sepadados) e enviar esta informação periodicamente (2 segundos) ao servidor central;
5. Monitorar a velocidade das vias (principal e auxiliar) através dos **sensores de velocidade**. A velocidade de cada carro deverá ser reportada para o servidor central periodicamente. Veídulos acima da velocidade permitida deverão ser reportados ao servidor central e contabilizados separadamente. Além disso, é necessário soar um alarme ao detectar um veículo acima da velocidade permitida;  
Velocidade Máxima:  
    a. Via Principal: 80 km/h;  
    b. Via auxiliar: 60 km/h.
6. Efetuar o controle de *avanço do sinal vermelho* em ambas as vias. O número de veículos que avançam o sinal vermelho deverá ser reportado ao servidor central e o alarme deve ser disparado a cada detecção de infração;
7. Cada instância dos servidores distribuídos a ser executada deve automaticamente se configurar para o controle do cruzamento 1 ou 2 à partir de um arquivo de configuração (configuração de portas, pinos, etc).

### **Servidor Central**

O código do Servidor Central pode ser desenvolvido em **Python**, **C** ou **C++**. 

O servidor central tem as seguintes responsabilidades:  
1. Manter conexão com os servidores distribuídos (TCP/IP);  
2. Prover uma **interface** que mantenham atualizadas as seguintes informações por cruzamento:  
    a. **Fluxo de trânsito** na via principal e em cada via auxiliar (Carros/min);    
    b. **Velocidade média de cada via** (km/h);   
    c. **Número de infrações** (Por tipo: avanço de sinal e velocidade acima da permitida);  
3. Prover **mecanismo na interface** para:  
    a. **Modo de emergência**: liberar o fluxo de trânsito em uma via (os dois cruzamentos com a via principal em verde);     
    b. **Modo noturno** fazer o sinal amarelo piscar em todos os cruzamento;  
4. Armazenar de modo persistente (arquivo) o estado atual (número de infrações, velocidade da pista, etc) para que possa ser re-estabelecido em caso de queda de energia.