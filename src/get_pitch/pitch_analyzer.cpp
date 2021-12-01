/// @file

#include <iostream>
#include <math.h>
#include "pitch_analyzer.h"
#include <iomanip>

using namespace std;

/// Name space of UPC
namespace upc {
  void PitchAnalyzer::autocorrelation(const vector<float> &x, vector<float> &r) const {

    for (unsigned int l = 0; l < r.size(); ++l) { //r és vector on s'emmagatzema autocorrelació
  		/// \TODO Compute the autocorrelation r[l]
      /// \DONE :) hem implementat l'autocorrelació d'un senyal real.
      r[l] = 0;
      for ( unsigned int n = 0; n < x.size()-l ; n++) {
        r[l] += x[n] * x[n+l]; 
      }
    }

    if (r[0] == 0.0F) //to avoid log() and divide zero 
      r[0] = 1e-10; 
  }

  void PitchAnalyzer::amdf(const vector<float> &x, vector<float> &t) const {

    for (unsigned int l = 0; l < t.size(); ++l) { //t és vector on s'emmagatzema amdf
  		
      t[l] = 0;
      for (unsigned int n = 0; n < x.size()-l ; n++) {
        t[l] += abs(x[n] - x[n+l]); 
      }
      t[l] = t[l] / (t.size() - l);
      //cout << l<< " --> AMDF = " << t.at(l) << endl;
      //t = t/t.size();
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
      /// \TODO Implement the Hamming window -> Sara
    //cout << "La Hamming ; Framelen: " << frameLen <<" \n";
    for(unsigned int i = 0; i < frameLen; i++){
        window[i] =  0.54 - 0.46*cos(2*M_PI*i/frameLen-1); // no se si estic assignant bé el vector 
      }
    /*for (vector<float>::iterator iXaux = window.begin(); iXaux < window.end(); iXaux++) {
      cout << *iXaux << "\n";
    }*/
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

  /*void PitchAnalyzer::compute_zcr(const float x, unsigned int N, float fm) {
    int i,sum,factor,zeros;
    sum=0;
    zeros=0;
    for (i=0; i<=N-1; i++){
        if(x[i]==0){
            zeros++;
         }
        if(x[i]*x[i+1]<=0){ //punt zero es compta com a creuament.
            sum++;
            }
    }
    factor=fm/2*(N-1);
    return((sum-zeros));
}*/

  //Mirar los pasos por cero
  bool PitchAnalyzer::unvoiced(float pot, float r1norm, float rmaxnorm) const { //r1norm = r[1]/r[0];  rmaxnorm= r[lag]/r[0]
    /// \TODO Implement a rule to decide whether the sound is voiced or not. --> en funció del llindar donat pel valor màxim de l'autocorr -> Anna y Sara 
    /// * You can use the standard features (pot, r1norm, rmaxnorm),
    ///   or compute and use other ones.
    if((rmaxnorm > 0.40 && r1norm > 0.45) && pot >= -20){ //sonoro
        return false; 
    } else { //sordo
      return true;
    }
      

    //Como tiene que funcionar: devuelve T si la trama es sorda, F si la trama es sonora
    
  }
  float PitchAnalyzer::compute_pitch(vector<float> & x) const {  //funció compute_pitch es troba dins la classe PitchAnalyser --> acces amb ::
    if (x.size() != frameLen)
      return -1.0F;

    //Window input frame
    for (unsigned int i=0; i<x.size(); ++i)
      x[i] *= window[i]; //apliquem la finestra

    vector<float> r(npitch_max); // vector r de dimensió npitch_max --> 300
    vector<float> t(npitch_max); // vector t
    //Compute correlation
    autocorrelation(x, r);
    amdf(x,t);

    /// \TODO 
    //analitzar els valors de l'autocorrelació
        
    //cout << "min " << npitch_min << " max" << npitch_max << "\n";
    vector<float>::const_iterator iR = r.begin(), iRMax = iR + npitch_min; //Si no no encuentra valores lógicos

    //El pitch s'ha de trobar en primera instància entre les npitch_min i npitch_max primeres mostres de la correlació.
    for (iR = r.begin() + npitch_min ; iR < r.begin() + npitch_max ; iR++){
      if(*iR > *iRMax){ //queda't amb el màxim
        iRMax = iR;
      }
    }

   /* vector<float>::const_iterator iT;// = t.begin(), iTMin = iT + npitch_min;
    float iTMin=1000;
    cout <<"\n\nInic "<< iTMin << endl;
    //Valor mínimo de la AMDF
    for (iT = t.begin(); iT < t.end(); iT++){
      cout << "AMDF " << *iT;
      if(*iT < iTMin){ //queda't amb el mínim
        cout << "\t\t\t --> " << t.at(*iT);
        iTMin = *iT;
        
      }
      cout << endl;
    }*/
    float iTMin = 1000;
    long unsigned int index;
    int finestra = 0;
    long unsigned int i;
    for (i = npitch_min ; i < npitch_max; i++){
      //cout << " " << t.at(i);
      if(t.at(i) < iTMin){
        iTMin = t.at(i);
        //cut << " i --> " << i << endl;
        index = i;
      //cout << "\t\t --> " << iTMin ;        
      }

      //cout << endl;
    }
  //cout<< "K min: " << index << endl;

	/// Find the lag of the maximum value of the autocorrelation away from the origin.<br>
	/// Choices to set the minimum value of the lag are:
	///    - The first negative value of the autocorrelation.
	///    - The lag corresponding to the maximum value of the pitch.
    ///	   .
	/// In either case, the lag should not exceed that of the minimum value of the pitch.

   /// DONE

    unsigned int lag = iRMax - r.begin(); //Desplazamiento correlación
    unsigned int lag_2 = index; //Desplazamiento amdf


    float pot = 10 * log10(r[0]); //Potencia de la señal

    //You can print these (and other) features, look at them using wavesurfer
    //Based on that, implement a rule for unvoiced
    //change to #if 1 and compile

     vector<float> results(2);
     results.at(0) = (float) samplingFreq/(float)lag;
     results.at(1) = (float) samplingFreq/(float)lag_2;
    
#if 0
    if (r[0] > 0.0F)
    //cout << pot << '\t' << r[1]/r[0] << '\t' << r[lag]/r[0] << endl;~
     if (unvoiced(pot, r[1]/r[0], r[lag]/r[0])){
       results.at(0) = 0;
       results.at(1) = 0;
     } 
      

    cout << "Corr: " << results.at(0) << "\t AMDF: " << results.at(1) << endl; 
#endif
    
    if (unvoiced(pot, r[1]/r[0], r[lag]/r[0])) 
      return 0;
    else
      return (float) samplingFreq/(float)lag;
  }
}
