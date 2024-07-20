# Cache Simulation und Analyse (GRA Abschlussprojekt A13)

## Autoren

- Ivan Logvynenko
- Nguyen Quoc Anh Pham
- Tuan Khang Nguyen

## Motivation

Caches sind heutzutage ein unverzichtbarer Bestandteil der PC-Architektur. Eine effiziente Nutzung und Verwaltung ist entscheidend für die Leistungsfähigkeit moderner Systeme. Ein Cache ist einen schneller Zwischenspeicher zwischen dem Prozessor und dem Hauptspeicher, wodurch häufig benötigte Daten schneller abgerufen werden können. Dies reduziert die Zugriffszeit auf Daten und verbessert die Performance des Systems.

Unser Projekt zielt darauf, ein tieferes Verständnis für die Funktionsweise von Caches zu entwiekeln und die Auswirkungen verschiedener Parameter und Strategien auf ihre Leistung zu analysieren. Durch die Implementierung und Simulation eines direkt assoziativen Write-Through-Caches möchten wir die theoretischen Konzepte in die Praxis umsetzen.

## Projektbeschreibung

In diesem Projekt haben wir eine Simulation eines direkt assoziativen Write-Through-Caches implementiert. Der Cache-Simulator unterstützt grundlegende Operationen wie das Lesen und Schreiben von Daten und simuliert die Cache-Treffer und -Fehlerraten unter verschiedenen Bedingungen. Die Simulation hilft, die Auswirkungen verschiedener Cache-Parameter wie Cache-Größe, Blockgröße und Assoziativität zu analysieren.

### Funktionsweise des Simulators

Der Cache-Simulator wurde in C++ unter Verwendung der SystemC-Bibliothek implementiert. SystemC ermöglicht es, Hardware-Komponenten auf hoher Abstraktionsebene zu modellieren und zu simulieren. Unser Simulator modelliert einen direkt assoziativen Cache, der die folgenden Hauptmerkmale aufweist:

 * Write-Through-Politik: Jede Schreiboperation im Cache wird direkt an den Hauptspeicher weitergeleitet, um Datenkonsistenz zu gewährleisten.
 * Direkte Assoziativität: Jeder Speicherblock wird genau einer Cache-Linie zugeordnet, was die Implementierung vereinfacht, aber die Flexibilität verringert.

Implementierung

Das Projekt ist in mehrere Module unterteilt, die jeweils eine spezifische Funktionalität bieten:

 * api: Stellt die Schnittstellen für die Cache-Operationen bereit.
 * args_parser: Verarbeitet die Befehlszeilenargumente und konfiguriert die Simulation.
 * csv_reader: Liest Eingabedaten aus CSV-Dateien, die die zu simulierenden Speicherzugriffe darstellen.
 * cache: Implementiert die Cache-Logik und die Speicherverwaltung.
 * controller: Koordiniert die Cache-Operationen und überwacht die Simulation.
 * gates_count: Simuliert die Hardware-Gatter und deren Nutzung im Cache.
 * memory: Modelliert den Hauptspeicher, auf den der Cache zugreift.

Anwendungsfälle

Der Simulator kann für verschiedene Szenarien und Anwendungen eingesetzt werden, darunter:

 1. Leistungsanalyse: Bewertung der Cache-Leistung unter unterschiedlichen Konfigurationen und Lastbedingungen.
 2. Bildung: Veranschaulichung von Cache-Konzepten und -Mechanismen für Bildungszwecke.
 3. Forschung: Untersuchung neuer Cache-Architekturen und Optimierungsstrategien.

### Technische Einzelheiten

Für die Simulation haben wir die [SystemC-Bibliothek](https://systemc.org/) verwendet. SystemC ist eine vielseitige und weit verbreitete Bibliothek zur Modellierung von Hardware und zur Durchführung von Systemsimulationen.

#### Parametern

- Positional arguments:

| Parameter name | Default Wert           | Wert Typ     | Beschreibung                                              |
|----------------|------------------------|--------------|-----------------------------------------------------------|
| Dateiname      | ./examples/default.csv | Path/zu/file | Die Eingabedatei, die die zu verarbeitenden Daten enthält |

- Optional arguments:

| Parameter name   | Default Wert  |  Wert Typ     | Beschreibung                                       |
|------------------|---------------|---------------|----------------------------------------------------|
| -c/--cycles      | 500           |  Integer      | Die Anzahl der Zyklen, die simuliert werden sollen.|
| --cacheline-size | 4             |  Unsigned     | Die Größe einer Cachezeile in Byte.                |
| --l1-lines       | 8             |  Unsigned     | Die Anzahl der Cachezeilen des L1 Caches.          |
| --l2-lines       | 16            |  Unsigned     | Die Anzahl der Cachezeilen des L2 Caches.          |
| --l1-latency     | 2             |  Unsigned     | Die Latenzzeit des L1 Caches in Zyklen.            |
| --l2-latency     | 5             |  Unsigned     | Die Latenzzeit des L2 Caches in Zyklen.            |
| --memory-latency | 10            |  Unsigned     | Die Latenzzeit des Hauptspeichers in Zyklen.       |
| --tf=            | None          |  Path/zu/file | Ausgabedatei für ein Tracefile mit allen Signalen  |
| -h/--help        |---------------|---------------| Help message zeigen                                |

#### CSV-File

Um cache operationen zum simulation uberzugeben unterstutzt unsere Program eine CSV-file ubergabe.
Damit es erfolgreich gelesen werde soll es folgende Regeln folgen:
Lese zugrif:

| Operation | Address                 | Data |
|-----------|-------------------------|------|
| R         | decimal / 0xhexadecimal |------|

Schreib zugrif:

| Operation | Address                 | Data                    |
|-----------|-------------------------|-------------------------|
| R         | decimal / 0xhexadecimal | decimal / 0xhexadecimal |

Einen [Beispiel](/examples/default.csv):

```csv
W 1 4
R 1
R 2
W 2 3
R 2
W 0x4 0x5
R 4
R 2
W 0x4 0x6
W 6 7
R 0x6
```

## Aufbau

### Anforderungen

* gcc/++ oder clang/++ (default: gcc/++)
* Make
* SYSTEMC_HOME-Umgebungsvariable, die den Pfad zu Ihrer `systemc` Installation enthält

### Kompilieren und Ausführen

Um das Projekt zu kompilieren, führen Sie die folgenden Befehle aus:
``` bash
chmod +x ./build.sh
./build.sh
```

Dies startet die Kompilierung des Projekts. Alternativ können Sie das Projekt manuell kompilieren und ausführen:
``` bash
make
./cache
``` 

## Arbeitsteilung

- Ivan Logvynenko: 
	- CSV-parsing
	- Makefile
	- README.md
	- slides
	- run_simulation
	- examples
- Nguyen Quoc Anh Pham: 
	- Cache
	- slides
	- run_simulation
	- examples
- Tuan Khang Nguyen: 
	- Args_parsing
	- slides
	- examples