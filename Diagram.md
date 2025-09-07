[INÍCIO DO SISTEMA]
        ↓
[Auto-Teste de Hardware]
→ Verifica se sensores estão conectados e dentro da faixa esperada
        ↓
[Leitura dos Sensores]
→ Temperatura, Gás, Chama (com média móvel)
        ↓
[Verificação de Botão de Calibração]
→ Se pressionado, inicia calibração de sensores
        ↓
[Verificação de Alarme]
→ Se qualquer sensor exceder o limite:
    → Aciona LED e Buzzer
    → Registra evento crítico
    → Publica status via Serial
        ↓
[Verificação de Sinal Falso]
→ Se alarme foi disparado mas leituras voltam ao normal rapidamente:
    → Registra evento de possível sinal falso
    → Mantém sistema em monitoramento
        ↓
[Verificação de Comandos Seriais]
→ Se receber:
    - `I<id>` → Atualiza Module ID
    - `T<type>` → Atualiza App Type
    - `C` → Recalibra sensores
    - `S` → Imprime log de eventos
        ↓
[Loop de Monitoramento Contínuo]
→ Reinicia ciclo a cada intervalo (ex.: 2 segundos)
→ Mantém supervisão da fonte (Balanced)
→ Atualiza watchdog (Balanced)
→ Mantém sistema em operação segura
        ↓
[RETORNA AO INÍCIO DO SISTEMA]
