const char *PSK = "<>";
const char *MQTT_BROKER = "<>";

WiFiClient espClient;
PubSubClient client(espClient);

int pin = D2;
volatile unsigned int pulse;
const float pulses_per_kwh = 2.285 * 10000; // insert scaling if needed
const int interval = 300000;
int lastTick = 0;

int RESET_COUNTER = 0;

// https://community.blynk.cc/t/error-isr-not-in-iram/37426/20
ICACHE_RAM_ATTR void count_pulse()
{
    pulse++;
}

void setup_wifi()
{
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(SSID);

    WiFi.begin(SSID, PSK);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect()
{
    while (!client.connected())
    {
        Serial.print("Reconnecting...");
        if (!client.connect("ESP8266Client", "<mqtt_user>", "<mqtt_password>"))
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
}

void setup()
{
    Serial.begin(9600);

    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin), count_pulse, RISING);

    setup_wifi();
    client.setServer(MQTT_BROKER, 1883);
}

void loop()
{

    unsigned long ms = millis();
    if (ms - lastTick > interval)
    {
        if (!client.connected())
        {
            reconnect();
        }
        client.loop();
        char msg[64] = {0};
        lastTick = ms;
        snprintf(msg, sizeof(msg) - 1, "%f",
                 double(pulse) / double(pulses_per_kwh));

        client.publish("<mqtt_topic>", msg);
        pulse = 0;
    }
}
