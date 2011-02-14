// vim lg-tv-command.c && gcc -std=gnu99 -Wall lg.c -o lg-tv-command && ./lg-tv-command

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

#define DEF_SUBLIST(list)		\
	LIST(list)(LIST_CODE(list))	\
	enum { LIST(list)(LIST_ENUM(list)) LENGTH2(list) };	\
	LIST_STYPE(list) LIST(list)[] = { LIST(list)(LIST_STRUCT(list)) };

/*
 * the list types
 */

typedef struct
{
	unsigned char i;
	const char *name;
} SubDef;

#define NAME_INDEX_CODE(m,i,...)
#define NAME_INDEX_ENUM(m,i,...)		//m = i,
#define NAME_INDEX_STRUCT(m,i,...)		{ i, #m },
#define NAME_INDEX_STYPE				SubDef

typedef struct
{
	//const char *name;
	//SubDef *list;
} Main2Def;

#define LISTNAME_CODE(l)				DEF_SUBLIST(l)
#define LISTNAME_ENUM(l)				l##_INDEX,
#define LISTNAME_STRUCT(l)				//{ #l, LIST(l) },
#define LISTNAME_STYPE					Main2Def

typedef struct
{
	const char *name;
	char cmd1;
	char cmd2;
	SubDef *list;
	int listlen;
} CmdDef;

#define NAME_CMD1_CMD2_LIST_CODE(m,c1,c2,l)
#define NAME_CMD1_CMD2_LIST_ENUM(m,c1,c2,l)			m##_INDEX,
#define NAME_CMD1_CMD2_LIST_STRUCT(m,c1,c2,l)		{ #m, c1, c2, LIST(l), LENGTH(l) },
#define NAME_CMD1_CMD2_LIST_STYPE					CmdDef



/*
 * the lists
 */

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

#define INPUT_SELECT2_TYPE(_)		NIT(_,NAME_INDEX)
#define INPUT_SELECT2_LIST(_)\
	_(DTVAntenna,		0x00)\
	_(DTVCable,			0x01)\
	_(AnalogAntenna,	0x10)\
	_(AnalogCable,		0x11)\
	_(AV1,				0x20)\
	_(AV2,				0x21)\
	_(Component1,		0x40)\
	_(Component2,		0x41)\
	_(RGB,				0x60)\
	_(HDMI1,			0x90)\
	_(HDMI2,			0x91)\

#define ASPECT_RATIO_TYPE(_)		NIT(_,NAME_INDEX)
#define ASPECT_RATIO_LIST(_)\
	_(_4_3,				1)\
	_(_16_9,			2)\
	_(Horizon,			3)\
	_(Zoom1,			4)\
	_(Zoom2,			5)\
	_(Original,			6)\
	_(_14_9,			7)\
	_(JustScan,			9)\

#define ISM_METHOD_TYPE(_)			NIT(_,NAME_INDEX)
#define ISM_METHOD_LIST(_)\
	_(Inversion,		1)\
	_(Orbiter,			2)\
	_(WhiteWash,		4)\
	_(Normal,			8)\

#define COLOR_TEMPERATURE_TYPE(_)		NIT(_,NAME_INDEX)
#define COLOR_TEMPERATURE_LIST(_)\
	_(Medium,			0)\
	_(Cool,				1)\
	_(Warm,				2)\
	_(User,				3)\

