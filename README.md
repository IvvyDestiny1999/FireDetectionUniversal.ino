Fire Detection System Universal (Digispark ATtiny85 + External Logic)

Este projeto apresenta duas variantes de um sistema avançado de detecção de incêndio com ATtiny85 como “cérebro” e circuitos auxiliares dedicados:

- Low-Cost: Custo mínimo, proteção básica e conversão simples de tensão.  
- Balanced: Melhor equilíbrio custo-benefício, isolamento completo, filtragem EMI, proteção contra transientes e uso de EEPROM externa.

Ambas as variantes suportam entradas de 5 V a 380 V (AC ou DC), isolamento de sinal, proteção contra falsos disparos e armazenamento de configurações/calibrações em memória não volátil.

---

Descrição do Projeto

Este sistema detecta anomalias de temperatura, gás (fumaça) e chama, fornecendo alarmes sonoros e visuais, registro de eventos e interface serial para configuração remota. O projeto enfatiza:

- Faixa de alimentação universal (5 V a 380 V).  
- Isolamento galvânico de sensores e circuitos digitais.  
- Proteção contra surtos, EMI/RFI e sinais falsos.  
- Armazenamento em EEPROM interna (ATtiny85) e externa (24C256 I²C).  
- Dois níveis de implementação: Low-Cost e Balanced.
- configuração de pinos usando dip-4 switch

Variantes de Projeto


| Característica             | Low-Cost                          | Balanced                               |
|----------------------------|-----------------------------------|----------------------------------------|
| Fonte Universal            | Módulo AC/DC genérico (85–264 VAC)| SMPS custumizado com PFC ativo         |
| Proteção de Entrada        | Fusível lento + MOV básico        | Fusível lento; MOV; TVS; choke common-mode |
| Isolamento                 | Opto-isoladores simples           | Transformador isolador; isoladores digitais reforçados |
| EEPROM Externa             | Opcional                          | 24C256 (I²C)                           |
| Filtragem EMI/RFI          | Capacitores de bypass             | Filtro EMI em modo comum e diferencial |
| Monitor de Fonte           | ADC interno do ATtiny85           | Supervisório de tensão + watchdog externo |
| Teste de Sinais Falsos     | Debounce digital                  | Filtros de média ponderada + detecção de lock-up |
| Conformidade               | Básica (ABNT NBR5410)             | Completa (ISO 9001/27001, NR07/10/12)  |

Diagrama em Blocos

`
┌───────────────────────┐
│     Fonte Universal   │───┐
│(5–380 V AC/DC → 5 V DC)│   │
└───────────────────────┘   │
                            ▼
                  ┌──────────────────┐
                  │ Proteção Entrada │
                  │(fusível, MOV, TVS│
                  │ choke, EMI filter)│
                  └──────────────────┘
                            │
                            ▼
           ┌─────────────────────────────┐
           │ Conversor 5 V isolado (SMPS)│
           └─────────────────────────────┘
                            │
                            ▼
┌───────────────┐  ┌────────┴────────┐  ┌───────────────┐
│Sensores Temp  │  │  Digispark ATtiny85  │  │EEPROM 24C256   │
│(isolados via  │◀─▶│    (cérebro)        │◀─▶│   I²C Bus      │
│opto ou digital│  │                    │  │               │
└───────────────┘  └────────┬────────┘  └───────────────┘
                            │
        ┌───────────────────┴────────────────────┐
        │       Alarmes Visuais/Sonoros & I/O    │
        └────────────────────────────────────────┘
`

Funcionalidades Comuns

- Leitura e filtragem de sensores de temperatura, gás e chama.  
- Média móvel e detecção de lock-out para sinais falsos.  
- Armazenamento de moduleID, appType e calibrações.  
- Alarmes com LED, buzzer e saída digital.  
- Interface serial (9600 bps) para:
  - I<id> definir Module ID  
  - T<type> definir App Type  
  - C forçar calibração  
  - S solicitar status completo  
- Registro circular de eventos em RAM e EEPROM externa.  
- Supervisão de tensão de fonte (variant-Balanced).  

Hardware e Componentes

Low-Cost

- Módulo AC/DC 85–264 VAC → 5 V DC (isolado)  
- Fusível lento 500 mA; MOV 275 VAC; capacitor 0,1 µF/275 VAC  
- Opto-isolador 4-ch (PC817) para sensores  
- Resistor de pull-up interno para botão  
- Digispark ATtiny85 16.5 MHz  
- MQ-2, TMP36, sensor chama simples  
- Buzzer piezo; LED vermelho; botão de recalibração  

Balanced

- SMPS customizado 5 V isolado (PFC ativo; 85–380 VAC/DC)  
- Fusível lento 500 mA; MOV 275 VAC; TVS 600 W; choke common-mode  
- Transformador isolador de sinal + isoladores digitais (ISO7211)  
- EEPROM 24C256 (I²C)  
- Supervisório externo de tensão (TLV8031)  
- Filtros EMI em placa (X e Y capacitores; common-mode choke)  
- Sensores industriais: MQ-7 (gás), PT100 (temp) via amplificador (PT100→4–20 mA), detector de chama UV/IR  
- Buzzer piezo industrial; LED alto-brilho; botão anti-falha  

