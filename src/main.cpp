// ===== Підключення =====
// + на 3,3v
// - на GND
// SCL на А5
// SDA на А4
// ===== ./Підключення =====

// List of fonts: https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts

// List of fonts that support right alignment:
// FreeMono9pt7b.h
// FreeMono12pt7b.h
// FreeMono18pt7b.h
// FreeMono24pt7b.h
// FreeMonoBold9pt7b.h
// FreeMonoBold12pt7b.h
// FreeMonoBold18pt7b.h
// FreeMonoBold24pt7b.h
// FreeMonoBoldOblique9pt7b.h
// FreeMonoBoldOblique12pt7b.h
// FreeMonoBoldOblique18pt7b.h
// FreeMonoBoldOblique24pt7b.h
// FreeMonoOblique9pt7b.h
// FreeMonoOblique12pt7b.h
// FreeMonoOblique18pt7b.h
// FreeMonoOblique24pt7b.h

#include <Adafruit_GFX.h>             // Додайте основну графічну бібліотеку для дисплея
#include <Adafruit_SSD1306.h>         // Включіть бібліотеку Adafruit_SSD1306 для керування дисплеєм
#include <Fonts/FreeMono9pt7b.h>      // Add a custom font
#include <Fonts/FreeMono18pt7b.h>     // Add a custom font
#include <Fonts/FreeMonoBold12pt7b.h> // Add a custom font
#include <Fonts/FreeMonoBold18pt7b.h> // Add a custom font
#include <Fonts/FreeMonoBold24pt7b.h> // Add a custom font
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT); // Створити дисплей

// Определение пинов ENGINE
const int PWMA = 9;
const int AIN2 = 8;
const int AIN1 = 7;
// Определение пинов ENGINE

const int sensorPin = 2; // Пін сенсорної кнопки

int buttonState = 0; // Зберігає поточний стан кнопки
int menuOption = 0;  // Зберігає вибраний пункт меню

unsigned long lastButtonPressTime = 0;   // Змінна для зберігання часу останнього натискання кнопки
const unsigned long debounceDelay = 300; // Затримка для уникнення дребітів контактів

unsigned long previousMillis = 0; // Змінна для збереження попереднього часу
const long interval = 1000;       // Інтервал у мілісекундах (1 секунда)
int previousMenuOption = -1;      // Попередній вибраний пункт меню

unsigned long timerStartMillis; // Змінна для збереження часу початку виконання додаткового коду
int count = 0;                  // Змінна для лічильника рахунку

bool buttonLocked = false; // Прапорець для блокування кнопки
bool buttonStop = false;   // Прапорець для зупинки двигуна

void executeAfterDelay(int currentMenuOption, int countTime)
{
    if (currentMenuOption != previousMenuOption)
    {
        previousMenuOption = currentMenuOption;
        timerStartMillis = millis();
    }

    unsigned long currentMillis = millis();
    if (currentMillis - timerStartMillis >= interval)
    {
        buttonLocked = true;
        display.clearDisplay();
        count++;
        float percentage = ((float)count / (float)countTime) * 100.0;
        int displayPercentage = (int)round(percentage); // де maxCount - максимальне значення count

        display.setFont(&FreeMonoBold24pt7b);

        // Конвертуємо відсотки в рядок
        String percentString = String(displayPercentage) + "%";

        // Отримуємо розміри тексту
        int16_t x1, y1;
        uint16_t w, h;
        display.getTextBounds(percentString, 0, 0, &x1, &y1, &w, &h);

        // Розраховуємо позицію курсора для центрування тексту
        int16_t x = (SCREEN_WIDTH - w) / 2;
        int16_t y = (SCREEN_HEIGHT + h) / 2;

        // Встановлюємо курсор та відображуємо текст
        display.setCursor(x, y);
        display.print(percentString);

        if (count <= countTime)
        {
            // ENGINE
            digitalWrite(AIN1, HIGH);
            digitalWrite(AIN2, LOW);
            analogWrite(PWMA, 250);
            // ENGINE
        }
        else
        {
            digitalWrite(AIN1, LOW);
            digitalWrite(AIN2, LOW);
            analogWrite(PWMA, 0);
            // Конвертуємо відсотки в рядок
            display.clearDisplay();
            display.setCursor(x, y);
            display.print("100%");
            static unsigned long finishTime = 0;
            if (finishTime == 0)
            {
                finishTime = millis();
            }
            if (millis() - finishTime >= 2000)
            {
                menuOption = 0;
                previousMenuOption = -1;
                count = 0;
                finishTime = 0;
                buttonLocked = false;
            }
        }
    }
}

