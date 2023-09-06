#include <iostream>
#include <limits.h>
#include <vector>
#include <iomanip>
#include <chrono>

std::string tostring(__int128 num) {
    std::string str;
    do {
        int digit = num % 10;
        str = std::to_string(digit) + str;
        num = (num - digit) / 10;
    } while (num != 0);
    return str;
}

int integer_arithmetic()
{
    //signed variables
    int64_t x, y;
    // Addition of two positive integers
    x = LONG_MAX;
    y = LONG_MAX;

    // result -2, overflow occured
    std::cout << x + y << std::endl;

    // Addition of two negative integers
    x = LONG_MIN;
    y = LONG_MIN;

    // result 0, overflow occured
    std::cout << x + y << std::endl;    

    //unsigned variables
    uint64_t u_x, u_y;
    // Addition of two positive integers
    u_x = ULONG_MAX;
    u_y = ULONG_MAX;

    // result 18446744073709551614, overflow occured
    std::cout << u_x + u_y << std::endl;
    
    // Addition of two negative integers
    u_x = 0;
    u_y = 0;

    // result 0, no overflow
    std::cout << u_x + u_y << std::endl;        

    // Multiplication of two positive integers
    u_x = ULONG_MAX;
    u_y = ULONG_MAX;

    // result 0, no overflow
    std::cout << u_x * u_y << std::endl;      
}

int integer_arithmetic_safe()
{
    //signed variables
    int64_t x, y, result;

    // Addition of two positive integers
    x = LONG_MAX;
    y = LONG_MAX;

    if (__builtin_add_overflow(x, y, &result))
        std::cout << "Overflow occured Addition of two positive integers x+y" << std::endl;
    else
        // result -2, overflow occured
        std::cout << x + y << std::endl;

    // Addition of two negative integers
    x = LONG_MIN;
    y = LONG_MIN;   

    if (__builtin_add_overflow(x, y, &result))
        std::cout << "Overflow occured Addition of two negative integers x+y" << std::endl;
    else
        // result -2, overflow occured
        std::cout << x + y << std::endl;

    //unsigned variables
    uint64_t u_x, u_y;
    // Addition of two positive integers
    u_x = ULONG_MAX;
    u_y = ULONG_MAX;

    if (__builtin_add_overflow(u_x, u_y, &result))
        std::cout << "Overflow occured Addition of two positive integers x+y" << std::endl;
    else
        // result 18446744073709551614, overflow occured
        std::cout << u_x + u_y << std::endl;              

    // Multiplication of two positive integers
    u_x = ULONG_MAX;
    u_y = ULONG_MAX;

    if (__builtin_mul_overflow(u_x, u_y, &result))
        std::cout << "Overflow occured Multiplication of two positive integers x+y" << std::endl;
    else
        std::cout << u_x * u_y << std::endl;             
}

