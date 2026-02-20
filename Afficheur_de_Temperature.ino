#include <Wire.h>                // Bibliothèque pour la communication I2C
#include <LiquidCrystal_I2C.h>   // Bibliothèque pour l'écran LCD
#include <DHT.h>                 // Bibliothèque pour le capteur DHT22

// --- CONFIGURATION DES BROCHES ---
#define DHTPIN 2          // Le capteur DHT22 est branché sur la Pin 2
#define DHTTYPE DHT22     // On précise le type de capteur (DHT22)
#define BUZZER_PIN 8      // Le Buzzer est branché sur la Pin 8

// --- PARAMÈTRES ---
float seuilTemperature = 30.0; // Seuil d'alerte en degrés Celsius

// Initialisation de l'écran LCD (adresse 0x27, 16 colonnes, 2 lignes)
// Note : l'adresse peut parfois être 0x3F selon le modèle.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Initialisation du capteur DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialisation du port série pour le débogage
  Serial.begin(9600);
  
  // Initialisation du capteur DHT22
  dht.begin();
  
  // Initialisation de l'écran LCD
  lcd.init();
  lcd.backlight(); // Allume le rétroéclairage
  
  // Configuration du buzzer en mode sortie
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Message d'accueil sur l'écran
  lcd.setCursor(0, 0);
  lcd.print("Station Meteo");
  lcd.setCursor(0, 1);
  lcd.print("Initialisation...");
  delay(2000); // Pause de 2 secondes
  lcd.clear();
}

void loop() {
  // Lecture de l'humidité (en %)
  float humidite = dht.readHumidity();
  // Lecture de la température (en °Celsius)
  float temperature = dht.readTemperature();

  // Vérification si la lecture a échoué
  if (isnan(humidite) || isnan(temperature)) {
    lcd.setCursor(0, 0);
    lcd.print("Erreur capteur!");
    return; // Sort de la boucle loop pour recommencer
  }

  // --- AFFICHAGE SUR L'ÉCRAN LCD ---
  // Ligne 1 : Température
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1); // Affiche avec 1 décimale
  lcd.print(" C");

  // Ligne 2 : Humidité
  lcd.setCursor(0, 1);
  lcd.print("Hum : ");
  lcd.print(humidite, 1);
  lcd.print(" %");

  // --- LOGIQUE DE L'ALERTE (BUZZER) ---
  if (temperature >= seuilTemperature) {
    // Si la température dépasse le seuil : bip intermittent
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    
    // Alerte visuelle (optionnel)
    lcd.setCursor(13, 0);
    lcd.print("!!!"); 
  } else {
    // Sinon, on s'assure que le buzzer est éteint
    digitalWrite(BUZZER_PIN, LOW);
    lcd.setCursor(13, 0);
    lcd.print("   "); // Efface les points d'exclamation
  }

  // Le DHT22 est un peu lent, on attend 2 secondes avant la prochaine lecture
  delay(2000);
}