# Script

## Was ist cache?

Der Cache ist ein Hochgeschwindigkeits-Datenspeichermechanismus, der verwendet wird, um häufig zugegriffene oder kürzlich verwendete Daten und Anweisungen vorübergehend zu speichern. Er befindet sich zwischen dem Hauptspeicher (RAM) und der zentralen Verarbeitungseinheit (CPU) in einem Computersystem. Durch das Speichern von häufig zugegriffenen Daten näher an der CPU ermöglicht der Cache schnellere Zugriffszeiten und reduziert die Latenz, die mit dem Abrufen von Daten aus dem langsameren Hauptspeicher verbunden ist.​

## Geschichte

Im April 1965 führte der britische Computerwissenschaftler Maurice Wilkes das Konzept des Speichercachings ein. Anfangs nannte er es „Slave-Speicher“. „Cache“ ist ein Lehnwort, das in diesem Zusammenhang vermutlich erstmals bei IBM in Amerika aus dem Französischen entnommen wurde.

## Motivation

Auf das Bild sehen wir, das Performance von CPU steigt schneller als Performance von RAM. Damit RAM kein Bottleneck fur das system bekommt, fugen wir memory naher CPU ein.

## Wichtigkeit von Cache

### Schnellerer Datenzugriff​

Durch das Speichern häufig abgerufener Daten näher am Prozessor reduziert Caching die Zeit, die zum Abrufen von Daten von Festplatte  erforderlich ist. ​

### Reduzierte Latenz​

Caching hilft, die Latenz zu verringern, die mit dem Zugriff auf Daten von entfernten oder langsameren Speichergeräten verbunden ist. ​

### Bandbreiteneinsparung​

Caching spart Netzwerkbandbreite, indem häufig angeforderte Inhalte lokal bereitgestellt werden, anstatt sie wiederholt von Remote-Servern abzurufen. ​

### Skalierbarkeit​

Caching verbessert die Systemskalierbarkeit, indem es Verarbeitungs- und Speicherlasten von Backend-Systemen entlastet. 

