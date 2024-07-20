#include "args_parser.h"

const char *help_msg =
        "Positional arguments:\n"
        "<Dateiname>  (default: %s)    — Die Eingabedatei, die die zu verarbeitenden Daten enthält\n"
        "\n"
        "Optional arguments:\n"
        "-c <int> /--cycles <int>    (default: %d)        — Die Anzahl der Zyklen, die simuliert werden sollen.\n"
        "--cacheline-size <unsigned> (default: %d)          — Die Größe einer Cachezeile in Byte.\n"
        "--l1-lines <unsigned>       (default: %d)          — Die Anzahl der Cachezeilen des L1 Caches.\n"
        "--l2-lines <unsigned>       (default: %d)         — Die Anzahl der Cachezeilen des L2 Caches.\n"
        "--l1-latency <unsigned>     (default: %d)          — Die Latenzzeit des L1 Caches in Zyklen.\n"
        "--l2-latency <unsigned>     (default: %d)          — Die Latenzzeit des L2 Caches in Zyklen.\n"
        "--memory-latency <unsigned> (default: %d)         — Die Latenzzeit des Hauptspeichers in Zyklen.\n"
        "--tf=<Dateiname>                                  — Ausgabedatei für ein Tracefile mit allen Signalen\n"
        "-h/--help                                         — Show help message\n";

const char *USAGE_MSG =
        "Usage: %s -h or --help         Show help message and exit\n"
        "       %s                      Run Cache Simulation with default value\n";

void print_usage(const char *progname) {
    fprintf(stderr, USAGE_MSG, progname, progname);
}

// stderr and stdout
void print_help(const char *progname) {
    print_usage(progname);
    fprintf(stderr, help_msg,
            DEFAULT_INPUT_FILE_PATH,
            DEFAULT_CYCLES_VALUE,
            DEFAULT_CACHELINE_SIZE_VALUE,
            DEFAULT_L1_LINES_VALUE,
            DEFAULT_L2_LINES_VALUE,
            DEFAULT_L1_LATENCY_VALUE,
            DEFAULT_L2_LATENCY_VALUE,
            DEFAULT_MEMORY_LATENCY_VALUE);
}

// nameCase
// duplicateOption????
void printDuplicateOption(const char *optionName) {
    fprintf(stderr, "ERROR: Option %s wurde mehrmals eingegeben\n", optionName);
}

// try to convert input to Integer
int convert_int(char *c, long *l, char *message) {
    errno = 0;
    char *endptr;
    *l = strtol(c, &endptr, 10);
    if (endptr == c || *endptr != '\0') {
        fprintf(stderr, "ERROR: Invalid number in %s: %s could not be converted to integer\n", message, c);
        return 1;
    } else if (errno == ERANGE || *l > INT_MAX) {
        fprintf(stderr, "ERROR: Invalid number in %s: %s over- or underflows integer\n", message, c);
        return 1;
    } else if (*l <= 0) {
        fprintf(stderr, "ERROR: Invalid number in %s: must be positiv\n", message);
        return 1;
    }
    return 0;
}

// Is the number power of 2
int isPotenzOf2(const unsigned *number, char *name) {
    //Use bitshift to check if a number is power of 2
    //eg. 16=(b10000) is power of 2 -> 15 = (b01111) => 16&15=0
    //when (n & (n-1))!=0 and n>=2 then n is not power of 2
    if (*number < 2 || ((*number & (*number - 1)) != 0)) {
        fprintf(stderr, "ERROR: %s should be Power of 2\n", name);
        if (strcmp(name, "cacheline-size")==0){
            fprintf(stderr,
                    "ERKLÄRUNG: 1 Cacheline Size hat eine positive Integer Zahl Bits für Offset-Bits, so die Größe von CacheLineSize (in Byte) muss die Potenz von 2 (größer oder gleich 2) sein)\n");
        }
        else{
            fprintf(stderr,
                    "ERKLÄRUNG: 1 Cachezeile hat eine positive Integer Zahl Bits für Index-Bits, so die Anzahl von Cachezeilen muss die Potenz von 2 (größer oder gleich 2) sein)\n");
        }
        return 1;
    }
    return 0;
}

