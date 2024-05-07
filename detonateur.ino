#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "TP-LINK_B05";
const char* password = "BTSbts22";
const char* serverUrl = "http://172.16.200.54/sw/arduinoESP8266/recevoirValeur.php"; // URL du fichier PHP

int startButtonPin = 4; // Bouton de démarrage (Bouton vert), broche GPIO 4 : D2
int stopButtonPin = 0; // Bouton d'arrêt (Bouton bleu), broche GPIO 0 : D3
const int greenLedPin = 14; // LED verte, broche GPIO 14 / D5
const int blueLedPin = 12; // LED bleue, broche GPIO 12 / D6

void setup() {
    pinMode(startButtonPin, INPUT);
    pinMode(stopButtonPin, INPUT);
    pinMode(greenLedPin, OUTPUT);
    pinMode(blueLedPin, OUTPUT);
    Serial.begin(9600);
    
    // Connexion au réseau Wi-Fi
    Serial.println();
    Serial.print("Connexion au réseau ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("Connexion Wi-Fi établie");
    Serial.println("Adresse IP: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    // Vérifier si le bouton de démarrage est enfoncé
    if (digitalRead(startButtonPin) == LOW) { // Bouton vert appuyé
        digitalWrite(greenLedPin, LOW); // Allumer la LED verte
        envoyerValeur(1); // Envoyer la valeur 1
    } else {
        digitalWrite(greenLedPin, HIGH); // Éteindre la LED verte
    }
     // Vérifier si le bouton bleu est enfoncé
    if (digitalRead(stopButtonPin) == LOW) { // Bouton bleu appuyé
        digitalWrite(blueLedPin, LOW); // Allumer la LED bleue
        envoyerValeur(0); // Envoyer la valeur 0
    } else {
        digitalWrite(blueLedPin, HIGH); // Éteindre la LED bleue
    }

    
    // Autres parties de la boucle de programme...
}

// Fonction pour envoyer la valeur 1 au serveur via POST
void envoyerValeur(int valeur) {
    HTTPClient http;

    // Commencer la communication HTTP POST
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Envoyer la requête POST avec la valeur
    String postData = "valeur=" + String(valeur);
    unsigned long startTime = millis(); // Temps de départ
    int httpCode = http.POST(postData);
    unsigned long endTime = millis(); // Temps de fin

    // Calculer le temps de réponse
    unsigned long responseTime = endTime - startTime;
    Serial.printf("Temps de réponse: %lu ms\n", responseTime);
    // Vérifier la réponse du serveur
    if (httpCode > 0) {
        String response = http.getString();
        Serial.println(response);
    } else {
        Serial.printf("Erreur lors de l'envoi de la requête POST: %s\n", http.errorToString(httpCode).c_str());
    }

    // Fermer la connexion HTTP
    http.end();
}