/*
integer_performance_test overflow: 37937
integer_performance_test: 33359
*/
int integer_performance_test()
{
    srand(time(0));

    std::vector<uint64_t> number_arrays;

    for(int i = 0; i < 10000000; i++)
        number_arrays.push_back( rand() );
    uint64_t result = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < number_arrays.size()-1; ++i) {
        uint64_t tmp_result = 0;
        bool overflow = __builtin_add_overflow(number_arrays[i], number_arrays[i+1], &tmp_result);

        if( !overflow )
        {
            result += tmp_result;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "integer_performance_test overflow: " << duration.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < number_arrays.size()-1; ++i) {
        uint64_t tmp_result = (number_arrays[i] + number_arrays[i+1]);
        result += tmp_result;
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "integer_performance_test: " << duration.count() << std::endl;    

}

int integer128_arithmetic_safe()
{
    //signed variables
    __int128_t x, y, result;

    // Addition of two positive integers
    x = 0x7FFFFFFFFFFFFFFF * 0x7FFFFFFFFFFFFFFF;
    y = 0x7FFFFFFFFFFFFFFF * 0x7FFFFFFFFFFFFFFF;
    
    if (__builtin_add_overflow(x, y, &result))
    {
        std::cout << "1 Overflow occured Addition of two positive integers x+y" << std::endl;            
    }

    // Addition of two negative integers
    x = LONG_MIN;
    y = LONG_MIN;   

    if (__builtin_add_overflow(x, y, &result))
        std::cout << "2 Overflow occured Addition of two negative integers x+y" << std::endl;


    //unsigned variables
    __uint128_t u_x, u_y, u_result, u_result_2;
    // Addition of two positive integers
    u_x = ULONG_MAX;
    u_y = ULONG_MAX;
    u_y += 2;

    // 18446744073709551615 + 18446744073709551617 = 36893488147419103232 (0x2000000000000000)
    if (__builtin_add_overflow(u_x, u_y, &u_result))
        std::cout << "3 Overflow occured Addition of two positive integers u_x+u_y" << std::endl;

    std::cout << tostring(u_x) << std::endl; 
    std::cout << tostring(u_y) << std::endl; 
    std::cout << tostring(u_result) << std::endl;               

    // Addition of two positive integers
    u_x = ULONG_MAX;
    u_y = ULONG_MAX;
    u_x *= 10000000000000000000;
    u_y *= 10000000000000000000;

    // 184467440737095516150000000000000000000 + 184467440737095516150000000000000000000 = 368934881474191032300000000000000 (0x1A736F1D098086000000000000000000)
    if (__builtin_add_overflow(u_x, u_y, &u_result))
        std::cout << "4 Overflow occured Addition of two positive integers u_x+u_y" << std::endl;           
    
    std::cout << tostring(u_result) << std::endl; 

    // Multiplication of two positive integers
    u_x = ULONG_MAX;
    u_y = ULONG_MAX;
    u_x = u_x * 1;

    if (__builtin_mul_overflow(u_x, u_y, &u_result))
        std::cout << "5 Overflow occured Multiplication of two positive integers u_x*u_y" << std::endl;

    // Multiplication of two positive integers
    u_x = ULONG_MAX;
    u_y = ULONG_MAX;
    u_x = u_x * 10;

    if (__builtin_mul_overflow(u_x, u_y, &u_result))
        std::cout << "6 Overflow occured Multiplication of two positive integers u_x*u_y" << std::endl;        
    
}

//integer128_performance_test overflow: 57580
//integer128_performance_test: 45757
int integer128_performance_test()
{
    srand(time(0));

    std::vector<__int128_t> number_arrays;

    for(int i = 0; i < 10000000; i++)
        number_arrays.push_back( rand() );
    __int128_t result = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < number_arrays.size()-1; ++i) {
        __int128_t tmp_result = 0;
        bool overflow = __builtin_add_overflow(number_arrays[i], number_arrays[i+1], &tmp_result);

        if( !overflow )
        {
            result += tmp_result;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "integer128_performance_test overflow: " << duration.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < number_arrays.size()-1; ++i) {
        __int128_t tmp_result = (number_arrays[i] + number_arrays[i+1]);
        result += tmp_result;
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "integer128_performance_test: " << duration.count() << std::endl;    

}

void double_arithmetic()
{
    double x = 1554.56;
    double y = 1532.92;
    double cur_conversion = 0.16;
    x = x * cur_conversion;
    y = y * cur_conversion;
    
    double price = 0.00000783;
    double min_prof = 0.01;
    double spread = 0.02;
    double comm_multiplier = 1 + 0.0002;    

    double min_profit_bid_multiplier = 0.9999;

    std::cout << std::fixed << std::setprecision(20);
    std::cout << "x-y: " << x - y << std::endl;
    std::cout << std::numeric_limits<double>::epsilon() << std::endl;
    std::cout << "price: "<< price << std::endl;
    std::cout << "min_profit_bid_multiplier: "<< min_profit_bid_multiplier << std::endl;
    std::cout << "comm_multiplier: "<< comm_multiplier << std::endl;
    std::cout << "spread: "<< spread << std::endl;

    double bid_w_both_comm = price*comm_multiplier;

    std::cout << "bid_w_both_comm: "<< bid_w_both_comm << std::endl;

    double w_min_profit = bid_w_both_comm*min_profit_bid_multiplier;

    std::cout << "w_min_profit: " << w_min_profit << std::endl;

    double spread_coefficint = (spread / 100.0);
    double profit_buffer = spread_coefficint * w_min_profit;

    std::cout << "spread_coefficint: " << spread_coefficint << std::endl;
    std::cout << "profit_buffer: " << profit_buffer << std::endl;
    //0.000000001566157

    double spread_pow = 1.0;
    for(int i = 0; i < 10; i++ )
    {
        spread_pow = spread_pow * spread;
        std::cout << "spread_pow: " << spread_pow << std::endl;
    }

    double spread_sqr = spread*spread*2;
    std::cout << "spread_sqr: " << spread_sqr << std::endl;

    std::cout << "spread_sqr*10: " << spread_sqr*10 << std::endl;

    double spread_sqr_1000_result = 0.8;
    std::cout << "spread_sqr*100: " << spread_sqr*100 << std::endl;
    std::cout << "spread_sqr*1000: " << spread_sqr*1000 << std::endl;
    std::cout << "spread_sqr_1000_result: " << spread_sqr_1000_result << std::endl;
    std::cout << "spread_sqr*10000: " << spread_sqr*10000 << std::endl;
    std::cout << "spread_sqr*100000: " << spread_sqr*100000 << std::endl;
    std::cout << "spread_sqr*1000000: " << spread_sqr*1000000 << std::endl;

}

int main()
{
    integer_arithmetic();

    integer_arithmetic_safe();

    integer_performance_test();

    integer128_arithmetic_safe();

    integer128_performance_test();

    //double_arithmetic();
}