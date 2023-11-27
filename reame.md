# Projeto de Monitoramento de Temperatura e Umidade com ESP32/Arduino e Visualização em Python Dash

## Descrição

Este projeto tem como objetivo criar um dispositivo de monitoramento de temperatura e umidade utilizando um ESP32 ou Arduino, com conexão de rede TCP/IP. O dispositivo realizará medições minuto a minuto, publicando os dados na nuvem por meio do serviço MQTT fornecido pela HiveMQ. A visualização dos dados será feita utilizando Python Dash para criar uma interface gráfica interativa.

## Funcionalidades

1. **Sensor de Temperatura e Umidade:**

    - Utilização de um sensor de temperatura e umidade para realizar medidas precisas.
    - As medições serão realizadas minuto a minuto.

2. **Formato de Publicação:**

    - Os dados medidos serão publicados no formato: `[ {data-hora} {Temperatura C} {Umidade %} ]`
    - O formato da data-hora será DD/MM/AA, onde DD representa o dia do mês, MM o mês do ano e AA os últimos dois dígitos do ano.
    - Temperatura e umidade serão apresentadas como números decimais.

3. **Conexão com MQTT:**

    - Os dados são enviados para a nuvem através do serviço MQTT fornecido pela HiveMQ.

4. **Visualização em Python Dash:**
    - A interface gráfica permite uma visualização clara e interativa das variações de temperatura e umidade ao longo do tempo.

## Instruções de Uso

1. **Montagem do Dispositivo:**

    - Conecte o sensor de temperatura e umidade ao ESP32 ou Arduino.
    - Configure a conexão de rede TCP/IP no dispositivo.

2. **Configuração do MQTT:**

    - Cadastre-se no serviço MQTT da HiveMQ e obtenha as credenciais de acesso.
    - Configure o dispositivo para se conectar ao serviço MQTT com as credenciais fornecidas.

3. **Execução do Projeto:**

    - Alimente o dispositivo e aguarde as medições serem realizadas minuto a minuto.
    - Os dados serão automaticamente publicados na nuvem.

4. **Visualização dos Dados com Python Dash:**
    - Execute o mqtt_dashoboard.py para visualizar e interagir com os gráficos das medidas realizadas.

## Referências

1. [Documentação do ESP32](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
2. [Documentação do Arduino](https://www.arduino.cc/reference/en/)
3. [HiveMQ - MQTT as a Service](https://www.hivemq.com/docs/)
4. [Documentação Python Dash](https://dash.plotly.com/)
