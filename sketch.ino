#include <Servo.h>

#define TRIG_PIN        9
#define ECHO_PIN        10
#define SERVO_PIN       6
#define GREEN_LED       4
#define RED_LED         3
#define BUZZER_PIN      5

#define DETECT_DISTANCE   50    // cm - hand detection threshold
#define OPEN_ANGLE        90    // servo degrees open
#define CLOSE_ANGLE       0     // servo degrees closed

// ── Key fix: require N consecutive "clear" readings before closing
#define CLEAR_READINGS_NEEDED  8   // must see >15cm this many times in a row

Servo lidServo;
bool  lidIsOpen      = false;
int   clearCount     = 0;     // consecutive clear readings counter

// =============================================
void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN,   OUTPUT);
  pinMode(ECHO_PIN,   INPUT);
  pinMode(GREEN_LED,  OUTPUT);
  pinMode(RED_LED,    OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  lidServo.attach(SERVO_PIN);
  closeLid();
  Serial.println("=== Smart Dustbin Ready ===");
}

// =============================================
void loop() {
  long distance = getStableDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // ── Hand detected ──
  if (distance > 0 && distance <= DETECT_DISTANCE) {
    clearCount = 0;            // reset clear counter immediately

    if (!lidIsOpen) {
      openLid();
    }
  }

  // ── No hand ──
  else if (lidIsOpen) {
    clearCount++;

    Serial.print("  [clear count: ");
    Serial.print(clearCount);
    Serial.print("/");
    Serial.print(CLEAR_READINGS_NEEDED);
    Serial.println("]");

    // Only close after enough consecutive clear readings
    if (clearCount >= CLEAR_READINGS_NEEDED) {
      clearCount = 0;
      closeLid();
    }
  }

  delay(150);   // 150ms × 8 readings = ~1.2 sec before closing
}

// =============================================
//  AVERAGED DISTANCE — filters noisy spikes
// =============================================
long getStableDistance() {
  long readings[5];
  int  count = 0;

  for (int i = 0; i < 5; i++) {
    long d = getRawDistance();
    if (d > 0 && d < 300) {
      readings[count++] = d;
    }
    delay(15);
  }

  if (count == 0) return -1;

  // Sort to get median (best noise filter)
  for (int i = 0; i < count - 1; i++)
    for (int j = i + 1; j < count; j++)
      if (readings[j] < readings[i]) {
        long tmp = readings[i];
        readings[i] = readings[j];
        readings[j] = tmp;
      }

  return readings[count / 2];   // return median
}

// =============================================
//  RAW DISTANCE READING
// =============================================
long getRawDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(4);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 25000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}

// =============================================
void openLid() {
  lidIsOpen = true;
  lidServo.write(OPEN_ANGLE);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED,   LOW);
  beep(2, 100);
  Serial.println(">> LID OPENED");
}

void closeLid() {
  lidIsOpen = false;
  lidServo.write(CLOSE_ANGLE);
  digitalWrite(RED_LED,   HIGH);
  digitalWrite(GREEN_LED, LOW);
  beep(1, 200);
  Serial.println(">> LID CLOSED");
}

// void beep(int count, int duration) {
//   for (int i = 0; i < count; i++) {
//     digitalWrite(BUZZER_PIN, HIGH);
//     delay(duration);
//     digitalWrite(BUZZER_PIN, LOW);
//     if (i < count - 1) delay(80);
//   }
// }
void beep(int count, int duration) {
  for (int i = 0; i < count; i++) {
    tone(BUZZER_PIN, 1000, duration);   // 1000Hz tone
    delay(duration);
    noTone(BUZZER_PIN);
    if (i < count - 1) delay(80);
  }
}