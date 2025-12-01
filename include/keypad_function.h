#ifndef KEYPAD_FUNCTION_H
#define KEYPAD_FUNCTION_H

// Keypad mapping
extern const char keypadMap[12];

// Keypad functions
void initKeypad(void);
void reconfigureKeypadPins(void);
void readKeypad(unsigned int *keypadValue);

// Keypad and LCD functions
void activateKeyPadAndDeactivateLCD(void);
void deactivateKeyPadAndActivateLCD(void);

#endif // KEYPAD_FUNCTION_H
