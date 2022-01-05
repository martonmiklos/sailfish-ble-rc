# sailfish-ble-rc

![Icon](https://raw.githubusercontent.com/martonmiklos/sailfish-ble-rc/master/icons/172x172/sailfish-ble-rc.png "Sailfish BLE RC")

App for controlling a BLE toy cars from a Sailfish OS device

## Motivation
* The Android app is a bloatware: heats up the phone (maybe due to the 3D graphics), loads slowly
* Aliendalvik has no Bluetooth support on SFOS
* Reverse engineering is fun!

## Supported devices

* [Nissan Formula E manufactured by Brandbase used by Shell in promotional campagin](https://gist.github.com/martonmiklos/e960f17d7ba0f08327b0b588f9aae04b)
  * [Official Android app](https://play.google.com/store/apps/details?id=nl.brandbase.shellsupercars&hl=hu&gl=US)
* Ferrari models manufactured by Bburago sold by the Shell in a promotional campaign  
  * [Official Android app](https://play.google.com/store/apps/details?id=com.TDF.ShellRacingLegends&hl=hu&gl=US)

## Protocol info

### Brandbase
The Brandbase implementation is based on [@scrool's work](https://gist.github.com/scrool/e79d6a4cb50c26499746f4fe473b3768)

### Bburago
The Bburago implementation is more simple: it does not have AES encryption on payloads (neither on the command nor the battery notificaition). 
It had been reverse engineered by using [frida](https://frida.re/) and  [optiv's blemon frida plugin](https://github.com/optiv/blemon).

Protocol is quite simple:

Command is sent in 8 byte to the "0000fff1-0000-1000-8000-00805f9b34fb" characteristics see here for details:
https://github.com/martonmiklos/sailfish-ble-rc/blob/master/bburago_rc_car.cpp#L81

The battery notification is sent periodically through the "00002a19-0000-1000-8000-00805f9b34fb" characteristic in a single byte in percentage format. (You need to enable notifications to this characteristics see: [here]](https://github.com/martonmiklos/sailfish-ble-rc/blob/master/bburago_rc_car.cpp#L112))

## Issues
I got random disconnects on SFOS which investigation is still ongoing. See: https://forum.sailfishos.org/c/app-development/5
