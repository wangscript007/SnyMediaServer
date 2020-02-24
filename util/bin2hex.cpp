#include <iostream>
#include <sstream>
 
using std::cout;
using std::endl;
 
std::string bin2hex(const std::string& s);
 
int main(int argc, char *argv[])
{
    cout << bin2hex("ALPHA") << endl;//414c504841
    return 0;
}
 
std::string bin2hex(const std::string& s)
{
    const static char bin2hex_lookup[] = "0123456789abcdef";
    unsigned int t=0,i=0,leng=s.length();
    std::stringstream r;
    for(i=0; i<leng; i++)
    {
        r<< bin2hex_lookup[ s[i] >> 4 ];
        r<< bin2hex_lookup[ s[i] & 0x0f ];
    }
    return r.str();
}