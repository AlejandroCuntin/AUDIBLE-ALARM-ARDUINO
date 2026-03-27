// Pines
const int PIN_LDR = A0;
const int PIN_BOTON = 2;
const int PIN_ZUMBADOR = 8;

// Variables globales
int valorUmbral;
bool botonAnterior = false;

// Control de melodía
bool reproduciendo = false;
unsigned long tiempoFinMelodia = 0;
int numeroMelodia = 0;
unsigned long tiempoActual;

// Control de pitido triple
int pitidosPendientes = 0;
unsigned long tiempoProximoPitido = 0;
bool pitidoActivo = false;
a
// Control de repeticiones (cuando se suelta el botón)
int repeticionesPendientes = 0;
int melodiaParaRepetir = 0;
unsigned long tiempoProximaRepeticion = 0;
bool esperandoSilencio = false;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_BOTON, INPUT);
  pinMode(PIN_ZUMBADOR, OUTPUT);

  randomSeed(analogRead(A5));
  valorUmbral = random(200, 801);  // <-- DESCOMENTADO, umbral aleatorio

  Serial.print("Umbral elegido: ");
  Serial.println(valorUmbral);
}

void loop() {
  tiempoActual = millis();

  bool botonPulsado = (digitalRead(PIN_BOTON) == HIGH);
  int valorLDR = analogRead(PIN_LDR);
  bool superaUmbral = valorLDR >= valorUmbral;
  int nuevaMelodia = superaUmbral ? 2 : 1;
  unsigned long duracionMelodia = superaUmbral ? 10000UL : 5000UL;

  gestionarMelodia();
  gestionarPitidos();

  // CUANDO EL BOTÓN ESTÁ PULSADO
  if (botonPulsado) {
    repeticionesPendientes = 0;  // cancelar repeticiones pendientes
    esperandoSilencio = false;

    // *** CAMBIO CLAVE: detectar cambio de melodía AUNQUE esté sonando ***
    if (nuevaMelodia != numeroMelodia && pitidosPendientes == 0) {
      noTone(PIN_ZUMBADOR);
      reproduciendo = false;
      numeroMelodia = nuevaMelodia;

      // Lanzar pitido triple
      pitidosPendientes = 3;
      tiempoProximoPitido = tiempoActual;
      pitidoActivo = false;
    }

    // Arrancar melodía si no hay nada sonando ni pitidos
    if (!reproduciendo && pitidosPendientes == 0) {
      iniciarMelodia(nuevaMelodia, duracionMelodia);
    }
  }

  // CUANDO EL BOTÓN NO ESTÁ PULSADO
  else {
    // Flanco de bajada: botón recién soltado
    if (botonAnterior == true && botonPulsado == false) {
      noTone(PIN_ZUMBADOR);
      reproduciendo = false;
      pitidosPendientes = 0;

      if (numeroMelodia != 0) {
        repeticionesPendientes = 2;
        melodiaParaRepetir = numeroMelodia;
        tiempoProximaRepeticion = tiempoActual;
        esperandoSilencio = false;
      }
    }

    // Gestionar las 2 repeticiones
    gestionarRepeticiones();
  }

  // MENSAJE EN MONITOR
  Serial.print("Botón: ");
  Serial.print(botonPulsado ? "PULSADO" : "NO pulsado");
  Serial.print(" | LDR: ");
  Serial.print(valorLDR);
  Serial.print(" | Umbral: ");
  Serial.print(valorUmbral);
  Serial.print(" | ");
  Serial.print(superaUmbral ? "SUPERADO umbral" : "NO superado");
  Serial.print(" | Melodia: ");
  Serial.println(nuevaMelodia);

  botonAnterior = botonPulsado;
  delay(50);  // reducido de 150 a 50 para mejor respuesta
}

// Inicia una melodía según su número y duración
void iniciarMelodia(int numero, unsigned long duracion) {
  numeroMelodia = numero;
  reproduciendo = true;
  tiempoFinMelodia = tiempoActual + duracion;
  if (numero == 1) {
    tone(PIN_ZUMBADOR, 440);
  } else {
    tone(PIN_ZUMBADOR, 900);
  }
}

// Comprueba si la melodía ha terminado
void gestionarMelodia() {
  if (reproduciendo && tiempoActual >= tiempoFinMelodia) {
    noTone(PIN_ZUMBADOR);
    reproduciendo = false;
  }
}

// Genera un pitido triple de forma no bloqueante
void gestionarPitidos() {
  if (pitidosPendientes <= 0) return;

  if (!pitidoActivo && tiempoActual >= tiempoProximoPitido) {
    tone(PIN_ZUMBADOR, 1200);
    pitidoActivo = true;
    tiempoProximoPitido = tiempoActual + 200;
  } else if (pitidoActivo && tiempoActual >= tiempoProximoPitido) {
    noTone(PIN_ZUMBADOR);
    pitidoActivo = false;
    pitidosPendientes--;
    tiempoProximoPitido = tiempoActual + 200;
  }
}

// Repite 2 veces la última melodía al soltar el botón
void gestionarRepeticiones() {
  if (repeticionesPendientes <= 0) return;
  if (reproduciendo) return;
  if (pitidosPendientes > 0) return;

  if (!esperandoSilencio && tiempoActual >= tiempoProximaRepeticion) {
    unsigned long duracion = (melodiaParaRepetir == 1) ? 5000UL : 10000UL;
    iniciarMelodia(melodiaParaRepetir, duracion);
    repeticionesPendientes--;
    tiempoProximaRepeticion = tiempoActual + duracion + 5000UL;
    esperandoSilencio = true;
  } else if (esperandoSilencio && tiempoActual >= tiempoProximaRepeticion) {
    esperandoSilencio = false;
  }
}