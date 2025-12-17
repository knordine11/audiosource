#include "note.h"
#include <iostream>
#include "note.h"
using namespace std;

// double noteA_oct;
// double noteA_no;
// double noteA_acc;
// double noteC_oct;
// double noteC_no;


float noteA_oct;
float noteA_no;
float note_acc;
float noteC_oct;
float noteC_no;
//  = 2^(X/12)
float nn_l[] = {1.000000, 1.0594630944, 1.1224620483, 1.1892071150,
                1.25992104991, 1.3348398542, 1.4142135624, 1.4983070769,
                1.5874010520, 1.6817928305, 1.7817974363, 1.8877486254};

void Note::fill_note_l(){
    float nspace = 1.0/12.0;
    for(int i=0;i<=12;i++){
        note_l[i] = 1+(nspace * i);
    }
}

void Note::freq_to_note(float freq)
{
    cout<<endl;
    float base = 27.5;
    int oct_a = 1;
    int note_a_no  = 12;
    float b_line_const = 0.9715319412;  //  2^(-1/24) BOARDER HALF WAY BETWEEN NOTES
    float botton_line = base*2 * b_line_const;
    cout<<"[ f_to_n ]      init  DEFIND   --->       botton_line       "  << botton_line <<endl;
    // float botton_line =53.854;  // (((2 * base - 1.917 *base)/2) -(2 * base)
    while(freq > botton_line){
        oct_a ++;
        botton_line = botton_line * 2;
        base = base * 2;
        // cout<<oct_a<<"  botton_line "  <<botton_line<< "    space_diff "<<space_diff<<endl;
    }
    // cout<<oct_a<<" [ oct ]     EXIT  base "<<base<<"  space_diff "<<space_diff<<endl;
    noteA_oct = oct_a;
    cout<<"  ----------------   GOT OCT A = "<<oct_a<<endl<<endl;

    while(freq < botton_line){
        cout<<" note_a_no = " << note_a_no <<"   botton_line "<<botton_line<<endl;
        note_a_no --;
        botton_line =nn_l[note_a_no] * base * b_line_const;
    }
    noteA_no = note_a_no;
    float note_pit = base * nn_l[note_a_no] ;
    cout<<"   ----------------------------------- HAVE NOTE"<<endl;
    cout<<" note_a_no = " << note_a_no<<"   botton_line "<<botton_line
         <<"          freq "<<freq<<"   note_pit "<<note_pit<<endl<<endl;
    note_acc = ((freq/note_pit) -1) /** 1000*/;
    cout<< "noteA_oct = "<<noteA_oct<<"    noteA_no = "<<noteA_no<<"       note_pit "<<note_pit<<"   noteA_acc "<<note_acc<<endl<<endl;

    cout<<" CONVERT FROM A BASE TO C BASED"<<endl;
    noteC_no = noteA_no -3;
    noteC_oct = noteA_oct;
    if(noteC_no<0){
        noteC_no = noteC_no + 12;
        noteC_oct --;}
}

// void Note::fill_note_l(){
//     float nspace = 1.0/12.0;
//     for(int i=0;i<=12;i++){
//         note_l[i] = 1+(nspace * i);
//         cout<< i<<"  "<< note_l[i]<<endl;
//     }
// }

// float nn_l[] = {1.000000, 1.059463, 1.122462, 1.189207, 1.259921, 1.259921,
//               1.414214, 1.498307, 1.587401, 1.681793, 1.781797, 1.887749};

// // float nn_dl[] = {0.9715319412,1.0293022366,1.0905077327,1.1553526969,1.2240535433,1.2968395547
// //                   ,1.3739536475,1.4556531828,1.5422108254,1.6339154532,1.6339154532,1.7310731220,1.8340080864,1.9430638823};
// float nn_dl[] = {0.97153194,1.02930223,1.09050773,1.15535269,1.22405354,1.29683955,1.37395364
//                  ,1.45565318,1.54221082,1.63391545,1.63391545,1.73107312,1.83400808,1.94306388};



// void Note::harmonic(double freq_l, double freq_h)
// {
//     double raito = freq_h/freq_l;

// }




void Note::f_to_n(double freq)
{
    cout<<endl;
    double base = 27.5;
    int oct_a = 1;
    int note_a_no  = 12;
    double note_space = 1.0/12.0;
    double space_diff = note_space * base ;
    double botton_line = base*2 - space_diff/2;  // (((2 * base - 1.917 *base)/2) -(2 * base)

    // cout<<"[ f_to_n ]      init  DEFIND   --->   space_diff  "<<space_diff<<" botton_line "<< botton_line <<endl;
    // float botton_line =53.854;  // (((2 * base - 1.917 *base)/2) -(2 * base)
    while(freq > botton_line){
        oct_a ++;
        botton_line = botton_line * 2;
        base = base * 2;
        space_diff = space_diff *2;
        cout<<oct_a<<"  botton_line "<<botton_line<< "space_diff "<<space_diff<<endl;
    }
    cout<<oct_a<<" [ oct ]     EXIT  base "<<base<<"  space_diff "<<space_diff<<endl;
    noteA_oct = oct_a;
    cout<<"  ----------------   GOT OCT A = "<<oct_a<<endl<<endl;

    cout<<" START OF NOTE NO     space_diff "<<space_diff<<endl<<endl;
    while(freq < botton_line){
        cout<<" note_a_no = " << note_a_no <<"   botton_line "<<botton_line<<endl;

        note_a_no --;
        if(note_a_no == 0){break;};
        botton_line =note_l[note_a_no] * base - space_diff/2;
        }

    // while(freq < botton_line){
    //     cout<<" note_a_no = " << note_a_no <<"   botton_line "<<botton_line<<endl;
    //     note_a_no --;
    //     if(note_a_no == 0){break;};
    //     botton_line =note_l[note_a_no] * base - space_diff/2;
    //     }

    // botton_line =note_l[note_a_no] * base - space_diff/2;
        // }
    noteA_no = note_a_no;
    // float note_pit = base * note_l[note_a_no] ;
    float note_pit = base * nn_l[note_a_no] ;
    cout<<"   ----------------------------------- HAVE NOTE"<<endl;
    cout<<" note_a_no = " << note_a_no<<"   botton_line "<<botton_line
         <<"   freq "<<freq<<"   note_pit "<<note_pit<<endl<<endl;
    note_acc = ((freq/note_pit) -1) /** 1000*/;
    cout<< "noteA_oct "<<noteA_oct<<"    noteA_no "<<noteA_no<<"   note_pit "<<note_pit<<"   noteA_acc "<<note_acc<<endl<<endl;

    cout<<" CONVERT FROM A BASE TO C BASED"<<endl;
    noteC_no = noteA_no -3;
    noteC_oct = noteA_oct;
    if(noteC_no<0){
        noteC_no = noteC_no + 12;
        noteC_oct --;}
}





