#include <Arduino.h>
// Определение пинов
const int PWMA = 9;
const int AIN1 = 7;
const int AIN2 = 8;

void setup()
{
    // Настройка пинов как OUTPUT
    pinMode(PWMA, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);

    // Инициализация мотора (остановлен)
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, 0); // Установка скорости на 0
}

void loop()
{
    // Включение мотора вперед на 2 секунды
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, 255); // Управление скоростью - изменяйте значение от 0 до 255

    delay(2000);

    // Остановка мотора
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, 0); // Установка скорости на 0

    // Пауза перед следующей итерацией
    delay(1000);
}
