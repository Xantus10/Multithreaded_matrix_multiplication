#include <fstream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>////////////////////////////////////////////////////////////////// Eventually create class/struct for resmatrix and its dimensions

// Maximum number of threads
int MAXTHREADS = 4;
// Number of rows/cols from result matrix, that 1 thread will handle
int SPLITTHREAD = 100;


// 2D array
typedef std::vector<std::vector<int>> Matrix;


// Result matrix
struct ResultMatrix {
  int** matrix;
  int xlen;
  int ylen;
};


// Calculate a range of specified rows of result matrix
void calculateRows(Matrix* mat1, Matrix* mat2, int** resmat, int iRange, int split) {
  // Set the top boundary of the loop to the next threads start boundary or to the end of the vec
  int topRange = (iRange+split) < mat1->size() ? iRange+split : mat1->size();
  for (int i=iRange; i<topRange; i++) {
    for (int j=0; j<(*mat2)[0].size(); j++) {
      resmat[i][j] = 0;
      for (int rowcolptr=0; rowcolptr<(*mat1)[0].size(); rowcolptr++) {
        resmat[i][j] += (*mat1)[i][rowcolptr] * (*mat2)[rowcolptr][j];
      }
    }
  }
}


// Calculate a range of specified cols of result matrix
void calculateCols(Matrix* mat1, Matrix* mat2, int** resmat, int iRange, int split) {
  // Set the top boundary of the loop to the next threads start boundary or to the end of the vec
  int topRange = (iRange+split) < (*mat2)[0].size() ? iRange+split : mat1->size();
  for (int j=iRange; j<topRange; j++) {
    for (int i=0; i<mat1->size(); i++) {
      resmat[i][j] = 0;
      for (int rowcolptr=0; rowcolptr<(*mat1)[0].size(); rowcolptr++) {
        resmat[i][j] += (*mat1)[i][rowcolptr] * (*mat2)[rowcolptr][j];
      }
    }
  }
}


// Return a pointer to result of multiplication of two matrixes
int** multiplyMatrixes (Matrix* mat1, Matrix* mat2) {
  // Initialize the result matrix
  int** resmat = new int*[mat1->size()];
  for (int i=0; i<mat1->size(); i++) {
    resmat[i] = new int[mat2[0].size()];
  }

  // Vector of all threads
  std::vector<std::thread> threads;

  // We will use multithreading on rows/cols depending on what is bigger and therefore better to split into multiple threads
  if (mat1->size() > (*mat2)[0].size()) {
    // If we can fit the input into MAXTHREADS
    if (mat1->size() <= MAXTHREADS*SPLITTHREAD) {
      for (int i=0; i<mat1->size(); i+=SPLITTHREAD) {
        threads.push_back(std::thread(calculateRows, mat1, mat2, resmat, i, SPLITTHREAD));
      }
    } else { // If we cannot we we will create max # of threads and each will have to handle more than SPLITTHREADS amount
      for (int i=0; i<MAXTHREADS; i++) {
        threads.push_back(std::thread(calculateRows, mat1, mat2, resmat, i*(mat1->size()/MAXTHREADS), mat1->size()/MAXTHREADS));
      }
    }
  } else {
    if ((*mat2)[0].size() <= MAXTHREADS*SPLITTHREAD) {
      for (int j=0; j<(*mat2)[0].size(); j+=SPLITTHREAD) {
        threads.push_back(std::thread(calculateCols, mat1, mat2, resmat, j, SPLITTHREAD));
      }
    } else {
      for (int i=0; i<MAXTHREADS; i++) {
        threads.push_back(std::thread(calculateRows, mat1, mat2, resmat, i*((*mat2)[0].size()/MAXTHREADS), (*mat2)[0].size()/MAXTHREADS));
      }
    }
  }

  // Wait for all threads to finish
  for (auto& t : threads) {
    t.join();
  }

  return resmat;
}


