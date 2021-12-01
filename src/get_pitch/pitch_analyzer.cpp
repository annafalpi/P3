/// @file

#include <iostream>
#include <fstream>
#include <math.h>
#include "pitch_analyzer.h"
#include <iomanip>
#include <string> 
#include <cstring>

using namespace std;

/// Name space of UPC

namespace upc {
  int count = 1 ; 
  ofstream myfile;
  void PitchAnalyzer::autocorrelation(const vector<float> &x, vector<float> &r) const {
    string num = to_string(count);
    string str1("autocorrelacio");
    string str2(".txt");
    //myfile.open (str1 + num + str2);
    for (unsigned int l = 0; l < r.size(); ++l) { //r és vector on s'emmagatzema autocorrelació
  		/// \TODO Compute the autocorrelation r[l]
      /// \DONE
      r[l] = 0;
      for ( unsigned int n = 0; n < x.size()-l ; n++) {
        r[l] += x[n] * x[n+l]; 
      }
    //myfile << r[l] << "\n";
    }

    if (r[0] == 0.0F){
        r[0] = 1e-10;     
    } //to avoid log() and divide zero 
      
    //myfile.close();
    count++;
  }

  void PitchAnalyzer::amdf(const vector<float> &x, vector<float> &t) const {

    for (unsigned int l = 0; l < t.size(); ++l) { //t és vector on s'emmagatzema amdf
  		
      t[l] = 0;
      for (unsigned int n = 0; n < x.size()-l ; n++) {
        t[l] += abs(x[n] - x[n+l]); 
      }
      t[l] = t[l] / (t.size() - l); //normalització
      //cout << l<< " --> AMDF = " << t.at(l) << endl;
    }

    /*if (t[0] == 0.0F) //to avoid log() and divide zero 
      t[0] = 1e-10; */
  }

  void PitchAnalyzer::set_window(Window win_type) {
    if (frameLen == 0)
      return;

    window.resize(frameLen);

    switch (win_type) {
    case HAMMING:
    /// \TODO Implement the Hamming window 
   for(unsigned int i = 0; i < frameLen; i++){
        window[i] =  0.54 - 0.46*cos(2*M_PI*i/frameLen-1); 
      }
    /// \DONE 
    break;
    case RECT:
    default:
      window.assign(frameLen, 1); // finestra rectagular
    }
  }

  void PitchAnalyzer::set_f0_range(float min_F0, float max_F0) {
    npitch_min = (unsigned int) samplingFreq/max_F0;
    if (npitch_min < 2)
      npitch_min = 2;  // samplingFreq/2

    npitch_max = 1 + (unsigned int) samplingFreq/min_F0;

    //frameLen should include at least 2*T0
    if (npitch_max > frameLen/2)
      npitch_max = frameLen/2;
  }
  int N=300;
  float PitchAnalyzer::compute_zcr(const vector<float> &x, unsigned int N, float samplingFreq) const{
    int i,sum,factor,zeros;
    sum=0;
    zeros=0;
    for (i=0; i < N-1; i++){
        if(x[i]==0){
            zeros++;
         }
        if(x[i]*x[i+1]<=0){ //punt zero es compta com a creuament.
            sum++;
            }
    }
    factor=samplingFreq/2*(N-1);
    return((sum-zeros));
}

  //Mirar los pasos por cero

  bool PitchAnalyzer::unvoiced(float pot, float r1norm, float rmaxnorm, float zcr) const { //r1norm = r[1]/r[0];  rmaxnorm= r[lag]/r[0]
    /// \TODO Implement a rule to decide whether the sound is voiced or not.     
    if((rmaxnorm > threshold1 && r1norm > threshold2) && (pot >= threshold3 && zcr < threshold4 )){ //sonoro
        return false; 
    } else { //sordo
      return true;
    }
    /// \DONE
  }

  float PitchAnalyzer::compute_pitch(vector<float> & x) const { 
    if (x.size() != frameLen)
      return -1.0F;

    //Window input frame
    for (unsigned int i=0; i<x.size(); ++i)
      x[i] *= window[i];          //apliquem la finestra

    vector<float> r(npitch_max);  // vector r de dimensió npitch_max --> 300
    vector<float> t(npitch_max);  // vector t
    
    autocorrelation(x, r);
    amdf(x,t);
    float zcr = compute_zcr(x, N, samplingFreq);
    //cout <<"ZCR: "<< zcr << endl;


    /// \TODO 
    /// Find the lag of the maximum value of the autocorrelation away from the origin.<br>
    vector<float>::const_iterator iR = r.begin(), iRMax = iR + npitch_min; 

    //El pitch s'ha de trobar en primera instància entre les npitch_min i npitch_max primeres mostres de la correlació.
    for (iR = r.begin() + npitch_min ; iR < r.begin() + npitch_max ; iR++){
      if(*iR > *iRMax){ //queda't amb el màxim
        iRMax = iR;
      }
    }
    /// \DONE 

    /// \Ampliation --> ajust the search of AMDF index
    float iTMin = 1000;         //inicialització  a valor elevat
    long unsigned int index;
    long unsigned int i;
    for (i = npitch_min ; i < npitch_max; i++){
      //cout << " " << t.at(i);
      if(t.at(i) < iTMin){
        iTMin = t.at(i);
        index = i;       
      }
    }	

    unsigned int lag = iRMax - r.begin(); //Desplazamiento correlación
    unsigned int lag_2 = index;           //Desplazamiento amdf


    float pot = 10 * log10(r[0]); //Potencia de la señal
    
    //You can print these (and other) features, look at them using wavesurfer
    //Based on that, implement a rule for unvoiced
    //change to #if 1 and compile

    //per comparar pitch detectat amb Rxx i AMDF
     vector<float> results(2);
     results.at(0) = (float) samplingFreq/(float)lag;
     results.at(1) = (float) samplingFreq/(float)lag_2;
    
#if 1
    if (r[0] > 0.0F)
    //cout << pot << '\t' << r[1]/r[0] << '\t' << r[lag]/r[0] << endl;~
     if (unvoiced(pot, r[1]/r[0], r[lag]/r[0], zcr)){
       results.at(0) = 0;
       results.at(1) = 0;
     } 
      

    cout << "Corr: " << results.at(0) << "\t AMDF: " << results.at(1) << " ZCR: "<< zcr <<endl; 
#endif
    
    if (unvoiced(pot, r[1]/r[0], r[lag]/r[0], zcr)) 
      return 0;
    else
      return (float) samplingFreq/(float)lag_2;
  }
}
