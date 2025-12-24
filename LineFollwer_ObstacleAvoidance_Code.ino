//                 PIN CONNECTIONS
// =====================================================

// Motors (L298N)
#define enA 10
#define in1 9
#define in2 8
#define in3 7
#define in4 6
#define enB 5

// Line Sensors
#define L_S A0
#define R_S A1

// Ultrasonic
#define echo A2
#define trigger A3

// Servo (NO library)
#define servoPin A5

// =====================================================
//                 CONSTANTS
// =====================================================
#define SAFE_DISTANCE 15     // cm
#define TURN_TIME 350        // ms
#define NOISE_MARGIN 3       // تجاهل القراءات الصغيرة

// =====================================================
//                 VARIABLES
// =====================================================
int svl = 0;
int svr = 0;

int vspeed = 110;
int tspeed = 160;
int tdelay = 10;

const int centerAngle = 50;  // مركز السيرفو

// =====================================================
//                     SETUP
// =====================================================
void setup() {

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  pinMode(L_S, INPUT);
  pinMode(R_S, INPUT);

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(servoPin, OUTPUT);

  // ضبط السيرفو على المركز
  moveServoToAngle(centerAngle);

  delay(600);
}

// =====================================================
//                      LOOP
// =====================================================
void loop() {

  int cm = readDistance();

  if (cm > NOISE_MARGIN && cm < SAFE_DISTANCE) {
    avoidObstacle();
    return;
  }

  // ---------- LINE FOLLOWING ----------
  svl = digitalRead(L_S);
  svr = digitalRead(R_S);

  if (svl == LOW && svr == LOW) {
    forward();
  }
  else if (svl == HIGH && svr == LOW) {
    left();
    delay(tdelay);
  }
  else if (svl == LOW && svr == HIGH) {
    right();
    delay(tdelay);
  }
  else {
    stopRobot();
  }
}

// =====================================================
//               ULTRASONIC FUNCTIONS
// =====================================================
long readDistance() {
  long sum = 0;
  int count = 3;

  for (int i = 0; i < count; i++) {
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);

    long duration = pulseIn(echo, HIGH, 25000);
    long cm = duration / 58;
    
    if (cm > 0 && cm < 400) {
      sum += cm;
    }
    delay(5);
  }

  long avg = sum / count;
  return avg;
}

// =====================================================
//          SERVO FUNCTIONS - محسّنة للحركة التدريجية
// =====================================================
void moveServoToAngle(int targetAngle) {
  targetAngle = constrain(targetAngle, 0, 180);
  
  // حركة تدريجية من الزاوية الحالية للزاوية المطلوبة
  static int currentAngle = 90;
  
  int step = (targetAngle > currentAngle) ? 2 : -2;
  
  if (targetAngle > currentAngle) {
    for (int angle = currentAngle; angle <= targetAngle; angle += 2) {
      int pulseWidth = (angle * 11) + 500;
      for (int i = 0; i < 5; i++) {
        digitalWrite(servoPin, HIGH);
        delayMicroseconds(pulseWidth);
        digitalWrite(servoPin, LOW);
        delay(5);
      }
    }
  } else {
    for (int angle = currentAngle; angle >= targetAngle; angle -= 2) {
      int pulseWidth = (angle * 11) + 500;
      for (int i = 0; i < 5; i++) {
        digitalWrite(servoPin, HIGH);
        delayMicroseconds(pulseWidth);
        digitalWrite(servoPin, LOW);
        delay(5);
      }
    }
  }
  
  currentAngle = targetAngle;
  
  // نبضات إضافية للتثبيت
  int finalPulse = (targetAngle * 11) + 500;
  for (int i = 0; i < 30; i++) {
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(finalPulse);
    digitalWrite(servoPin, LOW);
    delay(5);
  }
}

// =====================================================
//               LOOK FUNCTIONS
// =====================================================
int lookRight() {
  // تحريك السيرفو لليمين تدريجياً
  moveServoToAngle(10);
  delay(400);
  
  // قراءة المسافة
  int distance = readDistance();
  delay(150);
  
  return distance;
}

int lookLeft() {
  // تحريك السيرفو للشمال تدريجياً
  moveServoToAngle(110);
  delay(400);
  
  // قراءة المسافة
  int distance = readDistance();
  delay(150);
  
  return distance;
}

// =====================================================
//             OBSTACLE AVOID FUNCTION
// =====================================================

void avoidObstacle() {
  // التوقف فوراً
  stopRobot();
  delay(300);

  // فحص اليمين
  int rightDist = lookRight();
  delay(200);
  
  // فحص الشمال
  int leftDist = lookLeft();
  delay(200);
  
  // إرجاع السيرفو للمركز
  moveServoToAngle(centerAngle);
  delay(300);

  // اختيار الطريق الأوسع
  if (rightDist > leftDist && rightDist > SAFE_DISTANCE) {
    // اليمين أوسع - دور يمين
    stopRobot();
    delay(200);
    
    right();
    delay(TURN_TIME + 100);
    
    stopRobot();
    delay(100);
    // امشي للأمام

    forward();
    delay(1250);

    left();
    delay(750);
  }
  else if (leftDist > rightDist && leftDist > SAFE_DISTANCE) {
    // الشمال أوسع - دور شمال
    stopRobot();
    delay(200);
    
    left();
    delay(TURN_TIME + 100);
    
    stopRobot();
    delay(100);
    
    // امشي للأمام
     forward();
    delay(1250);

    right();
    delay(700);
  }

}

// =====================================================
//               MOVEMENT FUNCTIONS
// =====================================================
void forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, vspeed);
  analogWrite(enB, vspeed);
}

void backward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, vspeed);
  analogWrite(enB, vspeed);
}

void right() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, tspeed);
  analogWrite(enB, tspeed);
}

void left() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, tspeed);
  analogWrite(enB, tspeed);
}

void stopRobot() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}
