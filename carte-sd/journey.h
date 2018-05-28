#ifndef JOURNEY_H
#define JOURNEY_H

//extern value
extern float flat, flon;
extern float total_distance;
extern unsigned long age;
extern unsigned long time, date;
extern unsigned long startTime, JourneyTime;

// start recording new journey
void newJourney();
// end recording current journey
void endJourney();

#endif