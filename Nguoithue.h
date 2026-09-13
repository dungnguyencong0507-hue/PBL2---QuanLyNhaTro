#include <string.h>
#include <iostream>
#include "Phong.h"

class Nguoithue
{
    private:
        std::string id;
        std::string name;
        int gender;
        std::string phone;
        std::string IDcard;
        std::string hometown;
        std::string roomID;
        bool tamTru;

    public:
        Nguoithue();
        ~Nguoithue();

        void Hire(Phong&);
        void Unhire(Phong&);
        void breakResponse();
};