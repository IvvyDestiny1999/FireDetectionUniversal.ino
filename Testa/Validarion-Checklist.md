# ✅ Checklist de Validação do Sistema

Este documento lista os testes obrigatórios para validar o funcionamento seguro e confiável do sistema de detecção de incêndio.

## 🔌 Testes de Hardware

- [ ] Fonte converte corretamente de 5–380 V para 5 V DC isolado
- [ ] Fusível e MOV atuam em sobrecarga simulada
- [ ] TVS responde a transiente de alta tensão
- [ ] Choke EMI reduz ruído em linha AC

## 🔍 Testes de Sensores

- [ ] TMP36 responde a variação de temperatura com precisão
- [ ] MQ-2 detecta fumaça com tempo de resposta < 5s
- [ ] Sensor de chama detecta IR/UV com distância mínima de 30 cm
- [ ] Calibração grava valores corretos na EEPROM

## 🔔 Testes de Alarmes

- [ ] LED pisca em condição de alarme
- [ ] Buzzer emite som contínuo quando alarme é disparado
- [ ] Alarme é desativado automaticamente após normalização

## 🧠 Testes de Software

- [ ] Autoanálise detecta sensores desconectados
- [ ] Comando `C` recalibra sensores corretamente
- [ ] Comando `S` imprime log completo de eventos
- [ ] Comando `I` e `T` atualizam configurações na EEPROM

## 🛡️ Testes de Segurança

- [ ] Isolamento galvânico entre sensores e microcontrolador
- [ ] Supervisão de fonte detecta queda de tensão (Balanced)
- [ ] Sistema ignora sinais falsos e registra evento
- [ ] Watchdog reinicia sistema em travamento (Balanced)

## 📋 Testes de Conformidade

- [ ] Mapeamento de requisitos ISO 9001 e 27001
- [ ] Verificação de ABNT NBR 5410
- [ ] Simulação de ambiente NR07, NR10 e NR12
