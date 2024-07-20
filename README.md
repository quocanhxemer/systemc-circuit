# Cache Simulation und Analyse (GRA Abschlussprojekt A13)

## Autoren

- Ivan Logvynenko
- Nguyen Quoc Anh Pham
- Tuan Khang Nguyen

## Motivation

Caches sind heutzutage ein unverzichtbarer Bestandteil der PC-Architektur. Ihre effiziente Nutzung und Verwaltung ist entscheidend für die Leistungsfähigkeit moderner Computersysteme. Ein Cache fungiert als schneller Zwischenspeicher zwischen dem Prozessor und dem Hauptspeicher, wodurch häufig benötigte Daten schneller abgerufen werden können. Dies reduziert die Zugriffszeit auf Daten erheblich und verbessert die Gesamtperformance des Systems.

Die zunehmende Komplexität moderner Anwendungen und die stetig wachsende Datenmenge stellen immer höhere Anforderungen an die Effizienz von Caches. Durch die Simulation und Analyse von Cache-Mechanismen können wir besser verstehen, wie unterschiedliche Strategien und Konfigurationen die Leistung beeinflussen. Dieses Wissen ist nicht nur theoretisch von Interesse, sondern hat auch praktische Anwendungen in der Optimierung realer Systeme.

Unser Projekt zielt darauf ab, ein tieferes Verständnis für die Funktionsweise von Caches zu vermitteln und die Auswirkungen verschiedener Parameter und Strategien auf ihre Leistung zu analysieren. Durch die Implementierung und Simulation eines direkt assoziativen Write-Through-Caches möchten wir die theoretischen Konzepte in die Praxis umsetzen und wertvolle Einblicke in die Cache-Architektur gewinnen.

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

Für die Simulation haben wir die SystemC-Bibliothek verwendet. SystemC ist eine vielseitige und weit verbreitete Bibliothek zur Modellierung von Hardware und zur Durchführung von Systemsimulationen. Mehr konnen Sie [hier](https://systemc.org/) erfahren


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