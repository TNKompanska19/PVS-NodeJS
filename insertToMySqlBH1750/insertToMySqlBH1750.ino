#include <MySQL_Generic.h>
#include <MySQL_Generic.hpp>
 
#include <Wire.h>
#include <SPI.h>
#include "DHT.h"

DHT dht2(2,DHT11);
 
// MySQL Debug Level from 0 to 4
//#define _MYSQL_LOGLEVEL_      1
//#define MYSQL_DEBUG_PORT      Serial
#define DHTPIN 2
 
// Network Info
char ssid[] = "OS";
char pass[] = "123456789";
 
 
// Database Adress
char server[] = "192.168.0.1";
uint16_t server_port = 3306;
 
 
// Database Account
char user[] = "html";
char password[] = "password";
 

//Database Name
char database[] = "sensors";
char table[] = "meteorological_station";
 
// Default insert data values
int distance;
 
// MySQL something
MySQL_Connection conn((Client *)&client);
MySQL_Query *query_mem;
 
void setup()
{
  // Initialize status LED
  pinMode(LED_BUILTIN, OUTPUT);
  // Setup Serial
  Serial.begin(115200);
  dht2.begin();
  // Begin WiFi section
  Serial.println("================================================");
  Serial.println(String("Connecting to ") + ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
 
  // Print out info about the WiFi connection
  Serial.print("Connected to network. My IP address is: ");
  Serial.println(WiFi.localIP());
 
  // Test MySQL connection
  Serial.print("Connecting to SQL Server @ ");
  Serial.print(server);
  Serial.println(String(", Port = ") + server_port);
  Serial.println(String("User = ") + user + String(", PW = ") + password + String(", DB = ") + database);
}
 
 
void loop()
{
  // Turn on status LED
  digitalWrite(LED_BUILTIN, LOW);
 
  // Measuring data
  Serial.println("================================================");
  Serial.println("Measuring data...");
  delay(500);
  Serial.println();
 
   float h = dht2.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht2.readTemperature();
 
  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.println("Temperature in C:");  
  Serial.println(dht2.computeHeatIndex(t, h, false));
 
  Serial.println("================================================");
 
  // change Insert Query with new data
  //String INSERT_SQL = String("UPDATE TABLE ") + database + "." + table + " (light, temperature, humidity) VALUES (" + light + "," + dht2.computeHeatIndex(t, h, false) + "," + h +")";
  String UPDATE_SQL 
  // Insert data in to database
  Serial.println("Connecting to MySQL...");
  Serial.println();
  if (conn.connectNonBlocking(server, server_port, user, password) != RESULT_FAIL)
  {
    delay(500);
    // Initiate the query class instance
    MySQL_Query query_mem = MySQL_Query(&conn);
 
    if (conn.connected())
    {
      Serial.println(INSERT_SQL);
 
      // Execute the query
      // KH, check if valid before fetching
      if ( !query_mem.execute(INSERT_SQL.c_str()) )
        Serial.println("Insert error");
      else
        Serial.println("Data Inserted.");
    }
    else
    {
      Serial.println("Disconnected from Server. Can't insert.");
    }
    conn.close();                     // close the connection
  }
  else
  {
    Serial.println("\nConnect failed. Trying again on next iteration.");
  }
 
  // Turn off status LED
  digitalWrite(LED_BUILTIN, HIGH);
 
  // Sleep till next data measure and insert
  Serial.println("================================================");
  Serial.println("\nSleeping...");
  delay(5000);
}
