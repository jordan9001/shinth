#ifndef PITCHTABLE_H
#define PITCHTABLE_H

// NOTES
// pitch shifting
// so when we want to play an a, then a c
// we need to do the math
// A4 is 440
// We want to go to C4
// so we do a4 * (2^(1/12))^(-9) because it is -9 semisteps away
// we do not want to do this kind of math real time
// so we make a table for (2^(1/12))^(x)

#define An12 0.5
#define An11 0.5297315471796476
#define An10 0.5612310241546865
#define An9 0.5946035575013605
#define An8 0.6299605249474366
#define An7 0.6674199270850172
#define An6 0.7071067811865475
#define An5 0.7491535384383407
#define An4 0.7937005259840997
#define An3 0.8408964152537145
#define An2 0.8908987181403393
#define An1 0.9438743126816935
#define A0 1.0
#define A1 1.059463094359295
#define A2 1.122462048309373
#define A3 1.189207115002721
#define A4 1.259921049894873
#define A5 1.334839854170034
#define A6 1.414213562373095
#define A7 1.498307076876681
#define A8 1.587401051968199
#define A9 1.681792830507429
#define A10 1.781797436280679
#define A11 1.887748625363387
#define A12 2.0

float a_table[] = {An12, An11, An10, An9, An8, An7, An6, An5, An4, An3, An2, An1, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12};
float* a_speed = &a_table[12]; // A0, use this +- up to 12

#endif
