#include <fstream>
#include <string>
#include <cstdint>
#include <cmath>


int f(const int8_t n) {
  return ((int)n * (abs(n) + 1)) / 2;
}

// Returns a sum n and a remainder, such that f(a) + f(b) = f(n) + r and r <= n
int8_t fInverse(const int n) {
  int sum = abs(n);

  int i = 0;
  while(sum > i) {
    i++;
    sum -= i;
  }
  return (n < 0 ? -1 : 1) * i;
}



void printDirectConversion(std::ofstream& out) {
  out << "  constexpr int direct[256] = {";
  for(int i = 0; i < 255; ++i) {
    if(i % 64 == 0) {
      out << std::endl << "    ";
    }
    if(i < 0b01000000 || i > 0b11000000) {
      out << f(static_cast<int8_t>(i)) << ", ";
    }
    else {
      out << "0, ";
    }
  }
  out << f(static_cast<int8_t>(255U)) << std::endl;
  out << "  };" << std::endl;
}

void printInverseConversion(std::ofstream& out) {
  const int max = f(64) + f(63);
  out << "  constexpr int8_t inverse[" << max << "] = {";
  for(int i = 0; i < max; ++i) {
    if(i % 64 == 0) {
      out << std::endl << "    ";
    }
    out << static_cast<int>(fInverse(i));
    if(i + 1 != max) {
      out << ", ";
    }
  }
  out << std::endl;
  out << "  };" << std::endl;
}

void printInverseRemainder(std::ofstream& out) {
  const int max = f(64) + f(63);
  out << "  constexpr int8_t remainder[" << max << "] = {";
  for(int i = 0; i < max; ++i) {
    if(i % 64 == 0) {
      out << std::endl << "    ";
    }
    out << i - f(fInverse(i));
    if(i + 1 != max) {
      out << ", ";
    }
  }
  out << std::endl;
  out << "  };" << std::endl;
}


int main() {
  constexpr int8_t test = static_cast<int8_t>((int)0b11111111);

  const std::string outputFileName = "exponentlookup.h";

  std::ofstream out(outputFileName);

  out << "#include <cstdint>" << std::endl;
  out << std::endl;
  out << "namespace ExponentLookup {" << std::endl;

  printDirectConversion(out);
  out << std::endl;

  printInverseConversion(out);
  out << std::endl;
  
  printInverseRemainder(out);

  out << "}" << std::endl;
  out.close();
}