void showBatteryCharge()
{
    int isChargeValue = analogRead(A1); // Перевіряємо чи підєднана зярядка
    if (isChargeValue > 500)            // Робимо перевірку, якщо зарядка підєднаня, то 5в це буде 1024, беремо +- середне значеня, 500 буде достатньо
    {
        // ==== Іконка вилки ====
        display.fillRoundRect(2, 2, 3, 2, 0, WHITE);
        display.fillRoundRect(5, 0, 5, 6, 0, WHITE);
        display.fillRoundRect(10, 1, 3, 1, 0, WHITE);
        display.fillRoundRect(10, 4, 3, 1, 0, WHITE);
        // ==== Іконка вилки ====
    }
}

void showBatteryLevel(int batterySensorValue)
{
// Оголошення констант для розташування та розміру батареї на дисплеї
#define BATTERY_AREA_START_X 106
#define BATTERY_AREA_START_Y 0
#define BATTERY_AREA_WIDTH 20
#define BATTERY_AREA_HEIGHT 6

    static bool show = true; // Початкове значення - показувати

    uint8_t width;

    float voltage = batterySensorValue * (5.0 / 1023.0);

    if (voltage < 3.5)
    {
        if (show) // Показуємо частину заряду батареї
        {
            display.drawRect(BATTERY_AREA_START_X, BATTERY_AREA_START_Y, BATTERY_AREA_WIDTH, BATTERY_AREA_HEIGHT, WHITE);
            width = 0;
            display.fillRect(BATTERY_AREA_START_X, BATTERY_AREA_START_Y, width, BATTERY_AREA_HEIGHT, WHITE);

            // Додамо квадрат справа, вирівняний по центру
            uint8_t squareSize = 2;
            int squareX = BATTERY_AREA_START_X + BATTERY_AREA_WIDTH + 0;                 // Початок квадрата
            int squareY = BATTERY_AREA_START_Y + (BATTERY_AREA_HEIGHT - squareSize) / 2; // Вирівнюємо по центру
            display.fillRect(squareX, squareY, squareSize, squareSize, WHITE);
        }

        // Змінюємо стан мігання кожні 500 мс
        static unsigned long lastUpdateTime = 0;
        if (millis() - lastUpdateTime > 500) // Час для мігання
        {
            show = !show;              // Змінюємо стан показу
            lastUpdateTime = millis(); // Оновлюємо час останнього оновлення
        }
    }
}

void setup() // Start of setup
{
    delay(100);                                // Ця затримка потрібна для ініціалізації дисплея
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Ініціалізуйте дисплей за допомогою адреси I2C 0x3C
    display.clearDisplay();                    // Очистити буфер
    display.setTextColor(WHITE);               // Встановити колір тексту
    display.setRotation(0);                    // Встановити орієнтацію. Переходить від 0, 1, 2 або 3
    display.setTextWrap(false);
    display.dim(0); // Встановити яскравість (0 — максимум, 1 — темний)

    pinMode(A0, INPUT); //  Вимір напруги акб
    pinMode(A1, INPUT); //  Перевіряємо чи підєднана зярядка

    // Настройка пинов как OUTPUT ENGINE
    pinMode(PWMA, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);

    digitalWrite(AIN1, LOW); // Инициализация мотора (остановлен)
    digitalWrite(AIN2, LOW); // Инициализация мотора (остановлен)
    analogWrite(PWMA, 0);    // Установка скорости на 0
}