#define REMOTE_TYPE(_)				NIT(_,NAME_INDEX)
#define REMOTE_LIST(_)\
	_(TV,			0x0F)\
	_(INPUT,		0x0B)\
	_(POWER,		0x08)\
	_(RATIO,		0x79)\
	_(TIMER,		0x0E)\
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
	_(DASH,			0x4C)\
	_(BACK,			0x1A)\
	_(MUTE,			0x09)\
	_(VOL_UP,		0x02)\
	_(VOL_DOWN,		0x03)\
	_(CHAN_UP,		0x00)\
	_(CHAN_DOWN,	0x01)\
	_(FAV,			0x1E)\
	_(SIMPLINK,		0x7E)\
	_(CC,			0x39)\
	_(UP,			0x40)\
	_(DOWN,			0x41)\
	_(LEFT,			0x07)\
	_(RIGHT,		0x06)\
	_(ENTER,		0x44)\
	_(MENU,			0x43)\
	_(SOUND,		0x52)\
	_(PICTURE,		0x4D)\
	_(EXIT,			0x5B)\
	_(SAP,			0x0A)\
	_(ADJUST,		0xCB)\
	_(BRIGHT_DOWN,	0xE1)\
	_(BRIGHT_UP,	0xE0)\
	_(TV,			0xD6)\
	_(POWER_ON,		0xC4)\
	_(POWER_OFF,	0xC5)\
	_(AV1, 			0x5A)\
	_(AV2,			0xD0)\
	_(COMPONENT1,	0xBF)\
	_(COMPONENT2,	0xD4)\
	_(RGB,			0xD5)\
	_(HDMI1,		0xCE)\
	_(HDMI2,		0xCC)\
	_(RATIO_4_3,	0x76)\
	_(RATIO_16_9,	0x77)\
	_(RATIO_ZOOM,	0xAF)\

#define LISTS_TYPE(_)			NIT(_,LISTNAME)
#define LISTS_LIST(_)\
	_(INPUT_SELECT1)\
	_(INPUT_SELECT2)\
	_(ASPECT_RATIO)\
	_(ISM_METHOD)\
	_(COLOR_TEMPERATURE)\
	_(REMOTE)\

DEF_LIST(LISTS)

#define _LIST			NULL

#define CMD_TYPE(_)			NIT(_,NAME_CMD1_CMD2_LIST)
#define CMD_LIST(_)\
	_(Power,			'k','a',)\
	/*_(InputSelect,		'k','b',INPUT_SELECT1)*/\
	_(AspectRatio,		'k','c',ASPECT_RATIO)\
	_(ScreenMute,		'k','d',)\
	_(VolumeMute,		'k','e',)\
	_(VolumeControl,	'k','f',)\
	_(Contrast,			'k','g',)\
	_(Brightness,		'k','h',)\
	_(Color,			'k','i',)\
	_(Tint,				'k','j',)\
	_(Sharpness,		'k','k',)\
	_(OSDSelect,		'k','l',)\
	_(RemoteLock,		'k','m',)\
	_(Treble,			'k','r',)\
	_(Bass,				'k','s',)\
	_(Balance,			'k','t',)\
	_(ColorTemperature,	'k','u',COLOR_TEMPERATURE)\
	_(ISMMethod,		'j','p',ISM_METHOD)\
	_(LowPower,			'j','q',)\
	_(ChannelTuning,	'm','a',)/*TODO: extra data bytes*/\
	_(ChannelAddDel,	'm','b',)\
	_(Key,				'm','c',REMOTE)\
	_(InputSelect2,		'x','b',INPUT_SELECT2)\

DEF_LIST(CMD)


/*
 *
 */

#define TIMEOUT		1.0
const int set_id = 1;

int fd;		// filedescriptor for serial port

struct termios tios_stdin;

void ResetTerminal()
{
    tcsetattr(fileno(stdin), TCSANOW, &tios_stdin);
}


/*
 * value 0xFF = read status
 *
 * -256 = timeout
 * +n = OK
 * -n = NG
 *   -1 illegal code
 *   -2 not supported function
 *   -3 wait more time
 */
int SendCommand(char cmd1, char cmd2, unsigned char value)
{
	char cmd[20];
	sprintf(cmd, "%c%c %02x %02x\r", cmd1, cmd2, set_id, (int)value);
//	printf("%s\n", cmd);
 	write(fd, cmd, strlen(cmd));
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
			printf("timeout\n");
			return -256;
		}

        if (read(fd, p, 1) >= 1) if (p - cmd < sizeof(cmd))
        {
			p++;
			char cmd2r;
			int id;
			int data;
			if ((sscanf(cmd, "%c %02x OK%02xx", &cmd2r, &id, &data) == 3) && (id == set_id) && (cmd2r == cmd2))
			{
				//printf("OK %02X\n", data);
				return data;
			}
			if ((sscanf(cmd, "%c %02x NG%02xx", &cmd2r, &id, &data) == 3) && (id == set_id) && (cmd2r == cmd2))
			{
				//printf("NG %02X\n", data);
				return -data;
			}
		}

		usleep(1*1000);
	}
}

