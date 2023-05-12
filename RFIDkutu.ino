#include <SPI.h>
#include <RFID.h>

const int ROLE_PIN = 8;

RFID lrt720(10, 9);

const String REAL_ORDER[] = {"24313311224", "16324467222", "17671147163199", "121169156152212", "48135124165110", "176105150163236"};
String cardQueue[] = {"", "", "", "", "", ""};

void setup()
{
    pinMode(ROLE_PIN, OUTPUT);
    Serial.begin(9600);
    SPI.begin();
    lrt720.init();
    digitalWrite(ROLE_PIN, HIGH);

    // Print a message indicating that the setup is completed
    Serial.println("Setup completed");
}

void loop()
{
    if (lrt720.isCard())
    {
        if (lrt720.readCardSerial())
        {
            String cardID = getCardID(lrt720);

            // Add the card to the queue if it is not already the last card
            if (cardQueue[5] != cardID)
            {
                addCardToQueue(cardID);

                // Print the current card queue
                Serial.println(cardQueue[0] + " " + cardQueue[1] + " " + cardQueue[2] + " " + cardQueue[3] + " " + cardQueue[4] + " " + cardQueue[5]);

                // Check if the card queue matches the predefined order
                if (checkOrder())
                {
                    // Unlock the system for a duration of 3000 milliseconds
                    unlockForDuration(3000);

                    // Print a message indicating that access is granted
                    Serial.println("Access granted");
                }
            }
        }
        lrt720.halt();
    }
}

// Get the ID of the card as a string
String getCardID(RFID &cardReader)
{
    String cardID = "";
    for (byte i = 0; i < 5; i++)
    {
        cardID += String(cardReader.serNum[i]);
    }
    return cardID;
}

// Add a card ID to the card queue
void addCardToQueue(String cardID)
{
    for (int i = 0; i < 5; i++)
    {
        cardQueue[i] = cardQueue[i + 1];
    }
    cardQueue[5] = cardID;
}

// Check if the card queue matches the predefined order
bool checkOrder()
{
    for (int i = 0; i < 6; i++)
    {
        if (REAL_ORDER[i] != cardQueue[i])
        {
            return false;
        }
    }
    return true;
}

// Unlock the system for a specified duration
void unlockForDuration(int duration)
{
    digitalWrite(ROLE_PIN, LOW);
    delay(duration);
    digitalWrite(ROLE_PIN, HIGH);
}
