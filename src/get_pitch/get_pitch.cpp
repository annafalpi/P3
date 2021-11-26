/// @file

#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>

#include "wavfile_mono.h"
#include "pitch_analyzer.h"

#include "docopt.h"

#define FRAME_LEN   0.030 /* 30 ms. */
#define FRAME_SHIFT 0.015 /* 15 ms. */

using namespace std;
using namespace upc;

//Pongo un comentario para ver si me funciona el commit
//docopt [editar si queremos introducir parámetros por terminal] modificado en clase
static const char USAGE[] = R"(
get_pitch - Pitch Detector 

Usage:
    get_pitch [options] <input-wav> <output-txt>
    get_pitch (-h | --help)
    get_pitch --version

Options:
    -1 FLOAT, --threshold1 FLOAT  Threshold Autocorrelation sonoro/sordo para rmaxnorm [default: 0.5]
    -l1 FLOAT, --theshold_clipping_1 FLOAT  Threshold superior clipping [default: 0.005]
    -l2 FLOAT, --theshold_clipping_2 FLOAT  Threshold inferior clipping [default: -0.005]
    -h, --help  Show this screen
    --version   Show the version of the project

Arguments:
    input-wav   Wave file with the audio signal
    output-txt  Output file: ASCII file with the result of the detection:
                    - One line per frame with the estimated f0
                    - If considered unvoiced, f0 must be set to f0 = 0
)";

int main(int argc, const char *argv[]) {
	/// \TODO 
	///  Modify the program syntax and the call to **docopt()** in order to
	///  add options and arguments to the program -> Anna.
  // Es lo que haciamos en el fichero a parte del docopt en C, en C++ mejora y se incluye en el propio fichero
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
        {argv + 1, argv + argc},	// array of arguments, without the program name
        true,    // show help if requested
        "2.0");  // version string

//el espacio de nombres es estandar (std) en la clase de string
	std::string input_wav = args["<input-wav>"].asString();
	std::string output_txt = args["<output-txt>"].asString();
  float threshold1 = atof(args["--threshold1"].asString().c_str()); //modificado en clase
  float l1 = atof(args["--theshold_clipping_1"].asString().c_str()); 
  float l2 = atof(args["--theshold_clipping_2"].asString().c_str());  

  // Read input sound file
  unsigned int rate;
  vector<float> x;
  if (readwav_mono(input_wav, rate, x) != 0) {
    cerr << "Error reading input file " << input_wav << " (" << strerror(errno) << ")\n";
    return -2;
  }

  int n_len = rate * FRAME_LEN;
  int n_shift = rate * FRAME_SHIFT;

  // Define analyzer (creamos el objeto, tenemos que poner los parámetros que introducimos por terminal)
  PitchAnalyzer analyzer(n_len, rate, PitchAnalyzer::RECT, 50, 500); //tambien puede ser una ventana de HAMMING
  analyzer.threshold1 = threshold1;

  /// \TODO
  /// Preprocess the input signal in order to ease pitch estimation. For instance,
  /// central-clipping or low pass filtering may be used.
  
  //Printing the signal
  /* cout << "Original Signal:\n ";
    for (vector<float>::iterator iXaux = x.begin(); iXaux + n_len < x.end(); iXaux = iXaux + n_shift) {
      cout << *iXaux << " \n";
    }*/

  //central-clipping implementation
  vector<float>::iterator iX2;
  //cout << "Clipped Signal:\n ";
  for (iX2 = x.begin(); iX2 + n_len < x.end(); iX2 = iX2 + n_shift) {
    if(*iX2<l1 && *iX2>l2){
      *iX2=0;
    }
    //cout << *iX2 << " \n";
  }
  /// \DONE  Central clipping bàsic


  // Iterate for each frame and save values in f0 vector
  vector<float>::iterator iX;
  vector<float> f0;
  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift) {
    float f = analyzer(iX, iX + n_len);
    f0.push_back(f);
  }

  /// \TODO
  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.

  /*cout << "Original Pitch\n";
  for (vector<float>::iterator iXaux = f0.begin(); iXaux != f0.end(); ++iXaux) {
      cout << *iXaux << " \n"; 
  }*/

  //Printing the pitch detection
  //cout << "Pitch post-processed\n";
    vector<float>::iterator iX3;
    vector<float> median(3,0); //3 elements inicializats a 0
    for (iX3 = f0.begin(); iX3 != f0.end(); ++iX3) {
      //cout << *iX3 << " \n";
      median.assign({*prev(iX3,1),*iX3,*next(iX3,1)});
      //cout << "\nNo sorted:\n" << median.at(0)<< "\t" << median.at(1)<< "\t" << median.at(2) <<"\n" ;
      sort(median.begin(),median.end()); // ordenem 
      //cout <<"Sorted:\n" << median.at(0)<< "\t" << median.at(1)<< "\t" << median.at(2) <<" " ;
      if((median.at(0)<= median.at(1)/2 || median.at(2)>= 2*median.at(1)) && median.at(0) != 0 ){
        //cout << "Here ";
        *iX3 = median.at(1);
    }
    //cout <<"\t -->"<< *iX3 << " \n"; 
    }

  /*cout << "Postprocessed Pitch\n";
  for (vector<float>::iterator iXaux = f0.begin(); iXaux != f0.end(); ++iXaux) {
      cout << *iXaux << " \n"; 
  }*/


  // Write f0 contour into the output file
  ofstream os(output_txt);
  if (!os.good()) {
    cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
    return -3;
  }

  os << 0 << '\n'; //pitch at t=0
  for (iX = f0.begin(); iX != f0.end(); ++iX) 
    os << *iX << '\n';
  os << 0 << '\n';//pitch at t=Dur

  return 0;
}