/*
 *
 */
void strfix(char *x)
{
	for (; *x; x++)
	{
		if (*x >= 'A' && *x <= 'Z') *x += 'a' - 'A';
		if (*x <= ' ') *x = '_';
	}
}

/*
 * main
 */
int main(int argc, char *argv[])
{
	char cmd1, cmd2;
	int value = -1;

	if (argc < 2)
	{
		printf("Syntax: lg [cmd] [param]\n\n");
		LIST_STYPE(CMD) *commands = LIST(CMD);
		for (int l=0; l<LENGTH(CMD); l++)
		{
			printf("%s\n", commands[l].name);
		}
		return 0;
	}
	
	char *a = strdup(argv[1]);
	strfix(a);

	LIST_STYPE(CMD) *commands = LIST(CMD);
	for (int l=0; l<LENGTH(CMD); l++)
	{
		char *b = strdup(commands[l].name);
		strfix(b);
		if (strstr(b, a) != NULL)
		{
			printf("%s:\n\n", commands[l].name);
			cmd1 = commands[l].cmd1;
			cmd2 = commands[l].cmd2;

			if (argc < 3)
			{
				SubDef *list = commands[l].list;
				if (list != NULL)
				{
					for (int s=0; s<commands[l].listlen; s++)
					{
						printf("%s\n", list[s].name);
					}
				}
				else
				{
					printf("numeric value\n");
				}
				return 0;
			}

			char *a2 = strdup(argv[2]);
			strfix(a2);

			SubDef *list = commands[l].list;
			if (list != NULL)
			{
				for (int s=0; s<commands[l].listlen; s++)
				{
					char *c = strdup(list[s].name);
					strfix(c);
					if (strstr(c, a2) != NULL)
					{
						printf("%s\n", list[s].name);
						value = list[s].i;
						break;
					}
					free(c);
				}
			}
			else
			{
				value = strtoul(argv[2],0,0);
			}

			break;
		}
		free(b);
	}

	if (value < 0)
	{
		printf("Invalid value\n");
		return -1;
	}

    tcgetattr(fileno(stdin), &tios_stdin);
	atexit(ResetTerminal);

    struct termios ts;
    tcgetattr(fileno(stdin), &ts);
    ts.c_lflag &=~ (ICANON | ECHO /*| ISIG*/);
    ts.c_cc[VMIN] = 1;      // non blocking
    ts.c_cc[VTIME] = 1;     // no timer
    tcsetattr(fileno(stdin), TCSANOW, &ts);
    setbuf(stdout, 0);

	char *device = "/dev/tty.usbserial";	// argv[1]
    fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) { fprintf(stderr, "Cannot open '%s'\n", device); return 1; }
    tcflush(fd, TCIOFLUSH);

	struct termios tios;
    memset(&tios, 0, sizeof(tios));
    tios.c_ispeed = tios.c_ospeed = B9600;
    cfsetispeed(&tios, B9600);
    cfsetospeed(&tios, B9600);
    tios.c_cflag = CS8 | CLOCAL | CREAD;
    tios.c_iflag = IGNPAR | IGNBRK | IXANY;// | INPCK;
    tios.c_lflag = ISIG;
    tios.c_cc[VMIN] = 1;        // non blocking
    tios.c_cc[VTIME] = 0;       // no timer
    tcsetattr(fd, TCSANOW, &tios);

 	write(fd, "\r\r\r", 3);		// blah blah blah
	int r = SendCommand(cmd1, cmd2, value);
	if (r >= 0)
	{
		//printf("OK!\n");
	}

	close(fd);
	return r;
}

