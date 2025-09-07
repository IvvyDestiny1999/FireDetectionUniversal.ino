# 📖 Manual do Usuário

## 🧰 Instalação

1. Conecte o Digispark ATtiny85 via USB.
2. Instale sensores: temperatura (TMP36), gás (MQ-2), chama (IR).
3. Conecte LED, buzzer e botão de recalibração.
4. Faça upload do firmware via Arduino IDE.
5. Abra o monitor serial a 9600 bps.

## ⚙️ Comandos Seriais

- `I<id>` → Define o ID do módulo (ex: `I3`)
- `T<tipo>` → Define o tipo de aplicação (ex: `T2`)
- `C` → Inicia calibração dos sensores
- `S` → Exibe o log de eventos registrados

## 🔔 Alarmes

- LED vermelho pisca em caso de alarme
- Buzzer emite som contínuo
- Alarme dispara se:
  - Temperatura ≥ 60 °C
  - Gás ≥ 350 unidades
  - Chama detectada ≥ 400 unidades

## 🔄 Calibração

- Pressione o botão físico ou envie `C` via serial
- O sistema coleta 50 amostras de cada sensor
- Valores médios são gravados na EEPROM

## 🧪 Diagnóstico

- Autoanálise de sensores no boot
- Verificação de sinais falsos
- Supervisão de fonte (variante Balanced)
- Registro de eventos com timestamp

## 🛡️ Segurança

- Isolamento de sinais
- Proteção contra surtos
- Fonte universal 5–380 V com conversão segura