void loop() // Start of loop
{

    display.setFont(&FreeMonoBold18pt7b); // Встановити спеціальний шрифт
    display.setTextSize(0);               // Встановити розмір тексту. Ми використовуємо спеціальний шрифт, тому ви завжди повинні використовувати розмір тексту 0

    unsigned long currentTime = millis(); // Поточний час

    buttonState = digitalRead(sensorPin); // Зчитування стану сенсорної кнопки
    if (!buttonLocked)
    {

        if (buttonState == HIGH && currentTime - lastButtonPressTime > debounceDelay)
        {                                      // Якщо кнопка натиснута та пройшла достатня затримка
            lastButtonPressTime = currentTime; // Оновлення часу останнього натискання кнопки

            menuOption++; // Збільшення вибраного пункту меню

            if (menuOption > 11)
            { // Якщо досягнуто останній пункт меню, повернення на початок
                menuOption = 0;
            }
        }
    }

    // Екстрена зупинка двигуна, і скидання все на початкові налаштування
    if (buttonLocked)
    {
        if (buttonState == HIGH && currentTime - lastButtonPressTime > debounceDelay)
        {                                      // Якщо кнопка натиснута та пройшла достатня затримка
            lastButtonPressTime = currentTime; // Оновлення часу останнього натискання кнопки
            menuOption = 0;
            previousMenuOption = -1;
            count = 0;
            buttonLocked = false;
            digitalWrite(AIN1, LOW);
            digitalWrite(AIN2, LOW);
            analogWrite(PWMA, 0);
        }
    }

    // Виведення відповідного пункту меню на дисплей
    switch (menuOption)
    {
    case 0:
        display.clearDisplay();
        display.setCursor(12, 42);
        display.print("START");
        break;
    case 1:
        display.clearDisplay();
        display.setCursor(12, 42);
        display.print("100ML");
        executeAfterDelay(menuOption, 40);
        break;
    case 2:
        display.clearDisplay();
        display.setCursor(12, 42);
        display.print("200ML");
        executeAfterDelay(menuOption, 86);
        break;
    case 3:
        display.clearDisplay();
        display.setCursor(12, 42);
        display.print("300ML");
        executeAfterDelay(menuOption, 129);
        break;
    case 4:
        display.clearDisplay();
        display.setCursor(12, 42);
        display.print("400ML");
        executeAfterDelay(menuOption, 175);
        break;
    case 5:
        display.clearDisplay();
        display.setCursor(12, 42);
        display.print("500ML");
        executeAfterDelay(menuOption, 220);
        break;
    case 6:
        display.clearDisplay();
        display.setCursor(12, 42);
        display.print("600ML");
        executeAfterDelay(menuOption, 263);
        break;
    case 7:
        display.clearDisplay();
        display.setCursor(12, 42);
        display.print("700ML");
        executeAfterDelay(menuOption, 306);
        break;
    case 8:
        display.clearDisplay();
        display.setCursor(12, 42);
        display.print("800ML");
        executeAfterDelay(menuOption, 349);
        break;
    case 9:
        display.clearDisplay();
        display.setCursor(12, 42);
        display.print("900ML");
        executeAfterDelay(menuOption, 392);
        break;
    case 10:
        display.clearDisplay();
        display.setCursor(2, 42);
        display.print("1000ML");
        executeAfterDelay(menuOption, 435);
        break;
    }

    // АКБ
    const int batterySensorValue = analogRead(A0);
    showBatteryLevel(batterySensorValue);
    showBatteryCharge(); // Показуємо іконку зарядки, якщо вона підключена.

    display.display(); // Роздрукуйте все, що ми встановили раніше
}

// Перед завантаженням кода на Arduino mini треба затиснути кнопку перезавантаження