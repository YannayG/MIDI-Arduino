// Controlador MIDI para Arduino UNO R3 con 9 botones
// Adaptado para usar con Hairless MIDI + loopMIDI

const byte TOTAL_BUTTONS = 9;

// Pines digitales utilizados (2-10)
const byte BUTTONS_PIN[TOTAL_BUTTONS] = {2, 3, 4, 5, 6, 7, 8, 9, 10};

// Notas MIDI correspondientes (C2 a G#2)
const byte BUTTONS_PITCH[TOTAL_BUTTONS] = {36, 37, 38, 39, 40, 41, 42, 43, 44};

// Estado actual de los botones
byte currentRead[TOTAL_BUTTONS];

// Lectura temporal
byte tempRead;

void setup() {
  // Inicializar comunicación serie para MIDI
  Serial.begin(115200); // Baud rate compatible con Hairless MIDI
  
  // Configurar todos los pines como pull-up input
  for (byte i = 0; i < TOTAL_BUTTONS; i++) {
    pinMode(BUTTONS_PIN[i], INPUT_PULLUP);
  }
}

void loop() {
  for (byte i = 0; i < TOTAL_BUTTONS; i++) {
    // Leer estado digital del botón
    // En pull-up: HIGH = no presionado, LOW = presionado
    byte buttonState = digitalRead(BUTTONS_PIN[i]);
    
    // Almacenar lectura temporal
    tempRead = buttonState;
    
    // Continuar solo si el estado cambió
    if (currentRead[i] != tempRead) {
      // Debounce simple
      delay(1);
      
      // Obtener la nota MIDI correspondiente
      byte pitch = BUTTONS_PITCH[i];
      
      // Guardar nuevo estado
      currentRead[i] = tempRead;
      
      // Ejecutar Note On o Note Off según el estado
      if (buttonState == LOW) {
        noteOn(pitch);
      } else {
        noteOff(pitch);
      }
    }
  }
}

void noteOn(byte pitch) {
  // Enviar mensaje MIDI Note On por serie
  Serial.write(0x90); // Note On, canal 1
  Serial.write(pitch); // Nota
  Serial.write(127);   // Velocidad máxima
}

void noteOff(byte pitch) {
  // Enviar mensaje MIDI Note Off por serie
  Serial.write(0x80); // Note Off, canal 1
  Serial.write(pitch); // Nota
  Serial.write(0);     // Velocidad 0
}