void fillMatrixFromFile(Matrix& matrix, const std::string& file) {
  // Input file
  std::ifstream f(file);
  // Line of file, singular values in a line
  std::string line, value;
  // All lines in a file
  while (std::getline(f, line)) {
    // Vector of all values in a file
    std::vector<int> lineVec;
    // Change line to a stringstream
    std::stringstream strStreamOfLine (line);
    // For every comma separated value
    while (std::getline(strStreamOfLine, value, ',')) {
      // Push the integer value
      lineVec.push_back(std::stoi(value));
    }
    // Push the whole line vector
    matrix.push_back(lineVec);
  }
}


// Check if provided string is only numeric
bool checkNumeric(std::string& s) {
  bool ret = true;
  for (auto c : s) {
    ret = ret && std::isdigit(c);
  }
  return ret;
}


// Load configuration from config.cfg
void loadConfig(std::string& f1, std::string& f2) {
  std::ifstream file("config.cfg");

  std::string tmp;

  std::getline(file, f1);
  std::getline(file, f2);
  std::getline(file, tmp);
  if (checkNumeric(tmp)) MAXTHREADS = std::stoi(tmp);
  std::getline(file, tmp);
  if (checkNumeric(tmp)) SPLITTHREAD = std::stoi(tmp);

  file.close();
}


// Configure config.cfg
void configure() {
  std::string input;

  std::string f1, f2;

  loadConfig(f1, f2);

  // Clean the buffer from std::cin.get()
  std::cin.clear();
  
  std::cout << "[1] Change the filenames of input matrixes\n[2] Change the maximum number of active threads\n[3] Change the number of rows/collumns a thread will compute\n[exit] To exit configuration\n";

  while (input != "exit") {
    std::cout << ">>>";
    std::cin >> input;
    if (input == "1") {
      std::cin.clear();
      std::getline(std::cin, input);
      std::cout << "Input file 1: ";
      std::getline(std::cin, f1);
      std::cout << "Input file 2: ";
      std::getline(std::cin, f2);
    } else if (input == "2") {
      std::cout << "NOTE: Default number of threads is 4, you should never enter more threads than your machine supports, as it may hinder your performance\n";
      std::cout << "Maximum number of threads this program can utilize: ";
      std::cin >> input;
      if (checkNumeric(input)) {
        MAXTHREADS = std::stoi(input);
      } else {
        std::cout << "Use a numeric value!";
      }
    } else if (input == "3") {
      std::cout << "NOTE: It is not recomended to change the number of rows/collumns a thread will compute unless you know what you are doing, increasing this number may hinder performance, default is 150\n";
      std::cout << "Number of rows/collumns a thread will compute: ";
      std::cin >> input;
      if (checkNumeric(input)) {
        SPLITTHREAD = std::stoi(input);
      } else {
        std::cout << "Use a numeric value!";
      }
    }
  }

  // Write to a file
  std::ofstream out("config.cfg");
  std::string output = f1 + "\n" + f2 + "\n" + std::to_string(MAXTHREADS) + "\n" + std::to_string(SPLITTHREAD);
  out << output;
  std::cout << "Configuration changed\n";
}


int main(int argc, char** argv) {
  // filename strings
  std::string mat1file, mat2file;
  
  std::cout << "Would you like to enter configuration [y/N]";
  if (std::cin.get() == 'y') {
    configure();
  }

  loadConfig(mat1file, mat2file);

  // Two input vectors
  Matrix mat1;
  Matrix mat2;

  // Fill the matrixes
  fillMatrixFromFile(mat1, mat1file);
  fillMatrixFromFile(mat2, mat2file);

  // Get the result
  int** result = multiplyMatrixes(&mat1, &mat2);

  // Write result to a file
  std::ofstream out("outputmatrix.txt");
  for (int i=0; i<mat1.size(); i++) {
    for (int j=0; j<mat2[0].size(); j++) {
      out << result[i][j];
      if (j != mat2[0].size()-1) {
        out << ",";
      }
    }
    if (i != mat1.size()-1) {
        out << "\n";
      }
  }

  return 0;
}
