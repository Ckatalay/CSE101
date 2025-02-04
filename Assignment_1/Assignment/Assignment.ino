const char* to_morse(char c);
void blink(const char* code);
#define INTERVAL 250

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 bps
  pinMode(LED_BUILTIN, OUTPUT);
  char hello[] = "Hello, World!";
  int len = strlen(hello);
  for (int i = 0; i < len; i++) {
    blink(to_morse(toupper(hello[i]))); // Displays "Hello, World" during the setup, this is the first part of the assignment
  }
}

void loop() {
  if (Serial.available()) {
    char letter = Serial.read();  // Read the incoming letter
    blink(to_morse(toupper(letter))); // Converts the character to dots and dashes and then to LED blinking, bonus part of the assignment
  }
}

// Converts characters to dots and dashes
const char* to_morse(char c) {
    switch (c) {
        // Letters A-Z
        case 'A': return ".-";
        case 'B': return "-...";
        case 'C': return "-.-.";
        case 'D': return "-..";
        case 'E': return ".";
        case 'F': return "..-.";
        case 'G': return "--.";
        case 'H': return "....";
        case 'I': return "..";
        case 'J': return ".---";
        case 'K': return "-.-";
        case 'L': return ".-..";
        case 'M': return "--";
        case 'N': return "-.";
        case 'O': return "---";
        case 'P': return ".--.";
        case 'Q': return "--.-";
        case 'R': return ".-.";
        case 'S': return "...";
        case 'T': return "-";
        case 'U': return "..-";
        case 'V': return "...-";
        case 'W': return ".--";
        case 'X': return "-..-";
        case 'Y': return "-.--";
        case 'Z': return "--..";

        // Numbers 0-9
        case '0': return "-----";
        case '1': return ".----";
        case '2': return "..---";
        case '3': return "...--";
        case '4': return "....-";
        case '5': return ".....";
        case '6': return "-....";
        case '7': return "--...";
        case '8': return "---..";
        case '9': return "----.";

        // Punctuation
        case '.': return ".-.-.-";
        case ',': return "--..--";
        case '?': return "..--..";
        case '\'': return ".----.";
        case '!': return "-.-.--";
        case '/': return "-..-.";
        case '(': return "-.--.";
        case ')': return "-.--.-";
        case '&': return ".-...";
        case ':': return "---...";
        case ';': return "-.-.-.";
        case '=': return "-...-";
        case '+': return ".-.-.";
        case '-': return "-....-";
        case '_': return "..--.-";
        case '"': return ".-..-.";
        case '$': return "...-..-";
        case '@': return ".--.-.";
        case ' ': return "/";

        default: return "";
    }
}


// turns on and off the LED depending on the character
void blink(const char* code) {
  int len = strlen(code);  // Get the length of the Morse code string
  for (int i = 0; i < len; i++) {  // Iterates through each character in the string
    if (code[i] == '.') {  // Dot
      digitalWrite(LED_BUILTIN, HIGH);
      delay(INTERVAL);
      digitalWrite(LED_BUILTIN, LOW);
      delay(INTERVAL);
    } 
    else if (code[i] == '-') {  // Dash
      digitalWrite(LED_BUILTIN, HIGH);
      delay(INTERVAL * 3);
      digitalWrite(LED_BUILTIN, LOW);
      delay(INTERVAL);
    } 
    else if (code[i] == '/') {  // Space
      delay(INTERVAL * 7);
    }
  }
  delay(INTERVAL * 3);  // Gap between letters
}