Instalação e Montagem

1. Monte o módulo de fonte conforme variante (Low-Cost ou Balanced).  
2. Faça as conexões de sensores através de isoladores/opto-isoladores.  
3. Conecte EEPROM 24C256 no barramento I²C (A4=SDA, A5=SCL no Digispark USB expander).  
4. Carregue FireDetectionUniversal.ino no Digispark via Arduino IDE.  
5. Abra monitor serial a 9600 bps e acompanhe inicialização e calibração.  

---


Com este repositório você tem:

- Dois esquemas de hardware (Low-Cost e Balanced).  
- Firmware universal com detecção de falhas, filtros e logs.  
- EEPROM interna e, opcionalmente, externa para alta confiabilidade.  
- Suporte a faixas de tensão de 5 V a 380 V com isolamento e proteção.  

Próximos passos possíveis:

- Integração MQTT via módulo ESP8266 isolado.  
- Dashboard web com monitoramento em tempo real.  
- Registro de dados em cartão SD isolado.  
- Expansão para rede de sensores modulares via RS-485 isolado.  

Bom desenvolvimento!                  │ Proteção Entrada    │
                  │(fusível, MOV, TVS   │
                  │ choke, EMI filter). │
                  └──────────────────┘
                            │
                            ▼
           ┌─────────────────────────────┐
           │ Conversor 5 V isolado (SMPS).    |
           └─────────────────────────────┘
                           │
                           ▼
┌──────────────────────┴────────────────┐  ┌───────────────┐     ┌──────────────────┐
│Sensores Temp     │    │  Digispark ATtiny85  │EEPROM 24C256     │    │
│(isolados via     │◀─▶│    (cérebro)        │◀─▶│   I²C Bus      │
│opto ou digital│  │                    │  │               │
└───────────────┘  └────────┬────────┘  └───────────────┘
                            │
        ┌───────────────────┴────────────────────┐
        │       Alarmes Visuais/Sonoros & I/O    │
        └────────────────────────────────────────┘
`

Funcionalidades Comuns

- Leitura e filtragem de sensores de temperatura, gás e chama.  
- Média móvel e detecção de lock-out para sinais falsos.  
- Armazenamento de moduleID, appType e calibrações.  
- Alarmes com LED, buzzer e saída digital.  
- Interface serial (9600 bps) para:
  - I<id> definir Module ID  
  - T<type> definir App Type  
  - C forçar calibração  
  - S solicitar status completo  
- Registro circular de eventos em RAM e EEPROM externa.  
- Supervisão de tensão de fonte (variant-Balanced).  

Hardware e Componentes

t

- Módulo AC/DC 85–264 VAC → 5 V DC (isolado)  
- Fusível lento 500 mA; MOV 275 VAC; capacitor 0,1 µF/275 VAC  
- Opto-isolador 4-ch (PC817) para sensores  
- Resistor de pull-up interno para botão  
- Digispark ATtiny85 16.5 MHz  
- MQ-2, TMP36, sensor chama simples  
- Buzzer piezo; LED vermelho; botão de recalibração  

Balanced

- SMPS customizado 5 V isolado (PFC ativo; 85–380 VAC/DC)  
- Fusível lento 500 mA; MOV 275 VAC; TVS 600 W; choke common-mode  
- Transformador isolador de sinal + isoladores digitais (ISO7211)  
- EEPROM 24C256 (I²C)  
- Supervisório externo de tensão (TLV8031)  
- Filtros EMI em placa (X e Y capacitores; common-mode choke)  
- Sensores industriais: MQ-7 (gás), PT100 (temp) via amplificador (PT100→4–20 mA), detector de chama UV/IR  
- Buzzer piezo industrial; LED alto-brilho; botão anti-falha  

Instalação e Montagem

1. Monte o módulo de fonte conforme variante (Low-Cost ou Balanced).  
2. Faça as conexões de sensores através de isoladores/opto-isoladores.  
3. Conecte EEPROM 24C256 no barramento I²C (A4=SDA, A5=SCL no Digispark USB expander).  
4. Carregue FireDetectionUniversal.ino no Digispark via Arduino IDE.  
5. Abra monitor serial a 9600 bps e acompanhe inicialização e calibração.  

---


Com este repositório você tem:

- Dois esquemas de hardware (Low-Cost e Balanced).  
- Firmware universal com detecção de falhas, filtros e logs.  
- EEPROM interna e, opcionalmente, externa para alta confiabilidade.  
- Suporte a faixas de tensão de 5 V a 380 V com isolamento e proteção.  

Próximos passos possíveis:

- Integração MQTT via módulo ESP8266 isolado.  
- Dashboard web com monitoramento em tempo real.  
- Registro de dados em cartão SD isolado.  
- Expansão para rede de sensores modulares via RS-485 isolado.  

Bom desenvolvimento!
