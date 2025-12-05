#include "fftstuff.h"
#include <iostream>
#include "fftw3/fftw3.h"
#include <cmath>
#include <iomanip>

#include "fftstuff.h"
#include <iostream>
#include "fftw3/fftw3.h"
#include "qpoint.h"
#include <cmath>
#include <iomanip>
#include "note.h"
// #include <QLineSeries>

using namespace std;
// added by note class
// double noteA_oct;
// double noteA_no;
// double noteA_acc;
// double noteC_no;
// double noteC_oct;







double temp_freq = 0;   // temp used for testing only
double rec_arr[1000000];
int rec_arr_cnt = 0;
int frame_start = 0;
int frame_size = 2048;
int frame_end = frame_start + frame_size;
double binA1;
double binA2;
double binA3;
double binA4;
double binA5;
int binNo1;
int binNo2;
int binNo3;
double freq1;
double freq2;
double freq3;
double bin_amp[3];
int bin_no[3];
double Fs = 44100;
const int N = 16384*4; // 2^14 * 4 =  2^16 = 65,536
double bin_size = Fs/N;

fftw_complex* in, * out;

double last_peak = 0;
double peak_val = 0;
double peak_f_bin = 0;
double peak_diff = 0;
double lev1 = 0;
double lev2 = 0;
double lev3 = 0;
double TlevL = 0;
double TlevH = 0;
double levA = 0;
double move_by = 0;
double freq_got =0;
bool Flag_up = true;

FftStuff::FftStuff(QObject *parent)
    : QObject{parent} { }

// added begin ----------------------------
// void FftStuff::next_frame()

void FftStuff:: make_sin(double freq ,int beg, int leng){
    double Pi = 3.14159265358979323846;
    int i = beg;
    int en = leng+beg;
    temp_freq = freq;
    cout << endl << "--------( " << freq << " Hz )-- " << beg <<" { make_sin  } " << en << endl;
    while (i < en) {
        rec_arr[i] = sin(2 * Pi * freq * i /44100 ) ;
        // rec_arr[i] = i*2 ;
        // cout << i <<" ~ "<< rec_arr[i]<<endl;
        i=i+1;
    }
}

void FftStuff::look_rec_arr(int beg, int lengh)
{
    int end = beg + lengh;
    cout << endl << "-----FftStuff-----> " << beg  << " { look_rec_arr < "<<lengh<<" > } " << end << endl;
    int i = beg;
    while (i < end) {
        cout << i << " >  " << rec_arr[i]<<endl;
        i++;
    }
    cout <<"             END IN INPUT     START OF OUTPUT"<<endl;
}

// //![1]   --------------------------------------------------
// auto series = new QLineSeries;
// for (int i = 0; i < 500; i++) {
//     QPointF p((qreal) i, qSin(M_PI / 50 * i) * 100);

//     // p.ry() += QRandomGenerator::global()->bounded(20);
//     *series << p;
// }
// //![1]   -----------------------------------------------------

void FftStuff::graph_rec_arr(int beg, int lengh)
{
    int end = beg + lengh;
    cout << endl << "------------->    " << beg  << " { GRAPH_rec_arr  } " << end << endl;
    int i = beg;
    while (i < end) {
        // cout << i << " >  " << rec_arr[i]<<endl;
        QPointF p((qreal) i, rec_arr[i]);
        i++;
        // *series << p;
    }
}
// added end ----------------------------

// ================         THE MAIN MODUAL STARTS         ============================

