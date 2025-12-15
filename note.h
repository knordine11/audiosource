#ifndef NOTE_H
#define NOTE_H
#include <iostream>
using namespace std;
extern float noteA_oct;
extern float noteA_no;

// extern float noteA_acc;
extern float note_acc;

extern float noteC_no;
extern float noteC_oct;
extern float nn_l[12];

class Note
{
public:
    Note(){
        cout<<" in header   THE CONSTRUCTER"<<endl;
        fill_note_l();
    };

    float note_l[12];
    void fill_note_l();
    // void fill_note_l()
    void f_to_n(double freq);
    void freq_to_note(float freq);

    // void harmonic(double freq_l, double freq_h);

};



#endif // NOTE_H
