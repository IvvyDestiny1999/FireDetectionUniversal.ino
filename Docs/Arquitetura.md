# Arquitetura do Sistema

O sistema é composto por:

- Chip ATtiny85 como núcleo lógico (digispark em proto)
- Sensores isolados: temperatura, gás, chama, etc
- EEPROM externa 24C256 via I²C
- Fonte universal com proteção contra surtos
- Alarmes visuais e sonoros
- Interface serial para configuração e auditoria

## Fluxo Operacional

1. Auto-teste de hardware
2. Leitura de sensores com média móvel
3. Verificação de calibração
4. Detecção de incêndio
5. Acionamento de alarmes
6. Registro de eventos
7. Supervisão de fonte (Balanced)
8. Loop contínuo de monitoramento
