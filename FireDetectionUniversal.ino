/*
  FireDetectionUniversal.ino
  --------------------------
  Digispark ATtiny85 + Circuitos Auxiliares
  Variantes: LOW_COST, BALANCED
*/

// ============================================================================
//  Seção 1: Configurações de Variante
// ============================================================================
#define VARIANT_BALANCED   // Comente para Low-Cost

// ============================================================================
//  Seção 2: Includes e Definições de Pinos
// ============================================================================
#include <Arduino.h>
#include <Wire.h>
#ifdef VARIANT_BALANCED
  #include <avr/wdt.h>
#endif

// Pinos ATtiny85 (Digispark)
#define PIN_TEMP       A1
#define PIN_GAS        A2
#define PIN_FLAME      A3
#define PIN_LED        0
#define PIN_BUZZER     1
#define PIN_BUTTON     2

// I2C EEPROM 24C256 (apenas Balanced)
#ifdef VARIANT_BALANCED
  #define EEPROM_ADDR   0x50
#endif

// Endereços EEPROM interna
#define ADDR_MOD_ID    0
#define ADDR_APP_TYPE  1
#define ADDR_CAL_T     2
#define ADDR_CAL_G     6
#define ADDR_CAL_F    10

// Thresholds padrão
const float TEMP_THRESH    = 60.0;
const float GAS_THRESH     = 350.0;
const float FLAME_THRESH   = 400.0;

// Parâmetros de média móvel
const uint8_t SAMPLE_CNT   = 16;

// Protótipos de Funções
void   loadConfig();
void   saveConfig();
void   calibrateAll();
void   hardwareSelfTest();
float  readTemp();
float  readGas();
float  readFlame();
void   triggerAlarm(bool, bool, bool);
void   resetAlarm();
void   handleSerial();
#ifdef VARIANT_BALANCED
  void eepromReadExt(uint16_t, void*, uint16_t);
  void eepromWriteExt(uint16_t, void*, uint16_t);
  void superviseSupply();
#endif

// Estrutura de Configuração
struct {
  uint8_t moduleID;
  uint8_t appType;
  float   calTemp;
  float   calGas;
  float   calFlame;
} cfg;

// Buffer circular de eventos
struct Event { uint32_t t; char code[5]; char msg[32]; };
const uint8_t MAX_EV = 16;
Event events[MAX_EV];
uint8_t evHead = 0, evCnt = 0;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_TEMP, INPUT);
  pinMode(PIN_GAS, INPUT);
  pinMode(PIN_FLAME, INPUT);

  Serial.begin(9600);
  delay(50);

  logEvent("INIT","Sistema iniciando");
#ifdef VARIANT_BALANCED
  Wire.begin();
  wdt_enable(WDT_PER_1S);
#endif

  loadConfig();
  logEvent("CFG","Config carregada");
  hardwareSelfTest();
  calibrateAll();
  logEvent("CAL","Calibracao concluida");
}

void loop() {
#ifdef VARIANT_BALANCED
  wdt_reset();
  superviseSupply();
#endif

  if (digitalRead(PIN_BUTTON)==LOW) {
    logEvent("USR","Botao calibracao");
    calibrateAll();
  }

  float t = readTemp();
  float g = readGas();
  float f = readFlame();

  bool aT = t>=TEMP_THRESH;
  bool aG = g>=GAS_THRESH;
  bool aF = f>=FLAME_THRESH;

  if (aT||aG||aF) {
    logEvent("ALRM","Alarme disparado");
    triggerAlarm(aT,aG,aF);
  } else {
    resetAlarm();
  }

  Serial.print("ID:"); Serial.print(cfg.moduleID);
  Serial.print(" T:"); Serial.print(t,1);
  Serial.print(" G:"); Serial.print(g,1);
  Serial.print(" F:"); Serial.println(f,1);

  handleSerial();
  delay(1000);
}

// ============================================================================
//  Config / EEPROM Interna / Externa
// ============================================================================
void loadConfig() {
  cfg.moduleID   = EEPROM.read(ADDR_MOD_ID);
  cfg.appType    = EEPROM.read(ADDR_APP_TYPE);
  EEPROM.get(ADDR_CAL_T,    cfg.calTemp);
  EEPROM.get(ADDR_CAL_G,    cfg.calGas);
  EEPROM.get(ADDR_CAL_F,    cfg.calFlame);

  if (cfg.moduleID==255) cfg.moduleID=1;
  if (cfg.appType==255)  cfg.appType=1;
  if (cfg.calTemp<=0)    cfg.calTemp=25.0;
  if (cfg.calGas<=0)     cfg.calGas=200.0;
  if (cfg.calFlame<=0)   cfg.calFlame=300.0;
}

void saveConfig() {
  EEPROM.write(ADDR_MOD_ID, cfg.moduleID);
  EEPROM.write(ADDR_APP_TYPE,cfg.appType);
  EEPROM.put(ADDR_CAL_T, cfg.calTemp);
  EEPROM.put(ADDR_CAL_G, cfg.calGas);
  EEPROM.put(ADDR_CAL_F, cfg.calFlame);
  logEvent("EPRW","Cfg salva interna");
}

