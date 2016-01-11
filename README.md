# lg\_tv\_command

cloned from https://code.google.com/p/lg-tv-command/


## Description

LG TV has a serial port for sending commands.
This small utility allows you to do just that, from command-line.
Right now, it has most common commands and accepts a command-name and (enum)value. It retrieves the current value when value is omitted.

## Requirements / compatibility

You just need GCC and some standard headers, I think.
You need to change /dev/tty.usbserial in the sourcecode to your serial port device.
It is currently only tested on OS X.
Throw me a diff if you get it to compile on linux/cygwin/mingw.

## Ideas / Todos

Need to scan undocumented commands. (some might whack things up)
Add some standard/user color profiles. Don't know if there are advanced color commands.
In the future, perhaps it will send power management commands and input selection based on the status of attached devices (PC, game console).
It would be cool if it ARPed/PINGed your Xbox/PS3 MAC and switch to the right input automatically.

## Usage / example commands

Names are case-insensitive and you can type just a part (except tooloptions). e.g. 'volume' might match 'VolumeMute' and 'VolumeControl'.

```sh
lg-tv-command osdselect 0        # disable OSD message cruft
lg-tv-command remotelock 1       # we don't need the old IR remote anymore :)
lg-tv-command input hdmi2        # this could be your PC/MAC
lg-tv-command aspect justscan    # pixel-perfect please
lg-tv-command saving off         # disable power saving mode
lg-tv-command backlight 100      # burn your eyes!
lg-tv-command ToolOption1        # get option1
lg-tv-command kf 0x32            # two-character command. e.g. kf is VolumeControl.
lg-tv-command -t                 # open terminal. then type "debug" or something.
lg-tv-command -sl                # Open EZ-ADJUST menu.
lg-tv-command -sk                # Open IN-START menu.
lg-tv-command -sk                # Factory defaults (IN-STOP).
```
Note that sommand commands do not work for certain video inputs. Issue commands in the correct order.

## Troubleshooting

While trying out some non-listed commands, you might accidently make the serial port inaccessible. You can try different LG_BAUDRATE settings. Or you could try powering down or change the baudrate in the IN-START menu (via other means).
To go to the IN-START menu without serial port commands, hold both MENU buttons (TV and remote) for 5 seconds and enter password 0000. (both OK buttons for EZ-ADJUST).
From firmware v3.67, you can only do it with a special infrared command. You need an universal remote that has the "-/--" button or use the IR hack.

## IR hack

You can hook up an infrared LED to your speaker output of your computer (or better yet, an amplifier with headphone output). I hooked an infrared LED to my iMac and was powerful enough. Use a camera to check for infrared light and check the TV power LED if it accepts the signal. Adjust volume where needed. An MP3 player's output will probably not work, but if it does, you will need to hold the LED close to the TV's receiver.
Also, if you do get the password dialog, the password was changed from 0000 to 0413 in newer models. Stop the infrared or take away the transmitter before entering the password. 
The IN-START command can be found as srvc-menu-menu.wav/mp3. The EZ-ADJUST command can be found as serv-ok-ok.wav/mp3.

## Disclaimer

You can do a lot of tweaking in the menu's and possibly bricking your TV, for which I will not be held responsible for. Some nice things: enable DTS passthrough by changing region, enable THX/THX Media Director, Divx, disable 3D warnings (not found yet unfortunately), ???, profit. Make sure the room is well lit when watching anime. _

## License
Copyright (c) 2011, Rafael Vuijk
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* Neither the name of the <organization> nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

http://opensource.org/licenses/BSD-3-Clause
