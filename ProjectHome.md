## Description ##
LG TV has a serial port for sending commands.<br>
This small utility allows you to do just that, from command-line.<br>
Right now, it has most common commands and accepts a command-name and (enum)value. It retrieves the current value when value is omitted.<br>

<h2>Requirements / compatibility</h2>
You just need GCC and some standard headers, I think.<br>
You need to change /dev/tty.usbserial in the sourcecode to your serial port device.<br>
It is currently only tested on OS X.<br>
Throw me a diff if you get it to compile on linux/cygwin/mingw.<br>

<h2>Ideas / Todos</h2>
Need to scan undocumented commands. (some might whack things up)<br>
Add some standard/user color profiles. Don't know if there are advanced color commands.<br>
In the future, perhaps it will send power management commands and input selection based on the status of attached devices (PC, game console).<br>
It would be cool if it ARPed/PINGed your Xbox/PS3 MAC and switch to the right input automatically.<br>

<h2>Usage / example commands</h2>
Names are case-insensitive and you can type just a part (except tooloptions). e.g. 'volume' might match 'VolumeMute' and 'VolumeControl'.<br>
<pre><code>lg-tv-command osdselect 0        # disable OSD message cruft<br>
lg-tv-command remotelock 1       # we don't need the old IR remote anymore :)<br>
lg-tv-command input hdmi2        # this could be your PC/MAC<br>
lg-tv-command aspect justscan    # pixel-perfect please<br>
lg-tv-command saving off         # disable power saving mode<br>
lg-tv-command backlight 100      # burn your eyes!<br>
lg-tv-command ToolOption1        # get option1<br>
lg-tv-command kf 0x32            # two-character command. e.g. kf is VolumeControl.<br>
lg-tv-command -t                 # open terminal. then type "debug" or something.<br>
lg-tv-command -sl                # Open EZ-ADJUST menu.<br>
lg-tv-command -sk                # Open IN-START menu.<br>
lg-tv-command -sk                # Factory defaults (IN-STOP).<br>
</code></pre>

Note that sommand commands do not work for certain video inputs. Issue commands in the correct order.<br>

<h2>Troubleshooting</h2>
While trying out some non-listed commands, you might accidently make the serial port inaccessible. You can try different LG_BAUDRATE settings. Or you could try powering down or change the baudrate in the IN-START menu (via other means).<br>
To go to the IN-START menu without serial port commands, hold both MENU buttons (TV and remote) for 5 seconds and enter password 0000. (both OK buttons for EZ-ADJUST).<br>
From firmware v3.67, you can only do it with a special infrared command. You need an universal remote that has the "-/--" button or use the IR hack.<br>
<br>
<h2>IR hack</h2>
You can hook up an infrared LED to your speaker output of your computer (or better yet, an amplifier with headphone output). I hooked an infrared LED to my iMac and was powerful enough. Use a camera to check for infrared light and check the TV power LED if it accepts the signal. Adjust volume where needed. An MP3 player's output will probably not work, but if it does, you will need to hold the LED close to the TV's receiver.<br>
Also, if you do get the password dialog, the password was changed from 0000 to 0413 in newer models. Stop the infrared or take away the transmitter before entering the password.<br>
<br>
The IN-START command can be found as srvc-menu-menu.wav/mp3.<br>
The EZ-ADJUST command can be found as serv-ok-ok.wav/mp3.<br>
<br>
<h2>Disclaimer</h2>
You can do a lot of tweaking in the menu's and possibly bricking your TV, for which I will not be held responsible for. Some nice things: enable DTS passthrough by changing region, enable THX/THX Media Director, Divx, disable 3D warnings (not found yet unfortunately), ???, profit.<br>
Make sure the room is well lit when watching anime.<br>
<sup>_</sup>