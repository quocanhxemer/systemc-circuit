/**
 * @file debugKit.hpp
 * @author Ivan Logvynenko
 * @brief Macro for outputting logs from your code
 * @version 0.2
 * @date 2023-12-20
 * 
 * Quick guide:
 *      If you want to use it just make certain definitions for your debug version
 *      If you would want to disable logs completely switch to release version
 * 
 * @copyright Copyright (c) 2024
 */

#ifndef EXECUTABLE_NAME
    #define EXECUTABLE_NAME "UNNAMED"
#endif // !EXECUTABLE_NAME

#ifndef LOGGING
    #define ILOG(x) std::cout << " [ IMPORTANT ] [ " << EXECUTABLE_NAME << " ] : " << x << "\n"; 
#else
    #define ILOG(x)
#endif // DEBUG 

#ifndef FULL_LOGGING
    #define  LOG(x) std::cout << " [   INFO    ] [ " << EXECUTABLE_NAME << " ] : " << x << "\n"; 
#else
    #define LOG(x)
#endif // FULL_LOGGING

#ifndef ERROR
    #define  EL(x) std::cerr << "  [   ERROR   ] [ " << EXECUTABLE_NAME << " ] : " << x << "\n"; 
#else
    #define EL(x)
#endif // ERROR