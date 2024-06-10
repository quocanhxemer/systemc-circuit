#ifndef CSV_READER
#define CSV_READER

#include <iostream>
#include <fstream>

#include <string>

#include <functional>

#include <vector>

#include "Request.hpp"

//I would gladly accept every better naming suggestion
class csv_reader
{
private:
    std::ifstream fd;

    std::vector<struct Request> container;

    /**
     * @brief parses a string from csv file
     * @returns a structure with parsed data
     */
    struct Request parse(std::string);
public:
    /**
     * @brief a basic empty constructor
     */
    csv_reader();
    /**
     * @brief Constructor that opens a specified file
     * @param path to a csv file
     * @throws File not found exception
     * @throws File is not csv 
     * @throws File is in wrong format
     */
    csv_reader(std::string);

    /**
     * @brief Opens a new csv file and drops data if there was any
     * Optimized for sequense of calls
     * @param path to a csv file
     */
    csv_reader* open(std::string);
private:
    /**
     * @brief Pretty powerful functions that let's you run some code before and after line parsing
     * @param string_callback is a function that processes a line before parsing it
     * @param request_callback is a function that processes a line before parsing it
     */
    std::vector<struct Request> consume(
        std::function<std::string(std::string)> = 
                            [](std::string line){return line;},
        std::function<struct Request(struct Request)> = 
                            [](struct Request data){return data;}
    ){}

public:
    /**
     * @brief stupidly parses a given csv file
     */
    csv_reader* consume();

    /*casters*/
    /**
     * @brief casts the reader to a return type
     * @return std::vector<struct Request> parsed data
     */
    operator std::vector<struct Request>();
    /**
     * @brief casts reader to the array of requests
     * @return struct Request* 
     */
    operator struct Request*();


    bool isHoldingData();

    /*terminators*/
    void drop();
    void close();
    void flush();

    ~csv_reader();
};

#endif // !CSV_READER