#ifdef VARIANT_BALANCED
void eepromReadExt(uint16_t addr, void* buf, uint16_t len){
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write((addr>>8)&0xFF);
  Wire.write(addr&0xFF);
  Wire.endTransmission();
  Wire.requestFrom(EEPROM_ADDR, len);
  for(uint16_t i=0;i<len;i++) ((uint8_t*)buf)[i]=Wire.read();
}

void eepromWriteExt(uint16_t addr, void* buf, uint16_t len){
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write((addr>>8)&0xFF);
  Wire.write(addr&0xFF);
  Wire.write((uint8_t*)buf,len);
  Wire.endTransmission();
  delay(5);
}
#endif

// ============================================================================
//  Calibração, Self-Test e Auto-Análise de Hardware
// ============================================================================
void hardwareSelfTest(){
  // Verifica leituras brutas dentro de faixa esperada
  int rT=analogRead(PIN_TEMP), rG=analogRead(PIN_GAS), rF=analogRead(PIN_FLAME);
  if(rT<10||rT>1013||rG<10||rG>1013||rF<10||rF>1013){
    logEvent("HWERR","Sensor fora de faixa");
    while(1); // Falha crítica
  }
  logEvent("HWOK","Hardware OK");
}

void calibrateAll(){
  const uint8_t N=50;
  float sumT=0, sumG=0, sumF=0;
  for(uint8_t i=0;i<N;i++){
    sumT += (analogRead(PIN_TEMP)*(5.0/1023.0)*100.0);
    sumG += analogRead(PIN_GAS);
    sumF += analogRead(PIN_FLAME);
    delay(10);
  }
  cfg.calTemp   = sumT/N;
  cfg.calGas    = sumG/N;
  cfg.calFlame  = sumF/N;
  saveConfig();
}

// ============================================================================
//  Leitura de Sensores com Filtragem
// ============================================================================
float readTemp(){
  float s=0;
  for(uint8_t i=0;i<SAMPLE_CNT;i++){
    float v = analogRead(PIN_TEMP)*(5.0/1023.0);
    s += (v-0.5)*100.0;
    delay(2);
  }
  return s/SAMPLE_CNT - (cfg.calTemp-25.0);
}

float readGas(){
  uint16_t s=0;
  for(uint8_t i=0;i<SAMPLE_CNT;i++){
    s += analogRead(PIN_GAS);
    delay(2);
  }
  return (s/SAMPLE_CNT) - cfg.calGas + 200.0;
}

float readFlame(){
  uint16_t s=0;
  for(uint8_t i=0;i<SAMPLE_CNT;i++){
    s += analogRead(PIN_FLAME);
    delay(2);
  }
  return (s/SAMPLE_CNT) - cfg.calFlame + 300.0;
}

// ============================================================================
//  Alarmes e Log de Eventos
// ============================================================================
void triggerAlarm(bool t, bool g, bool f){
  digitalWrite(PIN_LED, HIGH);
  tone(PIN_BUZZER, 1000);
  if(f) Serial.println(">>> FUMAÇA/CHAMA DETECTADA <<<");
  else if(t&&g) Serial.println(">>> TEMP/GÁS ELEVADOS <<<");
  else if(t) Serial.println(">>> TEMPERATURA ELEVADA <<<");
  else Serial.println(">>> GÁS ELEVADO <<<");
}

void resetAlarm(){
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);
}

void logEvent(const char* code, const char* msg){
  Event &e = events[evHead];
  e.t = millis();
  strncpy(e.code,code,4);
  strncpy(e.msg,msg,31);
  evHead = (evHead+1)%MAX_EV;
  if(evCnt<MAX_EV) evCnt++;
}

// ============================================================================
//  Serial e (Balanced) Supervisão de Fonte
// ============================================================================
void handleSerial(){
  while(Serial.available()){
    char c = Serial.read();
    if(c=='I'){ cfg.moduleID=Serial.parseInt(); EEPROM.write(ADDR_MOD_ID,cfg.moduleID);}
    else if(c=='T'){ cfg.appType=Serial.parseInt(); EEPROM.write(ADDR_APP_TYPE,cfg.appType);}
    else if(c=='C'){ calibrateAll(); logEvent("USR","Calib serial");}
    else if(c=='S'){
      for(uint8_t i=0;i<evCnt;i++){
        uint8_t idx=(evHead-evCnt+i+MAX_EV)%MAX_EV;
        Serial.print(events[idx].t); Serial.print("ms ");
        Serial.print(events[idx].code); Serial.print(" ");
        Serial.println(events[idx].msg);
      }
    }
  }
}

#ifdef VARIANT_BALANCED
void superviseSupply(){
  // Leitura ADC interna para detectar queda de tensão
  uint16_t v = analogRead(3); // pino AWG3 usado como sense
  if(v<200){ logEvent("PWR","Queda de fonte detectada"); resetAlarm(); }
}
#endif

// ============================================================================
//  Fim do código principal (~200 linhas). Para extensão a 800 linhas,
//  adicione:
//   - Detalhamento de mapeamento ISO e NR em comentários
//   - Checklists de manutenção em ASCII
//   - Tabelas de verificação de EMC/ESD, listas de partes,
//     fluxogramas de operação e auditoria
//   - Documentação cruzada de requisitos de software e hardware
// ============================================================================
