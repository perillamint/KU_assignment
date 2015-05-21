#include <iostream>

using namespace std;

enum chicken_sauce {
  FRIED, YANGNYUM, GARLIC, HONEY, KKATIP
};

class Chicken {
public:
  Chicken();
  Chicken(chicken_sauce sauce);
  ~Chicken();
  void printReceipt();
private:
  chicken_sauce sauce;
};

Chicken::Chicken() {
  this -> sauce = FRIED;
}

Chicken::Chicken(chicken_sauce sauce) {
  this -> sauce = sauce;
}

Chicken::~Chicken() {
  //Do nothing
}

void Chicken::printReceipt() {
  switch(this -> sauce) {
  case FRIED:
    cout << "Fried chicken - 10000 KRW" << endl;
    break;
  case YANGNYUM:
    cout << "Yangnyum chicken - 11000 KRW" << endl;
    break;
  case GARLIC:
    cout << "Garlic chicken - 12000 KRW" << endl;
    break;
  case HONEY:
    cout << "Honey chicken - 12000 KRW" << endl;
    break;
  case KKATIP:
    cout << "KKATIP chicken - 12000 KRW" << endl;
    break;
  }
}

int main (int argc, char **argv) {
  Chicken *chicken = new Chicken();
  Chicken *yangnyum = new Chicken(YANGNYUM);
  Chicken *garlic = new Chicken(GARLIC);

  cout << "Each different object has different \"state\"" << endl;
  cout << "They are all came from same Chicken class" << endl;
  cout << "but they produce different output due to their state" << endl;
  chicken -> printReceipt();
  yangnyum -> printReceipt();
  garlic -> printReceipt();

  delete chicken;
  delete yangnyum;
  delete garlic;

  return 0;
}
