#include <OneWire.h>
#include <DallasTemperature.h>

// conectar os dados de temperatura no pino 2 do Arduino
#define ONE_WIRE_BUS 2

// resolução em 0,5°C(9) 
#define TEMPERATURE_PRECISION 9 //0.5°[9], 0.25°C[10], 0.125°C[11], and 0.0625°C[12].
#define DEVICE_DISCONNECTED_C -120

// instanciando oneWire para que qualquer dispositivo de fio único (onewired) se comunique com o Arduino
OneWire oneWire(ONE_WIRE_BUS);

// passando objeto oneWire como referênca para o DallasTemperature
DallasTemperature sensores(&oneWire);

// vetor que terá o endereço dos sensores
DeviceAddress termCab, termHab, termExt, termMotor;

//------------- 
void printData(DeviceAddress deviceAddress)
{

  lendoTemperatura(deviceAddress);
  Serial.println();
}

// ------------- função que imprime a temperatura no Monitor Serial-----------
void lendoTemperatura(DeviceAddress deviceAddress)
{
  float tempC = sensores.getTempC(deviceAddress);
  String sensor;

  if(tempC == DEVICE_DISCONNECTED_C) 
  {
    Serial.println("Erro: Nenhum sensor encontrado");
    return;
  }
  else
  {
    if (sensores.getAddress(termCab, 0))
    {
      sensor = "da cabine";
    }
    if (sensores.getAddress(termHab, 1))
    {
      sensor = "do habitáculo";
    }
    if (sensores.getAddress(termExt, 2))
    {
      sensor = "externa";
    }
    if (sensores.getAddress(termMotor, 3))
    {
      sensor = "do motor";
    }
  
    Serial.print("Temperatura ");
    Serial.print(sensor);
    Serial.print(" = ");
    Serial.print(tempC);
  }
}

//-----------------SETUP-------------------------------------------------------

void setup() {
  Serial.begin(9600);  //iniciando porta serial para usarmos o Monitor Serial
  Serial.println("Sensores de temperatura para KombiHome");

  sensores.begin();  // comando para inicializar a biblioteca

  // localizar os sensores
  Serial.print("Localizando sensores...");
  Serial.print("Encontrado ");
  Serial.print(sensores.getDeviceCount(), DEC);  // conta quantos sensores na rede 
  Serial.println(" sensores de temperatura.");

  // Obtendo o endereço dos sensores
  if (!sensores.getAddress(termCab, 0)) Serial.println("Endereço do term. da cabine não encontrado!");
  if (!sensores.getAddress(termHab, 1)) Serial.println("Endereço do term. do habitáculo não encontrado!");
  if (!sensores.getAddress(termExt, 2)) Serial.println("Endereço do term. externo não encontrado!");
  if (!sensores.getAddress(termMotor, 3)) Serial.println("Endereço do term. do motor não encontrado!");
  Serial.println();

  sensores.setResolution(termCab, TEMPERATURE_PRECISION);
  sensores.setResolution(termHab, TEMPERATURE_PRECISION);
  sensores.setResolution(termExt, TEMPERATURE_PRECISION);
  sensores.setResolution(termMotor, TEMPERATURE_PRECISION);

}

//-----------------LOOP-------------------------------------------------------

void loop(void)
{
  
  sensores.requestTemperatures(); // solicitando todas as temperaturas do sensores nos barramento.
  delay(500);
  printData(termCab);
  delay(500);
  printData(termHab);
  delay(500);
  printData(termExt);
  delay(500);
  printData(termMotor);
  delay(500);
}