// message for print out console
int convert_unsigned(char *c, unsigned long *l, char *message) {
    errno = 0;
    char *endptr;
    *l = strtoul(c, &endptr, 10);
    if (endptr == c || *endptr != '\0') {
        fprintf(stderr, "ERROR: Invalid number in %s: %s could not be converted to unsigned\n", message, c);
        return 1;
    } else if (errno == ERANGE || *l > UINT_MAX) {
        fprintf(stderr, "ERROR: Invalid number in %s: %s over- or underflow unsigned integer\n", message, c);
        return 1;
    } else if (*l <= 0) {
        fprintf(stderr, "ERROR: Invalid number in %s: must be positiv\n", message);
        return 1;
    }
    return 0;
}


//Check if the Input for valid CacheLines, Latency (e.g. l1CacheLines must be smaller than l2CacheLines)
int checkValid(
               unsigned l1CacheLines,
               unsigned l2CacheLines,
               unsigned l1CacheLatency,
               unsigned l2CacheLatency,
               unsigned memoryLatency) {
    if (l1CacheLines > l2CacheLines) {
        fprintf(stderr, "ERROR: L1 Cache Lines müssen kleiner als L2 Cache Lines\n");
        fprintf(stderr,
                "ERKLÄRUNG: Wenn L1 Caches mehr Lines als L2 hat, dann L1 Cache hat größere Größe als L2 Cache, im Widerspruch mit Inclusiv Cache\n");
        return 1;
    } else if (l1CacheLatency > l2CacheLatency) {
        fprintf(stderr,
                "ERROR: L1 Cache Latency muss kleiner oder gleich als L2 Cache Latency sein, da Cache inklusiv ist. (Alles in L1 Cache kann man auch in L2 Cache finden)\n");
        return 1;
    } else if (l2CacheLatency >= memoryLatency) {
        fprintf(stderr,
                "ERROR: L2 Cache Latency muss kleiner als Memory Latency sein, ansonsten Cache-Nutzung macht keinen Sinn.\n");
        return 1;
    } else if (l1CacheLatency >= memoryLatency) {
        fprintf(stderr,
                "ERROR: L1 Cache Latency muss kleiner als Memory Latency sein, ansonsten Cache-Nutzung macht keinen Sinn.\n");
        return 1;
    }
    //Warning but not invalid
//    if (l1CacheLatency>cycles){
//        fprintf(stderr, "WARNING: L1 Latency ist größer als cycles, deswegen kann wahrscheinlich keine Requests bearbeitet werden\n");
//    }
    if (l1CacheLatency >= 100) {
        fprintf(stderr, "WARNING: L1 CacheLatency ist größer als normal\n");
    }
    if (l2CacheLatency>=100){
        fprintf(stderr, "WARNING: L1 CacheLatency ist größer als normal\n");
    }
    if (memoryLatency >= 1000) {
        fprintf(stderr, "WARNING: MemoryLatency ist größer als normal\n");
    }
    return 0;
}

//check valid filename for Tracefile
int is_file_name_valid(const char *filename) {
    const char *invalid_chars = "<>:\"/\\|?*[]";
    char *t;
    //When filename contains invalid character
    if ((t = strpbrk(filename, invalid_chars)) != 0) {
        fprintf(stderr, "ERROR: TraceFile \"%s\" hat invalid Characters: %c\n",filename, *t);
        return 1;
    }
    //When filename has valid length
    if (strlen(filename) > 255) {
        fprintf(stderr, "ERROR: TraceFile Name ist zu lang!\n");
        return 1;
    }
    return 0;
}
//check permission r (readable)  for filename
bool can_read_file(const char *filename) {
    return (access(filename, R_OK) != -1);
}
//check permission w (writeable) for filename
bool can_write_file(const char *filename) {
    return (access(filename, W_OK) != -1);
}

