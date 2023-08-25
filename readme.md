# WEMOS D1 Mini BMP280 mit MQTT

![Fühler](CAD%20Files/Fühler.jpg)
Das Projekt nutzt einen WEMOS D1 Mini mit einem BMP280 Temperatur- und Luftdrucksensor, um Messdaten zu sammeln und diese an einen MQTT-Broker zu senden.

Kostenpunkt des Fühlers liegt insgesamt unter 25€ pro Stück.

Entwickelt wurde das Projekt von Johannes Röring mit Unterstützung der EMAG Zerbst Maschinenfabrik GmbH welche freundlicherweise die Räumlichkeiten und Hardware bereitgestellt hat.

## Inhaltsverzeichnis
1. [Voraussetzungen](#voraussetzungen)
2. [Installation](#installation)
3. [Schaltplan](#schaltplan)
4. [3D-Gehäuse](#3d-gehäuse)
5. [Konfiguration](#konfiguration)
6. [Nutzung](#nutzung)
7. [Support](#support)
8. [Lizenz](#lizenz)

## Voraussetzungen

- **PlatformIO** installiert in **Visual Studio Code**
- Internetverbindung für die Installation der Bibliotheken
- Bestehende WLAN Infrastruktur und MQTT Broker
-> Sollte noch kein Broker und weiterführende Infrastruktur bestehen sprechen sie mich gern per Mail (jroering@emag.com) an und ich zeige ihnen wie es weitergeht

## Installation

1. **Klone das Repository** oder lade den Code herunter.
2. **Öffne den Projektordner in PlatformIO** unter Visual Studio Code.
3. **PlatformIO wird automatisch die benötigten Bibliotheken installieren**.

## Schaltplan

Der Schaltplan für das Anschließen des BMP280 an den WEMOS D1 Mini ist wie folgt:

- **VCC** des BMP280 an **3.3V** des WEMOS D1 Mini
- **GND** des BMP280 an **GND** des WEMOS D1 Mini
- **SDA** des BMP280 an **D2** des WEMOS D1 Mini
- **SCL** des BMP280 an **D1** des WEMOS D1 Mini

## 3D-Gehäuse

Das Gehäuse für dieses Projekt kann 3D-gedruckt werden. Die STL-Dateien für das Gehäuse befinden sich im Ordner **CAD Files** im Hauptverzeichnis des Projekts.

## Konfiguration

1. **Kopiere die `credentials_template.h`-Datei** im Ordner src und benenne sie in `credentials.h` um.
2. **Öffne die `credentials.h`-Datei** und trage die erforderlichen Informationen ein:
   - `ssid` und `password` für deine WLAN-Verbindung.
   - `mqtt_server`, `mqtt_username`, und `mqtt_password` für deinen MQTT-Broker.
   - Weitere funktionale Konfigurationen wie `MEASURE_INTERVAL`, `SENSOR_ID`, usw.

## Nutzung

1. **Lade den Sketch auf deinen WEMOS D1 Mini hoch**.
2. **Öffne den seriellen Monitor**, um Debug-Informationen und Messdaten anzuzeigen.
3. **Der Sensor sendet nun Temperatur- und Druckdaten** an den angegebenen MQTT-Broker.

In den Kommenden Monaten wird weitere Dokumentation veröffentlicht um dann diese Sensordaten auch aufzuzeichnen und zu Visualisieren
dafür Verwendet wird Mosquitto, Node-Red, TimescaleDB und Grafana.
Gerne gebe ich diese Informationen aber auch auf Anfrage früher heraus, es existiert Stand 25.08.2023 lediglich noch keine Fertige Dokumentation

## Support

Falls du Fragen oder Probleme hast, kannst du dich gerne an mich wenden.

## Lizenz

Dieses Projekt steht unter der MIT-Lizenz - siehe die [LICENSE.md](LICENSE.md) Datei für Details.
