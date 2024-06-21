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
#include <Fonts/FreeMonoBold12pt7b.h> // Add a custom font
#include <Fonts/FreeMono9pt7b.h>      // Add a custom font

Adafruit_SSD1306 display(128, 64); // Створити дисплей

int Variable1;

const int sensorPin = A0; // Пін сенсорної кнопки

int buttonState = 0; // Зберігає поточний стан кнопки
int menuOption = 0;  // Зберігає вибраний пункт меню

unsigned long lastButtonPressTime = 0;   // Змінна для зберігання часу останнього натискання кнопки
const unsigned long debounceDelay = 300; // Затримка для уникнення дребітів контактів

unsigned long previousMillis = 0; // Змінна для збереження попереднього часу
const long interval = 5000;       // Інтервал у мілісекундах (1 секунда)
int previousMenuOption = -1;      // Попередній вибраний пункт меню

// Функція виклику після певного інтервалу без зміни пункту меню
void executeAfterDelay(int currentMenuOption)
{
    if (currentMenuOption == previousMenuOption)
    {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval)
        {
            // Виконуємо додатковий код, наприклад, вивід на екран
            display.clearDisplay();
            display.setCursor(30, 40);
            display.print("Finish");

            // Скидаємо попередній час та попередній пункт меню
            previousMillis = currentMillis;
            previousMenuOption = -1;
        }
    }
    else
    {
        // Якщо вибраний новий пункт меню, оновлюємо попередній пункт та час
        previousMillis = millis();
        previousMenuOption = currentMenuOption;
    }
}

void setup() // Start of setup
{

    delay(100); // Ця затримка потрібна для ініціалізації дисплея

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Ініціалізуйте дисплей за допомогою адреси I2C 0x3C

    display.clearDisplay(); // Очистити буфер

    display.setTextColor(WHITE); // Встановити колір тексту

    display.setRotation(0); // Встановити орієнтацію. Переходить від 0, 1, 2 або 3

    display.setTextWrap(false); // За замовчуванням довгі рядки тексту автоматично повертаються до крайнього лівого стовпця.
                                // Щоб змінити цю поведінку (тобто текст збігатиме з правого боку дисплея – корисно для
                                // ефекти рамки прокручування), використовуйте setTextWrap(false). Нормальна поведінка обгортання відновлюється
                                // із setTextWrap(true).

    display.dim(0); // Встановити яскравість (0 — максимум, 1 — темний)
}

void loop() // Start of loop
{
    // Serial.write("Hello, world!");
    // Variable1++;         // Збільште змінну на 1
    // if (Variable1 > 500) // Якщо Variable1 більше 150
    // {
    //     Variable1 = 0; // Установіть для змінної1 значення 0
    // }

    // Перетворіть Variable1 на рядок, щоб ми могли змінити вирівнювання тексту праворуч:
    // Його також можна використовувати для додавання або видалення десяткових чисел.
    // char string[10]; // Створіть масив із 10 символів
    // Перетворення float на рядок:
    // dtostrf(Variable1, 3, 0, string); // (<змінна>,<кількість цифр, яку ми будемо використовувати>,<кількість десяткових цифр>,<назва рядка>)

    // display.clearDisplay(); // Очистіть дисплей, щоб ми могли оновити

    display.setFont(&FreeMono9pt7b); // Встановити спеціальний шрифт
    display.setTextSize(0);          // Встановити розмір тексту. Ми використовуємо спеціальний шрифт, тому ви завжди повинні використовувати розмір тексту 0

    unsigned long currentTime = millis(); // Поточний час

    buttonState = digitalRead(sensorPin); // Зчитування стану сенсорної кнопки

    if (buttonState == HIGH && currentTime - lastButtonPressTime > debounceDelay)
    {                                      // Якщо кнопка натиснута та пройшла достатня затримка
        lastButtonPressTime = currentTime; // Оновлення часу останнього натискання кнопки

        menuOption++; // Збільшення вибраного пункту меню

        if (menuOption > 5)
        { // Якщо досягнуто останній пункт меню, повернення на початок
            menuOption = 0;
        }
    }

    // Виведення відповідного пункту меню на дисплей
    switch (menuOption)
    {
    case 0:
        display.clearDisplay();
        display.setCursor(30, 40);
        display.print("Start");
        executeAfterDelay(menuOption);
        break;
    case 1:
        display.clearDisplay();
        display.setCursor(30, 40);
        display.print("100ml");
        executeAfterDelay(menuOption);
        break;
    case 2:
        display.clearDisplay();
        display.setCursor(30, 40);
        display.print("200ml");
        executeAfterDelay(menuOption);
        break;
    case 3:
        display.clearDisplay();
        display.setCursor(30, 40);
        display.print("300ml");
        executeAfterDelay(menuOption);
        break;
    case 4:
        display.clearDisplay();
        display.setCursor(30, 40);
        display.print("400ml");
        executeAfterDelay(menuOption);
        break;
    case 5:
        display.clearDisplay();
        display.setCursor(30, 40);
        display.print("500ml");
        executeAfterDelay(menuOption);
        break;
    }

    // Намалюйте трикутник:
    // display.drawTriangle(40, 40, 50, 20, 60, 40, WHITE); // Намалюйте трикутник. Координати X, Y для трьох кутових точок, що визначають трикутник, а потім колір

    // // Намалюйте зафарбований трикутник:
    // display.fillTriangle(0, 63, 15, 45, 30, 63, WHITE); // Намалюйте зафарбований трикутник. Координати X, Y для трьох кутових точок, що визначають трикутник, а потім колір

    // // Намалюйте лінію:
    // display.drawLine(40, 63, 70, 63, WHITE); // Накресліть лінію (x0,y0,x1,y1,колір)

    // // Намалюйте коло:
    // display.drawCircle(47, 36, 20, WHITE); //  Намалюйте коло (x, y, радіус, колір). X і Y - це координати центральної точки

    // // Намалюйте зафарбоване коло:
    // display.fillCircle(12, 27, 10, WHITE); // Намалюйте заповнене коло (x, y, радіус, колір). X і Y - це координати центральної точки

    // // Намалюйте округлений прямокутник і заповніть:
    // display.fillRoundRect(58, 0, 18, 18, 5, WHITE); // Намалюйте заповнений округлений прямокутник (x, y, ширина, висота, колір)
    //                                                 // Він малює від місця донизу праворуч

    // // Намалювати прямокутник:
    // display.drawRect(79, 0, 49, 27, WHITE); // Намалюйте прямокутник (x, y, ширина, висота, колір)
    //                                         // Він малює від місця донизу праворуч

    // display.setFont(&FreeMonoBold12pt7b); // Встановити спеціальний шрифт

    // // Вивести змінну з вирівнюванням по лівому краю:
    // display.setCursor(83, 20);  // (x,y)
    // display.println(Variable1); // Текст або значення для друку

    // // Намалюйте заокруглений прямокутник:
    // display.drawRoundRect(79, 37, 49, 27, 8, WHITE); // Намалюйте округлений прямокутник (x, y, ширина, висота, радіус, колір)
    //                                                  // Він малює від місця донизу праворуч

    // // Друк змінної з вирівнюванням по правому краю:
    // display.setCursor(83, 57); // (x,y)
    // display.println(string);   // Текст або значення для друку

    display.display(); // Роздрукуйте все, що ми встановили раніше

} // End of loop
