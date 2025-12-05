#include "note.h"
#include <iostream>
#include "note.h"
using namespace std;

double noteA_oct;
double noteA_no;
double noteA_acc;
double noteC_oct;
double noteC_no;



void Note::fill_note_l(){
    float nspace = 1.0/12.0;
    for(int i=0;i<=12;i++){
        note_l[i] = 1+(nspace * i);
        // cout<< i<<"  "<< note_l[i]<<endl;
    }
}

void Note::f_to_n(double freq)
{
    cout<<endl;
    double base = 27.5;
    int oct_a = 1;
    int note_a_no  = 12;
    double note_space = 1.0/12.0;
    double space_diff = note_space * base ;
    double botton_line = base*2 - space_diff/2;  // (((2 * base - 1.917 *base)/2) -(2 * base)
    cout<<"[ f_to_n ]      init  DEFIND   --->   space_diff  "<<space_diff<<" botton_line "<< botton_line <<endl;
    // float botton_line =53.854;  // (((2 * base - 1.917 *base)/2) -(2 * base)
    while(freq > botton_line){
        oct_a ++;
        botton_line = botton_line * 2;
        base = base * 2;
        space_diff = space_diff *2;
        // cout<<oct_a<<"  botton_line "<<botton_line<< "space_diff "<<space_diff<<endl;
    }
    cout<<oct_a<<" [ oct ]     EXIT  base "<<base<<"  space_diff "<<space_diff<<endl;
    noteA_oct = oct_a;
    cout<<"  ----------------   GOT OCT A = "<<oct_a<<endl<<endl;

    // cout<<" START OF NOTE NO     space_diff "<<space_diff<<endl<<endl;
    while(freq < botton_line){
        cout<<" note_a_no = " << note_a_no <<"   botton_line "<<botton_line<<endl;
        note_a_no --;
        if(note_a_no == 0){break;};
        botton_line =note_l[note_a_no] * base - space_diff/2;
        }
    noteA_no = note_a_no;
    float note_pit = base * note_l[note_a_no] ;
    cout<<"   ----------------------------------- HAVE NOTE"<<endl;
    cout<<" note_a_no = " << note_a_no<<"   botton_line "<<botton_line
         <<"   freq "<<freq<<"   note_pit "<<note_pit<<endl<<endl;
    noteA_acc = ((freq/note_pit) -1) /** 1000*/;
    cout<< "noteA_oct "<<noteA_oct<<"    noteA_no "<<noteA_no<<"   note_pit "<<note_pit<<"   noteA_acc "<<noteA_acc<<endl<<endl;

    cout<<" CONVERT FROM A BASE TO C BASED"<<endl;
    noteC_no = noteA_no -3;
    noteC_oct = noteA_oct;
    if(noteC_no<0){
        noteC_no = noteC_no + 12;
        noteC_oct --;}
}





