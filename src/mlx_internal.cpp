/*
 * ---------------------------------------------------
 * mlx_internal.cpp
 *
 * Author: Thomas Choquet <thomas.publique@icloud.com>
 * Date: 2024/06/08 15:00:50
 * ---------------------------------------------------
 */

#include "mlx_internal.hpp"

namespace mlx
{

int convertKeyCode(int code)
{
    switch (code)
    {
        case  0: return 53;
        case 32: return 49;
        case 49: return 18;
        case 50: return 19;
        case 51: return 20;
        case 52: return 21;
        case 53: return 23;
        case  7: return 123;
        case  6: return 124;
        case  9: return 126;
        case  8: return 125;
        case 82: return 15;
        default: return code;
    }
}
int convertMouseCode(int code)
{
    switch (code)
    {
        case 0:  return 1;
        case 1:  return 2;
        default: return code;
    }
}

}