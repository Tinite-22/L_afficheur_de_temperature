/*
 * Affichage température DHT22 sur LCD I2C
 * Connexions:
 * DHT22: VCC -> 5V, GND -> GND, DATA -> Pin 2
 * LCD I2C: VCC -> 5V, GND -> GND, SDA -> A4, SCL -> A5
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Configuration DHT22
#define DHTPIN 2          // Pin de données du DHT22
#define DHTTYPE DHT22     // Type de capteur
DHT dht(DHTPIN, DHTTYPE);

// Configuration LCD I2C (adresse 0x27, 16 colonnes, 2 lignes)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialisation du port série pour debug
  Serial.begin(9600);
  Serial.println("Initialisation DHT22 + LCD I2C");
  
  // Initialisation du DHT22
  dht.begin();
  
  // Initialisation du LCD
  lcd.init();
  lcd.backlight();
  
  // Message de bienvenue
  lcd.setCursor(0, 0);
  lcd.print("DHT22 Capteur");
  lcd.setCursor(0, 1);
  lcd.print("Initialisation..");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Lecture de la température et de l'humidité
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Vérification si la lecture a échoué
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Erreur de lecture du capteur DHT22!");
    lcd.setCursor(0, 0);
    lcd.print("Erreur capteur!");
    lcd.setCursor(0, 1);
    lcd.print("Verifier cablage");
    delay(2000);
    return;
  }
  
  // Affichage sur le moniteur série
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C | Humidite: ");
  Serial.print(humidity);
  Serial.println(" %");
  
  // Affichage sur le LCD
  lcd.clear();
  
  // Ligne 1: Température
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1);  // 1 décimale
  lcd.print(" ");
  lcd.write(0xDF);  // Symbole degré
  lcd.print("C");
  
  // Ligne 2: Humidité
  lcd.setCursor(0, 1);
  lcd.print("Hum:  ");
  lcd.print(humidity, 1);  // 1 décimale
  lcd.print(" %");
  
  // Attendre 2 secondes avant la prochaine lecture
  delay(2000);
}