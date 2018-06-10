int clavier() {
  debouncer1.update();
  debouncer2.update();
  debouncer3.update();
  
  int bit0 = debouncer1.read();
  int bit1 = debouncer2.read();
  int enable = debouncer3.read();

  int input = enable*100+bit1*10+bit0;
  if (enable != current_enable) {
    current_enable = enable;
    switch (input) {
      case 100:
        return 1;

      case 101:
        return 2;

      case 110:
        return 3;

      case 111:
        return 4;

      default:
        return 0;
    }
  } else {
      return 0;
  }
}

