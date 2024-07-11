#include <csv_reader.h>

void fail_with(
	const char *msg,
	const char *line,
	FILE *file,
	char *buf,
	struct Request *requests)
{
	fprintf(stderr, msg, line);
	fclose(file);
	free(buf);
	free(requests);
	exit(EXIT_FAILURE);
}

struct csv_file_data csv_parse_file(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		fprintf(stderr, "No such file\n");
		exit(EXIT_FAILURE);
	}
	struct stat st;
	if (fstat(fileno(file), &st) == -1)
	{
		perror("Error getting file status\n");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	if (!S_ISREG(st.st_mode) || st.st_size <= 0)
	{
		fprintf(stderr, "File is not regular or empty\n");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	char *buf = malloc(st.st_size + 1);
	if (buf == NULL)
	{
		perror("Error allocating memory");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	if (fread(buf, 1, st.st_size, file) != st.st_size)
	{
		perror("Error reading file");
		free(buf);
		fclose(file);
		exit(EXIT_FAILURE);
	}
	buf[st.st_size] = '\0'; // Null-terminate the buffer
	// Count lines
	size_t lines = 1;
	for (char *p = buf; *p; p++)
		if (*p == '\n')
			lines++;

	struct Request *requests = malloc(sizeof(struct Request) * lines);
	if (requests == NULL)
	{
		fprintf(stderr, "Error allocating memory for requests\n");
		free(buf);
		fclose(file);
		exit(EXIT_FAILURE);
	}

	// Parse buffer into requests
	char *line = strtok(buf, "\n");
	for (size_t index = 0; line != NULL && index < lines; index++, line = strtok(NULL, "\n"))
	{
		if (line == "") {
			fprintf(stderr, "Skipping empty line # %ld\n", index);
            continue;
		}
		char access_type;
		char addr_data_str[40] = "";

		// Parse line format: "W/R addr data(optional)"
		if (sscanf(line, "%c %39[^\n]", &access_type, addr_data_str) != 2)
			fail_with("Error parsing line: %s\n", line, file, buf, requests);

		char addr_str[20] = "", data_str[20] = "";
		uint32_t addr = 0, data = 0;
		switch (access_type)
		{
		case 'R':
		case 'r':
			requests[index].we = 0;
			if (sscanf(addr_data_str, "%19s", addr_str) != 1)
				fail_with("Read request should contain only address, but found %s\n",
						  line, file, buf, requests);
			if (sscanf(addr_str, "%x", &addr) != 1 && sscanf(addr_str, "%u", &addr) != 1)
				fail_with("Invalid address on line %s\n", line, file, buf, requests);
			requests[index].addr = addr;
			requests[index].data = 0;
			break;
		case 'W':
		case 'w':
			requests[index].we = 1;
			if (sscanf(addr_data_str, "%19s %19s", addr_str, data_str) != 2)
				fail_with("Write request should contain address and data, but found %s\n",
						  line, file, buf, requests);
			if (sscanf(addr_str, "%x", &addr) != 1 && sscanf(addr_str, "%u", &addr) != 1)
				fail_with("Invalid address on line %s\n", line, file, buf, requests);
			if (sscanf(data_str, "%x", &data) != 1 && sscanf(data_str, "%u", &data) != 1)
				fail_with("Invalid data on line %s\n", line, file, buf, requests);
			requests[index].addr = addr;
			requests[index].data = data;
			break;
		default:
			fail_with("Invalid access type: %s\n",
					  line, file, buf, requests);
			break;
		}
	}

	free(buf);
	fclose(file);
	return (struct csv_file_data){lines, requests};
}