//implement strdup allocate (like strcpy)
// Because i can't use strdup
char *strdup1(const char *c) {
    char *dup = malloc(strlen(c) + 1);
    if (dup != NULL)
        strcpy(dup, c);
    return dup;
}
//check directory exists
bool does_dir_exist(const char *directory) {
    struct stat buffer;
    return (stat(directory, &buffer) == 0 && S_ISDIR(buffer.st_mode));
}
//check file exists
bool does_file_exist(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}
//try to get directory from path
char *get_directory(const char *filepath) {
    char *path_copy = strdup1(filepath);
    //fail by allocata strdup
    if (!path_copy) {
        perror("ERROR: Problem bei Allocate strdup (Copy tracefilepath) in Heap!");
        exit(EXIT_FAILURE);
    }
    char *dir = dirname(path_copy);
    char *result = strdup1(dir);
    free(path_copy);
    if (!result) {
        perror("ERROR: Problem bei Allocate strdup (Copy tracefilepath) in Heap!");
        exit(EXIT_FAILURE);
    }
    return result;
}

//Check if the input file exists, is regular and readable
int check_input_file(const char *filename) {
    struct stat buffer;
    //Check file exists
    if (stat(filename, &buffer) != 0) {
        fprintf(stderr, "ERROR: Input File \"%s\" existiert nicht!\n", filename);
        return 1;
    }
        //Check if filename is a regular file
    else if (S_ISREG(buffer.st_mode) == 0) {
        fprintf(stderr, "ERROR: Input File \"%s\" ist keiner normale File!\n", filename);
        return 1;
    }
        //check if filename is readable
    else if (!can_read_file(filename)) {
        fprintf(stderr, "ERROR: Input File \"%s \" ist nicht lesbar\n", filename);
        return 1;
    }
    //Check input file has extension .csv
    //Pointer to last char '.' in filename
    const char * extension= strrchr(filename, '.');
    if (strcmp(extension, ".csv")!=0){
        fprintf(stderr, "ERROR: Input File \"%s\" hat keine Extension \".csv\"!\n", filename);
        return 1;
    }
    return 0;
}

//check valid the tracefile name: valid length,  has permission when file exists
int check_trace_file(char *filename) {
    //I want to add .vcd at the end, so is strlen(filename)+4<= PATH_MAX
    if (filename==NULL){
        fprintf(stderr, "ERROR: TraceFile Name ist null\n");
        return 1;
    }
    if (strlen(filename)==0){
        fprintf(stderr, "ERROR: Missing Arguments for --tf=\n");
        return 1;
    }
    if (strlen(filename) + 4 > PATH_MAX) {
        fprintf(stderr, "ERROR: TraceFile Name(Path Name) ist zu lang");
        return 1;
    }
    //filename has .vcd at the end -> append filename to the end
    size_t n = strlen(filename);
    char newFilename[n + 5];
    //newFilename has ".vcd" extensio
    strncpy(newFilename, filename, n);
    newFilename[n] = '.';
    newFilename[n + 1] = 'v';
    newFilename[n + 2] = 'c';
    newFilename[n + 3] = 'd';
    newFilename[n + 4] = '\0';

    // if tracefile exists, then check if tracefile is writeable
    if (does_file_exist(newFilename)) {
        fprintf(stderr, "WARNING: Du überschreibst eine existierte Datei!!!\n");
        if (!can_write_file(newFilename)) {
            fprintf(stderr, "ERROR: TraceFile \"%s\"ist nicht schreibbar!\n", newFilename);
            return 1;
        }
        return 0;
    }
        //if tracefile does not exist, then check valid path and filename
    else {
        char *directory = get_directory(newFilename);
        //if the directory exists, then check, if the filename is valid or not
        if (does_dir_exist(directory)) {
            //strdup1 : ist nur strcpy mit malloc
            char *dup_filename = strdup1(filename);
            //basename: filename
            char *only_filename = basename(dup_filename);
            //Check if tracefile has valid name
            if (is_file_name_valid(only_filename) != 0) {
                free(dup_filename);
                free(directory);
                return 1;
            }
            free(directory);
            return 0;
        } else {
            fprintf(stderr, "ERROR: Directory \"%s\" aus TraceFile Path existiert nicht!\n", directory);
            free(directory);
            return 1;
        }

    }
}

