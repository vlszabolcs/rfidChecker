void beepInit()
{
    pinMode(buzzer, OUTPUT);
}

void beepAccept()
{
    tone(buzzer, 600); // Send 1KHz sound signal...
    delay(500);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
}

void beepError()
{
    for(int i = 0; i < 3; i++) {
        tone(buzzer, 1000); // Send 1KHz sound signal...
        delay(300);        // ...for 1 sec
        noTone(buzzer);     // Stop sound...
        if(i < 2) {
            delay(300);    // Add a small delay between beeps
        }
    }
}