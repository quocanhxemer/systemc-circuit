RahmenProgramm:
Imlementierung: in" "src/args_parser.c"
1. Verwendung von "getopt_long"
Das Programm verwendet die Funktion "getopt_long" aus der GNU C-Bibliothek, um die Kommandozeilenargumente zu parsen.
2. Fehlerbehandlung bei Invalid Optionen:
- **Invalid input type** für Zahlen: Durch convert_int und convert_unsigned mit der Nutzung von strtol. Außerdem wird es auch geprüft, ob die Zahl kleiner als 0 oder overflow ist.
- Cacheline-size in Byte, L1-Cache-lines und L2-Cache-lines müssen Potenz of 2 sein. (isPotenzOf2)
- Richtige Optionen: L1-Lines ≤ L2-Lines sowie L1-Latency ≤ L2-Latency < Memory-Latency sein.
- Permission file: wenn Inputfile lesbar und Outputfile schreibar (wenn existiert)
- Valid path für Ausgabedatei (Directory existiert)
- Duplikate Optionen
- Unbekannte Argumente: erste unbekannte ist immer Inputfile, ab 2. ist invalid


Simulation testen
1. Invalid Optionen einzugeben (z.B. in tests/test_getopt.sh, Die Tests erwarten, der Programm mit Fehler terminate)
2. Valid Optionen testen (in tests/tests_parse_args.c)
3. Bestimmte Algorithmen durchführen, wie MergeSort und QuickSort, um Korrektheit sowie Laufzeit zu analysieren.