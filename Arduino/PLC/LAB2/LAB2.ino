#include <Wire.h>     // incluye libreria Wire

int pin_pwm = 11;
int resolucion = 255; // 8 bits
int rev_max = 1680;   // Frecuencia maxima del motor: 1680rpm

void setup(void) {
  Serial.begin(9600);             // inicializa monitor serie a 9600 bps
  Serial.println("Iniciando..."); // texto estatico
}

/* FUNCION ECUACION DE LA RECTA
 * Parametros de entrada:
 * t_i : Tiempo inicial
 * porc_i : Porcentaje inicial
 * t_f : Tiempo final
 * porc_f : Porcentaje final
 * 
 * La funcion recibe los anteriores cuatro parametros como entrada.
 * La ecuacion de la recta usada en la siguiente funcion corresponde a:
 * y = m(x - x1) + y1 
 * y : porc_f
 * x : t
 * x1 : t_i
 * y1 : porc_i
*/
void ec_recta(int t_i, int porc_i, int t_f, int porc_f)
{
    float frec = 1.0;             // Frecuencia. La division requiere de al menos un dato float para obtener una salida con decimales.
    float m = (porc_f - (float)porc_i) / (t_f - t_i);  // Pendiente de la recta entre dos puntos.
    float salida_porc;            // Ecuacion de la recta, se calcula dentro del ciclo for.
    int i;                        // Ciclo for. Se declara al inicio para realizar calculos luego.
    int t;                        // Variable que cambia en el tiempo.
    int ciclos = (t_f - t_i)*frec;// Limite de ciclos para el for.
    int periodo_ms = 1000/frec;   // Cuanto tarde en actualizar el voltaje.
    int salida_pwm;               // Muestra el valor en funcion de las rpm del motor.
    
    for(i = 0; i < ciclos; i++)
    {
      t = (i + t_i) / frec;                // Calcula el tiempo en cada instante.
      salida_porc = m*(t - t_i) + porc_i;  // Calcula el porcentaje de salida en cada punto de la rampa.
      salida_pwm = (salida_porc * resolucion) / 100;// Convierte el porcentaje de salida a digital.
      analogWrite(pin_pwm, salida_pwm);    // Funcion que obtiene un valor de 8 bits y da un voltaje de salida.
      
      Serial.print("Pot. = ");
      Serial.print((int)salida_porc);
      Serial.print("%    Frec. = ");
      Serial.print((salida_porc * rev_max) / 100); // Convierte el porcentaje de salida en el valor de rpm del motor.
      Serial.print("rpm    V = ");
      Serial.print(salida_porc / 10); // Convierte el porcentaje de salida en el valor de voltaje de entrada al variador.
      Serial.print("V    t = ");
      Serial.print(t);
      Serial.println("s");
      
      delay(periodo_ms);
    }
}

// Llamado a las funciones que se ejecutaran en bucle segun su orden
void loop(void) {
  
  /*
   * Inicializa el voltaje de salida en 0.0V por 5 segundos
   * y escribe en consola el conteo regresivo
  */
  for(int i = 0; i < 5; i++)
  {
    analogWrite(pin_pwm, 0); // Funcion que obtiene un valor de 8 bits y da un voltaje de salida.
    Serial.print("Espere ");
    Serial.print(5 - i);
    Serial.println(" segundos...");
    delay(1000);
  }

  Serial.println("===============================");
  Serial.println("  RECTA 1  P(0,0)  Q(10,30)    ");
  Serial.println("===============================");
  ec_recta(0,0,10,30);

  Serial.println("===============================");
  Serial.println("  RECTA 2  P(10,30)  Q(60,30)  ");
  Serial.println("===============================");
  ec_recta(10,30,60,30);

  Serial.println("===============================");
  Serial.println("  RECTA 3  P(60,30)  Q(70,60)  ");
  Serial.println("===============================");
  ec_recta(60,30,70,60);

  Serial.println("================================");
  Serial.println("  RECTA 4  P(70,60)  Q(120,60)  ");
  Serial.println("================================");
  ec_recta(70,60,120,60);

  Serial.println("==================================");
  Serial.println("  RECTA 5  P(120,60)  Q(130,90)   ");
  Serial.println("==================================");
  ec_recta(120,60,130,90);

  Serial.println("==================================");
  Serial.println("  RECTA 6  P(130,90)  Q(180,90)   ");
  Serial.println("==================================");
  ec_recta(130,90,180,90);

  Serial.println("==================================");
  Serial.println("  RECTA 7  P(180,90)  Q(190,100)  ");
  Serial.println("==================================");
  ec_recta(180,90,190,100);

  Serial.println("==================================");
  Serial.println("  RECTA 8  P(190,100)  Q(240,100) ");
  Serial.println("==================================");
  ec_recta(190,100,240,100);

  Serial.println("==================================");
  Serial.println("  RECTA 9  P(240,100)  Q(250,70)  ");
  Serial.println("==================================");
  ec_recta(240,100,250,70);

  Serial.println("==================================");
  Serial.println("  RECTA 10  P(250,70)  Q(300,70)  ");
  Serial.println("==================================");
  ec_recta(250,70,300,70);

  Serial.println("==================================");
  Serial.println("  RECTA 11  P(300,70)  Q(310,50)  ");
  Serial.println("==================================");
  ec_recta(300,70,310,50);

  Serial.println("==================================");
  Serial.println("  RECTA 12  P(310,50)  Q(360,50)  ");
  Serial.println("==================================");
  ec_recta(310,50,360,50);

  Serial.println("==================================");
  Serial.println("  RECTA 13  P(360,50)  Q(370,0)   ");
  Serial.println("==================================");
  ec_recta(360,50,370,0);
}
