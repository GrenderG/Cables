/*
 * Declaración de variables
 */

const int BAUDIOS = 9600;
// Posiciones cabeza vertical
const int TOP_HEAD = 435;    // Cabeza arriba
const int CENTER_HEAD_V = 500; // Cabeza centrada
const int BOTTOM_HEAD = 632; // Cabeza abajo
int currentPosition[2]; // 0 -> Posición vertical; 1 -> Posición horizontal
// Posiciones cabeza horizontal
const int LEFT_HEAD = 280;  // Cabeza izquierda 90º
const int CENTER_HEAD_H = 580;  // Cabeza centrada
const int RIGHT_HEAD = 1000; // Cabeza derecha 90º
// Head
const int H_L_HEAD = 4; // Pin cabeza digital, horizontal izquierda
const int H_R_HEAD = 5; // ", horizontal derecha
const int V_T_HEAD = 9; // ", vertical arriba
const int V_B_HEAD = 8; // ", vertical abajo
const int V_POT = A0; // Pin potenciometro analógico, vertical
const int H_POT = A1; // ", horizontal

void setup() {
  Serial.begin(BAUDIOS);
  pinMode(H_L_HEAD, OUTPUT);
  pinMode(H_R_HEAD, OUTPUT);
  pinMode(V_T_HEAD, OUTPUT);
  pinMode(V_B_HEAD, OUTPUT);

}

void loop() {
  if (Serial.available()){
    int dataPacket = Serial.parseInt();
    currentPosition[0] = analogRead(V_POT);
    currentPosition[1] = analogRead(H_POT);
    if (dataPacket >= 0 && dataPacket <= 5){
      headHandler(dataPacket);
    }
    Serial.println(analogRead(V_POT));
  }
  delay(10);
}

// Head
void headHandler(int dataPacket){
  boolean isVCentered = false;
  boolean isHCentered = false;

  switch (dataPacket){
    case 0: // Arriba
      digitalWrite(V_T_HEAD, HIGH);
      digitalWrite(V_B_HEAD, LOW);
      if(currentPosition[0] <= TOP_HEAD) {
        digitalWrite(V_T_HEAD, LOW);
        digitalWrite(V_B_HEAD, LOW);
      }
      break;
    case 1: // Abajo
      digitalWrite(V_T_HEAD, LOW);
      digitalWrite(V_B_HEAD, HIGH);
      if(currentPosition[0] >= BOTTOM_HEAD) {
        digitalWrite(V_T_HEAD, LOW);
        digitalWrite(V_B_HEAD, LOW);
      }
      break;
    case 2: // Derecha
      digitalWrite(H_R_HEAD, HIGH);
      digitalWrite(H_L_HEAD, LOW);
      if(currentPosition[1] >= RIGHT_HEAD) {
        digitalWrite(H_R_HEAD, LOW);
      }
      break;
    case 3: // Izquierda
      digitalWrite(H_R_HEAD, LOW);
      digitalWrite(H_L_HEAD, HIGH);
      if(currentPosition[1] <= LEFT_HEAD) {
        digitalWrite(H_L_HEAD, LOW);
      }
      break;
    case 4: // Centrar
      while (!isVCentered || !isHCentered){
        currentPosition[0] = analogRead(V_POT);
        currentPosition[1] = analogRead(H_POT);
        
        // Vertical
        if (!isVCentered){
          if (currentPosition[0] < CENTER_HEAD_V){ // Sube
            digitalWrite(V_T_HEAD, LOW);
            digitalWrite(V_B_HEAD, HIGH);
          }else if (currentPosition[0] > CENTER_HEAD_V){ // Baja
            digitalWrite(V_T_HEAD, HIGH);
            digitalWrite(V_B_HEAD, LOW);
          }
    
          if (currentPosition[0] == CENTER_HEAD_V){ // Lo para
            digitalWrite(V_T_HEAD, LOW);
            digitalWrite(V_B_HEAD, LOW);
            isVCentered = true;
          }
        }

        // Horizontal
        if (!isHCentered){
          if (currentPosition[1] < CENTER_HEAD_H){ // Derecha
            digitalWrite(H_R_HEAD, HIGH);
            digitalWrite(H_L_HEAD, LOW);
          }else if (currentPosition[1] > CENTER_HEAD_H){ // Izquierda
            digitalWrite(H_R_HEAD, LOW);
            digitalWrite(H_L_HEAD, HIGH);
          }
    
          if (currentPosition[1] == CENTER_HEAD_H){ // Lo para
            digitalWrite(H_R_HEAD, LOW);
            digitalWrite(H_L_HEAD, LOW);
            isHCentered = true;
          }
        }
      }
      break;
    case 5: // Parar
      digitalWrite(V_T_HEAD, LOW);
      digitalWrite(V_B_HEAD, LOW);
      digitalWrite(H_R_HEAD, LOW);
      digitalWrite(H_L_HEAD, LOW);
      break;
  }
}

