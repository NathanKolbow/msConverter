#ifndef SIMSUITE_CORE_MSEVENTS_HPP_
#define SIMSUITE_CORE_MSEVENTS_HPP_

#include <iostream>
#include <string>

enum MSEventType { split, join };

class MSEvent {
    public:
        virtual MSEventType getEventType(void) = 0;
        
        double getTime(void) { return time; }
        void setTime(double t) { time = t; }
        
    protected:
        double time = -1;
};

class MSJoinEvent:public MSEvent {
    public:
        MSJoinEvent(double t, int min, int maj) {
            time = t;
            majorTaxa = maj;
            minorTaxa = min;
        }
        MSJoinEvent(double t, std::string min, std::string maj) {
            time = t;
            majorTaxa = stoi(maj);
            minorTaxa = stoi(min);
        }
        MSJoinEvent(double t, int min, std::string maj) {
            time = t;
            majorTaxa = stoi(maj);
            minorTaxa = min;
        }

        int getMajorTaxa(void) { return majorTaxa; }
        void setMajorTaxa(int m) { majorTaxa = m; }

        int getMinorTaxa(void) { return minorTaxa; }
        void setMinorTaxa(int m) { minorTaxa = m; }

        MSEventType getEventType(void) { return join; }

        std::string toString(void) { return "-ej " + std::to_string(time) + " " + std::to_string(minorTaxa) + " " + std::to_string(majorTaxa); }
        void print(void) { std::cout << toString() << std::endl; }

    protected:
        int majorTaxa;
        int minorTaxa;
};

class MSSplitEvent:public MSEvent {
    public:
        MSSplitEvent(double t, int tax, double gam) {
            time = t;
            taxa = tax;
            gamma = gam;
        }
        MSSplitEvent(double t, std::string tax, double gam) {
            time = t;
            taxa = stoi(tax);
            gamma = gam;
        }

        int getTaxa(void) { return taxa; }
        void setTaxa(int t) { taxa = t; }

        double getGamma(void) { return gamma; }
        void setGamma(double g) { gamma = g; }

        MSEventType getEventType(void) { return split; }

        std::string toString(void) { return "-es " + std::to_string(time) + " " + std::to_string(taxa) + " " + std::to_string(gamma); }
        void print(void) { std::cout << toString() << std::endl; }

    protected:
        int taxa;
        double gamma;
};

#endif