void FftStuff::DoIt(int beg, int lengh)
{
    int at = beg;
    int end_at = beg+lengh;
    fftw_plan p;
    fftw_free(in);
    fftw_free(out);

    in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    for (int i = 0; i < N; i++)
    {
        if(i<end_at){
            in[i][0] = rec_arr[at];
            at++;
        }else {
            in[i][0] = 0.0;}
        in[i][1] = 0.0;
    }
    fftw_execute(p); /* repeat as needed */
    std::cout << std::fixed;
    std::cout << std::setprecision(5);

// =======      PROSSES FFT OUTPUT       FIND THE HIGHEST BIN FROM FFT OUTPUT

    clear_highest_peaks_arr();  //zero highest bins and prosses fft output
    for (size_t i = 0; i < N; i++)
    {
        if(i<6000){
            // double cur_bin = i * Fs/N;
            // std::cout<<i<<"  " << cur_bin << " Hz : " << abs_c(out[i]) << std::endl;
        }
    }
    // PROSSES FFT OUT PUT   AND GET TRUE PEAKS
    cout << endl << "  got here    PROSSES FFT OUT PUT   AND GET TRUE PEAKS "<<endl<<endl;
    double last_lev = 0;
    int last_peak = 0;
    double last_peak_val =0;
    bool up = true;
    bool peak_up = true;
    for(int i = 20; i<6000; i++){
        double val_out = abs_c(out[i]);
        cout << i <<" ll "<< last_lev<<" vo "<<val_out <<"  up "<<up<<"  pup  "<<peak_up<<endl;
        if(up)
        {
            if(last_lev > val_out)          // peak found @ i-1
                {
                int peak = i-1;
                cout<<"                    FOUND PEAK AT  BIN  "<< peak <<"  :  " << last_lev << endl;
                if(peak_up && last_peak_val>last_lev){
                    cout <<endl<< "                 ----------    TRUE PEAK AT  { BIN "<< last_peak
                         << " }  level "<< last_peak_val<<endl;
                    save_highest_bin_peaks(last_peak,last_peak_val);
                    bin_to_freq(last_peak);
                    }
                if(last_peak_val > last_lev){
                       peak_up = false;
                }else{ peak_up = true;}
                last_peak_val = last_lev;
                last_peak = peak;
                up = false;
                }
        }
    cout <<"** ";
    if(val_out>last_lev){up=true;}
    last_lev = val_out;
    }

// HAVE THE THREE HIGHEST PEAKS     now find the fundenatal frequency
    // fftw_destroy_plan(p); // SAVED FOR DONE NOW EXIT
// ===================     GET FUNDEMENTAL FREQUENCY   ========================
    double fun_freq = FftStuff::get_fund_freq();

    cout << " >>>>>>>>>>    RETURNED   FUND FREQUENCY IS = "<< fun_freq<<endl;

    Note note;
    // note.fill_note_l();

    note.f_to_n(fun_freq);

    cout <<"  ( A base) --> [ "<<noteA_oct<<" "<<noteA_no   << " ]           ( C base) -->  [ "
         <<noteC_oct<<" "<<noteC_no<< " ]     acc = "<<noteA_acc<< endl<< endl;
    cout<<endl<< "    ----- AT END OF   DOIT --------" <<endl<<endl;
}
// -------------------   end of main   modual   -------------------------------------------





double FftStuff::abs_c(fftw_complex c)
{
    return std::sqrt(c[0] * c[0] + c[1] * c[1]);
}

void FftStuff::save_highest_bin_peaks(int bin, double bin_amp)
{
    cout<< "FROM save_highest_bin_peaks         [ "<< binNo1<<" ]  "<<binA1
            <<"     [ "<< binNo2<<" ]  "<<binA2
            <<"     [ "<< binNo3<<" ]  "<<binA3<<"     LAST levals   "<<binA4<<"   "<<binA5 <<endl;

    if(bin_amp > binA1){
        binA5 = binA4;  binA4 = binA3;   binA3 = binA2;   binA2 = binA1;   binA1 = bin_amp;
        binNo3 = binNo2; binNo2= binNo1;  binNo1= bin; return;}
    if(bin_amp > binA2){
        binA5 = binA4;  binA4 = binA3;   binA3 = binA2;   binA2 = bin_amp;
        binNo3 = binNo2; binNo2= bin;   return;}
    if(bin_amp > binA3){
        binA5 = binA4;  binA4 = binA3;  binA3 = bin_amp;
        binNo3= bin;  return;}
    if(bin_amp > binA4) {binA5 = binA4; binA4 = bin_amp; return;}
    if(bin_amp > binA5) {binA5 = bin_amp;return;}
}

void FftStuff::clear_highest_peaks_arr()
{
    binA1 = 0;  binA2 = 0;  binA3 = 0; binA4 = 0; binA5 = 0;
    binNo1= 0;  binNo2= 0;  binNo3= 0;
}

double FftStuff::bin_to_freq(int bin){
    double bin_freq = bin*bin_size;
    double lev_l = abs_c(out[bin-1]);
    double lev_ = abs_c(out[bin]);
    double lev_h = abs_c(out[bin+1]);

    double bin_l = bin-1;
    double bin_ = bin;
    double bin_h = bin+1;

    double bin_la = bin_l/bin_;
    double bin_ha = bin_h/bin_;

    double bin_lb = 1-bin_la;
    double bin_hb = 1-bin_ha;

    double bin_lc = bin_lb/2;
    double bin_hc = bin_hb/2;

    double bin_ld = bin_lc+1;
    double bin_hd = bin_hc+1;

    double lev_la = lev_l * bin_ld;
    double lev_ha = lev_h * bin_hd;

    double lev_lt = lev_-lev_l;
    double lev_ht = lev_-lev_h;

    cout << bin-1 << " bin_la " << bin_la << "  bin_lb " <<bin_lb<< "  bin_lc " <<bin_lc
         << "  bin_ld " <<bin_ld
         << "    lev_l "<<lev_l << "  | lev_la " << lev_la
         << "  |  lev_lt " <<lev_lt         <<endl;
    cout << bin+1 << " bin_ha " << bin_ha << "  bin_hb " <<bin_hb<< "  bin_hc " <<bin_hc
         << "  bin_hd " <<bin_hd
         << "    lev_h "<<lev_h << "  | lev_ha " << lev_ha
         << "   | lev_ht " <<lev_ht         <<endl;

    double diff = lev_lt - lev_ht;
    double r = 0;   // used so r could be used in (if else) code

    if(lev_lt < lev_ht){
        double diff = lev_lt - lev_ht;
        r = diff / lev_ht;
    }else{
        r = diff / lev_lt;
    }
    double rh = r/2;
    double move = rh * bin_size;
    double freq = bin_freq + move;
    double p_diff = ((freq/temp_freq)-1)* 100;
    cout << "  diff = "<< diff <<"   r = "<< r << "   r/2 "<<rh <<
        "   move " << move<<endl<<
        "   freq in = "<<temp_freq<<"      freq got = "<< freq <<"      % diff = "<<p_diff<<" % "<<endl<<endl;
    return freq;
}

