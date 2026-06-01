#include <LiquidCrystal.h>

namespace {

constexpr int kJoyXPin = A0;
constexpr int kJoyYPin = A1;
constexpr int kSegmentPinA = 8;
constexpr int kSegmentPinB = 9;
constexpr int kSegmentPinC = A4;
constexpr int kSegmentPinD = A3;
constexpr int kSegmentPinE = A2;
constexpr int kSegmentPinF = 7;
constexpr int kSegmentPinG = 5;
constexpr int kSegmentPinDp = 3;
constexpr int kSegmentPinCount = 8;
constexpr int kLcdRsPin = 4;
constexpr int kLcdEnablePin = 6;
constexpr int kLcdDataPin4 = 10;
constexpr int kLcdDataPin5 = 11;
constexpr int kLcdDataPin6 = 12;
constexpr int kLcdDataPin7 = 13;
constexpr int kLcdColumns = 16;
constexpr int kLcdRows = 2;
constexpr int kInitialPlayerX = 7;
constexpr int kInitialPlayerY = 0;
constexpr int kInitialTimerSeconds = 9;
constexpr unsigned long kCountdownIntervalMs = 1000;
constexpr int kJoystickLowThreshold = 200;
constexpr int kJoystickHighThreshold = 800;
constexpr unsigned long kFrameDelayMs = 100;
constexpr unsigned long kGoalMessageDelayMs = 500;
constexpr unsigned long kClearMessageDelayMs = 2000;
constexpr unsigned long kStartMessageDelayMs = 1000;
constexpr int kRandomSeedPin = A5;
constexpr int kTopRow = 0;
constexpr int kBottomRow = 1;
constexpr bool kSegmentPatterns[10][kSegmentPinCount] = {
    {true, true, true, true, true, true, false, false},
    {false, true, true, false, false, false, false, false},
    {true, true, false, true, true, false, true, false},
    {true, true, true, true, false, false, true, false},
    {false, true, true, false, false, true, true, false},
    {true, false, true, true, false, true, true, false},
    {true, false, true, true, true, true, true, false},
    {true, true, true, false, false, false, false, false},
    {true, true, true, true, true, true, true, false},
    {true, true, true, true, false, true, true, false},
};

LiquidCrystal lcd(kLcdRsPin, kLcdEnablePin, kLcdDataPin4, kLcdDataPin5, kLcdDataPin6, kLcdDataPin7);
const int kSegmentPins[kSegmentPinCount] = {
    kSegmentPinA,
    kSegmentPinB,
    kSegmentPinC,
    kSegmentPinD,
    kSegmentPinE,
    kSegmentPinF,
    kSegmentPinG,
    kSegmentPinDp,
};

struct Position {
    int x;
    int y;
};

struct GameState {
    Position player;
    Position target;
    int remaining_seconds;
    unsigned long previous_time_ms;
};

GameState game_state = {{kInitialPlayerX, kInitialPlayerY}, {0, 0}, kInitialTimerSeconds, 0};

/**
 * @brief Turn off every segment on the 7-segment LED.
 */
void clear_segment_display() {
    for (int index = 0; index < kSegmentPinCount; ++index) {
        digitalWrite(kSegmentPins[index], LOW);
    }
}

/**
 * @brief Show one digit on the 7-segment LED.
 * @param number Digit from 0 to 9.
 */
void show_number(const int number) {
    clear_segment_display();
    if (number < 0 || number > 9) {
        return;
    }

    for (int index = 0; index < kSegmentPinCount; ++index) {
        digitalWrite(kSegmentPins[index], kSegmentPatterns[number][index] ? HIGH : LOW);
    }
}

/**
 * @brief Create a random target position on the LCD.
 * @return New target position.
 */
Position make_random_target() {
    return {random(0, kLcdColumns), random(0, kLcdRows)};
}

/**
 * @brief Reset the player and timer for a new round.
 */
void reset_round() {
    game_state.player = {kInitialPlayerX, kInitialPlayerY};
    game_state.target = make_random_target();
    game_state.remaining_seconds = kInitialTimerSeconds;
    game_state.previous_time_ms = millis();
}

/**
 * @brief Initialize all segment pins as outputs.
 */
void setup_segment_pins() {
    for (int index = 0; index < kSegmentPinCount; ++index) {
        pinMode(kSegmentPins[index], OUTPUT);
    }
    clear_segment_display();
}

/**
 * @brief Keep the player position inside the LCD bounds.
 */
void clamp_player_position() {
    if (game_state.player.x < 0) {
        game_state.player.x = 0;
    }
    if (game_state.player.x >= kLcdColumns) {
        game_state.player.x = kLcdColumns - 1;
    }
    if (game_state.player.y < kTopRow) {
        game_state.player.y = kTopRow;
    }
    if (game_state.player.y > kBottomRow) {
        game_state.player.y = kBottomRow;
    }
}

/**
 * @brief Update player position from joystick input.
 */
void update_player_from_joystick() {
    const int x_value = analogRead(kJoyXPin);
    if (x_value <= kJoystickLowThreshold) {
        game_state.player.x += 1;
    } else if (x_value >= kJoystickHighThreshold) {
        game_state.player.x -= 1;
    }

    const int y_value = analogRead(kJoyYPin);
    if (y_value <= kJoystickLowThreshold) {
        game_state.player.y = kBottomRow;
    } else if (y_value >= kJoystickHighThreshold) {
        game_state.player.y = kTopRow;
    }

    clamp_player_position();
}

/**
 * @brief Draw the target and player on the LCD.
 */
void draw_game_screen() {
    lcd.clear();
    lcd.setCursor(game_state.target.x, game_state.target.y);
    lcd.print("X");
    lcd.setCursor(game_state.player.x, game_state.player.y);
    lcd.print("o");
}

/**
 * @brief Show the game over message and start a new round.
 */
void handle_time_up() {
    lcd.clear();
    lcd.print("GAME OVER...");
    Serial.println("Time Up!");
    clear_segment_display();
    delay(kClearMessageDelayMs);
    reset_round();
}

/**
 * @brief Show the clear sequence and start the next round.
 */
void handle_goal_reached() {
    lcd.clear();
    lcd.setCursor(game_state.player.x, game_state.player.y);
    lcd.print("OK!");
    delay(kGoalMessageDelayMs);

    lcd.clear();
    lcd.print("CLEAR!!");
    clear_segment_display();
    delay(kClearMessageDelayMs);

    reset_round();

    lcd.clear();
    lcd.setCursor(game_state.target.x, game_state.target.y);
    lcd.print("X");
    delay(kStartMessageDelayMs);

    lcd.clear();
    lcd.print("START!!");
    delay(kStartMessageDelayMs);
}

/**
 * @brief Update the countdown once per second.
 */
void update_countdown() {
    const unsigned long current_time_ms = millis();
    if (current_time_ms - game_state.previous_time_ms < kCountdownIntervalMs) {
        return;
    }

    game_state.previous_time_ms = current_time_ms;

    if (game_state.remaining_seconds >= 0) {
        Serial.println(game_state.remaining_seconds);
        show_number(game_state.remaining_seconds);
        game_state.remaining_seconds -= 1;
        return;
    }

    handle_time_up();
}

/**
 * @brief Print initial debug information to the serial monitor.
 */
void print_startup_log() {
    Serial.println("START!");
    Serial.print("Target: ");
    Serial.print(game_state.target.x);
    Serial.print(",");
    Serial.println(game_state.target.y);
}

}  // namespace

void setup() {
    setup_segment_pins();

    Serial.begin(9600);
    lcd.begin(kLcdColumns, kLcdRows);
    lcd.clear();

    randomSeed(analogRead(kRandomSeedPin));
    reset_round();
    print_startup_log();
}

void loop() {
    update_countdown();
    update_player_from_joystick();

    if (game_state.player.x == game_state.target.x && game_state.player.y == game_state.target.y) {
        handle_goal_reached();
    }

    draw_game_screen();
    delay(kFrameDelayMs);
}