struct arguments *parse_args(int argc, char **argv) {
    const char *progname = argv[0];
//    if (argc == 1) {
//        print_usage(progname);
//        exit(EXIT_FAILURE);
//    }

    int opt;
    int option_index = 0;

    struct arguments *args = malloc(sizeof(struct arguments));
    if (args == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for arguments\n");
        exit(EXIT_FAILURE);
    }
    memset(args, 0, sizeof(struct arguments));

    //Default values for Input
    // default values
    long cycles = DEFAULT_CYCLES_VALUE;
    unsigned long lineSize = DEFAULT_CACHELINE_SIZE_VALUE;
    unsigned long l1Line = DEFAULT_L1_LINES_VALUE;
    unsigned long l2Line = DEFAULT_L2_LINES_VALUE;
    unsigned long l1Latency = DEFAULT_L1_LATENCY_VALUE;
    unsigned long l2Latency = DEFAULT_L2_LATENCY_VALUE;
    unsigned long memLatency = DEFAULT_MEMORY_LATENCY_VALUE;
    char *traceFile = NULL;
    char *fileInputName = DEFAULT_INPUT_FILE_PATH;
    //size_t numRequest = 0;

    //Request Flags: Überprüfen, welches Value von Cache eingegeben wurden.

    bool cycles_Flags = false;
    bool l1Line_Flags = false;
    bool l2Line_Flags = false;
    bool lineSize_Flags = false;
    bool l1Latency_Flags = false;
    bool l2Latency_Flags = false;
    bool memLatency_Flags = false;
    //bool inputFile_Flags = false;
    bool tf_Flags = false;