double FftStuff::get_fund_freq(){
    double freq1 = bin_to_freq(binNo1);
    double freq2 = bin_to_freq(binNo2);
    double freq3 = bin_to_freq(binNo3);
    double freq_found = 0.0;
    // double tol =.01;
    double tol_floor = 10;
    double tol_max_peak = 20;
    double tol_3rd_peak = 10;
    double tol_rem = 0.01;
    double a = freq1;
    double b = freq2;
    double ba;
    double ca;
    double r_int;
    double rem_;
    double rem_a;
    cout << " 1 [ " << freq1<<" ] lev "<<binA1<<endl<<
        " 2 [ " << freq2<<" ] lev "<<binA2<<endl<<
        " 3 [ " << freq3<<" ] lev "<<binA3<<endl<<
        "            --- prosses starts ---             <- last lev ->  "<<binA4<<"   "<<binA5<<endl;

    if(binA1 < tol_max_peak){cout<<"(1)   1st freq TOO LOW"<<endl;return 0.0;};
    if(binA2 < tol_floor){cout<<"(2)   2nd freq TOO LOW"<<endl;return freq1;};

    if(b<a){a = freq2; b = freq1;}; // set up for varables for test

    ba = b/a;  r_int = round(ba);  rem_ =  r_int - ba ;  rem_a = rem_/r_int; // set up is harmonic ?
    cout<<"(3)  ba "<<ba<<"   rint "<<r_int<<"   rem_ "<<rem_<<"   rem_a "<<rem_a<<endl;

    if(abs(rem_a)<tol_rem) { // TEST  is harmonic ( YES )
            if(a==freq1){
                freq_found = freq1;
                cout<<" 2nd freq = a "<<endl;
            }else{          // ELSE NEEDS TO GET THE AVE FREQ   << *************
            freq_found = freq2;
            cout<<" 2nd freq = b   freq1/r_int "<<endl;
            }
        // DONE ABOUVE   SAVE THIS UNTILL FIXED
        // if(a==freq1){freq_found = freq1; }          /*else{ freq_found = freq1/r_int;};*/ // ????????? DONE ELSE WHERE

            if(freq3 > freq_found){ cout<<"(3A)  freq3 > freq_found   NOT LOWER FREQUENCY "<<endl;
                return freq_found;};
            if(binA3<tol_3rd_peak){cout<<"(3B)  binA3<tol_rem   LEVEL TOO LOW "<<endl;
                return freq_found;};

            cout<<" test for a to be  the 2nd harmonic"<<endl;
            ba = a/freq3;  r_int = round(ba);  rem_ =  r_int - ba ;  rem_a = rem_/r_int; // set up is harmonic

            if(abs(rem_a) > tol_rem){ // is NOT  a harmonic
                cout<<"(3C) FREQ FOUND WAS  [ NOT ] THE 2ND HARMONIC "<<endl;
                return freq_found;};

            freq_found = a / r_int;
            cout<<"(3D) FREQ FOUND  [ WAS ] THE 2ND HARMONIC "<<endl;  return freq_found;
        };

        // -------- DONE WITH FOUND HARMONICS -------------------------------------------
    // is harmonic ( NO )

    cout<<" CK for ADDINITAL harmonics "<<endl;

    ba = b/(a/2);  r_int = round(ba);  rem_ =  r_int - ba ;  rem_a = rem_/r_int;
    cout<<"(4A) 2nd Har   ba "<<ba<<"   rint "<<r_int<<"   rem_ "<<rem_<<"   rem_a "<<rem_a<<endl<<endl;
    if(abs(rem_a)<tol_rem){
        if(a==freq1){ freq_found = freq1/2; cout<<" 2nd freq = freq1/2 "<<endl;
        }
        else {freq_found = freq1/r_int; cout<<" 2nd freq = freq1/r_int"<<endl;
        }
        return freq_found;
        }

    cout<<" CK for 3rd harmonics "<<endl;

    ba = b/(a/3);  r_int = round(ba);  rem_ =  r_int - ba ;  rem_a = rem_/r_int;
    cout<<"(4B) 3rd Har   ba "<<ba<<"   rint "<<r_int<<"   rem_ "<<rem_<<"   rem_a "<<rem_a<<endl<<endl;
    if(abs(rem_a)<tol_rem){
        if(a==freq1){ freq_found = freq1/3; cout<<" 3rd freq = freq1/2 "<<endl;
        }
        else {freq_found = freq1/r_int; cout<<" 3rd freq = freq1/r_int"<<endl;
        }
        return freq_found;
    }


    cout<<"(8) NO HARMONICS WERE FOUND RETORN ZERO  "<< endl;
    return 0.0;
}

