This contains esp8266ex development as an alternative to [latest official](https://github.com/SafeVisionID/esp-latestcode) development.
This hopes can be scale-up as implementation/requirement applied.

Based on [ESP-Open-SDK](https://github.com/pfalcon/esp-open-sdk) which in turn based on Espressif's [Non-OS SDK](https://github.com/espressif/ESP8266_NONOS_SDK)
To install required SDK:
- Arch Linux, available at [AUR](https://aur.archlinux.org/packages/esp-open-sdk-git/)
- Others, follow this [guide](https://github.com/pfalcon/esp-open-sdk/blob/master/README.md)

Fearture and To-Do list:
- [x] Basic LED Blink
- [x] UART Boot Info
- [x] UART Shell and Response/Callback
- [x] Basic HTTP Server
	- [x] Basic HTML Response
	- [x] Serial Out Response
	- [] Receive Serial Request
	- [] Reset Request
- [x] Wifi Station mode
	- [x] Config and Save Password
	- [] Config and Save SSID
- [] Wifi SoftAP mode
	- [] Config and Save Password
	- [] Config and Save SSID
- [] Switch Statio and SoftAP
	- [] by HTTP Request
	- [] by GPIO pin
- [] Sensor Interface
	- [] GPIO Logic
	- [] ADC
- [] Sleep and WakeUp
	- [] by RTC/Timer
	- [] by Interrupt GPIO 
	- [] by Sensor Trigger
