// TestProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include <ctime>
//#include "../../dSFMT-src-2.2.2/dSFMT.h"
#include "../../SFMT-src-1.4.1/SFMT.h"
sfmt_t seed;
int main()
{
    sfmt_init_gen_rand(&seed, uint32_t(time(NULL)));
    //dsfmt_gv_init_gen_rand(time(NULL));
    //srand(time(NULL));
    auto begin = time(NULL);
    for(int i = 0; i < 100000000; ++i)
    {
        sfmt_genrand_uint32(&seed);
        //dsfmt_gv_genrand_uint32();
        //rand();
    }

    auto end = time(NULL);
    std::cout << (end - begin);
    std::cin >> begin;
    return 0;
}

