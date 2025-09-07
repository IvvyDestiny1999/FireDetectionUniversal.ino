[ setup() ]
    ├─> EventLogger.log("INIT")
    ├─> Config.load()
    ├─> EventLogger.log("CFG loaded")
    ├─> HardwareSelfTest.run()
    ├─> SensorManager.calibrateAll()
    └─> EventLogger.log("Calibration done")

[ loop() ]
    ├─ VARIANT_BALANCED: PowerSupervisor.supervise()
    ├─ Botão pressionado?
    │     └─ sim → EventLogger.log("User calib") 
    │              SensorManager.calibrateAll()
    ├─ t := SensorManager.readTemp()
    ├─ g := SensorManager.readGas()
    ├─ f := SensorManager.readFlame()
    ├─ anyAlarm := (t≥T_THRESH ∨ g≥G_THRESH ∨ f≥F_THRESH)
    │     ├─ sim → EventLogger.log("ALARM")
    │     │        AlarmManager.trigger(t,g,f)
    │     └─ não → AlarmManager.reset()
    ├─ SerialHandler.handle()
    ├─ Serial prints status (ID, T, G, F)
    └─ delay(1000ms)
       ↺ volta ao início do loop
