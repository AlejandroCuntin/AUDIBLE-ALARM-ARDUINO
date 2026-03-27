1. Objetivo

El objetivo de esta práctica es implementar un sistema de alarma sonora controlado por la
luz ambiente. Para ello se utiliza un sensor LDR que mide la luminosidad, un botón para
habilitar o deshabilitar la respuesta dinámica, y un buzzer que emite dos melodías distintas
según si la luz supera o no un umbral fijado aleatoriamente al arrancar el sistema.

2. Descripción del sistema

Al arrancar, el sistema fija un umbral de luminosidad de forma aleatoria (entre 0 y 1023). El
LDR mide continuamente la luz y devuelve un valor analógico en ese mismo rango.
-Si el valor del LDR supera el umbral entonces suena la Melodía 2 durante 10 segundos.
-Si el valor del LDR no supera el umbral entonces suena la Melodía 1 durante 5 segundos.

3. Esquema de conexión
Descripción del circuito:
● LDR: un extremo a 5V y el otro a A0. Entre A0 y GND se coloca una resistencia de
10 kΩ formando un divisor de tensión. A mayor luminosidad, mayor tensión en A0.
● Botón: un extremo a 5V y el otro al Pin 2. Una resistencia de 10 kΩ entre Pin 2 y
GND garantiza que el pin lee LOW cuando el botón no está pulsado (configuración
pull-down).
● Buzzer: pin positivo (largo) al Pin 8 y pin negativo (corto) a GND.

5. Cambios en monitor serie
En las capturas del monitor serie se puede ver cómo el sistema imprime en cada iteración el
valor del LDR, el estado del botón y el umbral. En la primera captura el LDR no supera el
umbral y suena la Melodía 1, mientras que en la segunda el LDR sí lo supera y pasa a
sonar la Melodía 2. Esto confirma que el sistema detecta correctamente los cambios de
luminosidad y cambia de melodía en tiempo real.

6. Código Arduino MIRAR "ArduinoAudibleAlarm.ino"
