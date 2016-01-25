int battery_switch_pin = 7;
int pv_switch_pin = 6;

int high_voltage = 900;

bool on = HIGH;
bool off = LOW;

bool debug = LOW;

void setup() {
  pinMode(battery_switch_pin, OUTPUT);
  pinMode(pv_switch_pin, OUTPUT);

  Serial.begin(9600);

  Serial.println("Start");

  toggle_battery_switch(off);
  toggle_pv_switch(off);
}

void loop() {
  if (!load_connected())
  {
    toggle_pv_switch(on);
    if (!battery_voltage_high()) // This is to preserve the battery
    {
      toggle_battery_switch(on);
    }
    Serial.println("Load not connected");
  }else if (!load_voltage_high())
	{ 
//    Serial.println("Load connected");
		if (pv_voltage_high())
		{
      toggle_battery_switch(off);
			toggle_pv_switch(on);
		}
		else
		{
      toggle_pv_switch(off);
			toggle_battery_switch(on);
		}
	}

 if (debug){
    debug_msg();
  }
}

void debug_msg()
{
  Serial.print("PV panel efficiency: ");
//  Serial.print(100*analogRead(A5)/1023);
  long pv_eff = (100 * analogRead(A5))/1023;
  Serial.print(pv_eff);
  Serial.println(" %");
}

bool battery_voltage_high()
{
  return analogRead(A0) > high_voltage;
}

bool load_voltage_high()
{
  return analogRead(A1) > high_voltage;
}

bool pv_voltage_high()
{
  return analogRead(A2) > high_voltage;
}

bool control_voltage_high()
{
  return analogRead(A3) > high_voltage;
}

bool load_connected()
{
	return control_voltage_high() - load_voltage_high() == 0;
}

void toggle_pv_switch(bool state)
{
  digitalWrite(pv_switch_pin, !state);
}

void toggle_battery_switch(bool state)
{
  digitalWrite(battery_switch_pin, !state);
}

