/*
LG TV serial port command tool

Copyright (c) 2011, Rafael Vuijk
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

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
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdbool.h>
#include <limits.h>

#define READ_STATUS		0xFF

/*
 * list macros
 */

#define P1(p1,...)						p1
#define P2(p1,p2,...)					p2
#define P3(p1,p2,p3,...)				p3
#define P4(p1,p2,p3,p4,...)				p4

#define LENGTH2(list)					list##_LENGTH
#define LENGTH(list)					(sizeof(LIST(list)) / sizeof(LIST(list)[0]))
#define LIST(list)						list##_LIST
#define LIST_TYPE(list)					list##_TYPE

#define NIT(_,__)\
	_(__##_CODE, __##_ENUM, __##_STRUCT, __##_STYPE)

#define LIST_CODE(list)					LIST_TYPE(list)(P1)
#define LIST_ENUM(list)					LIST_TYPE(list)(P2)
#define LIST_STRUCT(list)				LIST_TYPE(list)(P3)
#define LIST_STYPE(list)				LIST_TYPE(list)(P4)

#define DEF_LIST(list)		\
	LIST(list)(LIST_CODE(list))	\
	enum { LIST(list)(LIST_ENUM(list)) LENGTH2(list) };	\
	LIST_STYPE(list) LIST(list)[] = { LIST(list)(LIST_STRUCT(list)) };

// allow 'recursion'
#define DEF_SUBLIST(list)		\
	LIST(list)(LIST_CODE(list))	\
	enum { LIST(list)(LIST_ENUM(list)) LENGTH2(list) };	\
	LIST_STYPE(list) LIST(list)[] = { LIST(list)(LIST_STRUCT(list)) };

/*
 * the list types
 */

typedef struct
{
	unsigned char value;
	const char *name;
} SubDef;

#define NAME_INDEX_CODE(m,i,...)
#define NAME_INDEX_ENUM(m,i,...)
#define NAME_INDEX_STRUCT(m,i,...)		{ i, #m },
#define NAME_INDEX_STYPE				SubDef

typedef struct
{
} Main2Def;

#define LISTNAME_CODE(l)				DEF_SUBLIST(l)
#define LISTNAME_ENUM(l)				l##_INDEX,
#define LISTNAME_STRUCT(l)
#define LISTNAME_STYPE					Main2Def

typedef struct
{
	const char *name;
	char cmd1;
	char cmd2;
	unsigned char min;
	unsigned char max;
	SubDef *list;
	int listlen;
} CmdDef;

#define NAME_CMD_LIST_CODE(m,c1,c2,min,max,l)
#define NAME_CMD_LIST_ENUM(m,c1,c2,min,max,l)		m##_INDEX,
#define NAME_CMD_LIST_STRUCT(m,c1,c2,min,max,l)		{ #m, c1, c2, min, max, LIST(l), LENGTH(l) },
#define NAME_CMD_LIST_STYPE							CmdDef



/*
 * the lists
 */
/*
#define INPUT_SELECT1_TYPE(_)		NIT(_,NAME_INDEX)
#define INPUT_SELECT1_LIST(_)\
	_(DTV,				0x00)\
	_(Analog,			0x01)\
	_(AV1,				0x02)\
	_(AV2,				0x03)\
	_(Component1,		0x04)\
	_(Component2,		0x05)\
	_(RGB,				0x07)\
	_(HDMI1,			0x08)\
	_(HDMI2,			0x09)\
*/
// add suffix and add value
#define M_(_,m,i)		\
	_(m##1,i+0)\
	_(m##2,i+1)\
	_(m##3,i+2)\
	_(m##4,i+3)\

#define INPUT_SELECT2_TYPE(_)		NIT(_,NAME_INDEX)
#define INPUT_SELECT2_LIST(_)\
	M_(_, DTV,				0x00)\
	M_(_, Analog,			0x10)\
	M_(_, AV,				0x20)\
	M_(_, Component,		0x40)\
	M_(_, RGB,				0x60)\
	M_(_, HDMI,				0x90)\

#define ASPECT_RATIO_TYPE(_)		NIT(_,NAME_INDEX)
#define ASPECT_RATIO_LIST(_)\
	_(_4_3,				0x01)\
	_(_16_9,			0x02)\
	_(Horizon,			0x03)\
	_(Zoom1,			0x04)\
	_(Zoom2,			0x05)\
	_(Original,			0x06)\
	_(_14_9,			0x07)\
	_(JustScan,			0x09)	/*pixel-perfect*/\
	_(FullWide,			0x0B)\
	_(CinemaZoom1,		0x10)\
	_(CinemaZoom2,		0x11)\
	_(CinemaZoom3,		0x12)\
	_(CinemaZoom4,		0x13)\
	_(CinemaZoom5,		0x14)\
	_(CinemaZoom6,		0x15)\
	_(CinemaZoom7,		0x16)\
	_(CinemaZoom8,		0x17)\
	_(CinemaZoom9,		0x18)\
	_(CinemaZoom10,		0x19)\
	_(CinemaZoom11,		0x1A)\
	_(CinemaZoom12,		0x1B)\
	_(CinemaZoom13,		0x1C)\
	_(CinemaZoom14,		0x1D)\
	_(CinemaZoom15,		0x1E)\
	_(CinemaZoom16,		0x1F)\

#define ISM_METHOD_TYPE(_)			NIT(_,NAME_INDEX)
#define ISM_METHOD_LIST(_)\
	_(Inversion,		1)\
	_(Orbiter,			2)\
	_(WhiteWash,		4)\
	_(Normal,			8)\

#define ENERGY_SAVING_TYPE(_)			NIT(_,NAME_INDEX)
#define ENERGY_SAVING_LIST(_)\
	_(Off,				0x00)\
	_(Minimum,			0x01)\
	_(Medium,			0x02)\
	_(Maximum,			0x03)\
	_(Auto,				0x04)\
	_(ScreenOff,		0x05)\
	_(IS_Low,			0x10)\
	_(IS_Middle,		0x11)\
	_(IS_High,			0x12)\

#define COLOR_TEMPERATURE_TYPE(_)		NIT(_,NAME_INDEX)
#define COLOR_TEMPERATURE_LIST(_)\
	_(Medium,			0)\
	_(Cool,				1)\
	_(Warm,				2)\
	_(User,				3)\

#define REMOTE_TYPE(_)				NIT(_,NAME_INDEX)
#define REMOTE_LIST(_)\
	_(CHAN_UP,		0x00)\
	_(CHAN_DOWN,	0x01)\
	_(VOL_UP,		0x02)\
	_(VOL_DOWN,		0x03)\
	_(RIGHT,		0x06)\
	_(LEFT,			0x07)\
	_(POWER,		0x08)\
	_(MUTE,			0x09)\
	_(SAP,			0x0A)\
	_(INPUT,		0x0B)	/*show/cycle input selection*/\
	_(TIMER,		0x0E)\
	_(TV,			0x0F)\
	_(_0,			0x10)\
	_(_1,			0x11)\
	_(_2,			0x12)\
	_(_3,			0x13)\
	_(_4,			0x14)\
	_(_5,			0x15)\
	_(_6,			0x16)\
	_(_7,			0x17)\
	_(_8,			0x18)\
	_(_9,			0x19)\
	_(BACK,			0x1A)\
	_(Q_VIEW,		0x1A)\
	_(FAV,			0x1E)\
	_(TEXT,			0x20)\
	_(T_OPTION,		0x21)\
	_(TEXT_ZOOM,	0x21)\
	_(TEXT_MODE,	0x22)\
	_(TEXT_MIX,		0x24)\
	_(TEXT_TIME,	0x26)\
	_(TEXT_RANGE,	0x27)\
	_(RETURN_EXIT,	0x28)\
	_(REVEAL,		0x2A)\
	_(TEXT_UNK,		0x2B)\
	_(AV_MODE,		0x30)	/*show/cycle picture modes: Cinema/Sport/Game/Off(=vivid/natural/expert)*/\
	_(SUBTITLE,		0x39)\
	_(UP,			0x40)\
	_(DOWN,			0x41)\
	_(MENU,			0x43)\
	_(OK,			0x44)	/*OK*/\
	_(ENTER,		0x44)	/*OK*/\
	_(DASH,			0x4C)\
	_(PICTURE,		0x4D)	/*show/cycle picture mode*/\
	_(SOUND,		0x52)\
	_(PROG_LIST,	0x53)\
	_(AV1, 			0x5A)\
	_(EXIT,			0x5B)\
	_(PIP,			0x60)\
	_(PIP_INPUT,	0x61)\
	_(BLUE_KEY,		0x61)\
	_(PIP_POSITION,	0x62)\
	_(YELLOW_KEY,	0x63)\
	_(FREEZE,		0x65)\
	_(PIP_SIZE,		0x64)\
	_(HOLD,			0x65)\
	_(INDEX,		0x70)\
	_(PIP_CHAN_DOWN,0x71)\
	_(GREEN_KEY,	0x71)\
	_(PIP_CHAN_UP,	0X72)\
	_(RED_KEY,		0x72)\
	_(RATIO_4_3,	0x76)\
	_(RATIO_16_9,	0x77)\
	_(RATIO,		0x79)	/*show/cycle aspect ratio mode*/\
	_(SIMPLINK,		0x7E)\
	_(ENERGY,		0x95)	/*show/cycle energy saving mode*/\
	_(HDMI3,		0x98)\
	_(INFO,			0xAA)\
	_(GUIDE,		0xAB)\
	_(RATIO_ZOOM,	0xAF)\
	_(COMPONENT1,	0xBF)\
	_(POWER_ON,		0xC4)\
	_(POWER_OFF,	0xC5)\
	_(HDMI,			0xC6)	/*cycles through HDMI inputs*/\
	_(ADJUST,		0xCB)	/*show position adjustment menu*/\
	_(HDMI1,		0xCE)\
	_(HDMI2,		0xCC)\
	_(AV2,			0xD0)\
	_(AV3,			0xD1)\
	_(AV4,			0xD2)\
	_(SVIDEO2,		0xD3)\
	_(COMPONENT2,	0xD4)\
	_(RGB,			0xD5)\
	_(TV,			0xD6)\
	_(RGB2,			0xD7)\
	_(BRIGHT_UP,	0xE0)\
	_(BRIGHT_DOWN,	0xE1)\
	_(TV_RADIO,		0xF0)\

#define LISTS_TYPE(_)			NIT(_,LISTNAME)
#define LISTS_LIST(_)\
	/*_(INPUT_SELECT1)*/\
	_(INPUT_SELECT2)\
	_(ASPECT_RATIO)\
	_(ISM_METHOD)\
	_(ENERGY_SAVING)\
	_(COLOR_TEMPERATURE)\
	_(REMOTE)\

DEF_LIST(LISTS)

#define _LIST			NULL

#define CMD_TYPE(_)			NIT(_,NAME_CMD_LIST)
#define CMD_LIST(_)\
	_(ToolOption1,		'a','b',0,0,)\
	_(ToolOption2,		'a','c',0,0,)\
	_(ToolOption3,		'a','f',0,0,)\
	_(ToolOption4,		'a','g',0,0,)\
	_(Power,			'k','a',0,1,)\
	/*_(InputSelect1,		'k','b',0,0,INPUT_SELECT1)*/\
	_(AspectRatio,		'k','c',0,0,ASPECT_RATIO)\
	_(ScreenMute,		'k','d',0,1,)\
	_(VolumeMute,		'k','e',0,1,)\
	_(VolumeControl,	'k','f',0,100,)\
	_(Contrast,			'k','g',0,100,)\
	_(Brightness,		'k','h',0,100,)\
	_(Color,			'k','i',0,100,)\
	_(Tint,				'k','j',0,100,)\
	_(Sharpness,		'k','k',0,100,)\
	_(OSDSelect,		'k','l',0,1,)\
	_(RemoteLock,		'k','m',0,1,)\
	_(Treble,			'k','r',0,100,)\
	_(Bass,				'k','s',0,100,)\
	_(Balance,			'k','t',0,100,)\
	_(ColorTemperature,	'k','u',0,0,COLOR_TEMPERATURE)\
	_(ISMMethod,		'j','p',0,0,ISM_METHOD)\
	_(EnergySaving,		'j','q',0,0,ENERGY_SAVING)\
	_(AutoConfiguration,'j','u',1,1,)\
	_(ChannelTuning,	'm','a',0,0,)/*TODO: extra data bytes*/\
	_(ChannelAddDel,	'm','b',0,1,)\
	_(Key,				'm','c',0,0,REMOTE)\
	_(ControlBackLight,	'm','g',0,100,)\
	_(InputSelect,		'x','b',0,0,INPUT_SELECT2)\

DEF_LIST(CMD)


/*
 * Global
 */

#define TIMEOUT		1.0

int fd = -1;		// filedescriptor for serial port
char *device;
int baudrate = 9600;
int set_id = 0;		// 0 = all devices

struct termios tios_stdin;

/*
 * Clean up at exit
 */
void AtExit()
{
	// Close serial port
	if (fd != -1) close(fd);

	// Restore stdin on exit
    tcsetattr(fileno(stdin), TCSANOW, &tios_stdin);
}

/*
 * Set unbuffered stdin/stdout
 */
void InitStdio()
{
    tcgetattr(fileno(stdin), &tios_stdin);
	atexit(AtExit);

    struct termios ts;
    tcgetattr(fileno(stdin), &ts);
    ts.c_lflag &=~ (ICANON | ECHO /*| ISIG*/);
    ts.c_cc[VMIN] = 0;
    ts.c_cc[VTIME] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &ts);

    setbuf(stdout, 0);
}

/*
 * Open serial port
 */
void InitSerial()
{
	if (fd != -1) return;	// already opened
	
	if (device == NULL) { fprintf(stderr, "You must set LG_DEVICE to the serial port connection device.\n"); exit(1); }
    fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd < 0) { fprintf(stderr, "Cannot open '%s'\n", device); exit(1); }
    tcflush(fd, TCIOFLUSH);

	struct termios tios;
    memset(&tios, 0, sizeof(tios));
    cfsetispeed(&tios, baudrate);
    cfsetospeed(&tios, baudrate);
    tios.c_cflag = CS8 | CLOCAL | CREAD;
    tios.c_iflag = IGNPAR | IGNBRK | IXANY;
    tios.c_lflag = ISIG;
    tios.c_cc[VMIN] = 0;
    tios.c_cc[VTIME] = 0;
    tcsetattr(fd, TCSANOW, &tios);

	tcflush(fd, TCIOFLUSH);
	write(fd, "\r", 1);		// reset, wake up, whatever
	//tcdrain(fd);
}

/*
 * Send command to LG TV
 *
 * value can be READ_STATUS to read current value
 *
 * return:
 * -256 = timeout/unknown response
 * +n = OK
 * -n = NG
 *   -1 illegal code
 *   -2 not supported function
 *   -3 wait more time
 *   hmm.. really?
 */
int SendCommand(char cmd1, char cmd2, int value)
{
	InitSerial();
	
	char cmd[20];
	int len;
	if (value >= 0x100)
		len = sprintf(cmd, "%c%c %02x %02x %02x\r", cmd1, cmd2, set_id, value>>8, value&255);
	else
		len = sprintf(cmd, "%c%c %02x %02x\r", cmd1, cmd2, set_id, value);
	//printf("%s\n", cmd);
	write(fd, cmd, len);
	tcdrain(fd);
	memset(cmd, 0, sizeof(cmd));
	char *p = cmd;

	struct timeval tv1; gettimeofday(&tv1, NULL);
	double t1 = tv1.tv_sec + (tv1.tv_usec / 1000000.0);
	
	while (true)
	{
        struct timeval tv2; gettimeofday(&tv2, NULL);
        double t2 = tv2.tv_sec + (tv2.tv_usec / 1000000.0);
        if (t2 > t1 + TIMEOUT)
		{
			fprintf(stderr, "Response timeout: %s\n", cmd);
			return -256;
		}

		// try reading a character
		int n = read(fd, p, 1);
		if (n < 0)
			usleep(1000);	//{ fprintf(stderr, "Could not read from device.\n"); exit(1); }
        else if (n == 0)
			usleep(1000);
		else if (p - cmd < sizeof(cmd))
        {
			// fix for missing first response character...
			if (p == cmd && cmd[0] == ' ') { cmd[1] = cmd[0]; cmd[0] = cmd2; p++; }

			if (*p == 'x')		// there's no real newline in response. hopefully cmd2 is never a 'x'
			{
				//int nbytes = (p - cmd - 7) / 2;
				char cmd2r;
				int id;
				char ok_ng[2];
				int data;
				if (sscanf(cmd, "%c %02x %2c%x", &cmd2r, &id, ok_ng, &data) >= 4)
				{
					if (cmd2r == cmd2)
					{
						if (ok_ng[0] == 'O' && ok_ng[1] == 'K')
						{
							if ((value == READ_STATUS) || (value == data))
							{
	//							printf("OK %02X\n", value);
								return data;
							}
						}
						else if (ok_ng[0] == 'N' && ok_ng[1] == 'G')
						{
	//						fprintf(stderr, "NG %02X\n", data);
							return -data;
						}
					}
					fprintf(stderr, "Unknown response: %s\n", cmd);
					return -256;
				}
			}
			p++;
		}
	}
}

/*
 * Make string lowercase etc.
 */
void strfix(char *str)
{
	char *x = str;
	for (; *x; x++)
	{
		if (*x >= 'A' && *x <= 'Z') *x += 'a' - 'A';
		if (*x <= ' ') *x = '_';
	}
	x = strstr(str, "colour");
	if (x != NULL) { strcpy(x, "color"); strcat(x, x + 6); }
}

/*
 * Simple terminal
 */
int Terminal()
{
	InitSerial();
	while (true)
	{ 
		char p[100];

		int n = read(fd, p, sizeof(p));
        if (n > 0)
        {
			write(STDOUT_FILENO, p, n);
			continue;
		}

		n = read(STDIN_FILENO, p, sizeof(p));
		if (n > 0)
		{
			write(fd, p, n);
			continue;
		}

		usleep(1000);
	}
	return 0;
}

/*
 *
 */
int Sequence(char *s)
{
	InitSerial();
	for (; *s; s++)
	{
		if (*s == '.')
			usleep(100*1000);
		else
			write(fd, s, 1);
	}
	return 0;
}

/*
 * main
 */
int main(int argc, char *argv[])
{
	InitStdio();

	device = getenv("LG_DEVICE");
	char *baudrate_str = getenv("LG_BAUDRATE");
	if (baudrate_str != NULL) baudrate = strtoul(baudrate_str,0,0);
	char *set_id_str = getenv("LG_ID");
	if (set_id_str != NULL) set_id = strtoul(set_id_str,0,0);
	
	char *argv_cmd = (argc >= 2) ? argv[1] : NULL;
	char *argv_value = (argc >= 3) ? argv[2] : NULL;
	
	char cmd1 = -1, cmd2 = -1;
	int value = -1;

	// Special features
	if ((argv_cmd != NULL) && (strcmp(argv_cmd, "-t") == 0)) return Terminal();
	if ((argv_cmd != NULL) && (strcmp(argv_cmd, "-sl") == 0)) return Sequence("\033.x.\r.debug.\r.u.l..........0.4.1.3.....\033.x.");	// EZ ADJUST
	if ((argv_cmd != NULL) && (strcmp(argv_cmd, "-sk") == 0)) return Sequence("\033.x.\r.debug.\r.u.k..........0.4.1.3.....\033.x.");	// IN-START
	if ((argv_cmd != NULL) && (strcmp(argv_cmd, "-ss") == 0)) return Sequence("\033.x.\r.debug.\r.u.s..........0.4.1.3.....\033.x.");	// IN-STOP

	/*
	 * Show usage
	 */
	if ((argv_cmd == NULL) && (argv_value == NULL))
	{
		printf("lg-tv-command (" __DATE__ ")\n");
		printf("Usage:     lg-tv-command [command] [value]     To get/set value.\n");
		printf("           lg-tv-command -t                    Enter terminal.\n");
		printf("           lg-tv-command -sl                   Open EZ-ADJUST menu.\n");
		printf("           lg-tv-command -sk                   Open IN-START menu.\n");
		printf("           lg-tv-command -ss                   Factory defaults (IN-STOP).\n");
		printf("Settings:  LG_DEVICE=%s, ", device);
		printf("LG_BAUDRATE=%d, ", baudrate);
		printf("LG_ID=%d\nCommands:\n", set_id);
		LIST_STYPE(CMD) *commands = LIST(CMD);
		for (int l=0; l<LENGTH(CMD); l++)
		{
			printf("  %s\n", commands[l].name);
		}
		return 1;
	}

	/*
	 * Determine command by matching enum
	 */
	LIST_STYPE(CMD) *command = NULL;
	if (strlen(argv_cmd) >= 3)
	{
		char *argv_cmd2 = strdup(argv_cmd);
		strfix(argv_cmd2);
		LIST_STYPE(CMD) *commands = LIST(CMD);
		for (int l=0; l<LENGTH(CMD); l++)
		{
			char *name2 = strdup(commands[l].name);
			strfix(name2);
			// match substring when it's not a dangerous command
			if (((commands[l].cmd1 != 'a') && (strstr(name2, argv_cmd2) != NULL)) ||
				(strcmp(name2, argv_cmd2) == 0))
			{
				cmd1 = commands[l].cmd1;
				cmd2 = commands[l].cmd2;
				command = &commands[l];
				break;
			}
			free(name2);
		}
	}
	if ((strlen(argv_cmd) == 2) && (command == NULL))
	{
		cmd1 = argv_cmd[0];
		cmd2 = argv_cmd[1];
	}
	if ((cmd1 <= 0) || (cmd2 <= 0))
	{
		printf("Invalid command.\n");
		return 1;
	}
	
	SubDef *list = (command != NULL) ? command->list : NULL;

	/*
	 * Show current value and possible values
	 */
	if (argv_value == NULL)
	{
		if (argv_value == NULL) if (command != NULL) printf("%s:\n\n", command->name);
		int r = SendCommand(cmd1, cmd2, READ_STATUS);
		if (r >= 0)
		{
			printf("Current value: %u (0x%02X).", r, r);
			if (command != NULL) if ((command->min != 0) || (command->max != 0))
				printf(" Range: %u-%u.", command->min, command->max);
			printf("\n\n");
		}
		else
		{
			printf("Response error %d.\n\n", r);
		}
		
		if (list != NULL)
		{
			for (int s=0; s<command->listlen; s++)
			{
				if (r == list[s].value)
					printf("=>");
				else
					printf("  ");
				printf("%s\n", list[s].name);
			}
		}
		return 0;
	}

	/*
	 * Determine value by matching enum
	 */
	if (list != NULL)
	{
		int best_length = INT_MAX;	// best length is smallest
		char *argv_value2 = strdup(argv_value);
		strfix(argv_value2);
		for (int s=0; s<command->listlen; s++)
		{
			char *c = strdup(list[s].name);
			strfix(c);
			if ((strstr(c, argv_value2) != NULL) && (strlen(c) < best_length))
			{
				value = list[s].value;
				best_length = strlen(c);
			}
			free(c);
		}
		free(argv_value2);
	}
	if (value < 0)		// no enum or it did not match
	{
		char *endp = argv_value;
		value = strtoul(argv_value, &endp, 0);
		if (endp == argv_value) value = -1;		// did not parse
	}
	if (value < 0)
	{
		printf("Invalid value.\n");
		return -1;
	}

	int r = SendCommand(cmd1, cmd2, value);
	if (r >= 0)
	{
		//printf("OK!\n");
		return 0;
	}

	return -r;		// make positive again
}
