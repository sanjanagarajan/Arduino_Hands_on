#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int score = 0;
bool allow_jump = false;

const int jumpButton = 5;
const int buzzer = 7;

byte dino_l[8] = {
  B00000111,
  B00000101,
  B00000111,
  B00010110,
  B00011111,
  B00011110,
  B00001110,
  B00000100
};

byte dino_r[8] = {
  B00000111,
  B00000101,
  B00000111,
  B00010110,
  B00011111,
  B00011110,
  B00001110,
  B00000010
};

byte cactus_small[8] = {
  0b00100,
  0b00101,
  0b10101,
  0b10101,
  0b10111,
  0b11100,
  0b00100,
  0b00000
};

byte cactus_big[8] = {
  B00000000,
  B00000100,
  B00000101,
  B00010101,
  B00010110,
  B00001100,
  B00000100,
  B00000100
};

char world[] = {
  32,32,32,32,32,32,32,83,99,111,114,101,58,32,32,32,
  32,0,32,32,32,32,32,32,32,32,32,32,32,32,32,32
};

bool get_button() {
  return digitalRead(jumpButton);
}

uint8_t scroll_world() {
  delay(100);

  char random_object = random(2, 35);

  if (random_object < 4) world[31] = random_object;
  else world[31] = 32;

  for (int i = 16; i < 32; i++) {
    if (world[i] == 2 || world[i] == 3) {
      char prev = (i < 31) ? world[i + 1] : 32;

      if (world[i - 1] < 2) return 1;

      world[i - 1] = world[i];
      world[i] = prev;
    }
  }

  world[15] = 32;
  if (world[16] < 2) world[16] = 32;

  return 0;
}

void update_world() {
  int game_over = scroll_world();

  if (score == 999) {
    lcd.setCursor(0, 0);
    lcd.print("    YOU WIN!    ");
    while (1);
  }

  if (game_over) {
    tone(buzzer, 500, 500);
    lcd.setCursor(0, 1);
    lcd.print(" GAME OVER! ");
    score = 0;
    delay(1500);

    lcd.setCursor(0, 1);
    lcd.print("Press Start!");

    while (get_button() == HIGH);
  }

  score++;

  lcd.setCursor(13, 0);
  lcd.print("   ");
  lcd.setCursor(13, 0);
  lcd.print(score);

  lcd.setCursor(0, 0);

  for (int i = 0; i < 32; i++) {
    if (world[i] < 2) world[i] ^= 1;

    if (i == 16) lcd.setCursor(0, 1);

    if (i < 13 || i > 15)
      lcd.write(byte(world[i]));
  }
}

void setup() {
  pinMode(jumpButton, INPUT_PULLUP);

  lcd.createChar(0, dino_l);
  lcd.createChar(1, dino_r);
  lcd.createChar(2, cactus_small);
  lcd.createChar(3, cactus_big);

  lcd.init();
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print("Press Start!");

  while (get_button() == HIGH);

  while (true) {
    allow_jump ^= 1;

    if (get_button() == LOW && allow_jump) {
      lcd.setCursor(1, 1);
      lcd.write(byte(32));
      lcd.setCursor(1, 0);
      lcd.write(byte(0));

      tone(buzzer, 1000, 100);

      world[1] = byte(0);
      world[17] = byte(32);

      for (int i = 0; i < 4; i++) update_world();

      world[1] = byte(32);
      world[17] = byte(0);

      lcd.setCursor(1, 0);
      lcd.write(byte(32));
      lcd.setCursor(1, 1);
      lcd.write(byte(0));
    }

    update_world();
  }
}
