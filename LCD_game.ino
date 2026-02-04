#include <LiquidCrystal.h>

#define joyX A0
#define joyY A1

// --- 7セグメントLEDの配線設定 ---
int b = 9;
int a = 8;
int f = 7;
int g = 5;
int dp = 3; 

// アナログピンをデジタルピンとして使用
int c = A4; 
int d = A3; 
int e = A2;

// ピンを配列にまとめる
int segmentPins[] = {a, b, c, d, e, f, g, dp};

LiquidCrystal lcd(4, 6, 10, 11, 12, 13);

// ゲーム用変数
int x = 7;
int y = 0;
int target_X = 0;
int target_Y = 0;

int timer = 9;
unsigned long previousTime = 0;

// --- 7セグ表示用関数群 ---

// 全消灯
void clearDisplay(void) {
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(dp, LOW);
}

// 数字ごとの点灯パターン
void display1(void) {
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
}

void display2(void) {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(d, HIGH);
}

void display3(void) {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(g, HIGH);
}

void display4(void) {
  digitalWrite(f, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(c, HIGH);
}

void display5(void) {
  digitalWrite(a, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
}

void display6(void) {
  digitalWrite(a, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
}

void display7(void) {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
}

void display8(void) {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void display9(void) {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(f, HIGH);
}

void display0(void) {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
}

// 【重要】数字を受け取って対応する関数を呼ぶ関数
void showNumber(int num) {
  clearDisplay(); // 重ならないように一度消す
  switch (num) {
    case 0: display0(); break;
    case 1: display1(); break;
    case 2: display2(); break;
    case 3: display3(); break;
    case 4: display4(); break;
    case 5: display5(); break;
    case 6: display6(); break;
    case 7: display7(); break;
    case 8: display8(); break;
    case 9: display9(); break;
    default: clearDisplay(); break; // 10以上のときは表示しない
  }
}

// --- セットアップ ---
void setup() {
  // 7セグ用のピンを出力に設定
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(dp, OUTPUT);

  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();

  randomSeed(analogRead(A5));
  target_X = random(0, 16);
  target_Y = random(0, 2);

  Serial.println("START!");
  Serial.print("Target: ");
  Serial.print(target_X);
  Serial.print(",");
  Serial.println(target_Y);
}

// --- メインループ ---
void loop() {
  unsigned long currentTime = millis();
  
  // 1秒ごとの処理（カウントダウン）
  if (currentTime - previousTime >= 1000) {
    previousTime = currentTime;

    if (timer >= 0) {
      Serial.println(timer);

      // 【ここが変わりました】7セグメントLEDに残り時間を表示
      // 10秒のときは消灯し、9〜0のときに数字を出します
      if (timer <= 9) {
        showNumber(timer);
      } else {
        clearDisplay();
      }

      timer -= 1;
    } else {
      // タイムアップ処理
      lcd.clear();
      lcd.print("GAME OVER...");
      Serial.println("Time Up!");
      
      clearDisplay(); // LEDも消す
      
      timer = 9;
      delay(2000);

      // リセット
      x = 7;
      y = 0;
      target_X = random(0, 16);
      target_Y = random(0, 2);
    }
  }

  // --- ジョイスティック操作 ---
  int x_val = analogRead(joyX);

  if (x_val <= 200) {
    x += 1;
  }
  else if(x_val >= 800) {
    x -= 1;
  }
  
  int y_val = analogRead(joyY);

  if (y_val <= 200) {
    y = 1;
  }
  else if(y_val >= 800) {
    y = 0;
  }

  if (x >= 16) {
    x = 15;
  }
  if (x < 0) { // 左端制限も追加
    x = 0;
  }

  // --- クリア判定 ---
  if (x == target_X && y == target_Y) {
    // 重なった瞬間
    lcd.clear();
    lcd.setCursor(x, y); // 位置を合わせて
    lcd.print("OK!");    // OKと表示
    delay(500);
    
    lcd.clear();
    lcd.print("CLEAR!!");
    clearDisplay(); // 7セグも消す
    delay(2000); 
    lcd.clear();

    // 次のゲームへ
    timer = 9;
    target_X = random(0, 16);
    target_Y = random(0, 2);
    x = 7;
    y = 0;
    
    // ターゲットのヒントを表示
    lcd.setCursor(target_X, target_Y);
    lcd.print("X");
    delay(1000);
    
    lcd.clear();
    lcd.print("START!!");
    delay(1000);
  }

  // --- 描画処理 ---
  lcd.clear();
  
  // ターゲット表示
  lcd.setCursor(target_X, target_Y);
  lcd.print("X");

  // プレイヤー表示
  lcd.setCursor(x, y);
  lcd.print("o");

  delay(100);
}