    //Option for the getopt_long
    struct option long_options[] = {
            {"cycles",         required_argument, 0, 'c'},
            {"cacheline-size", required_argument, 0, 'a'},
            {"l1-lines",       required_argument, 0, 'b'},
            {"l2-lines",       required_argument, 0, 'l'},
            {"l1-latency",     required_argument, 0, 'd'},
            {"l2-latency",     required_argument, 0, 'e'},
            {"memory-latency", required_argument, 0, 'm'},
            {"tf=",            required_argument, 0, 't'},
            {"help",           no_argument,       0, 'h'},
            {0, 0,                                0, 0}
    };
    while ((opt = getopt_long(argc, argv, "c:h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'c': {
                //Case cycles and c
                if (convert_int(optarg, (long *) &cycles, "cycles") != 0) {
                    free(args);
                    exit(EXIT_FAILURE);
                } else if (cycles_Flags) {// When Option Cycles duplicate
                    printDuplicateOption("--cycles | -c ");
                    free(args);
                    exit(EXIT_FAILURE);
                }
                cycles_Flags = true;
                break;
            }
            case 'a'://Case cacheline-size
                if (convert_unsigned(optarg, (unsigned long *) &lineSize, "cacheline-size") != 0) {
                    free(args);
                    exit(EXIT_FAILURE);
                }

                //Check duplication
                if (lineSize_Flags) {
                    printDuplicateOption("--cacheline-size");
                    free(args);
                    exit(EXIT_FAILURE);
                }
                if (isPotenzOf2((unsigned int *) &lineSize, "cacheline-size") != 0) {
                    free(args);
                    exit(EXIT_FAILURE);
                }
                lineSize_Flags = true;
                break;
            case 'b'://Case l1-Line
                if (convert_unsigned(optarg, (unsigned long *) &l1Line, "l1-lines") != 0) {
                    free(args);
                    exit(EXIT_FAILURE);
                }
                //Check if l1Line is potenz of 2
                if (isPotenzOf2((unsigned int *) &l1Line, "l1-lines") != 0) {
                    free(args);
                    exit(EXIT_FAILURE);
                }
                //Check duplication
                if (l1Line_Flags) {
                    printDuplicateOption("--l1-lines");
                    free(args);
                    exit(EXIT_FAILURE);
                }
                l1Line_Flags = true;
                break;
            case 'l'://Case l2-Line
                if (convert_unsigned(optarg, (unsigned long *) &l2Line, "l2-lines") != 0) {
                    free(args);
                    exit(EXIT_FAILURE);
                }
                //Check if l2Line is potenz of 2
                if (isPotenzOf2((unsigned int *) &l2Line, "l2-lines") != 0) {
                    free(args);
                    exit(EXIT_FAILURE);
                }
                //Check duplication
                if (l2Line_Flags) {
                    printDuplicateOption("--l2-lines");
                    free(args);
                    exit(EXIT_FAILURE);
                }
                l2Line_Flags = true;
                break;
            case 'd'://Case l1-latency
                if (convert_unsigned(optarg, (unsigned long *) &l1Latency, "l1-latency") != 0) {
                    free(args);
                    exit(EXIT_FAILURE);
                }
                //Check duplicate
                if (l1Latency_Flags) {
                    printDuplicateOption("--l1-latency");
                    free(args);
                    exit(EXIT_FAILURE);
                }
                l1Latency_Flags = true;
                break;
            case 'e': //Case l2-latency
                if (convert_unsigned(optarg, (unsigned long *) &l2Latency, "l2-latency") != 0) {
                    free(args);
                    exit(EXIT_FAILURE);
                }
                //Check Duplicate
                if (l2Latency_Flags) {
                    printDuplicateOption("--l2-latency");
                    free(args);
                    exit(EXIT_FAILURE);
                }
                l2Latency_Flags = true;
                break;
            case 'm': //Case memory-latency
                if (convert_unsigned(optarg, (unsigned long *) &memLatency, "memory-latency") != 0) {
                    free(args);
                    exit(EXIT_FAILURE);
                }
                //Check Duplicate
                if (memLatency_Flags) {
                    printDuplicateOption("--memory-latency");
                    free(args);
                    exit(EXIT_FAILURE);
                }
                memLatency_Flags = true;
                break;
            case 't': //Case tf
                if (tf_Flags) {
                    printDuplicateOption("--tf");
                    free(args);
                    exit(EXIT_FAILURE);
                }
//                if (checkNameTraceFile(optarg) != 0) {
//                    exit(EXIT_FAILURE);
//                }
                traceFile = optarg;
                tf_Flags = true;
                break;
            case 'h':
                print_help(progname);
                free(args);
                exit(0);
            default:
                print_usage(progname);
                free(args);
                exit(EXIT_FAILURE);
        }
        //numRequest++;
    }
    if (checkValid(l1Line, l2Line, l1Latency, l2Latency, memLatency) != 0) {
        free(args);
        exit(EXIT_FAILURE);
    }
    //Fail if missing the optional arguments (Eingabe Datei)
//    if (optind == argc) {
//        fprintf(stderr, "%s: Es fehlt die positionale Argument (Eingabe Datei)\n", progname);
//        print_usage(progname);
//        free(args);
//        exit(EXIT_FAILURE);
//    }
    //The first not optional arguments will be the name of Input file
    if (optind < argc) {
        fileInputName = argv[optind++];
        //Check invalid input file
        if (check_input_file(fileInputName) != 0) {
            exit(EXIT_FAILURE);
        }
        //inputFile_Flags = true;
        //numRequest++;
    }
    //When optind is still smaller than argc, then fail
    if (optind < argc) {
        fprintf(stderr, "Unbekannte Arguments: ");
        while (optind < argc) {
            printf("%s ", argv[optind++]);
        }
        free(args);
        exit(EXIT_FAILURE);
    }

    args->cycles = (int) cycles;
    args->cacheLineSize = lineSize;
    args->l1CacheLines = l1Line;
    args->l2CacheLines = l2Line;
    args->l1CacheLatency = l1Latency;
    args->l2CacheLatency = l2Latency;
    args->memoryLatency = memLatency;
//    if (tf_Flags && traceFile != NULL) {
//        args->tracefile = traceFile;
//    }
    if (tf_Flags) {
        if (check_trace_file(traceFile) != 0) {
            exit(EXIT_FAILURE);
        }
        args->tracefile = traceFile;
    }
    args->input_file = fileInputName;
    return args;
}
////DEBUG
//int main(int argc, char* args[]){
//    check_trace_file("//file1");
//}