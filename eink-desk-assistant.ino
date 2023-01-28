#include <SSLClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include <ntp.h>
#include "WiFi.h"

#include "YahooFinQuote.h"
#include "ForexQuote.h"

#include "epd4in2.h"
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Fonts/FreeMono12pt7b.h>

#include "config.h"
#include "cert.h"

#define SPX_SYMBOL "%5ESPX" // %5E is ^

#define WHITE 0xFF
#define BLACK 0x0


Epd epd;
GFXcanvas1 fb(400, 300);

WiFiClient wifi_client;
SSLClient ssl_client(wifi_client, TAs, (size_t)TAs_NUM, A6);
YahooFinQuote yahoo(ssl_client);
ForexQuote forex(ssl_client);

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting up the Desk Assistant");

    Serial.print("Connecting to WiFi... ");

    if(WiFi.begin(WIFI_SSID, WIFI_PASS) != WL_CONNECTED)
    {
        Serial.println("Error: WiFi connection");
        exit(1);
    }

    Serial.print("Connected; IP: ");
    Serial.println(WiFi.localIP());

    Serial.print("Getting time from NTP server.. ");

    set_time(ntp_get_time() + NTP_TZ);

    Serial.println("Done!");

    time_t epoch = time(NULL);
    Serial.print("Current time: ");
    Serial.println(asctime(gmtime(&epoch)));

    epd.Init();
}

void loop()
{
    Serial.println("Getting SPX price from Yahoo Finance... ");
    Quote quote;
    HTTPQuote::QuoteError err = yahoo.fetchQuote(SPX_SYMBOL, quote);
    if (err != HTTPQuote::OK) {
        Serial.print("Failed to get SPX price; Error code: ");
        Serial.println(err);
        return;
    }

    Serial.print("SPX price: ");
    Serial.println(quote.price);
    Serial.print("SPX previous close price: ");
    Serial.println(quote.previousClose);
    Serial.print("SPX \% change : ");
    Serial.println(quote.changeSincePreviousClose);

    epd.ClearFrame();

    fb.setRotation(3);
    fb.setTextColor(BLACK);

    fb.fillScreen(WHITE);
    fb.drawRect(180, 0, 120, 100, BLACK);
    fb.drawRect(180, 100, 120, 100, BLACK);
    fb.drawRect(180, 200, 120, 100, BLACK);
    fb.drawRect(180, 300, 120, 100, BLACK);
    fb.drawRect(0, 0, 180, 385, BLACK);
    fb.drawRect(0, 385, 180, 15, BLACK);

    fb.setTextSize(2);

    fb.setCursor(200, 2);
    fb.println("SPX:");
    fb.setCursor(190, 20);
    fb.print("$");
    fb.println(quote.price);
    fb.setCursor(190, 38);
    fb.print("%");
    fb.println(quote.changeSincePreviousClose);

    err = forex.fetchQuote("PLN", quote);
    if (err != HTTPQuote::OK) {
        Serial.print("Failed to get GBP/PLN price; Error code: ");
        Serial.println(err);
        return;
    }

    Serial.print("GBP/PLN exchange rate: ");
    Serial.println(quote.price);

    fb.setCursor(200, 102);
    fb.println("GBP2PLN:");
    fb.setCursor(190, 120);
    fb.print(quote.price);
    fb.println("zl");

    err = forex.fetchQuote("USD", quote);
    if (err != HTTPQuote::OK) {
        Serial.print("Failed to get GBP/USD price; Error code: ");
        Serial.println(err);
        return;
    }

    Serial.print("GBP/USD exchange rate: ");
    Serial.println(quote.price);

    fb.setCursor(200, 162);
    fb.println("GBP2USD:");
    fb.setCursor(190, 180);
    fb.print(quote.price);
    fb.println("$");

    fb.setCursor(2, 389);
    fb.setTextSize(1);
    time_t epoch = time(NULL);
    fb.println(asctime(localtime(&epoch)));

    //epd.SetPartialWindow(fb.getBuffer(), 0, 0, 100, 100);
    //epd.DisplayFrame();
    epd.DisplayFrame(fb.getBuffer());
    epd.Sleep();

    delay(UPDATE_INTERVAL_MS);
}