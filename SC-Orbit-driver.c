/*
 * SC-Orbit
 * Standalone ARMv7 Linux binary: no libc/runtime dependency.
 *
 * Public launch build. The approved mouse/trackball gyro, Gyro Paddle, hybrid
 * analog-stick gyro assist, keypad sensing, sounds, trackball, spinner, mappings,
 * profiles, and targeted duplicate-input suppression are frozen for release.
 */

typedef unsigned char      u8;
typedef signed char        s8;
typedef unsigned short     u16;
typedef signed short       s16;
typedef unsigned int       u32;
typedef signed int         s32;
typedef unsigned long      usize;
typedef signed long        ssize;

#define NULL ((void*)0)

/* Short asynchronous haptic songs for mode and action feedback.
 *
 * The controller is treated as a four-voice mechanical synthesizer. Each frame
 * drives the two pad actuators and two rear haptic channels as a tiny chord or
 * arpeggio. These are deliberately musical signatures, not speech or PCM.
 * Value-up/value-down ladders remain separate and unchanged below. */
struct voice_frame {u16 f0,f1,f2,f3; s8 g0,g1,g2,g3; u8 ticks;};
#define MF(a,b,c,d,ga,gb,gc,gd,t) {a,b,c,d,ga,gb,gc,gd,t}
#define REST(t) {0,0,0,0,-128,-128,-128,-128,t}

enum voice_phrase_id {
    VOICE_GYRO_ON=0,
    VOICE_GYRO_OFF=1,
    VOICE_LEFT_STICK_ON=2,
    VOICE_LEFT_STICK_OFF=3,
    VOICE_RIGHT_STICK_ON=4,
    VOICE_RIGHT_STICK_OFF=5,
    VOICE_GYRO_NORMAL=6,
    VOICE_INVERT_X=7,
    VOICE_INVERT_Y=8,
    VOICE_INVERT_BOTH=9,
    VOICE_WHEEL_MODE=10,
    VOICE_SLIDE_MODE=11,
    VOICE_SWIPE_MODE=12,
    VOICE_GYRO_PADDLE=13,
    VOICE_MOUSE_MODE=14,
    VOICE_TRACKBALL_MODE=15,
    VOICE_PADS_SWAPPED=16,
    VOICE_PADS_NORMAL=17,
    VOICE_NUMPAD_ON=18,
    VOICE_NUMPAD_OFF=19,
    VOICE_RADIAL_PAD=20,
    VOICE_PHONE_PAD=21,
    VOICE_PROFILE_SAVED=22,
    VOICE_PROFILE_LOADED=23,
    VOICE_PROFILE_MISSING=24,
    VOICE_PROFILE_ERROR=25,
    VOICE_DEFAULTS_RESTORED=26,
    VOICE_SPINNER_NORMAL=27,
    VOICE_SPINNER_REVERSED=28,
    VOICE_PHRASE_COUNT=29
};

/* Enabled: ascending open fifth into a bright major chord. */
static const struct voice_frame voice_gyro_on[]={
    MF(196,262,330,392,-14,-16,-15,-14,1),
    MF(262,330,392,523,-13,-14,-13,-12,1),
    MF(330,392,523,659,-12,-13,-11,-10,2),REST(1)};
/* Disabled: a clear falling minor cadence. */
static const struct voice_frame voice_gyro_off[]={
    MF(659,523,392,330,-11,-12,-13,-13,1),
    MF(523,392,311,262,-12,-13,-14,-14,1),
    MF(392,311,262,196,-13,-14,-15,-15,2),REST(1)};
/* Left/right stick motifs lean toward their physical side. */
static const struct voice_frame voice_left_stick_on[]={
    MF(220,0,330,0,-9,-128,-10,-128,1),MF(294,196,440,294,-9,-18,-9,-18,1),
    MF(392,262,587,392,-8,-17,-8,-17,2),REST(1)};
static const struct voice_frame voice_left_stick_off[]={
    MF(392,0,587,0,-9,-128,-9,-128,1),MF(294,196,440,262,-10,-18,-10,-18,1),
    MF(220,0,330,0,-12,-128,-12,-128,2),REST(1)};
static const struct voice_frame voice_right_stick_on[]={
    MF(0,220,0,330,-128,-9,-128,-10,1),MF(196,294,294,440,-18,-9,-18,-9,1),
    MF(262,392,392,587,-17,-8,-17,-8,2),REST(1)};
static const struct voice_frame voice_right_stick_off[]={
    MF(0,392,0,587,-128,-9,-128,-9,1),MF(196,294,262,440,-18,-10,-18,-10,1),
    MF(0,220,0,330,-128,-12,-128,-12,2),REST(1)};
/* Four inversion identities: stable, horizontal crossing, vertical fall, both. */
static const struct voice_frame voice_gyro_normal[]={
    MF(262,330,392,523,-14,-14,-13,-12,1),MF(330,392,523,659,-13,-13,-12,-11,2),REST(1)};
static const struct voice_frame voice_invert_x[]={
    MF(262,523,330,659,-12,-12,-13,-12,1),MF(523,262,659,330,-11,-13,-11,-13,2),REST(1)};
static const struct voice_frame voice_invert_y[]={
    MF(659,523,392,330,-11,-12,-13,-13,1),MF(523,392,330,262,-12,-13,-13,-14,2),REST(1)};
static const struct voice_frame voice_invert_both[]={
    MF(659,262,523,330,-11,-13,-12,-13,1),MF(262,659,330,523,-13,-11,-13,-12,1),
    MF(311,392,466,587,-13,-13,-12,-12,2),REST(1)};
/* Spinner identities. Wheel rotates notes through all four voices; slide sweeps
 * left-to-right. Swipe remains defined for migration but is no longer selectable. */
static const struct voice_frame voice_wheel_mode[]={
    MF(262,0,0,0,-10,-128,-128,-128,1),MF(0,330,0,0,-128,-10,-128,-128,1),
    MF(0,0,392,0,-128,-128,-9,-128,1),MF(0,0,0,523,-128,-128,-128,-8,1),
    MF(262,330,392,523,-15,-14,-13,-12,2),REST(1)};
static const struct voice_frame voice_slide_mode[]={
    MF(220,262,0,0,-10,-13,-128,-128,1),MF(294,349,294,0,-13,-10,-14,-128,1),
    MF(0,392,440,523,-128,-13,-10,-9,2),REST(1)};
static const struct voice_frame voice_swipe_mode[]={
    MF(247,294,370,440,-13,-13,-12,-11,1),MF(294,370,440,587,-12,-12,-11,-10,2),REST(1)};
static const struct voice_frame voice_gyro_paddle[]={
    MF(196,294,392,587,-16,-14,-12,-10,1),MF(247,370,494,740,-15,-13,-11,-9,1),
    MF(294,440,587,880,-14,-12,-10,-8,2),REST(1)};
/* Pointer modes: bright precise mouse chirp versus deep rolling trackball chord. */
static const struct voice_frame voice_mouse_mode[]={
    MF(523,659,784,988,-14,-12,-10,-9,1),MF(659,784,988,1175,-13,-11,-9,-8,2),REST(1)};
static const struct voice_frame voice_trackball_mode[]={
    MF(147,196,247,294,-10,-12,-13,-14,1),MF(165,220,262,330,-9,-11,-12,-13,1),
    MF(196,247,294,392,-9,-10,-11,-12,2),REST(1)};
/* Swap physically crosses the melody; normal resolves outward symmetrically. */
static const struct voice_frame voice_pads_swapped[]={
    MF(220,440,330,659,-10,-10,-11,-11,1),MF(440,220,659,330,-9,-12,-9,-12,1),
    MF(294,294,440,440,-12,-12,-10,-10,2),REST(1)};
static const struct voice_frame voice_pads_normal[]={
    MF(294,294,440,440,-12,-12,-10,-10,1),MF(220,440,330,659,-11,-11,-10,-10,1),
    MF(262,330,392,523,-13,-12,-11,-10,2),REST(1)};
/* Number-pad and layout signatures. */
static const struct voice_frame voice_numpad_on[]={
    MF(262,330,392,523,-15,-14,-13,-12,1),MF(294,370,440,587,-14,-13,-12,-11,1),
    MF(330,415,494,659,-13,-12,-11,-10,2),REST(1)};
static const struct voice_frame voice_numpad_off[]={
    MF(494,415,330,262,-11,-12,-13,-14,1),MF(392,330,262,196,-12,-13,-14,-15,2),REST(1)};
static const struct voice_frame voice_radial_pad[]={
    MF(262,0,392,0,-11,-128,-10,-128,1),MF(0,330,0,523,-128,-11,-128,-9,1),
    MF(392,262,523,330,-10,-12,-9,-11,1),MF(262,330,392,523,-13,-12,-11,-10,2),REST(1)};
static const struct voice_frame voice_phone_pad[]={
    MF(262,330,392,0,-12,-11,-10,-128,1),MF(294,370,440,0,-12,-11,-10,-128,1),
    MF(330,415,494,659,-12,-11,-10,-9,2),REST(1)};
/* Profile and system actions. */
static const struct voice_frame voice_profile_saved[]={
    MF(262,330,392,523,-14,-13,-12,-11,1),MF(330,415,494,659,-13,-12,-11,-10,1),
    MF(392,494,587,784,-12,-11,-10,-8,2),REST(1)};
static const struct voice_frame voice_profile_loaded[]={
    MF(392,494,587,784,-12,-11,-10,-8,1),MF(330,415,494,659,-13,-12,-11,-10,1),
    MF(392,523,659,784,-12,-10,-9,-8,2),REST(1)};
static const struct voice_frame voice_profile_missing[]={
    MF(311,370,440,0,-11,-12,-13,-128,1),REST(1),MF(311,370,440,0,-13,-14,-15,-128,2),REST(1)};
static const struct voice_frame voice_profile_error[]={
    MF(185,233,277,330,-8,-9,-10,-11,1),MF(175,220,262,311,-8,-9,-10,-11,1),
    MF(165,208,247,294,-8,-9,-10,-11,2),REST(1)};
static const struct voice_frame voice_defaults_restored[]={
    MF(196,247,294,392,-15,-14,-13,-12,1),MF(247,311,370,494,-14,-13,-12,-11,1),
    MF(294,370,440,587,-13,-12,-11,-10,1),MF(392,494,587,784,-12,-11,-10,-8,2),REST(1)};
/* Direction: rising clockwise and falling counter-clockwise musical arrows. */
static const struct voice_frame voice_spinner_normal[]={
    MF(220,0,330,0,-11,-128,-10,-128,1),MF(0,294,0,440,-128,-11,-128,-9,1),
    MF(330,392,494,587,-12,-11,-10,-9,2),REST(1)};
static const struct voice_frame voice_spinner_reversed[]={
    MF(587,494,392,330,-9,-10,-11,-12,1),MF(440,0,294,0,-10,-128,-11,-128,1),
    MF(0,330,0,220,-128,-10,-128,-11,2),REST(1)};

struct voice_phrase_def {const struct voice_frame *frames;u16 count;};
#define VP(name) {name,(u16)(sizeof(name)/sizeof(name[0]))}
static const struct voice_phrase_def voice_phrases[VOICE_PHRASE_COUNT]={
    VP(voice_gyro_on),VP(voice_gyro_off),VP(voice_left_stick_on),VP(voice_left_stick_off),
    VP(voice_right_stick_on),VP(voice_right_stick_off),VP(voice_gyro_normal),VP(voice_invert_x),
    VP(voice_invert_y),VP(voice_invert_both),VP(voice_wheel_mode),VP(voice_slide_mode),
    VP(voice_swipe_mode),VP(voice_gyro_paddle),VP(voice_mouse_mode),VP(voice_trackball_mode),
    VP(voice_pads_swapped),VP(voice_pads_normal),VP(voice_numpad_on),VP(voice_numpad_off),
    VP(voice_radial_pad),VP(voice_phone_pad),VP(voice_profile_saved),VP(voice_profile_loaded),
    VP(voice_profile_missing),VP(voice_profile_error),VP(voice_defaults_restored),
    VP(voice_spinner_normal),VP(voice_spinner_reversed)
};
#undef VP
#undef REST
#undef MF


/* ARM EABI Linux syscalls. */
static inline long sc1(long n, long a) {
    register long r7 __asm__("r7") = n;
    register long r0 __asm__("r0") = a;
    __asm__ volatile("svc 0" : "+r"(r0) : "r"(r7) : "memory", "cc");
    return r0;
}
static inline long sc2(long n, long a, long b) {
    register long r7 __asm__("r7") = n;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    __asm__ volatile("svc 0" : "+r"(r0) : "r"(r1), "r"(r7) : "memory", "cc");
    return r0;
}
static inline long sc3(long n, long a, long b, long c) {
    register long r7 __asm__("r7") = n;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    __asm__ volatile("svc 0" : "+r"(r0) : "r"(r1), "r"(r2), "r"(r7) : "memory", "cc");
    return r0;
}

#define SYS_exit       1
#define SYS_read       3
#define SYS_write      4
#define SYS_open       5
#define SYS_close      6
#define SYS_ioctl      54
#define SYS_nanosleep  162
#define SYS_clock_gettime 263

static long xopen(const char *p, int f, int m) { return sc3(SYS_open,(long)p,f,m); }
static long xread(int f, void *p, u32 n) { return sc3(SYS_read,f,(long)p,n); }
static long xwrite(int f, const void *p, u32 n) { return sc3(SYS_write,f,(long)p,n); }
static long xclose(int f) { return sc1(SYS_close,f); }
static long xioctl(int f, u32 req, void *p) { return sc3(SYS_ioctl,f,req,(long)p); }
struct timespec32 { s32 tv_sec; s32 tv_nsec; };
static void sleep_ms(u32 ms) {
    struct timespec32 t;
    t.tv_sec = 0;
    while(ms>=1000U){t.tv_sec++;ms-=1000U;}
    t.tv_nsec = (s32)(ms * 1000000U);
    sc2(SYS_nanosleep,(long)&t,0);
}
static void xexit(int code) { sc1(SYS_exit,code); for(;;){} }

void *memset(void *dst, int c, usize n) {
    u8 *d=(u8*)dst; while(n){ *d++=(u8)c; n--; } return dst;
}
void *memcpy(void *dst, const void *src, usize n) {
    u8 *d=(u8*)dst; const u8 *s=(const u8*)src; while(n){ *d++=*s++; n--; } return dst;
}
static usize slen(const char *s) { usize n=0; while(s[n]) n++; return n; }
static int streq_n(const char *a,const char *b,usize n) { usize i=0; while(i<n){ if(a[i]!=b[i]) return 0; i++; } return 1; }
static void log_text(const char *s) { xwrite(1,s,(u32)slen(s)); }

/* No libgcc division helpers required. */
static u32 udiv32(u32 n,u32 d) {
    u32 q=0, bit=1;
    if(!d) return 0xffffffffU;
    while((d & 0x80000000U)==0 && d<n && bit){ d<<=1; bit<<=1; }
    while(bit){ if(n>=d){ n-=d; q|=bit; } d>>=1; bit>>=1; }
    return q;
}
static s32 sdiv32(s32 n,s32 d) {
    u32 un,ud,q; int neg=0;
    if(n<0){ un=(u32)(-n); neg=!neg; } else un=(u32)n;
    if(d<0){ ud=(u32)(-d); neg=!neg; } else ud=(u32)d;
    q=udiv32(un,ud);
    return neg ? -(s32)q : (s32)q;
}
static u32 monotonic_ms(void) {
    struct timespec32 t;
    if(sc2(SYS_clock_gettime,1,(long)&t)<0) return 0;
    return (u32)t.tv_sec*1000U + udiv32((u32)t.tv_nsec,1000000U);
}
static s32 iabs32(s32 v){ return v<0 ? -v : v; }
static s32 clamp32(s32 v,s32 lo,s32 hi){ return v<lo?lo:(v>hi?hi:v); }

/* ioctl encoding. */
#define IOC_NRBITS   8
#define IOC_TYPEBITS 8
#define IOC_SIZEBITS 14
#define IOC_NRSHIFT  0
#define IOC_TYPESHIFT (IOC_NRSHIFT+IOC_NRBITS)
#define IOC_SIZESHIFT (IOC_TYPESHIFT+IOC_TYPEBITS)
#define IOC_DIRSHIFT  (IOC_SIZESHIFT+IOC_SIZEBITS)
#define IOC_NONE  0U
#define IOC_WRITE 1U
#define IOC_READ  2U
#define IOC(dir,type,nr,size) (((dir)<<IOC_DIRSHIFT)|((u32)(type)<<IOC_TYPESHIFT)|((nr)<<IOC_NRSHIFT)|((size)<<IOC_SIZESHIFT))
#define IO(type,nr) IOC(IOC_NONE,type,nr,0)
#define IOW(type,nr,sz) IOC(IOC_WRITE,type,nr,sz)
#define IOR(type,nr,sz) IOC(IOC_READ,type,nr,sz)

/* Linux input/uinput ABI. */
struct input_id { u16 bustype,vendor,product,version; };
struct input_absinfo { s32 value,minimum,maximum,fuzz,flat,resolution; };
struct uinput_setup { struct input_id id; char name[80]; u32 ff_effects_max; };
struct uinput_abs_setup { u16 code; u16 pad; struct input_absinfo absinfo; };
struct timeval32 { s32 tv_sec,tv_usec; };
struct input_event { struct timeval32 time; u16 type,code; s32 value; };
struct hidraw_devinfo { u32 bustype; u16 vendor; u16 product; };

#define UINPUT_BASE 'U'
#define UI_DEV_CREATE   IO(UINPUT_BASE,1)
#define UI_DEV_DESTROY  IO(UINPUT_BASE,2)
#define UI_DEV_SETUP    IOW(UINPUT_BASE,3,sizeof(struct uinput_setup))
#define UI_ABS_SETUP    IOW(UINPUT_BASE,4,sizeof(struct uinput_abs_setup))
#define UI_SET_EVBIT    IOW(UINPUT_BASE,100,sizeof(int))
#define UI_SET_KEYBIT   IOW(UINPUT_BASE,101,sizeof(int))
#define UI_SET_RELBIT   IOW(UINPUT_BASE,102,sizeof(int))
#define UI_SET_ABSBIT   IOW(UINPUT_BASE,103,sizeof(int))
#define HIDIOCGRAWINFO  IOR('H',0x03,sizeof(struct hidraw_devinfo))
#define HIDIOCSFEATURE(len) IOC(IOC_WRITE|IOC_READ,'H',0x06,len)
#define HIDIOCGFEATURE(len) IOC(IOC_WRITE|IOC_READ,'H',0x07,len)
#define EVIOCGID        IOR('E',0x02,sizeof(struct input_id))
#define EVIOCGNAME(len) IOC(IOC_READ,'E',0x06,len)
#define EVIOCGBIT(ev,len) IOC(IOC_READ,'E',0x20+(ev),len)
#define EVIOCGRAB       IOW('E',0x90,sizeof(int))

#define EV_SYN 0
#define EV_KEY 1
#define EV_REL 2
#define EV_ABS 3
#define SYN_REPORT 0
#define REL_X 0
#define REL_Y 1
#define REL_DIAL 7
#define ABS_X 0
#define ABS_Y 1
#define ABS_Z 2
#define ABS_RX 3
#define ABS_RY 4
#define ABS_RZ 5
#define ABS_HAT0X 16
#define ABS_HAT0Y 17
#define BTN_0 0x100
#define BTN_1 0x101
#define BTN_2 0x102
#define BTN_3 0x103
#define BTN_LEFT 0x110
#define BTN_RIGHT 0x111
#define BTN_TRIGGER 0x120
#define BTN_THUMB 0x121
#define BTN_BASE 0x126
#define BTN_SOUTH 0x130
#define BTN_EAST 0x131
#define BTN_NORTH 0x133
#define BTN_WEST 0x134
#define BTN_TL 0x136
#define BTN_TR 0x137
#define BTN_TL2 0x138
#define BTN_TR2 0x139
#define BTN_SELECT 0x13a
#define BTN_START 0x13b
#define BTN_MODE 0x13c
#define BTN_THUMBL 0x13d
#define BTN_THUMBR 0x13e
#define KEY_1 2
#define KEY_2 3
#define KEY_3 4
#define KEY_4 5
#define KEY_5 6
#define KEY_6 7
#define KEY_7 8
#define KEY_8 9
#define KEY_9 10
#define BUS_VIRTUAL 0x06

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2
#define O_CREAT 0100
#define O_TRUNC 01000
#define O_APPEND 02000
#define O_NONBLOCK 04000
#define EAGAIN 11
#define EPIPE 32

#define STEAM_VENDOR 0x28de
#define PRODUCT_WIRED       0x1302
#define PRODUCT_BT          0x1303
#define PRODUCT_PUCK        0x1304
/* Original wired/wireless receiver IDs used by Linux evdev/hidraw. */
#define PRODUCT_SC_WIRED    0x1102
#define PRODUCT_SC_WIRELESS 0x1142
#define PRODUCT_SC_ALT      0x11fc
#define PRODUCT_SC_VIRTUAL  0x11ff
#define PRODUCT_SC_BLE      0x1201
#define PRODUCT_SC_LEGACY   0x0476

static void copy_name(char *d,const char *s) { int i=0; while(i<79 && s[i]){ d[i]=s[i]; i++; } d[i]=0; }
static int uinput_bit(int fd,u32 req,int code){ return xioctl(fd,req,(void*)(long)code)<0 ? -1:0; }
static int setup_abs(int fd,u16 code,s32 min,s32 max){
    struct uinput_abs_setup a; memset(&a,0,sizeof(a)); a.code=code; a.absinfo.minimum=min; a.absinfo.maximum=max;
    return xioctl(fd,UI_ABS_SETUP,&a)<0 ? -1:0;
}
static int create_gamepad(void){
    int fd=(int)xopen("/dev/uinput",O_RDWR|O_NONBLOCK,0); struct uinput_setup u;
    /* Match SCWrapper's default Linux gamepad button descriptor exactly.
     * BTN_0..BTN_3 are SCWrapper's MiSTer-toolchain fallback names for
     * BTN_GRIPL/BTN_GRIPR/BTN_GRIPL2/BTN_GRIPR2. */
    int keys[] = {BTN_SOUTH,BTN_EAST,BTN_WEST,BTN_NORTH,BTN_TL,BTN_TR,BTN_TL2,BTN_TR2,
                  BTN_THUMBL,BTN_THUMBR,BTN_0,BTN_1,BTN_2,BTN_3,
                  BTN_BASE,BTN_START,BTN_SELECT,BTN_MODE};
    int axes[] = {ABS_HAT0X,ABS_HAT0Y,ABS_X,ABS_Y,ABS_RX,ABS_RY,ABS_Z,ABS_RZ};
    int i;
    if(fd<0) return -1;
    if(uinput_bit(fd,UI_SET_EVBIT,EV_KEY)||uinput_bit(fd,UI_SET_EVBIT,EV_ABS)) return -1;
    for(i=0;i<(int)(sizeof(keys)/sizeof(keys[0]));i++) if(uinput_bit(fd,UI_SET_KEYBIT,keys[i])) return -1;
    for(i=0;i<(int)(sizeof(axes)/sizeof(axes[0]));i++) if(uinput_bit(fd,UI_SET_ABSBIT,axes[i])) return -1;
    if(setup_abs(fd,ABS_HAT0X,-1,1)||setup_abs(fd,ABS_HAT0Y,-1,1)||
       setup_abs(fd,ABS_X,-32767,32767)||setup_abs(fd,ABS_Y,-32767,32767)||
       setup_abs(fd,ABS_RX,-32767,32767)||setup_abs(fd,ABS_RY,-32767,32767)||
       setup_abs(fd,ABS_Z,0,32767)||setup_abs(fd,ABS_RZ,0,32767)) return -1;
    /* Use SCWrapper's proven MiSTer identity so Main_MiSTer does not reuse
     * the stale SC-Orbit VID:PID profile created by the broken repair builds. */
    memset(&u,0,sizeof(u)); copy_name(u.name,"Virtual Controller");
    u.id.bustype=BUS_VIRTUAL; u.id.vendor=0x0000; u.id.product=0x0000; u.id.version=0x0001;
    if(xioctl(fd,UI_DEV_SETUP,&u)<0 || xioctl(fd,UI_DEV_CREATE,0)<0) return -1;
    return fd;
}
static int create_spinner(void){
    int fd=(int)xopen("/dev/uinput",O_RDWR|O_NONBLOCK,0); struct uinput_setup u;
    if(fd<0) return -1;
    /* All three spinner modes deliberately speak the relative REL_DIAL
     * language consumed by Arkanoid and other optical-spinner cores. Mode 2
     * changes how thumb position generates velocity; it is not an EV_ABS
     * paddle and therefore cannot drift away from a core's hidden position. */
    if(uinput_bit(fd,UI_SET_EVBIT,EV_KEY)||uinput_bit(fd,UI_SET_EVBIT,EV_REL)||
       uinput_bit(fd,UI_SET_KEYBIT,BTN_TRIGGER)||uinput_bit(fd,UI_SET_KEYBIT,BTN_THUMB)||
       uinput_bit(fd,UI_SET_RELBIT,REL_DIAL)) return -1;
    memset(&u,0,sizeof(u)); copy_name(u.name,"MiSTer-S1 SC-Orbit Spinner");
    u.id.bustype=BUS_VIRTUAL; u.id.vendor=0x1209; u.id.product=0x5345; u.id.version=0x0006;
    if(xioctl(fd,UI_DEV_SETUP,&u)<0 || xioctl(fd,UI_DEV_CREATE,0)<0) return -1;
    return fd;
}
static int create_mouse(void){
    int fd=(int)xopen("/dev/uinput",O_RDWR|O_NONBLOCK,0); struct uinput_setup u;
    if(fd<0) return -1;
    if(uinput_bit(fd,UI_SET_EVBIT,EV_KEY)||uinput_bit(fd,UI_SET_EVBIT,EV_REL)||
       uinput_bit(fd,UI_SET_KEYBIT,BTN_LEFT)||uinput_bit(fd,UI_SET_KEYBIT,BTN_RIGHT)||
       uinput_bit(fd,UI_SET_RELBIT,REL_X)||uinput_bit(fd,UI_SET_RELBIT,REL_Y)) return -1;
    memset(&u,0,sizeof(u)); copy_name(u.name,"SC-Orbit Trackball");
    u.id.bustype=BUS_VIRTUAL; u.id.vendor=0x1209; u.id.product=0x5344; u.id.version=0x0004;
    if(xioctl(fd,UI_DEV_SETUP,&u)<0 || xioctl(fd,UI_DEV_CREATE,0)<0) return -1;
    return fd;
}
static int create_keypad(void){
    int fd=(int)xopen("/dev/uinput",O_RDWR|O_NONBLOCK,0); struct uinput_setup u;
    int keys[9]={KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9};int i;
    if(fd<0)return -1;
    if(uinput_bit(fd,UI_SET_EVBIT,EV_KEY))return -1;
    for(i=0;i<9;i++)if(uinput_bit(fd,UI_SET_KEYBIT,keys[i]))return -1;
    memset(&u,0,sizeof(u));copy_name(u.name,"SC-Orbit Number Pad");
    u.id.bustype=BUS_VIRTUAL;u.id.vendor=0x1209;u.id.product=0x5346;u.id.version=0x0001;
    if(xioctl(fd,UI_DEV_SETUP,&u)<0||xioctl(fd,UI_DEV_CREATE,0)<0)return -1;
    return fd;
}
static int write_bytes(int fd,const void *data,u32 bytes){
    const u8 *p=(const u8*)data;u32 left=bytes;
    while(left){long n=xwrite(fd,p,left);if(n>0){p+=n;left-=(u32)n;continue;}if(n==-EAGAIN){sleep_ms(1);continue;}return -1;}
    return 0;
}
static void emit(int fd,u16 type,u16 code,s32 value){ struct input_event e; memset(&e,0,sizeof(e)); e.type=type; e.code=code; e.value=value; write_bytes(fd,&e,(u32)sizeof(e)); }
static void syncfd(int fd){ emit(fd,EV_SYN,SYN_REPORT,0); }
static int numpad_key_down,numpad_click_prev,numpad_rearm,numpad_hover_key,numpad_hover_candidate,numpad_hover_reports;
static void release_numpad_key(int keypad){
    if(numpad_key_down>0&&keypad>=0){emit(keypad,EV_KEY,(u16)numpad_key_down,0);syncfd(keypad);}
    numpad_key_down=0;numpad_click_prev=0;
}
#define PAD_LEFT 0
#define PAD_RIGHT 1
/* Single cursor-emission boundary. active_trackball_pad is updated only from
 * the persistent pad-role setting; no raw right-pad or SCWrapper-style direct
 * translation can bypass this authorization check. */
static int mouse_suppressed;
static int active_trackball_pad=PAD_RIGHT;
static void rel_trackball_mouse(int fd,int source_pad,s32 x,s32 y){
    struct input_event e[3];int n=0;
    if(mouse_suppressed||source_pad!=active_trackball_pad||(!x&&!y))return;
    if(x){memset(&e[n],0,sizeof(e[n]));e[n].type=EV_REL;e[n].code=REL_X;e[n].value=x;n++;}
    if(y){memset(&e[n],0,sizeof(e[n]));e[n].type=EV_REL;e[n].code=REL_Y;e[n].value=y;n++;}
    memset(&e[n],0,sizeof(e[n]));e[n].type=EV_SYN;e[n].code=SYN_REPORT;n++;
    write_bytes(fd,e,(u32)(n*sizeof(e[0])));
}
static void rel_spinner(int fd,s32 d){ if(d){ emit(fd,EV_REL,REL_DIAL,d); syncfd(fd); } }
static int mouse_left_state,mouse_right_state,spinner_primary_state,spinner_secondary_state;
static int trackball_click_prev,trackball_click_button;
#define TRACKBALL_CLICK_NONE 0
#define TRACKBALL_CLICK_LEFT 1
#define TRACKBALL_CLICK_RIGHT 2
/* Signed pad coordinates span -32768..32767. Starting at +29491 makes the
 * right-click strip 3277 coordinate values, effectively the final 5 percent
 * of the physical width. The choice is latched on press so a held click can
 * never change buttons because the thumb shifts after the switch actuates. */
#define TRACKBALL_RIGHT_CLICK_X 29491
static void mouse_left(int fd,int down){ if(down!=mouse_left_state){ mouse_left_state=down; emit(fd,EV_KEY,BTN_LEFT,down); syncfd(fd); } }
static void mouse_right(int fd,int down){ if(down!=mouse_right_state){ mouse_right_state=down; emit(fd,EV_KEY,BTN_RIGHT,down); syncfd(fd); } }
static void spinner_primary(int fd,int down){ if(down!=spinner_primary_state){ spinner_primary_state=down; emit(fd,EV_KEY,BTN_TRIGGER,down); syncfd(fd); } }
static void spinner_secondary(int fd,int down){ if(down!=spinner_secondary_state){ spinner_secondary_state=down; emit(fd,EV_KEY,BTN_THUMB,down); syncfd(fd); } }
static void trackball_mouse_click(int fd,int down,s32 x){
    if(down&&!trackball_click_prev){
        trackball_click_button=x>=TRACKBALL_RIGHT_CLICK_X?TRACKBALL_CLICK_RIGHT:TRACKBALL_CLICK_LEFT;
        if(trackball_click_button==TRACKBALL_CLICK_RIGHT)mouse_right(fd,1);else mouse_left(fd,1);
    }else if(!down&&trackball_click_prev){
        if(trackball_click_button==TRACKBALL_CLICK_RIGHT)mouse_right(fd,0);
        else if(trackball_click_button==TRACKBALL_CLICK_LEFT)mouse_left(fd,0);
        trackball_click_button=TRACKBALL_CLICK_NONE;
    }
    trackball_click_prev=down;
}
static void release_trackball_mouse_click(int fd){
    mouse_left(fd,0);mouse_right(fd,0);trackball_click_prev=0;trackball_click_button=TRACKBALL_CLICK_NONE;
}


/* -------------------------------------------------------------------------
 * Controller-first settings. Speed, physical tightness, and momentum transfer ranges are 1..10.
 * Circular and horizontal-X spinner modes keep independent speed values so
 * each mode can have a useful default and remember its own adjustment.
 * ------------------------------------------------------------------------- */
#define CFG_PATH "/media/fat/linux/sc-orbit/sc-orbit.ini"
#define PROFILE_PATH "/media/fat/linux/sc-orbit/sc-orbit-user-profile.ini"
#define SETTINGS_VERSION 35
struct settings {
    int spinner_ipod_speed; /* 1..10; default 6 */
    int spinner_x_speed;    /* 1..10; default 6 */
    int trackball_speed;    /* 1..10; default 6 */
    int trackball_friction; /* 1=tightest friction, 10=loosest friction */
    int trackball_momentum; /* 1=zero coast, 10=210 percent transfer */
    int spinner_direction;  /* +1 or -1 */
    int spinner_mode;       /* 0=circular iPod, 1=horizontal X; gyro master temporarily owns the spinner pad */
    int haptic_intensity;    /* 0=off, 1=low, 2=medium, 3=high */
    int pads_swapped;        /* 0=spinner left/trackball right, 1=roles exchanged */
    int mouse_mode;          /* 0=physical trackball, 1=direct no-physics mouse */
    int gyro_enabled;        /* absolute gyro master: off blocks every gyro path */
    int gyro_sensitivity;    /* 1..10 */
    int gyro_inversion;      /* 0..3; profile 4/internal invert-both is natural physical tilt */
    int gyro_stick_mode;     /* armed bitmask: 1=left stick, 2=right stick; touch still clutches */
    int numpad_enabled;      /* assigned Spinner pad becomes a click-to-type 1-9 keypad */
    int numpad_layout;       /* 0=radial clock face, 1=phone 3x3 */
};
static struct settings cfg;
static void defaults(void){
    cfg.spinner_ipod_speed=6;
    cfg.spinner_x_speed=6;
    cfg.trackball_speed=6;
    cfg.trackball_friction=6;
    cfg.trackball_momentum=7;
    cfg.spinner_direction=1;
    cfg.spinner_mode=0;
    cfg.haptic_intensity=2;
    cfg.pads_swapped=0;
    cfg.mouse_mode=0;
    cfg.gyro_enabled=0;
    cfg.gyro_sensitivity=6;
    cfg.gyro_inversion=3;
    cfg.gyro_stick_mode=0;
    cfg.numpad_enabled=0;
    cfg.numpad_layout=0;
}
static int active_spinner_speed(void){return cfg.spinner_mode==0?cfg.spinner_ipod_speed:cfg.spinner_x_speed;}
static int spinner_haptic_channel(void){return cfg.pads_swapped?1:0;}
static int trackball_haptic_channel(void){return cfg.pads_swapped?0:1;}
static int parse_uint(const char *p){ int v=0; while(*p>='0'&&*p<='9'){v=v*10+(*p-'0');p++;} return v; }
static int cfg_value(const char *buf,int n,const char *key,int fallback){
    int k=(int)slen(key),i=0;
    while(i+k+1<n){
        if((i==0||buf[i-1]=='\n')&&streq_n(buf+i,key,(usize)k)&&buf[i+k]=='=') return parse_uint(buf+i+k+1);
        while(i<n&&buf[i]!='\n')i++; i++;
    }
    return fallback;
}
static void load_cfg(void){
    char b[768]; int fd,n,ver; defaults();
    fd=(int)xopen(CFG_PATH,O_RDONLY,0); if(fd<0)return;
    n=(int)xread(fd,b,sizeof(b)-1); xclose(fd); if(n<=0)return; b[n]=0;
    ver=cfg_value(b,n,"settings_version",0);
    if(ver<17||ver>SETTINGS_VERSION)return; /* preserve all checkpoint settings while migrating */
    if(ver<26){
        /* Preserve the relative position of every saved five-step setting.
         * Old 1..5 maps to new 2,4,6,8,10, keeping the established midpoint
         * at level 6 while opening new extremes and intermediate notches. */
        cfg.spinner_ipod_speed=clamp32(cfg_value(b,n,"spinner_ipod_speed",3),1,5)*2;
        cfg.spinner_x_speed=clamp32(cfg_value(b,n,"spinner_x_speed",3),1,5)*2;
        cfg.trackball_speed=clamp32(cfg_value(b,n,"trackball_speed",3),1,5)*2;
        cfg.trackball_friction=clamp32(cfg_value(b,n,"trackball_friction",3),1,5)*2;
    }else{
        cfg.spinner_ipod_speed=clamp32(cfg_value(b,n,"spinner_ipod_speed",cfg.spinner_ipod_speed),1,10);
        cfg.spinner_x_speed=clamp32(cfg_value(b,n,"spinner_x_speed",cfg.spinner_x_speed),1,10);
        cfg.trackball_speed=clamp32(cfg_value(b,n,"trackball_speed",cfg.trackball_speed),1,10);
        cfg.trackball_friction=clamp32(cfg_value(b,n,"trackball_friction",cfg.trackball_friction),1,10);
    }
    cfg.trackball_momentum=clamp32(cfg_value(b,n,"trackball_momentum",cfg.trackball_momentum),1,10);
    cfg.spinner_direction=cfg_value(b,n,"spinner_direction",1)?1:-1;
    /* v0.23 makes circular iPod mode the default once, while preserving the
     * user's ability to switch modes and have that choice persist afterward. */
    {int sm=(ver==17)?0:cfg_value(b,n,"spinner_mode",0);cfg.spinner_mode=(sm==1)?1:0;}
    if(ver>=21)cfg.haptic_intensity=clamp32(cfg_value(b,n,"haptic_intensity",cfg.haptic_intensity),0,3);
    else cfg.haptic_intensity=cfg_value(b,n,"haptics",1)?2:0;
    cfg.pads_swapped=(ver>=20&&cfg_value(b,n,"pads_swapped",0))?1:0;
    cfg.mouse_mode=(ver>=22&&cfg_value(b,n,"mouse_mode",0))?1:0;
    cfg.gyro_enabled=(ver>=34)?(cfg_value(b,n,"gyro_enabled",0)?1:0):0;
    cfg.gyro_sensitivity=(ver>=29)?clamp32(cfg_value(b,n,"gyro_sensitivity",6),1,10):6;
    if(ver>=31){
        cfg.gyro_inversion=clamp32(cfg_value(b,n,"gyro_inversion",cfg.gyro_inversion),0,3);
        cfg.gyro_stick_mode=clamp32(cfg_value(b,n,"gyro_stick_mask",cfg_value(b,n,"gyro_stick_mode",0)),0,3);
    }else if(ver>=30){
        cfg.gyro_inversion=clamp32(cfg_value(b,n,"gyro_inversion",cfg.gyro_inversion),0,3);
        cfg.gyro_stick_mode=clamp32(cfg_value(b,n,"gyro_stick_mode",0),0,2);
    }else if(ver>=29){
        int old_profile=clamp32(cfg_value(b,n,"gyro_profile",0),0,3);
        cfg.gyro_inversion=0;
        cfg.gyro_stick_mode=old_profile==0?2:0;
        if(old_profile==3)cfg.spinner_mode=0;
    }
    if(ver>=33){
        cfg.numpad_enabled=cfg_value(b,n,"numpad_enabled",0)?1:0;
        cfg.numpad_layout=clamp32(cfg_value(b,n,"numpad_layout",0),0,1);
    }
    /* Profile 4 is the natural physical-tilt default. Existing current-format
     * settings preserve later user changes; older settings migrate once. */
    if(ver<35)cfg.gyro_inversion=3;
}
static char *put_u(char *p,int v){
    char t[12]; int n=0,i;
    if(v<0){*p++='-';v=-v;}
    do{int q=(int)udiv32((u32)v,10U);t[n++]=(char)('0'+v-q*10);v=q;}while(v&&n<11);
    for(i=n-1;i>=0;i--)*p++=t[i]; return p;
}
static char *put_kv(char *p,const char *k,int v){while(*k)*p++=*k++;*p++='=';p=put_u(p,v);*p++='\n';return p;}
static char *put_str(char *p,const char *s){while(*s)*p++=*s++;return p;}
/* Runtime diagnostic and telemetry writes remain disabled. Only the necessary
 * settings and explicitly saved user profile persist on the SD card. */
static void telemetry_write(const char *b,u32 n){(void)b;(void)n;}
static void telemetry_header(void){}static char *serialize_settings(char *p,const char *header){
    while(*header)*p++=*header++;
    p=put_kv(p,"settings_version",SETTINGS_VERSION);
    p=put_kv(p,"spinner_ipod_speed",cfg.spinner_ipod_speed);
    p=put_kv(p,"spinner_x_speed",cfg.spinner_x_speed);
    p=put_kv(p,"trackball_speed",cfg.trackball_speed);
    p=put_kv(p,"trackball_friction",cfg.trackball_friction);
    p=put_kv(p,"trackball_momentum",cfg.trackball_momentum);
    p=put_kv(p,"spinner_direction",cfg.spinner_direction>0?1:0);
    p=put_kv(p,"spinner_mode",cfg.spinner_mode);
    p=put_kv(p,"haptic_intensity",cfg.haptic_intensity);
    p=put_kv(p,"haptics",cfg.haptic_intensity>0?1:0);
    p=put_kv(p,"pads_swapped",cfg.pads_swapped);
    p=put_kv(p,"mouse_mode",cfg.mouse_mode);
    p=put_kv(p,"gyro_enabled",cfg.gyro_enabled);
    p=put_kv(p,"gyro_sensitivity",cfg.gyro_sensitivity);
    p=put_kv(p,"gyro_inversion",cfg.gyro_inversion);
    p=put_kv(p,"gyro_stick_mask",cfg.gyro_stick_mode);
    p=put_kv(p,"gyro_stick_mode",cfg.gyro_stick_mode);
    p=put_kv(p,"numpad_enabled",cfg.numpad_enabled);
    p=put_kv(p,"numpad_layout",cfg.numpad_layout);
    return p;
}
static int write_settings_file(const char *path,const char *header){
    char b[768],*p=b;int fd;
    p=serialize_settings(p,header);
    fd=(int)xopen(path,O_WRONLY|O_CREAT|O_TRUNC,0644);
    if(fd<0)return 0;
    if(xwrite(fd,b,(u32)(p-b))!=(long)(p-b)){xclose(fd);return 0;}
    xclose(fd);return 1;
}
static void save_cfg(void){
    (void)write_settings_file(CFG_PATH,"# SC-Orbit\n");
}
static int save_user_profile(void){
    return write_settings_file(PROFILE_PATH,"# SC-Orbit user profile - all controller-first values\nprofile_version=1\n");
}
static int load_user_profile(void){
    char b[768];int fd,n,ver;struct settings loaded=cfg;
    fd=(int)xopen(PROFILE_PATH,O_RDONLY,0);if(fd<0)return 0;
    n=(int)xread(fd,b,sizeof(b)-1);xclose(fd);if(n<=0)return 0;b[n]=0;
    ver=cfg_value(b,n,"settings_version",0);if(ver<31||ver>SETTINGS_VERSION)return 0;
    loaded.spinner_ipod_speed=clamp32(cfg_value(b,n,"spinner_ipod_speed",loaded.spinner_ipod_speed),1,10);
    loaded.spinner_x_speed=clamp32(cfg_value(b,n,"spinner_x_speed",loaded.spinner_x_speed),1,10);
    loaded.trackball_speed=clamp32(cfg_value(b,n,"trackball_speed",loaded.trackball_speed),1,10);
    loaded.trackball_friction=clamp32(cfg_value(b,n,"trackball_friction",loaded.trackball_friction),1,10);
    loaded.trackball_momentum=clamp32(cfg_value(b,n,"trackball_momentum",loaded.trackball_momentum),1,10);
    loaded.spinner_direction=cfg_value(b,n,"spinner_direction",loaded.spinner_direction>0?1:0)?1:-1;
    {int sm=cfg_value(b,n,"spinner_mode",loaded.spinner_mode);loaded.spinner_mode=(sm==1)?1:0;}
    loaded.haptic_intensity=clamp32(cfg_value(b,n,"haptic_intensity",loaded.haptic_intensity),0,3);
    loaded.pads_swapped=cfg_value(b,n,"pads_swapped",loaded.pads_swapped)?1:0;
    loaded.mouse_mode=cfg_value(b,n,"mouse_mode",loaded.mouse_mode)?1:0;
    loaded.gyro_enabled=cfg_value(b,n,"gyro_enabled",loaded.gyro_enabled)?1:0;
    loaded.gyro_sensitivity=clamp32(cfg_value(b,n,"gyro_sensitivity",loaded.gyro_sensitivity),1,10);
    loaded.gyro_inversion=clamp32(cfg_value(b,n,"gyro_inversion",loaded.gyro_inversion),0,3);
    loaded.gyro_stick_mode=clamp32(cfg_value(b,n,"gyro_stick_mask",cfg_value(b,n,"gyro_stick_mode",loaded.gyro_stick_mode)),0,3);
    loaded.numpad_enabled=cfg_value(b,n,"numpad_enabled",loaded.numpad_enabled)?1:0;
    loaded.numpad_layout=clamp32(cfg_value(b,n,"numpad_layout",loaded.numpad_layout),0,1);
    cfg=loaded;return 1;
}

/* -------------------------------------------------------------------------
 * Steam Controller device access.
 * ------------------------------------------------------------------------- */
static int is_steam_product(u16 p){
    return p==PRODUCT_WIRED||p==PRODUCT_BT||p==PRODUCT_PUCK||
           p==PRODUCT_SC_WIRED||p==PRODUCT_SC_WIRELESS||p==PRODUCT_SC_ALT||
           p==PRODUCT_SC_VIRTUAL||p==PRODUCT_SC_BLE||p==PRODUCT_SC_LEGACY;
}
static void path_num(char *out,const char *base,int n){
    int i=0,t=n,d=1;while(base[i]){out[i]=base[i];i++;}
    while(t>=10){d*=10;t=(int)udiv32((u32)t,10U);}
    while(d){int q=(int)udiv32((u32)n,(u32)d);out[i++]=(char)('0'+q);n-=q*d;d=(int)udiv32((u32)d,10U);}out[i]=0;
}
static int hid_scan_cursor=-1;
static u16 active_product;
static int find_hidraw(void){
    int pass,i,fd;char p[32];struct hidraw_devinfo info;
    for(pass=1;pass<=32;pass++){
        i=(hid_scan_cursor+pass)&31;path_num(p,"/dev/hidraw",i);fd=(int)xopen(p,O_RDWR|O_NONBLOCK,0);if(fd<0)continue;
        memset(&info,0,sizeof(info));
        if(xioctl(fd,HIDIOCGRAWINFO,&info)>=0&&info.vendor==STEAM_VENDOR&&is_steam_product(info.product)){
            hid_scan_cursor=i;active_product=info.product;return fd;
        }
        xclose(fd);
    }
    return -1;
}
static int feature_report_id(void){
    return (active_product==PRODUCT_WIRED||active_product==PRODUCT_BT||active_product==PRODUCT_PUCK)?1:0;
}
static int send_feature_cmd_id(int fd,int report_id,const u8 *cmd,int n){
    u8 b[64];int i,tries,rc=-1;
    for(tries=0;tries<50;tries++){
        memset(b,0,sizeof(b));b[0]=(u8)report_id;
        for(i=0;i<n&&i+1<(int)sizeof(b);i++)b[i+1]=cmd[i];
        rc=(int)xioctl(fd,HIDIOCSFEATURE(64),b);
        if(rc>=0)return rc;
        if(rc!=-EPIPE)break;
        memset(b,0,sizeof(b));b[0]=(u8)report_id;
        xioctl(fd,HIDIOCGFEATURE(64),b);sleep_ms(20);
    }
    return rc;
}
static int send_feature_cmd(int fd,const u8 *cmd,int n){
    int preferred=feature_report_id(),rc;
    rc=send_feature_cmd_id(fd,preferred,cmd,n);
    if(rc<0)rc=send_feature_cmd_id(fd,preferred?0:1,cmd,n);
    return rc;
}
static int configure_controller(int fd){
    /* Keep the proven lizard shutdown and additionally request raw accel+gyro.
     * SETTING_IMU_MODE is setting 48 (0x30); value 0x0018 combines
     * SEND_RAW_ACCEL (0x0008) and SEND_RAW_GYRO (0x0010). */
    u8 clear_map[1]={0x81};
    u8 pad_modes[8]={0x87,0x06,0x07,0x07,0x00,0x08,0x07,0x00};
    u8 imu_mode[5]={0x87,0x03,0x30,0x18,0x00};
    int a=send_feature_cmd(fd,clear_map,1);
    int b=send_feature_cmd(fd,pad_modes,8);
    int c=send_feature_cmd(fd,imu_mode,5);
    if(a>=0&&b>=0&&c>=0){
        log_text("SC-Orbit: lizard shutdown accepted; raw accelerometer+gyro requested\n");
        return 0;
    }
    if(c<0)log_text("SC-Orbit: WARNING - raw IMU enable command failed; gyro aim will remain inactive\n");
    else log_text("SC-Orbit: WARNING - firmware lizard shutdown incomplete; continuing virtual input fail-safe\n");
    return -1;
}

/* Target only paths capable of producing the reported duplicate right-pad
 * stream: Valve REL_X/REL_Y pointer nodes and hid-steam's pad-axis gamepad node
 * (right pad ABS_RX/ABS_RY plus left pad ABS_HAT0X/ABS_HAT0Y). Never suppress
 * every sibling and never stop virtual output because one grab failed. */
#define MAX_TARGETED_GRABS 16
static int grabs[MAX_TARGETED_GRABS];
static int grabbed_event[64];
static int grab_count;
static int bit_is_set(const u8 *bits,int bit){return (bits[bit>>3]>>(bit&7))&1;}
static int lower_ascii(int c){return c>='A'&&c<='Z'?c+('a'-'A'):c;}
static int contains_text_ci(const char *s,const char *needle){
    int i,j;if(!needle[0])return 1;
    for(i=0;s[i];i++){for(j=0;needle[j]&&s[i+j]&&lower_ascii(s[i+j])==lower_ascii(needle[j]);j++){}if(!needle[j])return 1;}
    return 0;
}
static int is_physical_valve_event(const struct input_id *id,const char *name){
    if(id->bustype==BUS_VIRTUAL||id->vendor==0x1209||contains_text_ci(name,"sc-orbit")||contains_text_ci(name,"virtual controller"))return 0;
    return (id->vendor==STEAM_VENDOR&&is_steam_product(id->product))||
           contains_text_ci(name,"steam controller")||contains_text_ci(name,"valve software")||
           contains_text_ci(name,"steamcontroller");
}
static void release_grabs(void){
    int i,zero=0;
    for(i=0;i<grab_count;i++){xioctl(grabs[i],EVIOCGRAB,&zero);xclose(grabs[i]);}
    grab_count=0;memset(grabbed_event,0,sizeof(grabbed_event));
}
static int scan_targeted_physical_events(void){
    int i,fd,one=1;char path[32],name[96];struct input_id id;u8 relbits[8],absbits[8];
    int newly=0;
    for(i=0;i<64&&grab_count<MAX_TARGETED_GRABS;i++){
        int pointer_node,pad_axis_node;
        if(grabbed_event[i])continue;
        path_num(path,"/dev/input/event",i);fd=(int)xopen(path,O_RDONLY|O_NONBLOCK,0);if(fd<0)continue;
        memset(&id,0,sizeof(id));memset(name,0,sizeof(name));memset(relbits,0,sizeof(relbits));memset(absbits,0,sizeof(absbits));
        if(xioctl(fd,EVIOCGID,&id)<0){xclose(fd);continue;}
        xioctl(fd,EVIOCGNAME(sizeof(name)-1),name);
        if(!is_physical_valve_event(&id,name)){xclose(fd);continue;}
        xioctl(fd,EVIOCGBIT(EV_REL,sizeof(relbits)),relbits);
        xioctl(fd,EVIOCGBIT(EV_ABS,sizeof(absbits)),absbits);
        pointer_node=bit_is_set(relbits,REL_X)&&bit_is_set(relbits,REL_Y);
        pad_axis_node=bit_is_set(absbits,ABS_RX)&&bit_is_set(absbits,ABS_RY)&&
                      bit_is_set(absbits,ABS_HAT0X)&&bit_is_set(absbits,ABS_HAT0Y);
        if(!pointer_node&&!pad_axis_node){xclose(fd);continue;}
        if(xioctl(fd,EVIOCGRAB,&one)<0){
            log_text("SC-Orbit: WARNING - targeted duplicate node grab failed; retrying later: ");log_text(path);log_text(" ");log_text(name);log_text("\n");
            xclose(fd);continue;
        }
        grabbed_event[i]=1;grabs[grab_count++]=fd;newly++;
        log_text(pointer_node?"SC-Orbit: grabbed physical Valve REL pointer node ":"SC-Orbit: grabbed hid-steam pad-axis gamepad node ");
        log_text(path);log_text(" ");log_text(name);log_text("\n");
    }
    return newly;
}

/* -------------------------------------------------------------------------
 * Advanced haptic tone output.
 *
 * This is the exact direct 0x83 command style used by the uploaded v2.4
 * reference build whose value-change noises worked on the real controller.
 * Values 1..10 have progressively higher pitch and stronger gain. A boundary
 * press remains silent because change_level() returns before calling it.
 * ------------------------------------------------------------------------- */
static int voice_busy,voice_rendering;
static void haptic_tone_raw(int hid,u8 channel,s8 gain,u16 freq,u16 dur,u16 lfo,u8 depth){
    if(voice_busy&&!voice_rendering)return;
    u8 r[10];
    if(hid<0)return;
    r[0]=0x83;r[1]=channel;r[2]=(u8)gain;
    r[3]=(u8)freq;r[4]=(u8)(freq>>8);
    r[5]=(u8)dur;r[6]=(u8)(dur>>8);
    r[7]=(u8)lfo;r[8]=(u8)(lfo>>8);r[9]=depth;
    xwrite(hid,r,10);
}
static void haptic_tone(int hid,u8 channel,s8 gain,u16 freq,u16 dur,u16 lfo,u8 depth){
    static const s8 gain_offset[4]={0,-12,-6,0};
    s32 scaled;
    if(hid<0||cfg.haptic_intensity<=0)return;
    scaled=clamp32((s32)gain+(s32)gain_offset[cfg.haptic_intensity],-40,31);
    haptic_tone_raw(hid,channel,(s8)scaled,freq,dur,lfo,depth);
}
/* Trackball rolling texture gets its own intensity curve. Low remains restrained;
 * Medium and High are deliberately stronger so the top two settings feel like
 * clearly heavier virtual ball bearings without affecting button-click strength. */
static void trackball_motion_tone(int hid,u8 channel,s8 gain,u16 freq,u16 dur,u16 lfo,u8 depth){
    static const s8 gain_offset[4]={0,-7,1,5};
    s32 scaled;
    if(hid<0||cfg.haptic_intensity<=0)return;
    scaled=clamp32((s32)gain+(s32)gain_offset[cfg.haptic_intensity],-40,31);
    haptic_tone_raw(hid,channel,(s8)scaled,freq,dur,lfo,depth);
}
static void adjustment_cue(int hid,int channel,int level){
    static const u16 freq[10]={170,220,280,350,430,520,620,730,860,1020};
    static const s8 gain[10]={-24,-20,-16,-11,-6,0,6,12,19,27};
    static const u16 dur[10]={20,22,24,27,30,34,38,43,49,56};
    int i=clamp32(level,1,10)-1;
    if(cfg.haptic_intensity<=0||hid<0)return;
    haptic_tone(hid,(u8)channel,gain[i],freq[i],dur[i],0,0);
}
/* A ten-step descending ladder keeps every controller-only adjustment distinct
 * without needing a settings screen. */
static void adjustment_decrease_cue(int hid,int channel,int level){
    static const u16 freq[10]={230,285,350,425,510,605,420,825,950,1090};
    static const s8 gain[10]={4,6,8,10,12,14,10,20,23,26};
    static const u16 dur[10]={68,64,60,56,52,49,46,43,40,38};
    int i=clamp32(level,1,10)-1;
    if(cfg.haptic_intensity<=0||hid<0)return;
    haptic_tone(hid,(u8)channel,gain[i],freq[i],dur[i],0,0);
}
/* Momentum uses a compact two-part cue so it cannot be confused with the
 * single-note speed/tightness ladders. Increasing plays low-then-level; decreasing
 * plays level-then-low. Frequencies deliberately avoid the controller's harsh
 * level-7 resonance. */
static void momentum_adjustment_cue(int hid,int channel,int level,int increasing){
    static const u16 freq[10]={250,290,330,375,425,480,540,610,685,770};
    static const s8 gain[10]={-18,-15,-12,-9,-6,-3,1,5,9,13};
    static const u16 dur[10]={20,21,22,23,24,25,27,29,31,34};
    int i=clamp32(level,1,10)-1;
    if(cfg.haptic_intensity<=0||hid<0)return;
    if(increasing){
        haptic_tone(hid,(u8)channel,-16,170,11,0,0);sleep_ms(15);
        haptic_tone(hid,(u8)channel,gain[i],freq[i],dur[i],0,0);
    }else{
        haptic_tone(hid,(u8)channel,gain[i],freq[i],dur[i],0,0);sleep_ms(15);
        haptic_tone(hid,(u8)channel,-16,170,11,0,0);
    }
}
static void haptic_intensity_cue(int hid,int level){
    int channel=trackball_haptic_channel();
    static const s8 gain[4]={-12,-8,6,20};
    static const u16 dur[4]={18,12,15,18};
    if(hid<0)return;
    /* This confirmation bypasses the newly selected global gate so the OFF
     * transition is still acknowledged. It is deliberately a low, tactile
     * pulse rather than a pitched tone. */
    haptic_tone_raw(hid,(u8)channel,gain[clamp32(level,0,3)],150,dur[clamp32(level,0,3)],0,0);
}


/* -------------------------------------------------------------------------
 * Asynchronous four-voice haptic song engine.
 *
 * Each mode/action signature is a short sequence of chords and arpeggios. The
 * scheduler advances beside normal input processing, so even the richer songs
 * never sleep inside the controller loop. A new action interrupts the old song.
 * ------------------------------------------------------------------------- */
#define VOICE_SUBFRAME_MS 18U
struct voice_runtime {
    int phrase,index,ticks_left,force_stop,phase;
    u32 next_ms;
};
static struct voice_runtime voice_rt;
static void voice_stop_channel(int hid,u8 channel){
    u8 r[10];if(hid<0)return;memset(r,0,sizeof(r));
    r[0]=0x83;r[1]=channel;r[2]=0x80;r[6]=0x80;xwrite(hid,r,10);
}
static void voice_stop_all(int hid){
    voice_stop_channel(hid,0);voice_stop_channel(hid,1);
    voice_stop_channel(hid,3);voice_stop_channel(hid,4);
}
static s8 voice_scaled_gain(s8 gain,u8 channel){
    /* G13 songbook: mode/action music is deliberately more present than G12.
     * Value ladders do not use this path and remain byte-for-byte unchanged. */
    static const s8 intensity_offset[4]={0,-3,4,9};
    s32 v=(s32)gain+(s32)intensity_offset[clamp32(cfg.haptic_intensity,0,3)];
    if(channel>=3)v-=1;
    return (s8)clamp32(v,-28,3);
}
static void voice_tone(int hid,u8 channel,s8 gain,u16 freq){
    u8 r[10];if(hid<0||!freq||gain<=-120)return;memset(r,0,sizeof(r));
    r[0]=0x83;r[1]=channel;r[2]=(u8)voice_scaled_gain(gain,channel);
    r[3]=(u8)freq;r[4]=(u8)(freq>>8);r[5]=0xff;r[6]=0x7f;xwrite(hid,r,10);
}
static u16 voice_answer_freq(u16 freq,int phrase,int frame){
    static const u8 num[4]={5,4,3,2};
    static const u8 den[4]={4,3,2,1};
    u32 out;int shape;if(!freq)return 0;
    shape=(phrase+frame)&3;out=udiv32((u32)freq*(u32)num[shape],(u32)den[shape]);
    while(out>1800U)out>>=1;
    if(out<90U)out=90U;
    return (u16)out;
}
static s8 voice_answer_gain(s8 gain){return (s8)clamp32((s32)gain+3,-24,3);}
static void voice_cancel(int hid){
    if(voice_busy)voice_stop_all(hid);
    memset(&voice_rt,0,sizeof(voice_rt));voice_busy=voice_rendering=0;
}
static void voice_say(int phrase){
    if(cfg.haptic_intensity<=0||phrase<0||phrase>=VOICE_PHRASE_COUNT)return;
    voice_rt.phrase=phrase;voice_rt.index=0;voice_rt.ticks_left=0;voice_rt.phase=0;
    voice_rt.next_ms=0;voice_rt.force_stop=1;voice_busy=1;
}
static void voice_tick(int hid,u32 now){
    const struct voice_phrase_def *pd;const struct voice_frame *fr;
    static const u8 channel_base[4]={3,4,0,1};
    static const u8 channel_answer[4]={4,0,1,3};
    if(!voice_busy)return;
    voice_rendering=1;
    if(cfg.haptic_intensity<=0){voice_stop_all(hid);memset(&voice_rt,0,sizeof(voice_rt));voice_busy=voice_rendering=0;return;}
    if(voice_rt.next_ms&&(s32)(now-voice_rt.next_ms)<0){voice_rendering=0;return;}
    pd=&voice_phrases[voice_rt.phrase];
    if(voice_rt.force_stop){voice_stop_all(hid);voice_rt.force_stop=0;}
    if(voice_rt.index>=pd->count){voice_stop_all(hid);memset(&voice_rt,0,sizeof(voice_rt));voice_busy=voice_rendering=0;return;}
    fr=&pd->frames[voice_rt.index];voice_stop_all(hid);
    if(voice_rt.ticks_left<=0)voice_rt.ticks_left=fr->ticks;
    if(voice_rt.phase==0){
        /* First half: the written four-voice chord. */
        voice_tone(hid,channel_base[0],fr->g0,fr->f0);voice_tone(hid,channel_base[1],fr->g1,fr->f1);
        voice_tone(hid,channel_base[2],fr->g2,fr->f2);voice_tone(hid,channel_base[3],fr->g3,fr->f3);
        voice_rt.phase=1;
    }else{
        /* Second half: rotate the voices physically and answer the chord with
         * a phrase-dependent harmonic interval. This turns each static frame
         * into a tiny sung call-and-response without blocking input. */
        voice_tone(hid,channel_answer[0],voice_answer_gain(fr->g0),voice_answer_freq(fr->f0,voice_rt.phrase,voice_rt.index));
        voice_tone(hid,channel_answer[1],voice_answer_gain(fr->g1),voice_answer_freq(fr->f1,voice_rt.phrase,voice_rt.index));
        voice_tone(hid,channel_answer[2],voice_answer_gain(fr->g2),voice_answer_freq(fr->f2,voice_rt.phrase,voice_rt.index));
        voice_tone(hid,channel_answer[3],voice_answer_gain(fr->g3),voice_answer_freq(fr->f3,voice_rt.phrase,voice_rt.index));
        voice_rt.phase=0;voice_rt.ticks_left--;if(voice_rt.ticks_left<=0)voice_rt.index++;
    }
    voice_rt.next_ms=now+VOICE_SUBFRAME_MS;voice_rendering=0;
}

/* Short controller-resident jingles. Valve documents haptic pulses as low-level
 * primitives intended to be sequenced into more sophisticated textures. These
 * patterns stay under roughly 90 ms, use modest gain, stereo motion, and small
 * LFO sweeps so they feel expressive without blocking gameplay for long. */
static void cute_note(int hid,u8 channel,s8 gain,u16 freq,u16 dur,u16 lfo,u8 depth){
    haptic_tone(hid,channel,gain,freq,dur,lfo,depth);
}
static void profile_action_cue(int hid,int action,int ok){
    (void)hid;
    if(ok)voice_say(action==0?VOICE_PROFILE_SAVED:VOICE_PROFILE_LOADED);
    else voice_say(action==0?VOICE_PROFILE_ERROR:VOICE_PROFILE_MISSING);
}
static void defaults_cue(int hid){
    (void)hid;voice_say(VOICE_DEFAULTS_RESTORED);
}
static void reverse_cue(int hid){
    (void)hid;voice_say(cfg.spinner_direction<0?VOICE_SPINNER_REVERSED:VOICE_SPINNER_NORMAL);
}
static void spinner_mode_cue(int hid,int mode){
    (void)hid;if(mode==0)voice_say(VOICE_WHEEL_MODE);else if(mode==1)voice_say(VOICE_SLIDE_MODE);else voice_say(VOICE_GYRO_PADDLE);
}
static void pointer_mode_cue(int hid,int mouse_mode){
    (void)hid;voice_say(mouse_mode?VOICE_MOUSE_MODE:VOICE_TRACKBALL_MODE);
}
static void pad_swap_cue(int hid,int swapped){
    (void)hid;voice_say(swapped?VOICE_PADS_SWAPPED:VOICE_PADS_NORMAL);
}
static void numpad_toggle_cue(int hid,int enabled){
    (void)hid;voice_say(enabled?VOICE_NUMPAD_ON:VOICE_NUMPAD_OFF);
}
static void numpad_layout_cue(int hid,int layout){
    (void)hid;voice_say(layout?VOICE_PHONE_PAD:VOICE_RADIAL_PAD);
}
static void numpad_hover_cue(int hid,int digit){
    int ch=spinner_haptic_channel();
    static const u16 f[9]={205,226,250,276,305,337,372,411,454};
    if(hid<0||cfg.haptic_intensity<=0||digit<1||digit>9)return;
    /* A compact textured bump makes every imaginary key discoverable in both
     * radial and telephone layouts. It is stronger than the old whisper-soft
     * tick, but remains far below a physical click or mode song. */
    haptic_tone(hid,(u8)ch,-10,f[digit-1],6,72,3);
}
static void numpad_digit_cue(int hid,int digit){
    static const u16 f[9]={270,310,350,395,445,500,565,640,725};
    int ch=spinner_haptic_channel(),i=clamp32(digit,1,9)-1;if(hid<0||cfg.haptic_intensity<=0)return;
    cute_note(hid,(u8)ch,(s8)(-10+i/2),f[i],8,80+i*12,(u8)(10+i));
}

/* Spinner detents are generated from emitted REL_DIAL movement. This does not
 * alter either spinner's motion math; it only follows the motion that actually
 * reached MiSTer. */
#define SPINNER_HAPTIC_SPACING 4
#define SPINNER_HAPTIC_MIN_MS 14
static s32 spinner_haptic_accum;
static int spinner_haptic_started;
static u32 spinner_haptic_last_ms;
static void spinner_motion_haptic(int hid,s32 emitted){
    static const u16 freq[4]={430,560,330,470};
    static const s8 gain[4]={-8,-9,-6,-7};
    s32 distance=iabs32(emitted);u32 now;int mode=cfg.gyro_enabled?3:clamp32(cfg.spinner_mode,0,1);
    if(cfg.haptic_intensity<=0||hid<0||distance<=0)return;
    spinner_haptic_accum+=distance;now=monotonic_ms();
    if(!spinner_haptic_started){spinner_haptic_started=1;spinner_haptic_accum=0;}
    if((!spinner_haptic_last_ms||(s32)(now-spinner_haptic_last_ms)>=SPINNER_HAPTIC_MIN_MS)&&
       (spinner_haptic_accum>=SPINNER_HAPTIC_SPACING||!spinner_haptic_last_ms)){
        while(spinner_haptic_accum>=SPINNER_HAPTIC_SPACING)spinner_haptic_accum-=SPINNER_HAPTIC_SPACING;
        spinner_haptic_last_ms=now;
        /* Very short tactile grains follow delivered REL_DIAL counts. The rate
         * limit is deliberately slower than controller reports so feedback
         * cannot starve motion input. */
        haptic_tone(hid,(u8)spinner_haptic_channel(),gain[mode],freq[mode],3,0,0);
    }
}

static s16 le16s(const u8 *p){return (s16)((u16)p[0]|((u16)p[1]<<8));}
static u16 le16u(const u8 *p){return (u16)((u16)p[0]|((u16)p[1]<<8));}
static u32 le32u(const u8 *p){return (u32)p[0]|((u32)p[1]<<8)|((u32)p[2]<<16)|((u32)p[3]<<24);}
static s32 consume_q16(s32 *acc);
static s32 mul_scale_q8(s32 v,s32 q8);

/* -------------------------------------------------------------------------
 * Touch-clutched player-space gyro engine.
 *
 * Design rules:
 *   1. A stationary controller emits exactly zero gyro output.
 *   2. Bias is learned from a rolling low-variance sensor window rather than
 *      guessed from one startup sample or hidden behind a large deadzone.
 *   3. Each touch-clutch activation receives a short, nonblocking zero check.
 *   4. Mouse gyro remains a direct angular-rate input with smoothing confined
 *      to the sensor-noise region.
 *   5. Stick gyro is a bounded fine-aim correction. Its component parallel to
 *      a physically moved stick is reduced more strongly than its perpendicular
 *      component, so the player's thumb retains authority over gross turning.
 *
 * G13 deliberately replaces the G11/G12 mode songs; ordinary value cues remain unchanged.
 * ------------------------------------------------------------------------- */
#define GYRO_NOMINAL_DT_US 4032
#define GYRO_TRACE_SAMPLES 128
#define GYRO_ACCEL_MAG_MIN 11800
#define GYRO_ACCEL_MAG_MAX 22200
#define GYRO_REST_WINDOW 20
#define GYRO_ACTIVATION_HOLD 12
#define GYRO_REST_GYRO_SPAN 28
#define GYRO_REST_ACCEL_SPAN 220
#define GYRO_REST_MAX_CORRECTED 36
#define GYRO_REST_EXIT_RATE 30
#define GYRO_STICK_OFF 0
#define GYRO_STICK_LEFT 1
#define GYRO_STICK_RIGHT 2
#define GYRO_OUTPUT_MOUSE 0
#define GYRO_OUTPUT_PADDLE 1
struct gyro_state {
    int enabled,sensitivity,inversion,stick_mode,active_stick_mask;
    int calibrated,active_prev,touch_prev;
    int bias_ready,timestamp_ready,timestamp_kind,sample_ready;
    int rest_index,rest_count,rest_latched,rest_exit_reports,activation_reports;
    s32 bias_x,bias_y,bias_z;
    s32 prev_gx,prev_gy,prev_gz,prev_ax,prev_ay,prev_az;
    s32 last_sample_delta,last_accel_delta;
    s32 noise_raw_q8;
    s32 grav_y_q15,grav_z_q15;int gravity_ready;
    s32 filt_pitch_q8,filt_yaw_q8,out_x_q16,out_y_q16,paddle_q16,stick_x,stick_y;
    s32 stick_l_angle_x_q16,stick_l_angle_y_q16,stick_r_angle_x_q16,stick_r_angle_y_q16;
    s32 stick_lx,stick_ly,stick_rx,stick_ry;
    s32 stick_l_pulse_q16,stick_r_pulse_q16;
    s32 stick_l_prev_px,stick_l_prev_py,stick_r_prev_px,stick_r_prev_py;
    int stick_clutch_mask,stick_hold_mask;
    s16 rest_gx[GYRO_REST_WINDOW],rest_gy[GYRO_REST_WINDOW],rest_gz[GYRO_REST_WINDOW];
    s16 rest_ax[GYRO_REST_WINDOW],rest_ay[GYRO_REST_WINDOW],rest_az[GYRO_REST_WINDOW];
    u32 total_reports,imu_changes,fresh_reports,duplicate_reports,last_ts32;u16 last_ts16;
    s32 last_dt_us;
    s32 min_gx,max_gx,min_gy,max_gy,min_gz,max_gz;
    s32 emitted_x,emitted_y,emitted_dial,active_reports,stationary_locks;
};
static struct gyro_state gyro;
static const s32 gyro_gain_q16[10]={42,60,83,112,147,190,243,309,389,488};
static const s32 gyro_paddle_gain_q16[10]={10,16,23,33,48,67,93,128,174,236};
static s32 approx_mag3(s32 x,s32 y,s32 z){
    s32 a=iabs32(x),b=iabs32(y),c=iabs32(z),t;
    if(a<b){t=a;a=b;b=t;}if(b<c){t=b;b=c;c=t;}if(a<b){t=a;a=b;b=t;}
    return a+(b>>1)+(c>>2);
}
static s32 gyro_max3(s32 x,s32 y,s32 z){
    s32 a=iabs32(x),b=iabs32(y),c=iabs32(z),m=a>b?a:b;return m>c?m:c;
}
static s32 approx_mag2(s32 x,s32 y){
    s32 ax=iabs32(x),ay=iabs32(y),hi=ax>ay?ax:ay,lo=ax>ay?ay:ax;
    return hi+((lo*3)>>3);
}
static s32 gyro_dynamic_deadzone(void){
    s32 n=gyro.noise_raw_q8>>8;
    /* Raw full scale is about 2000 deg/s, so 16 counts is roughly 1 deg/s. */
    return clamp32(4+n*2,6,18);
}
static s32 ratio_q11_wide(s32 num,s32 den,s32 max_q11);
static void gyro_tighten_vector(s32 x,s32 y,s32 deadzone,s32 *outx,s32 *outy){
    s32 mag=approx_mag2(x,y),remain;
    if(mag<=deadzone||mag<=0){*outx=*outy=0;return;}
    remain=mag-deadzone;
    *outx=sdiv32(x*remain,mag);*outy=sdiv32(y*remain,mag);
}
/* Analog-stick gyro uses two deliberately different behaviours.
 *
 * 1. Physical stick centred: gyro is a rate-to-stick precision translator.
 *    Rotating the controller produces a virtual-stick velocity only while the
 *    controller is moving. True rest is exactly zero. Very small desired
 *    outputs are temporally dithered across the game's likely anti-deadzone,
 *    following the same broad principle as Steam Input's Mouse Joystick mode.
 *
 * 2. Physical stick deflected: gyro becomes a stick-anchored micro-adjustment.
 *    Gyro movement integrates into a bounded correction that is held when the
 *    controller stops, but only while the player continues holding that physical
 *    stick away from centre. Returning the physical stick to centre or lifting
 *    the capacitive clutch clears the held correction immediately.
 *
 * This avoids the always-positional "balancing a plate" feel while preserving
 * the GoldenEye-style ability to trim and hold an already-deflected aim stick.
 */
#define GYRO_STICK_PHYS_ENTER 5000
#define GYRO_STICK_PHYS_EXIT 3000
#define GYRO_STICK_ANTI_DEAD 5000
static const s32 gyro_stick_rate_gain[10]={48,58,70,84,100,120,143,170,202,240};
static const s32 gyro_stick_rate_cap[10]={9000,10300,11700,13200,14800,16500,18300,20200,22200,24400};
static const s32 gyro_stick_full_angle_raw[10]={640,560,496,432,376,328,280,240,200,160};
static const s32 gyro_stick_max_assist[10]={7000,7900,8900,10000,11200,12500,13900,15400,17000,18800};
static void gyro_stick_reset_one(int which){
    if(which&GYRO_STICK_LEFT){
        gyro.stick_l_angle_x_q16=gyro.stick_l_angle_y_q16=0;gyro.stick_lx=gyro.stick_ly=0;
        gyro.stick_l_pulse_q16=0;gyro.stick_l_prev_px=gyro.stick_l_prev_py=0;gyro.stick_hold_mask&=~GYRO_STICK_LEFT;
    }
    if(which&GYRO_STICK_RIGHT){
        gyro.stick_r_angle_x_q16=gyro.stick_r_angle_y_q16=0;gyro.stick_rx=gyro.stick_ry=0;
        gyro.stick_r_pulse_q16=0;gyro.stick_r_prev_px=gyro.stick_r_prev_py=0;gyro.stick_hold_mask&=~GYRO_STICK_RIGHT;
    }
}
static void gyro_stick_reset_all(void){
    gyro_stick_reset_one(GYRO_STICK_LEFT|GYRO_STICK_RIGHT);gyro.stick_clutch_mask=0;gyro.stick_hold_mask=0;
}
static void gyro_stick_update_clutches(int mask){
    int changed=(mask^gyro.stick_clutch_mask)&3;
    if(changed&GYRO_STICK_LEFT)gyro_stick_reset_one(GYRO_STICK_LEFT);
    if(changed&GYRO_STICK_RIGHT)gyro_stick_reset_one(GYRO_STICK_RIGHT);
    gyro.stick_clutch_mask=mask&3;gyro.active_stick_mask=gyro.stick_clutch_mask;
}
static void gyro_stick_update_physical_one(int which,s32 px,s32 py){
    s32 mag=approx_mag2(px,py);int held=(gyro.stick_hold_mask&which)!=0;
    if(!held&&mag>=GYRO_STICK_PHYS_ENTER){
        gyro.stick_hold_mask|=which;
        if(which==GYRO_STICK_LEFT){gyro.stick_l_angle_x_q16=gyro.stick_l_angle_y_q16=0;gyro.stick_l_pulse_q16=0;gyro.stick_lx=gyro.stick_ly=0;}
        else{gyro.stick_r_angle_x_q16=gyro.stick_r_angle_y_q16=0;gyro.stick_r_pulse_q16=0;gyro.stick_rx=gyro.stick_ry=0;}
    }else if(held&&mag<=GYRO_STICK_PHYS_EXIT){
        gyro_stick_reset_one(which);
    }
    if(which==GYRO_STICK_LEFT){gyro.stick_l_prev_px=px;gyro.stick_l_prev_py=py;}
    else{gyro.stick_r_prev_px=px;gyro.stick_r_prev_py=py;}
}
static void gyro_stick_update_physical(int mask,s32 lpx,s32 lpy,s32 rpx,s32 rpy){
    if(mask&GYRO_STICK_LEFT)gyro_stick_update_physical_one(GYRO_STICK_LEFT,lpx,lpy);
    if(mask&GYRO_STICK_RIGHT)gyro_stick_update_physical_one(GYRO_STICK_RIGHT,rpx,rpy);
}
static s32 gyro_stick_smoothstep_q11(s32 t){
    s32 t2;if(t<=0)return 0;if(t>=2048)return 2048;
    t2=(t*t)>>11;return (t2*(6144-(t<<1)))>>11;
}
static void gyro_angle_to_stick(s32 ax_q16,s32 ay_q16,s32 *outx,s32 *outy){
    s32 mag=approx_mag2(ax_q16,ay_q16),dead,full,maxa,usable,t,amount,axs,ays,mags;
    int i=clamp32(gyro.sensitivity,1,10)-1;
    dead=8<<16;full=gyro_stick_full_angle_raw[i]<<16;maxa=gyro_stick_max_assist[i];
    if(mag<=dead||full<=dead){*outx=*outy=0;return;}
    usable=full-dead;t=ratio_q11_wide(mag-dead,usable,2048);t=gyro_stick_smoothstep_q11(t);amount=(maxa*t)>>11;
    axs=ax_q16>>9;ays=ay_q16>>9;mags=mag>>9;if(mags<=0){*outx=*outy=0;return;}
    *outx=clamp32(sdiv32(axs*amount,mags),-maxa,maxa);
    *outy=clamp32(sdiv32(ays*amount,mags),-maxa,maxa);
}
static void gyro_stick_integrate_hold_one(int which,s32 x,s32 y,s32 dt_us){
    s32 dt_q16=sdiv32(dt_us*65536,1000000),dx=x*dt_q16,dy=y*dt_q16;
    s32 limit=(gyro_stick_full_angle_raw[0]+96)<<16;
    if(which==GYRO_STICK_LEFT){
        gyro.stick_l_angle_x_q16=clamp32(gyro.stick_l_angle_x_q16+dx,-limit,limit);
        gyro.stick_l_angle_y_q16=clamp32(gyro.stick_l_angle_y_q16+dy,-limit,limit);
        gyro_angle_to_stick(gyro.stick_l_angle_x_q16,gyro.stick_l_angle_y_q16,&gyro.stick_lx,&gyro.stick_ly);
    }else{
        gyro.stick_r_angle_x_q16=clamp32(gyro.stick_r_angle_x_q16+dx,-limit,limit);
        gyro.stick_r_angle_y_q16=clamp32(gyro.stick_r_angle_y_q16+dy,-limit,limit);
        gyro_angle_to_stick(gyro.stick_r_angle_x_q16,gyro.stick_r_angle_y_q16,&gyro.stick_rx,&gyro.stick_ry);
    }
}
static void gyro_stick_rate_one(int which,s32 x,s32 y){
    s32 mag=approx_mag2(x,y),desired,amount,ox=0,oy=0,*acc;
    int i=clamp32(gyro.sensitivity,1,10)-1;
    if(which==GYRO_STICK_LEFT)acc=&gyro.stick_l_pulse_q16;else acc=&gyro.stick_r_pulse_q16;
    if(mag<=0){*acc=0;if(which==GYRO_STICK_LEFT)gyro.stick_lx=gyro.stick_ly=0;else gyro.stick_rx=gyro.stick_ry=0;return;}
    desired=clamp32(mag*gyro_stick_rate_gain[i],0,gyro_stick_rate_cap[i]);amount=desired;
    if(desired<GYRO_STICK_ANTI_DEAD){
        *acc+=desired<<16;
        if(*acc>=(GYRO_STICK_ANTI_DEAD<<16)){amount=GYRO_STICK_ANTI_DEAD;*acc-=GYRO_STICK_ANTI_DEAD<<16;}
        else amount=0;
    }else *acc=0;
    if(amount){ox=sdiv32(x*amount,mag);oy=sdiv32(y*amount,mag);}
    if(which==GYRO_STICK_LEFT){gyro.stick_lx=ox;gyro.stick_ly=oy;}else{gyro.stick_rx=ox;gyro.stick_ry=oy;}
}
static void gyro_stick_process(int mask,s32 x,s32 y,s32 dt_us){
    if(mask&GYRO_STICK_LEFT){
        if(gyro.stick_hold_mask&GYRO_STICK_LEFT)gyro_stick_integrate_hold_one(GYRO_STICK_LEFT,x,y,dt_us);
        else gyro_stick_rate_one(GYRO_STICK_LEFT,x,y);
    }
    if(mask&GYRO_STICK_RIGHT){
        if(gyro.stick_hold_mask&GYRO_STICK_RIGHT)gyro_stick_integrate_hold_one(GYRO_STICK_RIGHT,x,y,dt_us);
        else gyro_stick_rate_one(GYRO_STICK_RIGHT,x,y);
    }
}
static void gyro_stick_stationary(int mask){
    if((mask&GYRO_STICK_LEFT)&&!(gyro.stick_hold_mask&GYRO_STICK_LEFT)){gyro.stick_lx=gyro.stick_ly=0;gyro.stick_l_pulse_q16=0;}
    if((mask&GYRO_STICK_RIGHT)&&!(gyro.stick_hold_mask&GYRO_STICK_RIGHT)){gyro.stick_rx=gyro.stick_ry=0;gyro.stick_r_pulse_q16=0;}
}
static void clamp_stick_circle(s32 *x,s32 *y){
    s32 mag=approx_mag2(*x,*y);if(mag<=32767||mag<=0)return;
    *x=sdiv32(*x*32767,mag);*y=sdiv32(*y*32767,mag);
}
/* Combine the physical stick and gyro correction directly in the same
 * virtual-stick space. At centre, gyro is the entire precision stick. While the
 * physical stick is held, the retained gyro correction trims that position.
 * Circular clamping preserves the virtual stick gate without direction-specific
 * suppression or hidden tapering. */
static void gyro_blend_stick_assist(s32 px,s32 py,s32 gx,s32 gy,s32 *outx,s32 *outy){
    if(!gx&&!gy){*outx=px;*outy=py;return;}
    *outx=clamp32(px+gx,-32767,32767);*outy=clamp32(py+gy,-32767,32767);
    clamp_stick_circle(outx,outy);
}
static void gyro_reset_motion(void){
    gyro.filt_pitch_q8=gyro.filt_yaw_q8=0;gyro.out_x_q16=gyro.out_y_q16=gyro.paddle_q16=0;
    gyro.stick_x=gyro.stick_y=0;gyro.active_stick_mask=0;gyro_stick_reset_all();
}
static void gyro_reset_rest_window(void){
    gyro.rest_index=gyro.rest_count=0;gyro.rest_latched=gyro.rest_exit_reports=0;gyro.activation_reports=0;
}
static void gyro_apply_cfg(void){
    gyro.enabled=cfg.gyro_enabled;gyro.sensitivity=cfg.gyro_sensitivity;
    gyro.inversion=cfg.gyro_inversion;gyro.stick_mode=cfg.gyro_stick_mode;
    gyro_reset_motion();gyro_reset_rest_window();
}
static void gyro_defaults(void){
    memset(&gyro,0,sizeof(gyro));gyro_apply_cfg();gyro.noise_raw_q8=4<<8;
    gyro.min_gx=gyro.min_gy=gyro.min_gz=32767;gyro.max_gx=gyro.max_gy=gyro.max_gz=-32768;
}
static void gyro_toggle_cue(int hid,int enabled){(void)hid;voice_say(enabled?VOICE_GYRO_ON:VOICE_GYRO_OFF);}
static void gyro_level_cue(int hid,int level,int increasing){
    static const u16 f[10]={190,225,265,310,360,420,490,570,660,770};
    int i=clamp32(level,1,10)-1;if(hid<0||cfg.haptic_intensity<=0)return;
    if(increasing){haptic_tone(hid,1,-16,140,8,0,0);sleep_ms(12);haptic_tone(hid,1,-8+i*2,f[i],18+i,0,0);}
    else{haptic_tone(hid,1,-8+i*2,f[i],18+i,0,0);sleep_ms(12);haptic_tone(hid,1,-16,140,8,0,0);}
}
static void gyro_inversion_cue(int hid,int inversion){
    (void)hid;if(inversion==0)voice_say(VOICE_GYRO_NORMAL);else if(inversion==1)voice_say(VOICE_INVERT_X);
    else if(inversion==2)voice_say(VOICE_INVERT_Y);else voice_say(VOICE_INVERT_BOTH);
}
static void gyro_stick_mode_cue(int hid,int requested,int enabled){
    (void)hid;if(requested==GYRO_STICK_LEFT)voice_say(enabled?VOICE_LEFT_STICK_ON:VOICE_LEFT_STICK_OFF);
    else voice_say(enabled?VOICE_RIGHT_STICK_ON:VOICE_RIGHT_STICK_OFF);
}
static void gyro_trace_reset_session(void){}
static void gyro_trace_flush(void){}
static void gyro_change_level(int hid,int delta){
    int nv=clamp32(gyro.sensitivity+delta,1,10);if(nv==gyro.sensitivity)return;
    gyro.sensitivity=nv;cfg.gyro_sensitivity=nv;save_cfg();gyro_reset_motion();
    if(delta<0)adjustment_decrease_cue(hid,trackball_haptic_channel(),nv);else adjustment_cue(hid,trackball_haptic_channel(),nv);
    log_text(delta>0?"SC-Orbit gyro: sensitivity increased\n":"SC-Orbit gyro: sensitivity decreased\n");
}
static void gyro_cycle_inversion(int hid){
    gyro.inversion=(gyro.inversion+1)&3;cfg.gyro_inversion=gyro.inversion;save_cfg();gyro_reset_motion();
    gyro_inversion_cue(hid,gyro.inversion);log_text("SC-Orbit gyro: X/Y inversion profile changed\n");
}
static void gyro_toggle_stick_mode(int hid,int requested){
    int enabled;gyro.stick_mode^=requested;gyro.stick_mode&=3;cfg.gyro_stick_mode=gyro.stick_mode;save_cfg();gyro_reset_motion();
    enabled=(gyro.stick_mode&requested)!=0;gyro_stick_mode_cue(hid,requested,enabled);
    if(requested==GYRO_STICK_LEFT)log_text(enabled?"SC-Orbit gyro: LEFT-STICK assistance ENABLED\n":"SC-Orbit gyro: LEFT-STICK assistance DISABLED\n");
    else log_text(enabled?"SC-Orbit gyro: RIGHT-STICK assistance ENABLED\n":"SC-Orbit gyro: RIGHT-STICK assistance DISABLED\n");
}
static void gyro_toggle(int hid){
    gyro.enabled=!gyro.enabled;cfg.gyro_enabled=gyro.enabled;save_cfg();gyro_reset_motion();gyro_reset_rest_window();gyro_toggle_cue(hid,gyro.enabled);
    log_text(gyro.enabled?"SC-Orbit gyro: MASTER ENABLED\n":"SC-Orbit gyro: MASTER DISABLED - all gyro outputs blocked\n");
}
static int gyro_fresh_dt(const u8 *b,int n,s32 *dt_us){
    u32 d;int kind;if(n<46)return 0;kind=b[0]==0x47?1:2;
    if(kind==1){u16 t=le16u(b+32);if(!gyro.timestamp_ready||gyro.timestamp_kind!=kind){gyro.last_ts16=t;gyro.timestamp_ready=1;gyro.timestamp_kind=kind;*dt_us=GYRO_NOMINAL_DT_US;gyro.imu_changes++;return 1;}d=(u32)(u16)(t-gyro.last_ts16)*32u;if(!d){gyro.duplicate_reports++;return 0;}gyro.last_ts16=t;}
    else{u32 t=le32u(b+30);if(!gyro.timestamp_ready||gyro.timestamp_kind!=kind){gyro.last_ts32=t;gyro.timestamp_ready=1;gyro.timestamp_kind=kind;*dt_us=GYRO_NOMINAL_DT_US;gyro.imu_changes++;return 1;}d=t-gyro.last_ts32;if(!d){gyro.duplicate_reports++;return 0;}gyro.last_ts32=t;}
    gyro.imu_changes++;if(d<1000u)d=1000u;if(d>16000u)d=16000u;*dt_us=(s32)d;return 1;
}
static void gyro_sensor_deltas(s32 ax,s32 ay,s32 az,s32 gx,s32 gy,s32 gz){
    if(!gyro.sample_ready){gyro.prev_gx=gx;gyro.prev_gy=gy;gyro.prev_gz=gz;gyro.prev_ax=ax;gyro.prev_ay=ay;gyro.prev_az=az;gyro.last_sample_delta=gyro.last_accel_delta=0;gyro.sample_ready=1;return;}
    gyro.last_sample_delta=gyro_max3(gx-gyro.prev_gx,gy-gyro.prev_gy,gz-gyro.prev_gz);gyro.last_accel_delta=gyro_max3(ax-gyro.prev_ax,ay-gyro.prev_ay,az-gyro.prev_az);
    gyro.prev_gx=gx;gyro.prev_gy=gy;gyro.prev_gz=gz;gyro.prev_ax=ax;gyro.prev_ay=ay;gyro.prev_az=az;
}
static void gyro_update_gravity(s32 ax,s32 ay,s32 az,int active){
    s32 mag=approx_mag3(ax,ay,az),sy=az,sz=-ay,ty,tz,shift;
    if(mag<GYRO_ACCEL_MAG_MIN||mag>GYRO_ACCEL_MAG_MAX||gyro.last_accel_delta>1000)return;
    ty=sdiv32(sy*32767,mag);tz=sdiv32(sz*32767,mag);
    if(!gyro.gravity_ready){gyro.grav_y_q15=ty;gyro.grav_z_q15=tz;gyro.gravity_ready=1;return;}
    shift=active?7:4;gyro.grav_y_q15+=(ty-gyro.grav_y_q15)>>shift;gyro.grav_z_q15+=(tz-gyro.grav_z_q15)>>shift;
}
static void gyro_player_space(s32 gx,s32 gy,s32 gz,s32 *pitch,s32 *yaw){
    s32 p=gx-gyro.bias_x,local_yaw=gz-gyro.bias_z,roll=-(gy-gyro.bias_y),world;
    *pitch=p;if(!gyro.gravity_ready){*yaw=local_yaw;return;}
    world=(s32)((((long long)local_yaw*gyro.grav_y_q15)+((long long)roll*gyro.grav_z_q15))>>15);
    *yaw=world;
}
static void gyro_rest_push(s32 ax,s32 ay,s32 az,s32 gx,s32 gy,s32 gz){
    int i=gyro.rest_index;gyro.rest_gx[i]=(s16)gx;gyro.rest_gy[i]=(s16)gy;gyro.rest_gz[i]=(s16)gz;
    gyro.rest_ax[i]=(s16)ax;gyro.rest_ay[i]=(s16)ay;gyro.rest_az[i]=(s16)az;
    gyro.rest_index=(i+1)%GYRO_REST_WINDOW;if(gyro.rest_count<GYRO_REST_WINDOW)gyro.rest_count++;
}
static int gyro_rest_stats(s32 *mx,s32 *my,s32 *mz,s32 *noise){
    int i,n=gyro.rest_count;s32 mingx=32767,maxgx=-32768,mingy=32767,maxgy=-32768,mingz=32767,maxgz=-32768;
    s32 minax=32767,maxax=-32768,minay=32767,maxay=-32768,minaz=32767,maxaz=-32768,sx=0,sy=0,sz=0,span;
    if(n<GYRO_REST_WINDOW)return 0;
    for(i=0;i<n;i++){s32 x=gyro.rest_gx[i],y=gyro.rest_gy[i],z=gyro.rest_gz[i],ax=gyro.rest_ax[i],ay=gyro.rest_ay[i],az=gyro.rest_az[i];
        sx+=x;sy+=y;sz+=z;if(x<mingx)mingx=x;if(x>maxgx)maxgx=x;if(y<mingy)mingy=y;if(y>maxgy)maxgy=y;if(z<mingz)mingz=z;if(z>maxgz)maxgz=z;
        if(ax<minax)minax=ax;if(ax>maxax)maxax=ax;if(ay<minay)minay=ay;if(ay>maxay)maxay=ay;if(az<minaz)minaz=az;if(az>maxaz)maxaz=az;}
    if(maxgx-mingx>GYRO_REST_GYRO_SPAN||maxgy-mingy>GYRO_REST_GYRO_SPAN||maxgz-mingz>GYRO_REST_GYRO_SPAN)return 0;
    if(maxax-minax>GYRO_REST_ACCEL_SPAN||maxay-minay>GYRO_REST_ACCEL_SPAN||maxaz-minaz>GYRO_REST_ACCEL_SPAN)return 0;
    *mx=sdiv32(sx,n);*my=sdiv32(sy,n);*mz=sdiv32(sz,n);span=gyro_max3(maxgx-mingx,maxgy-mingy,maxgz-mingz);*noise=(span+1)>>1;return 1;
}
static void gyro_set_bias(s32 x,s32 y,s32 z,int snap,s32 noise){
    if(!gyro.bias_ready||snap){gyro.bias_x=x;gyro.bias_y=y;gyro.bias_z=z;gyro.bias_ready=1;gyro.calibrated=1;}
    else{gyro.bias_x+=(x-gyro.bias_x)>>3;gyro.bias_y+=(y-gyro.bias_y)>>3;gyro.bias_z+=(z-gyro.bias_z)>>3;}
    gyro.noise_raw_q8+=(((noise<<8)-gyro.noise_raw_q8)>>4);
}
static void gyro_adaptive_filter(s32 pitch,s32 yaw,s32 *outp,s32 *outy){
    s32 mag=approx_mag2(pitch,yaw),alpha,target,diff;
    if(mag>=96)alpha=256;else alpha=72+sdiv32(mag*184,96);
    target=pitch<<8;diff=target-gyro.filt_pitch_q8;gyro.filt_pitch_q8+=(diff*alpha)>>8;
    target=yaw<<8;diff=target-gyro.filt_yaw_q8;gyro.filt_yaw_q8+=(diff*alpha)>>8;
    *outp=gyro.filt_pitch_q8>>8;*outy=gyro.filt_yaw_q8>>8;
}
static void gyro_zero_transient_preserve_sticks(void){
    gyro.filt_pitch_q8=gyro.filt_yaw_q8=0;gyro.out_x_q16=gyro.out_y_q16=gyro.paddle_q16=0;gyro.stick_x=gyro.stick_y=0;
}
static void gyro_record_sample(s32 gx,s32 gy,s32 gz,s32 cx,s32 cz,s32 ox,s32 oy,s32 dt_us,s32 dz){
    (void)gx;(void)gy;(void)gz;(void)cx;(void)cz;(void)ox;(void)oy;(void)dt_us;(void)dz;
}
static void process_gyro(int hid,int mouse,int spinner,const u8 *b,int n,int active,int output_mode,int pointer_active,int stick_active_mask){
    s32 ax,ay,az,gx,gy,gz,pitch=0,yaw=0,cp=0,cy=0,mx,my,ox=0,oy=0,dt_us=GYRO_NOMINAL_DT_US,dt_q8,base,dz,meanx=0,meany=0,meanz=0,noise=0,raw_corrected;
    s32 plx=0,ply=0,prx=0,pry=0;
    int stable;
    if(n<46){if(gyro.touch_prev){gyro_trace_flush();gyro_trace_reset_session();}gyro_reset_motion();gyro_reset_rest_window();gyro.active_prev=gyro.touch_prev=0;gyro.timestamp_ready=gyro.timestamp_kind=0;return;}
    plx=le16s(b+10);ply=-le16s(b+12);prx=le16s(b+14);pry=-le16s(b+16);
    if(iabs32(plx)<1800)plx=0;if(iabs32(ply)<1800)ply=0;if(iabs32(prx)<1800)prx=0;if(iabs32(pry)<1800)pry=0;
    gyro_stick_update_clutches(stick_active_mask&gyro.stick_mode);
    gyro_stick_update_physical(stick_active_mask&gyro.stick_mode,plx,ply,prx,pry);
    if(active&&!gyro.touch_prev){gyro_trace_reset_session();gyro.activation_reports=0;gyro.rest_latched=0;gyro.rest_exit_reports=0;gyro.filt_pitch_q8=gyro.filt_yaw_q8=gyro.out_x_q16=gyro.out_y_q16=gyro.paddle_q16=0;}
    if(!active&&gyro.touch_prev){gyro_trace_flush();gyro_trace_reset_session();gyro_reset_motion();}
    gyro.touch_prev=active?1:0;if(!gyro_fresh_dt(b,n,&dt_us)){if(!active)gyro_reset_motion();return;}
    ax=le16s(b+34);ay=le16s(b+36);az=le16s(b+38);gx=le16s(b+40);gy=le16s(b+42);gz=le16s(b+44);
    gyro.total_reports++;gyro.fresh_reports++;gyro.last_dt_us=dt_us;gyro_sensor_deltas(ax,ay,az,gx,gy,gz);
    if(voice_busy){gyro_reset_motion();gyro_reset_rest_window();gyro.active_prev=0;return;}
    gyro_rest_push(ax,ay,az,gx,gy,gz);stable=gyro_rest_stats(&meanx,&meany,&meanz,&noise);
    gyro_update_gravity(ax,ay,az,active);
    if(!active){if(stable&&(!gyro.bias_ready||gyro_max3(meanx-gyro.bias_x,meany-gyro.bias_y,meanz-gyro.bias_z)<=48))gyro_set_bias(meanx,meany,meanz,!gyro.bias_ready,noise);gyro_reset_motion();gyro.rest_latched=0;gyro.activation_reports=0;gyro.active_prev=0;return;}
    gyro.active_prev=1;gyro.active_reports++;if(gx<gyro.min_gx)gyro.min_gx=gx;if(gx>gyro.max_gx)gyro.max_gx=gx;if(gy<gyro.min_gy)gyro.min_gy=gy;if(gy>gyro.max_gy)gyro.max_gy=gy;if(gz<gyro.min_gz)gyro.min_gz=gz;if(gz>gyro.max_gz)gyro.max_gz=gz;
    gyro.activation_reports++;
    if(stable&&gyro.activation_reports<=GYRO_ACTIVATION_HOLD){gyro_set_bias(meanx,meany,meanz,1,noise);gyro.rest_latched=1;gyro.stationary_locks++;gyro_stick_stationary(stick_active_mask&gyro.stick_mode);gyro_zero_transient_preserve_sticks();gyro_record_sample(gx,gy,gz,0,0,0,0,dt_us,gyro_dynamic_deadzone());return;}
    if(gyro.activation_reports<=GYRO_ACTIVATION_HOLD){gyro_zero_transient_preserve_sticks();gyro_record_sample(gx,gy,gz,0,0,0,0,dt_us,gyro_dynamic_deadzone());return;}
    if(!gyro.bias_ready){if(stable)gyro_set_bias(meanx,meany,meanz,1,noise);gyro_zero_transient_preserve_sticks();return;}
    raw_corrected=gyro_max3(gx-gyro.bias_x,gy-gyro.bias_y,gz-gyro.bias_z);
    if(stable&&raw_corrected<=GYRO_REST_MAX_CORRECTED){gyro_set_bias(meanx,meany,meanz,1,noise);if(!gyro.rest_latched)gyro.stationary_locks++;gyro.rest_latched=1;gyro.rest_exit_reports=0;gyro_stick_stationary(stick_active_mask&gyro.stick_mode);gyro_zero_transient_preserve_sticks();gyro_record_sample(gx,gy,gz,0,0,0,0,dt_us,gyro_dynamic_deadzone());return;}
    if(gyro.rest_latched){if(raw_corrected>GYRO_REST_EXIT_RATE)gyro.rest_exit_reports++;else gyro.rest_exit_reports=0;if(gyro.rest_exit_reports<2){gyro_stick_stationary(stick_active_mask&gyro.stick_mode);gyro_zero_transient_preserve_sticks();gyro_record_sample(gx,gy,gz,0,0,0,0,dt_us,gyro_dynamic_deadzone());return;}gyro.rest_latched=0;gyro.rest_exit_reports=0;}
    gyro_player_space(gx,gy,gz,&pitch,&yaw);dz=gyro_dynamic_deadzone();gyro_tighten_vector(pitch,yaw,dz,&cp,&cy);
    if(!cp&&!cy){gyro_stick_stationary(stick_active_mask&gyro.stick_mode);gyro_zero_transient_preserve_sticks();gyro_record_sample(gx,gy,gz,0,0,0,0,dt_us,dz);return;}
    gyro_adaptive_filter(cp,cy,&cp,&cy);dt_q8=clamp32(sdiv32(dt_us*256+GYRO_NOMINAL_DT_US/2,GYRO_NOMINAL_DT_US),64,1024);
    mx=(gyro.inversion&1)?-cy:cy;my=(gyro.inversion&2)?-cp:cp;
    if(output_mode==GYRO_OUTPUT_PADDLE){base=mx*gyro_paddle_gain_q16[gyro.sensitivity-1];gyro.paddle_q16+=mul_scale_q8(base,dt_q8);ox=consume_q16(&gyro.paddle_q16);ox=clamp32(ox,-12,12);
        if(ox){ox*=cfg.spinner_direction;rel_spinner(spinner,ox);spinner_motion_haptic(hid,ox);gyro.emitted_dial+=ox;}gyro.stick_x=gyro.stick_y=0;gyro.active_stick_mask=0;gyro_record_sample(gx,gy,gz,cp,cy,0,ox,dt_us,dz);return;}
    gyro.active_stick_mask=stick_active_mask&gyro.stick_mode;
    if(gyro.active_stick_mask)gyro_stick_process(gyro.active_stick_mask,mx,my,dt_us);
    gyro.stick_x=gyro.stick_y=0;
    if(pointer_active){base=mx*gyro_gain_q16[gyro.sensitivity-1];gyro.out_x_q16+=mul_scale_q8(base,dt_q8);base=my*gyro_gain_q16[gyro.sensitivity-1];gyro.out_y_q16+=mul_scale_q8(base,dt_q8);
        ox=consume_q16(&gyro.out_x_q16);oy=consume_q16(&gyro.out_y_q16);if(ox||oy){rel_trackball_mouse(mouse,active_trackball_pad,ox,oy);gyro.emitted_x+=ox;gyro.emitted_y+=oy;}}
    else gyro.out_x_q16=gyro.out_y_q16=0;gyro_record_sample(gx,gy,gz,cp,cy,ox,oy,dt_us,dz);
}
/* Ten thumb-on tracking families. Level 6 preserves the former midpoint;
 * the new endpoints deliberately extend both slow precision and fast arcade play. */
static const s32 tb_touch_scale_q16[10]={80,128,192,272,368,480,608,752,912,1120};
/* Approximate direct tracking ceilings in REL units/second:
 * 500, 800, 1200, 1700, 2300, 3000, 3800, 4700, 5700, 7000. */
static const s32 tb_speed_limit_q16[10]={131072,209715,314573,445645,602931,786432,996147,1232077,1494221,1835008};
#define TB_FLICK_SCALE_Q8 560
/* Independent coast-launch ceilings, REL units/second:
 * 300, 500, 750, 1050, 1450, 2000, 2600, 3300, 4100, 5000. */
static const s32 tb_launch_limit_q16[10]={78643,131072,196608,275251,380109,524288,681574,865075,1074790,1310720};
/* Momentum-transfer tuning, selected with ... + D-pad Up/Down.
 * Percentages: 0,10,20,35,50,70,100,130,165,210. Level 1 intentionally
 * transfers no release momentum at all; high levels exaggerate both normal
 * launch energy and the qualified hyper-flick. */
static const s32 tb_momentum_transfer_q8[10]={0,26,51,90,128,179,256,333,422,538};
#define TB_FLICK_MEASURE_LIMIT_Q16 1703936 /* common 6500 REL/s measurement guard */
/* A truly violent, coherent release may exceed the selected normal coast cap.
 * The bonus rises with squared excess speed, so ordinary and merely brisk flicks
 * remain capped. At the measurement ceiling the temporary cap can reach 175% of
 * the selected value, never above 7500 REL/s. */
#define TB_HYPER_THRESHOLD_Q16 786432       /* 3000 REL/s: deliberately hard but reachable */
#define TB_HYPER_PEAK_Q16 891290            /* 3400 REL/s terminal peak */
#define TB_HYPER_FULL_SPEED_Q16 1310720      /* 5000 REL/s earns full speed energy */
#define TB_HYPER_COHERENCE_Q8 205            /* about 80 percent directional coherence */
#define TB_HYPER_MIN_STROKE_Q16 524288       /* 8 REL of coherent terminal travel */
#define TB_HYPER_FULL_STROKE_Q16 1572864     /* 24 REL gives full stroke confidence */
#define TB_HYPER_MAX_BONUS_Q8 256            /* up to +100 percent launch energy/cap */
#define TB_HYPER_ABS_CAP_Q16 2359296          /* 9000 REL/s absolute guard */
/* Ten pure rolling-resistance grades. A launch is identical at every grade;
 * only opposing torque changes afterward. The expanded range reaches both a
 * firmer heavy ball and a freer bearing than the old five-step table. */
static const s32 tb_roll_friction_q16[10]={12000,10000,8500,7000,5600,4200,3300,2500,1800,1200};
#define TB_VISCOUS_SHIFT 13
#define COAST_HAPTIC_SPACING_Q16 393216
#define MOTION_HAPTIC_MIN_MS 14
#define BUTTON_HAPTIC_GUARD_MS 18
/* Ten spinner speed levels. Level 6 preserves the former level-3 circular and
 * horizontal response while new extremes broaden both ends. */
static const s32 sp_counts_rev[10]={30,42,56,70,86,103,123,146,174,208};
static const s32 sp_linear_scale_q16[10]={120,160,205,250,295,342,400,470,550,650};
static const s32 sp_linear_max_step[10]={14,18,24,30,36,42,50,60,72,88};
/* Fluid circular encoder confidence. There is no broad center dead zone and no
 * reacquire delay. Radius now scales confidence continuously: imperfect circles
 * near the center slow gracefully instead of freezing and then jumping. Large
 * but plausible fast-twirl samples are softly limited rather than discarded;
 * only physically absurd teleports are rebased. Direction remains locked against
 * isolated sensor reversals. */
#define PAD_RAW_DISCONTINUITY 24000
#define SPINNER_LINEAR_DISCONTINUITY 18000
#define PAD_NOISE_FLOOR 8
#define SPINNER_CENTER_ZERO_RADIUS 700
#define SPINNER_FULL_CONFIDENCE_RADIUS 6000
#define SPINNER_FULL_TURN_Q13 51472
#define SPINNER_DIRECT_ABS_MAX_DELTA_Q13 20018 /* about 140 degrees: impossible teleport */
#define SPINNER_DYNAMIC_FLOOR_Q13 2860         /* about 20 degrees */
#define SPINNER_DYNAMIC_HEADROOM_Q13 1144      /* about 8 degrees */
#define SPINNER_DYNAMIC_MAX_Q13 10725          /* about 75 degrees */
#define SPINNER_REVERSE_CONFIRM_Q13 286        /* about 2.0 degrees */
#define SPINNER_REVERSE_CONFIRM_REPORTS 3
#define SPINNER_DIRECTION_UNLOCK_REPORTS 4
#define SPINNER_MICRO_NOISE_Q13 10             /* about 0.07 degree */
#define SPINNER_TRACE_MAX 64
#define FLICK_HISTORY 8
/* Release analysis is entirely report-based. The stored values are actual
 * cursor-space displacement produced during each controller report. No wall
 * clock or raw-pad-distance value can manufacture release velocity. */
#define RELEASE_FINAL_REPORTS 4
#define RELEASE_SAMPLE_FLOOR 12
/* Release is terminal angular velocity, not average swipe duration. Skip up to
 * two lift-edge quiet reports, then estimate velocity from one to four newest
 * reports with continuous newest-first weighting. A single report is accepted
 * only when it is unmistakably energetic; this allows very fast short flicks
 * without turning sensor noise into coast. */
#define RELEASE_TAIL_ACTIVITY_Q16 4096        /* 0.0625 REL */
#define RELEASE_MAX_QUIET_TAIL 2              /* tolerate up to 8 ms after physical lift */
#define RELEASE_MOVING_STEP_Q16 12288          /* 0.1875 REL */
#define RELEASE_SINGLE_REPORT_MIN_Q16 98304   /* 1.5 REL/report = 375 REL/s */
#define RELEASE_MIN_DISTANCE_Q16 49152         /* 0.75 REL total terminal travel */
#define RELEASE_MIN_SPEED_Q16 24576            /* 0.375 REL/report = 93.75 REL/s */
#define RELEASE_MIN_COHERENCE_Q8 166            /* about 65% directional consistency */
#define RELEASE_MIN_LAUNCH_Q16 8192             /* below 31.25 REL/s, treat as no momentum */

struct motion_state{
    int spinner_touch_prev,trackball_touch_prev;
    int trackball_source_pad; /* physical pad that owns current touch/coast */
    int spinner_seeded;
    int spinner_rearm,trackball_rearm;
    s32 spinner_x_prev,spinner_y_prev,trackball_x_prev,trackball_y_prev;
    s32 spinner_fx_q8,spinner_fy_q8; /* retained for settings-compatible state layout; unused by circular mode */
    /* Spinner state. spinner_q16 retains fractional REL_DIAL movement in all
     * three relative modes; arc mode additionally tracks report timing. */
    s32 spinner_q16;
    int spinner_ring_active,spinner_reacquire_reports,spinner_last_direction;
    int spinner_reverse_reports,spinner_quiet_reports;
    s32 spinner_reverse_q13;
    s32 spinner_reject_x,spinner_reject_y;
    int spinner_reject_count;
    u32 spinner_arc_last_ms; /* retained compatibility state slot */
    s32 mouse_x_q16,mouse_y_q16;
    s32 mouse_vx_q16,mouse_vy_q16;
    s32 release_vx_q16,release_vy_q16;
    s32 flick_dx[FLICK_HISTORY],flick_dy[FLICK_HISTORY];
    int flick_head,flick_count;
    s32 filt_dx_q8,filt_dy_q8;
    int touch_quiet_reports;
    s32 touch_haptic_accum_q16,coast_haptic_accum_q16;
    int coast_haptic_started;
    u32 last_touch_ms,last_touch_haptic_ms,last_coast_haptic_ms,haptic_guard_until_ms;
    u32 button_haptic_prev;
};
static struct motion_state ms;
static int spinner_trace_count,spinner_trace_active;
static void spinner_trace_reset(int mode){(void)mode;spinner_trace_count=spinner_trace_active=0;}
static void spinner_trace_add(s32 x,s32 y,s32 radius,s32 delta_q13,s32 out,s32 status){
    (void)x;(void)y;(void)radius;(void)delta_q13;(void)out;(void)status;
}
static void spinner_trace_flush(void){spinner_trace_count=spinner_trace_active=0;}
enum release_reason {
    RR_ACCEPT=0,RR_HISTORY=1,RR_MOVING=2,RR_LAST_STEP=3,RR_DISTANCE=4,
    RR_COHERENCE=5,RR_ZERO_SPEED=6,RR_DIRECTION=7,RR_BELOW_SPEED=8,
    RR_SLOWED=9,RR_ZERO_CARRY=10,RR_SMALL_LAUNCH=11,RR_ZERO_LAUNCH=12
};
struct release_telemetry {
    u32 id;
    int active,reason,accepted;
    int speed_level,tightness,flick_count,moving,final_n,earlier_n;
    s32 final_x,final_y,final_scalar,last_mag,vector_mag,coherence;
    s32 final_vx,final_vy,final_speed,earlier_speed,limit,min_speed,full_speed;
    s32 score,carry,launch_x,launch_y,launch_mag,normal_limit,hyper_limit,hyper_stroke;
    s32 sample_x[FLICK_HISTORY],sample_y[FLICK_HISTORY];
    int sample_count,coast_ticks,coast_emitted_rel,haptic_pulses,hyper_flick;
    s32 coast_distance_q16;
};
static struct release_telemetry rt;
static const char *release_reason_name(int r){
    switch(r){
        case RR_ACCEPT:return "accepted";case RR_HISTORY:return "insufficient_history";
        case RR_MOVING:return "not_enough_recent_motion";case RR_LAST_STEP:return "paused_before_lift";
        case RR_DISTANCE:return "final_distance_too_small";case RR_COHERENCE:return "direction_coherence_low";
        case RR_ZERO_SPEED:return "final_speed_zero";case RR_DIRECTION:return "direction_mismatch";
        case RR_BELOW_SPEED:return "below_release_speed_gate";case RR_SLOWED:return "slowed_before_lift";
        case RR_ZERO_CARRY:return "transfer_zero";case RR_SMALL_LAUNCH:return "launch_below_minimum";
        case RR_ZERO_LAUNCH:return "launch_vector_zero";default:return "unknown";
    }
}
static void telemetry_snapshot_samples(void){
    int idx=ms.flick_head-1,n=0;
    if(idx<0)idx=FLICK_HISTORY-1;
    while(n<ms.flick_count&&n<FLICK_HISTORY){rt.sample_x[n]=ms.flick_dx[idx];rt.sample_y[n]=ms.flick_dy[idx];n++;idx--;if(idx<0)idx=FLICK_HISTORY-1;}
    rt.sample_count=n;
}
static void telemetry_begin_release(void){
    u32 id=rt.id+1;memset(&rt,0,sizeof(rt));rt.id=id;
    rt.speed_level=cfg.trackball_speed;rt.tightness=cfg.trackball_friction;rt.flick_count=ms.flick_count;
    telemetry_snapshot_samples();
}
static void telemetry_release_line(void){}
static void telemetry_reject(int reason){rt.reason=reason;rt.accepted=0;rt.active=0;telemetry_release_line();}
static void telemetry_accept(s32 x,s32 y,s32 mag){
    /* Defer accepted-release disk logging until coast ends. The release-to-roll
     * handoff therefore performs no FAT write at all. */
    rt.reason=RR_ACCEPT;rt.accepted=1;rt.active=1;rt.launch_x=x;rt.launch_y=y;rt.launch_mag=mag;
}
static void telemetry_finish_coast(int stop_reason){(void)stop_reason;rt.active=0;}
static s32 consume_q16(s32 *acc){s32 q=sdiv32(*acc,65536);*acc-=q*65536;return q;}
/* Fixed-point helpers avoid compiler-provided 64-bit multiply/divide routines. */
static s32 mul_scale_q8(s32 v,s32 q8){
    s32 whole=sdiv32(v,256),rem=v-whole*256;
    return whole*q8+sdiv32(rem*q8,256);
}
static s32 mul_scale_q11(s32 v,s32 q11){
    s32 whole=sdiv32(v,2048),rem=v-whole*2048;
    return whole*q11+sdiv32(rem*q11,2048);
}
static s32 consume_q16_limited(s32 *acc,s32 limit){
    s32 q=sdiv32(*acc,65536),out=clamp32(q,-limit,limit);
    *acc-=out*65536;return out;
}
static s32 soft_gate(s32 v){s32 a=iabs32(v);if(a<=PAD_NOISE_FLOOR)return 0;return v>0?v-(PAD_NOISE_FLOOR>>1):v+(PAD_NOISE_FLOOR>>1);}
static void zero_motion(void){
    telemetry_finish_coast(3);
    ms.spinner_q16=0;ms.spinner_fx_q8=ms.spinner_fy_q8=0;ms.spinner_ring_active=0;ms.spinner_reacquire_reports=0;
    ms.spinner_last_direction=0;ms.spinner_reverse_q13=0;ms.spinner_reverse_reports=0;ms.spinner_quiet_reports=0;ms.spinner_reject_count=0;
    ms.spinner_arc_last_ms=0;
    ms.mouse_x_q16=ms.mouse_y_q16=0;
    ms.mouse_vx_q16=ms.mouse_vy_q16=0;
    ms.release_vx_q16=ms.release_vy_q16=0;
    memset(ms.flick_dx,0,sizeof(ms.flick_dx));memset(ms.flick_dy,0,sizeof(ms.flick_dy));
    ms.flick_head=ms.flick_count=0;
    ms.filt_dx_q8=ms.filt_dy_q8=0;
    ms.touch_haptic_accum_q16=ms.coast_haptic_accum_q16=0;
    spinner_haptic_accum=0;spinner_haptic_started=0;spinner_haptic_last_ms=0;
    ms.coast_haptic_started=0;
    ms.last_touch_ms=0;
    ms.spinner_seeded=0;
    ms.trackball_source_pad=-1;
    spinner_trace_active=0;spinner_trace_count=0;
}
static void change_level(int hid,int *value,int delta,int pad){
    int nv=clamp32(*value+delta,1,10);
    if(nv==*value)return;
    *value=nv;save_cfg();
    if(delta<0)adjustment_decrease_cue(hid,pad,nv);
    else adjustment_cue(hid,pad,nv);
}
static void change_momentum_level(int hid,int delta){
    int nv=clamp32(cfg.trackball_momentum+delta,1,10);
    if(nv==cfg.trackball_momentum)return;
    cfg.trackball_momentum=nv;save_cfg();
    momentum_adjustment_cue(hid,trackball_haptic_channel(),nv,delta>0);
}
static void clear_flick_history(void);
/* Full-angle CORDIC. Unlike the old cross/radius approximation, this returns
 * the signed angle between two pad vectors across the complete -pi..pi range.
 * The first terms retain tiny movement accurately; no floating point, libc,
 * speed thresholds, or gain table are involved. */
static const s32 cordic_atan_q13[14]={6434,3798,2007,1019,511,256,128,64,32,16,8,4,2,1};
static s32 atan2_cordic_q13(s32 y,s32 x){
    s32 angle=0,oldx,maxv;int i;
    if(!x&&!y)return 0;
    maxv=iabs32(x);if(iabs32(y)>maxv)maxv=iabs32(y);
    while(maxv>67108864){x>>=1;y>>=1;maxv>>=1;}
    if(x<0){
        angle=y>=0?25736:-25736;
        x=-x;y=-y;
    }
    for(i=0;i<14;i++){
        oldx=x;
        if(y>0){x+=y>>i;y-=oldx>>i;angle+=cordic_atan_q13[i];}
        else if(y<0){x-=y>>i;y+=oldx>>i;angle-=cordic_atan_q13[i];}
        else break;
    }
    if(angle>25736)angle-=SPINNER_FULL_TURN_Q13;
    if(angle<-25736)angle+=SPINNER_FULL_TURN_Q13;
    return angle;
}
static s32 angular_delta_q13_direct(s32 px,s32 py,s32 x,s32 y){
    s32 cross,dot,maxv,shift=0;
    maxv=iabs32(px);if(iabs32(py)>maxv)maxv=iabs32(py);
    if(iabs32(x)>maxv)maxv=iabs32(x);if(iabs32(y)>maxv)maxv=iabs32(y);
    while(maxv>15000){maxv>>=1;shift++;}
    px>>=shift;py>>=shift;x>>=shift;y>>=shift;
    cross=px*y-py*x;
    dot=px*x+py*y;
    return atan2_cordic_q13(cross,dot);
}

/* Approximate radius without overflow. It is used only for confidence gates;
 * angle and sensitivity remain independent of radius. */
static s32 spinner_radius(s32 x,s32 y){
    s32 ax=iabs32(x),ay=iabs32(y),hi=ax>ay?ax:ay,lo=ax>ay?ay:ax;
    return hi+(lo>>1);
}
static int numpad_digit_from_position(s32 x,s32 y){
    static const int keys[10]={0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9};
    int digit;
    if(cfg.numpad_layout){
        /* Steam Controller pad Y is positive toward the physical top. Phone
         * layout is therefore exactly:
         *   1 2 3
         *   4 5 6
         *   7 8 9
         * with 1/4/7 forming the left column. */
        int col=x<-9000?0:(x>9000?2:1);
        int row=y>9000?0:(y<-9000?2:1);
        digit=1+row*3+col;
    }else{
        s32 a,sector;
        /* atan2(x,y) measures clockwise from physical top because pad Y is
         * positive upward: 1 is top-center, 2 is immediately clockwise/right,
         * and 3..9 continue clockwise around nine equal sectors. */
        a=atan2_cordic_q13(x,y);
        while(a<0)a+=SPINNER_FULL_TURN_Q13;
        while(a>=SPINNER_FULL_TURN_Q13)a-=SPINNER_FULL_TURN_Q13;
        sector=sdiv32(a*9+(SPINNER_FULL_TURN_Q13>>1),SPINNER_FULL_TURN_Q13);
        if(sector>=9)sector=0;digit=(int)sector+1;
    }
    return keys[clamp32(digit,1,9)];
}
static int numpad_digit_for_key(int key){return key>=KEY_1&&key<=KEY_9?key-KEY_1+1:0;}
static void process_numpad(int hid,int keypad,int touch,int click,s32 x,s32 y){
    int key=0;
    if(numpad_rearm){
        release_numpad_key(keypad);numpad_hover_key=numpad_hover_candidate=numpad_hover_reports=0;
        if(!touch&&!click)numpad_rearm=0;
        return;
    }
    if(!cfg.numpad_enabled){release_numpad_key(keypad);numpad_hover_key=numpad_hover_candidate=numpad_hover_reports=0;return;}
    if(touch){
        key=numpad_digit_from_position(x,y);
        if(key==numpad_hover_key){numpad_hover_candidate=numpad_hover_reports=0;}
        else{
            if(key==numpad_hover_candidate)numpad_hover_reports++;
            else{numpad_hover_candidate=key;numpad_hover_reports=1;}
            /* Two consecutive reports make sector edges tactile without
             * buzzing when a thumb rests directly on a boundary. */
            if(numpad_hover_reports>=2){
                numpad_hover_key=key;numpad_hover_candidate=numpad_hover_reports=0;
                numpad_hover_cue(hid,numpad_digit_for_key(key));
            }
        }
    }else numpad_hover_key=numpad_hover_candidate=numpad_hover_reports=0;
    if(click&&!numpad_click_prev){
        if(!key)key=numpad_digit_from_position(x,y);numpad_key_down=key;
        emit(keypad,EV_KEY,(u16)key,1);syncfd(keypad);numpad_digit_cue(hid,numpad_digit_for_key(key));
    }else if(!click&&numpad_click_prev)release_numpad_key(keypad);
    numpad_click_prev=click?1:0;
}
static s32 spinner_confidence_q8(s32 r){
    if(r<=SPINNER_CENTER_ZERO_RADIUS)return 0;
    if(r>=SPINNER_FULL_CONFIDENCE_RADIUS)return 256;
    /* Keep a small but useful 18.75% response immediately outside the tiny
     * singular center, then blend continuously to full confidence. */
    return 48+sdiv32((r-SPINNER_CENTER_ZERO_RADIUS)*208,
                     SPINNER_FULL_CONFIDENCE_RADIUS-SPINNER_CENTER_ZERO_RADIUS);
}
/* Spinner is touch-locked. With gyro off, mode 0 is the circular iPod
 * encoder and mode 1 is Horizontal-X drag. With gyro on, process_report()
 * suppresses both and routes touch-clutched controller rotation to REL_DIAL. */
static void process_spinner(int hid,int spinner,int touch,s32 x,s32 y){
    s32 d=0,out=0,dq13=0,raw_dq13=0,limit,r,pr,minr;
    s32 raw_mag,weighted_mag,dynamic_cap,confidence;
    int sign,clipped=0;
    if(ms.spinner_rearm){
        if(!touch)ms.spinner_rearm=0;
        ms.spinner_seeded=0;ms.spinner_ring_active=0;ms.spinner_reacquire_reports=0;
        ms.spinner_last_direction=0;ms.spinner_reverse_q13=0;ms.spinner_reverse_reports=0;
        ms.spinner_quiet_reports=0;ms.spinner_reject_count=0;ms.spinner_fx_q8=0;return;
    }
    if(!touch){
        if(ms.spinner_touch_prev)spinner_trace_flush();
        ms.spinner_seeded=0;ms.spinner_ring_active=0;ms.spinner_reacquire_reports=0;
        ms.spinner_last_direction=0;ms.spinner_reverse_q13=0;ms.spinner_reverse_reports=0;
        ms.spinner_quiet_reports=0;ms.spinner_reject_count=0;ms.spinner_q16=0;
        ms.spinner_fx_q8=ms.spinner_fy_q8=0;ms.spinner_arc_last_ms=0;
        spinner_haptic_accum=0;spinner_haptic_started=0;spinner_haptic_last_ms=0;
        return;
    }
    if(!ms.spinner_touch_prev){
        ms.spinner_x_prev=x;ms.spinner_y_prev=y;ms.spinner_seeded=1;
        ms.spinner_ring_active=1;ms.spinner_reacquire_reports=0;ms.spinner_last_direction=0;
        ms.spinner_reverse_q13=0;ms.spinner_reverse_reports=0;ms.spinner_quiet_reports=0;
        ms.spinner_reject_count=0;ms.spinner_q16=0;ms.spinner_fx_q8=0;
        spinner_trace_reset(cfg.spinner_mode);spinner_trace_add(x,y,spinner_radius(x,y),0,0,0);
        return;
    }
    if(cfg.spinner_mode==0){
        pr=spinner_radius(ms.spinner_x_prev,ms.spinner_y_prev);r=spinner_radius(x,y);
        minr=pr<r?pr:r;
        raw_dq13=angular_delta_q13_direct(ms.spinner_x_prev,ms.spinner_y_prev,x,y);
        /* Always follow the real thumb position. Nothing is queued for later. */
        ms.spinner_x_prev=x;ms.spinner_y_prev=y;
        if(iabs32(raw_dq13)>SPINNER_DIRECT_ABS_MAX_DELTA_Q13){
            ms.spinner_reverse_q13=0;ms.spinner_reverse_reports=0;
            ms.spinner_quiet_reports=0;ms.spinner_fx_q8=0;
            spinner_trace_add(x,y,r,raw_dq13,0,4);return;
        }
        confidence=spinner_confidence_q8(minr);
        if(!confidence){
            ms.spinner_last_direction=0;ms.spinner_reverse_q13=0;
            ms.spinner_reverse_reports=0;ms.spinner_quiet_reports=0;
            ms.spinner_fx_q8=0;spinner_trace_add(x,y,r,raw_dq13,0,2);return;
        }
        raw_mag=iabs32(raw_dq13);
        /* Soft spike control uses recent accepted magnitude only. It clips an
         * implausible one-report leap rather than dropping it and later feeling
         * stuck. The cap opens quickly during genuine acceleration. */
        dynamic_cap=ms.spinner_fx_q8*2+SPINNER_DYNAMIC_HEADROOM_Q13;
        if(dynamic_cap<SPINNER_DYNAMIC_FLOOR_Q13)dynamic_cap=SPINNER_DYNAMIC_FLOOR_Q13;
        if(dynamic_cap>SPINNER_DYNAMIC_MAX_Q13)dynamic_cap=SPINNER_DYNAMIC_MAX_Q13;
        if(raw_mag>dynamic_cap){raw_mag=dynamic_cap;clipped=1;}
        weighted_mag=mul_scale_q8(raw_mag,confidence);
        if(weighted_mag<=SPINNER_MICRO_NOISE_Q13){
            ms.spinner_quiet_reports++;
            if(ms.spinner_quiet_reports>=SPINNER_DIRECTION_UNLOCK_REPORTS){
                ms.spinner_last_direction=0;ms.spinner_reverse_q13=0;ms.spinner_reverse_reports=0;
                ms.spinner_fx_q8=0;
            }
            spinner_trace_add(x,y,r,raw_dq13,0,5);return;
        }
        ms.spinner_quiet_reports=0;sign=raw_dq13>0?1:-1;
        if(!ms.spinner_last_direction){
            ms.spinner_last_direction=sign;dq13=sign*weighted_mag;
        }else if(sign==ms.spinner_last_direction){
            ms.spinner_reverse_q13=0;ms.spinner_reverse_reports=0;dq13=sign*weighted_mag;
        }else{
            if(ms.spinner_reverse_q13&&((ms.spinner_reverse_q13>0)!=(raw_dq13>0))){
                ms.spinner_reverse_q13=0;ms.spinner_reverse_reports=0;
            }
            ms.spinner_reverse_q13+=sign*weighted_mag;ms.spinner_reverse_reports++;
            if(ms.spinner_reverse_reports<SPINNER_REVERSE_CONFIRM_REPORTS||
               iabs32(ms.spinner_reverse_q13)<SPINNER_REVERSE_CONFIRM_Q13){
                spinner_trace_add(x,y,r,raw_dq13,0,5);return;
            }
            ms.spinner_last_direction=sign;ms.spinner_reverse_q13=0;
            ms.spinner_reverse_reports=0;ms.spinner_q16=0;ms.spinner_fx_q8=weighted_mag;
            spinner_trace_add(x,y,r,raw_dq13,0,7);return;
        }
        /* Fast 50% magnitude tracking opens the spike cap within a report or two,
         * while never emitting movement after a quiet report. */
        ms.spinner_fx_q8+=(weighted_mag-ms.spinner_fx_q8)>>1;
        d=-dq13*cfg.spinner_direction;
        ms.spinner_q16+=d*sp_counts_rev[active_spinner_speed()-1];
        out=sdiv32(ms.spinner_q16,SPINNER_FULL_TURN_Q13);
        ms.spinner_q16-=out*SPINNER_FULL_TURN_Q13;
        spinner_trace_add(x,y,r,dq13,out,clipped?8:1);
    }else{
        d=(x-ms.spinner_x_prev)*cfg.spinner_direction;
        if(iabs32(d)>SPINNER_LINEAR_DISCONTINUITY){ms.spinner_x_prev=x;ms.spinner_y_prev=y;ms.spinner_q16=0;return;}
        d=soft_gate(d);
        ms.spinner_q16+=d*sp_linear_scale_q16[active_spinner_speed()-1];
        limit=sp_linear_max_step[active_spinner_speed()-1];
        out=consume_q16_limited(&ms.spinner_q16,limit);
        ms.spinner_x_prev=x;ms.spinner_y_prev=y;
    }
    if(out){rel_spinner(spinner,out);spinner_motion_haptic(hid,out);}
}

/* A modest response curve converts thumb movement to requested ball speed.
 * The hard level limit below, not this curve, determines maximum speed. */
static s32 trackball_gain_q8(s32 mag){
    /* Continuous acceleration avoids visible gain steps while preserving the
     * prior 132..264 response family. */
    mag=clamp32(mag,0,3168);
    return 132+sdiv32(mag,24);
}
static void cap_vector_q16(s32 *x,s32 *y,s32 limit){
    s32 ax=iabs32(*x),ay=iabs32(*y),hi=ax>ay?ax:ay,lo=ax>ay?ay:ax,m,scale;
    /* max + half-min is a cheap near-Euclidean magnitude, preventing diagonal
     * movement from quietly exceeding the advertised speed ceiling. */
    m=hi+(lo>>1);
    if(m<=limit||m<=0)return;
    scale=sdiv32(limit*256,m);
    *x=mul_scale_q8(*x,scale);*y=mul_scale_q8(*y,scale);
}
/* Only a short rolling window can contribute to launch momentum. Old motion
 * cannot survive a pause and reappear when the thumb is lifted. */
static void clear_flick_history(void){
    memset(ms.flick_dx,0,sizeof(ms.flick_dx));memset(ms.flick_dy,0,sizeof(ms.flick_dy));
    ms.flick_head=ms.flick_count=0;
    ms.release_vx_q16=ms.release_vy_q16=0;
}
static void push_flick_sample(s32 dx_q16,s32 dy_q16){
    int i=ms.flick_head;
    ms.flick_dx[i]=dx_q16;ms.flick_dy[i]=dy_q16;
    i++;if(i>=FLICK_HISTORY)i=0;ms.flick_head=i;
    if(ms.flick_count<FLICK_HISTORY)ms.flick_count++;
}
static s32 approx_mag(s32 x,s32 y){
    s32 ax=iabs32(x),ay=iabs32(y),hi=ax>ay?ax:ay,lo=ax>ay?ay:ax;
    return hi+(lo>>1);
}
static s32 ratio_q8(s32 num,s32 den){
    if(num<=0||den<=0)return 0;
    if(num>=den)return 256;
    num>>=4;den>>=4;if(den<=0)return 0;
    return clamp32(sdiv32(num*256,den),0,256);
}
/* Overflow-safe Q11 ratio for the widened 8000/12000 REL/s levels.
 * The old direct num*2048 expression exceeded signed 32-bit range above about
 * 4000 REL/s. Paired right shifts preserve the ratio before multiplication. */
static s32 ratio_q11_wide(s32 num,s32 den,s32 max_q11){
    if(num<=0||den<=0)return 0;
    while(num>1048575||den>1048575){num=(num+1)>>1;den=(den+1)>>1;if(den<=0)den=1;}
    return clamp32(sdiv32(num*2048,den),0,max_q11);
}
/* The release handoff uses only terminal physical-gesture displacement from the
 * last eight controller reports. Tracking sensitivity and tightness are both
 * deliberately absent: the former cannot inflate inertia, while the latter only
 * changes rolling resistance after launch. */
static int build_release_launch(s32 *ox,s32 *oy){
    s32 weighted_x=0,weighted_y=0,weighted_scalar=0;
    s32 final_vx,final_vy,final_speed,peak_speed=0,measured_speed;
    s32 vector_mag,coherence,limit,base_limit,carry_q8,momentum_limit_scale_q8,launch_mag,launch_scale_q11;
    s32 mag[FLICK_HISTORY];
    int start=0,n,used=0,moving=0,weight_sum=0,quiet_run=0;
    *ox=*oy=0;telemetry_begin_release();
    if(rt.sample_count<1){telemetry_reject(RR_HISTORY);return 0;}

    for(n=0;n<rt.sample_count;n++)mag[n]=approx_mag(rt.sample_x[n],rt.sample_y[n]);
    rt.last_mag=mag[0];
    while(start<rt.sample_count&&start<=RELEASE_MAX_QUIET_TAIL&&mag[start]<RELEASE_TAIL_ACTIVITY_Q16)start++;
    if(start>RELEASE_MAX_QUIET_TAIL||start>=rt.sample_count){telemetry_reject(RR_LAST_STEP);return 0;}

    /* Newest reports receive the greatest weight because release velocity is
     * the velocity at the instant the finger leaves the ball. One internal
     * fractional report is retained in the time span; two end the terminal
     * segment and prevent old motion from being resurrected. */
    for(n=0;n<RELEASE_FINAL_REPORTS&&start+n<rt.sample_count;n++){
        s32 dx=rt.sample_x[start+n],dy=rt.sample_y[start+n],m=mag[start+n];
        int w=RELEASE_FINAL_REPORTS-n;
        if(m<RELEASE_TAIL_ACTIVITY_Q16){quiet_run++;if(quiet_run>=2)break;}
        else quiet_run=0;
        weighted_x+=dx*w;weighted_y+=dy*w;weighted_scalar+=m*w;weight_sum+=w;used++;
        if(m>=RELEASE_MOVING_STEP_Q16){moving++;if(m>peak_speed)peak_speed=m;}
    }
    rt.moving=moving;rt.final_n=used;rt.final_scalar=weighted_scalar;
    if(!used||!weight_sum||!moving){telemetry_reject(RR_MOVING);return 0;}
    if(moving==1&&peak_speed<RELEASE_SINGLE_REPORT_MIN_Q16){telemetry_reject(RR_MOVING);return 0;}
    if(weighted_scalar<RELEASE_MIN_DISTANCE_Q16){telemetry_reject(RR_DISTANCE);return 0;}

    vector_mag=approx_mag(weighted_x,weighted_y);
    coherence=ratio_q8(vector_mag,weighted_scalar);
    rt.vector_mag=vector_mag;rt.coherence=coherence;
    if(coherence<RELEASE_MIN_COHERENCE_Q8){telemetry_reject(RR_COHERENCE);return 0;}

    final_vx=sdiv32(weighted_x,weight_sum);final_vy=sdiv32(weighted_y,weight_sum);
    final_speed=approx_mag(final_vx,final_vy);
    if(final_speed<=0){telemetry_reject(RR_ZERO_SPEED);return 0;}
    /* Preserve 87.5% of a real terminal peak when smoothing/weighting would
     * otherwise hide a very short fast flick. Direction still comes from the
     * coherent weighted vector, so a one-report diagonal spike cannot steer it. */
    measured_speed=mul_scale_q8(peak_speed,224);
    if(measured_speed<final_speed)measured_speed=final_speed;
    rt.final_vx=final_vx;rt.final_vy=final_vy;rt.final_speed=measured_speed;
    if(measured_speed<RELEASE_MIN_SPEED_Q16){telemetry_reject(RR_BELOW_SPEED);return 0;}

    base_limit=tb_launch_limit_q16[cfg.trackball_speed-1];
    /* Momentum directly controls the mechanical coupling and the reachable
     * coast range. Level 1 is a true zero-carry setting. Level 7 is 100 percent;
     * levels 8..10 deliberately amplify both normal and hyper-flick energy. */
    carry_q8=tb_momentum_transfer_q8[cfg.trackball_momentum-1];
    momentum_limit_scale_q8=clamp32(carry_q8,0,640);
    limit=mul_scale_q8(base_limit,momentum_limit_scale_q8);
    if(limit>TB_HYPER_ABS_CAP_Q16)limit=TB_HYPER_ABS_CAP_Q16;
    rt.limit=limit;rt.normal_limit=base_limit;rt.hyper_limit=limit;
    rt.score=ratio_q8(measured_speed,TB_FLICK_MEASURE_LIMIT_Q16);rt.carry=carry_q8;
    launch_mag=mul_scale_q8(measured_speed,carry_q8);
    /* Hyper-flick is based on both terminal speed and meaningful coherent travel.
     * Unlike G1, the bonus increases the ball's actual launch energy as well as
     * temporarily lifting the selected coast ceiling. That makes a successful
     * violent throw perceptible even at the highest normal speed level. */
    {
        s32 stroke_q16=0;int hs;
        for(hs=start;hs<rt.sample_count&&hs<start+6;hs++){
            if(mag[hs]<RELEASE_TAIL_ACTIVITY_Q16)break;
            stroke_q16+=mag[hs];
        }
        rt.hyper_stroke=stroke_q16;
        if(start==0&&moving>=2&&coherence>=TB_HYPER_COHERENCE_Q8&&
           peak_speed>=TB_HYPER_PEAK_Q16&&measured_speed>=TB_HYPER_THRESHOLD_Q16&&
           stroke_q16>=TB_HYPER_MIN_STROKE_Q16){
            s32 speed_q8=ratio_q8(measured_speed-TB_HYPER_THRESHOLD_Q16,
                                  TB_HYPER_FULL_SPEED_Q16-TB_HYPER_THRESHOLD_Q16);
            s32 stroke_q8=ratio_q8(stroke_q16-TB_HYPER_MIN_STROKE_Q16,
                                   TB_HYPER_FULL_STROKE_Q16-TB_HYPER_MIN_STROKE_Q16);
            s32 energy_q8=mul_scale_q8(speed_q8,speed_q8);
            s32 stroke_factor_q8=154+mul_scale_q8(stroke_q8,102); /* 60..100% */
            s32 bonus_q8=mul_scale_q8(mul_scale_q8(energy_q8,stroke_factor_q8),TB_HYPER_MAX_BONUS_Q8);
            s32 hyper_limit=limit+mul_scale_q8(limit,bonus_q8);
            if(hyper_limit>TB_HYPER_ABS_CAP_Q16)hyper_limit=TB_HYPER_ABS_CAP_Q16;
            if(bonus_q8>0){
                launch_mag+=mul_scale_q8(launch_mag,bonus_q8);
                rt.hyper_flick=1;rt.hyper_limit=hyper_limit;limit=hyper_limit;
            }
        }
    }
    if(launch_mag>limit)launch_mag=limit;
    if(launch_mag<RELEASE_MIN_LAUNCH_Q16){rt.launch_mag=launch_mag;telemetry_reject(RR_SMALL_LAUNCH);return 0;}

    launch_scale_q11=ratio_q11_wide(launch_mag,final_speed,4096);
    *ox=mul_scale_q11(final_vx,launch_scale_q11);*oy=mul_scale_q11(final_vy,launch_scale_q11);
    if(!(*ox||*oy)){rt.launch_mag=launch_mag;telemetry_reject(RR_ZERO_LAUNCH);return 0;}
    telemetry_accept(*ox,*oy,launch_mag);return 1;
}
static int motion_haptic_ready(u32 now,u32 last){
    return (s32)(now-ms.haptic_guard_until_ms)>=0&&(last==0||(s32)(now-last)>=MOTION_HAPTIC_MIN_MS);
}
static s32 trackball_click_speed_bonus(s32 speed_q16){
    if(speed_q16<131072)return 0;   /* under 2 REL/report */
    if(speed_q16<327680)return 1;   /* under 5 */
    if(speed_q16<655360)return 2;   /* under 10 */
    if(speed_q16<1179648)return 3;  /* under 18 */
    return 4;
}
static void trackball_touch_haptic(int hid,s32 dx_q16,s32 dy_q16){
    s32 dist=approx_mag(dx_q16,dy_q16),bonus;u32 now;
    if(cfg.haptic_intensity<=0||hid<0||dist<=0)return;
    ms.touch_haptic_accum_q16+=dist;now=monotonic_ms();
    if(ms.touch_haptic_accum_q16>=786432&&motion_haptic_ready(now,ms.last_touch_haptic_ms)){
        while(ms.touch_haptic_accum_q16>=786432)ms.touch_haptic_accum_q16-=786432;
        ms.last_touch_haptic_ms=now;bonus=trackball_click_speed_bonus(dist);
        trackball_motion_tone(hid,(u8)trackball_haptic_channel(),(s8)(-7+bonus),620,3,0,0);
    }
}
static void trackball_coast_click(int hid,u32 now,s32 speed_q16){
    if(cfg.haptic_intensity<=0||hid<0)return;
    /* Crisp v0.31L ball-bearing detent. Rate limiting prevents the haptic
     * command channel from starving raw reports at the new high velocities. */
    ms.last_coast_haptic_ms=now;
    trackball_motion_tone(hid,(u8)trackball_haptic_channel(),(s8)(-7+trackball_click_speed_bonus(speed_q16)),620,3,0,0);
    if(rt.active)rt.haptic_pulses++;
}
static void trackball_coast_haptic(int hid,s32 distance_q16,int cursor_moved){
    u32 now;
    if(cfg.haptic_intensity<=0||hid<0||distance_q16<=0)return;
    ms.coast_haptic_accum_q16+=distance_q16;now=monotonic_ms();
    if(cursor_moved&&!ms.coast_haptic_started){
        ms.coast_haptic_started=1;ms.coast_haptic_accum_q16=0;
        if(motion_haptic_ready(now,ms.last_coast_haptic_ms))trackball_coast_click(hid,now,distance_q16);
        return;
    }
    if(ms.coast_haptic_started&&ms.coast_haptic_accum_q16>=COAST_HAPTIC_SPACING_Q16&&motion_haptic_ready(now,ms.last_coast_haptic_ms)){
        while(ms.coast_haptic_accum_q16>=COAST_HAPTIC_SPACING_Q16)ms.coast_haptic_accum_q16-=COAST_HAPTIC_SPACING_Q16;
        trackball_coast_click(hid,now,distance_q16);
    }
}
static s32 smooth_trackball_axis_q8(s32 previous_q8,s32 raw,s32 raw_mag){
    s32 target=raw*256,alpha;
    /* Continuous one-pole velocity filter: 50% new input at precision speed,
     * rising smoothly to 87.5% for fast motion. No threshold changes can create
     * a visible step, and Q8 precision is retained through cursor scaling. */
    alpha=128+sdiv32(clamp32(raw_mag,0,1536),16);
    return previous_q8+mul_scale_q8(target-previous_q8,alpha);
}
static void trackpad_click_haptic(int hid,u8 channel){
    static const s8 click_gain[4]={0,-6,0,6};
    u32 now=monotonic_ms();
    if(cfg.haptic_intensity<=0||hid<0)return;
    ms.haptic_guard_until_ms=now+BUTTON_HAPTIC_GUARD_MS;
    /* Dedicated lower-gain curve keeps all three button-press levels deep but
     * clearly softer than rolling texture; no generic haptic offset is added. */
    haptic_tone_raw(hid,channel,click_gain[cfg.haptic_intensity],160,15,0,0);
}
static void restore_trackpad_click_haptics(int hid,int tuning,int lclick,int rclick){
    u32 cur=0,rising;
    if(tuning){ms.button_haptic_prev=0;return;}
    if(lclick)cur|=1u<<0;
    if(rclick)cur|=1u<<1;
    rising=cur&~ms.button_haptic_prev;ms.button_haptic_prev=cur;
    if(rising&(1u<<0))trackpad_click_haptic(hid,0);
    else if(rising&(1u<<1))trackpad_click_haptic(hid,1);
}

static void process_trackball(int hid,int mouse,int source_pad,int touch,int click,s32 x,s32 y){
    s32 dx,dy,raw_mag,filtered_mag,gain,limit;
    s32 touchx,touchy,samplex,sampley,tmpx,tmpy,outx,outy,step;
    u32 now=monotonic_ms();
    if(source_pad!=active_trackball_pad)return;
    ms.trackball_source_pad=source_pad;
    if(ms.trackball_rearm){if(!touch)ms.trackball_rearm=0;return;}
    if(click){
        if(ms.mouse_vx_q16||ms.mouse_vy_q16)telemetry_finish_coast(2);
        ms.mouse_vx_q16=ms.mouse_vy_q16=0;clear_flick_history();ms.coast_haptic_accum_q16=0;ms.coast_haptic_started=0;
    }
    if(!touch){
        if(ms.trackball_touch_prev){
            s32 launch_x=0,launch_y=0;
            if(!cfg.mouse_mode&&!click&&build_release_launch(&launch_x,&launch_y)){
                ms.mouse_vx_q16=launch_x;ms.mouse_vy_q16=launch_y;ms.coast_haptic_started=0;
            }else ms.mouse_vx_q16=ms.mouse_vy_q16=0;
            clear_flick_history();
            ms.filt_dx_q8=ms.filt_dy_q8=0;ms.mouse_x_q16=ms.mouse_y_q16=0;ms.last_touch_ms=0;
            ms.touch_quiet_reports=0;ms.touch_haptic_accum_q16=0;ms.coast_haptic_accum_q16=0;ms.coast_haptic_started=0;
        }
        return;
    }
    /* Touch catches the physical ball immediately. */
    if(ms.mouse_vx_q16||ms.mouse_vy_q16)telemetry_finish_coast(2);
    ms.mouse_vx_q16=ms.mouse_vy_q16=0;ms.coast_haptic_accum_q16=0;ms.coast_haptic_started=0;
    if(!ms.trackball_touch_prev){
        ms.trackball_x_prev=x;ms.trackball_y_prev=y;clear_flick_history();
        ms.filt_dx_q8=ms.filt_dy_q8=0;ms.mouse_x_q16=ms.mouse_y_q16=0;
        ms.last_touch_ms=now;ms.touch_quiet_reports=0;ms.touch_haptic_accum_q16=0;return;
    }

    /* Controller reports are the clock. Using userspace arrival milliseconds
     * made queued reports alternate between 1 ms and several ms, which changed
     * the cap from report to report and produced the observed roughness. */
    ms.last_touch_ms=now;
    dx=x-ms.trackball_x_prev;dy=-(y-ms.trackball_y_prev);ms.trackball_x_prev=x;ms.trackball_y_prev=y;
    if(iabs32(dx)>PAD_RAW_DISCONTINUITY||iabs32(dy)>PAD_RAW_DISCONTINUITY){
        ms.filt_dx_q8=ms.filt_dy_q8=0;ms.touch_quiet_reports=0;clear_flick_history();return;
    }
    dx=soft_gate(dx);dy=soft_gate(dy);
    raw_mag=iabs32(dx)>iabs32(dy)?iabs32(dx):iabs32(dy);
    if(!dx&&!dy){
        ms.touch_quiet_reports++;
        if(ms.touch_quiet_reports>=2)ms.filt_dx_q8=ms.filt_dy_q8=0;
        else{ms.filt_dx_q8>>=2;ms.filt_dy_q8>>=2;}
    }else{
        ms.touch_quiet_reports=0;
        ms.filt_dx_q8=smooth_trackball_axis_q8(ms.filt_dx_q8,dx,raw_mag);
        ms.filt_dy_q8=smooth_trackball_axis_q8(ms.filt_dy_q8,dy,raw_mag);
    }

    filtered_mag=(iabs32(ms.filt_dx_q8)>iabs32(ms.filt_dy_q8)?iabs32(ms.filt_dx_q8):iabs32(ms.filt_dy_q8))>>8;
    /* Direct mouse mode keeps smoothing for cursor polish but removes ball
     * acceleration, launch, friction, and coast. Trackball mode retains the
     * continuous velocity-sensitive gain that makes slow rolls slow and fast
     * rolls fast. */
    gain=cfg.mouse_mode?160:trackball_gain_q8(filtered_mag);limit=tb_speed_limit_q16[cfg.trackball_speed-1];
    tmpx=mul_scale_q8(ms.filt_dx_q8,gain);tmpy=mul_scale_q8(ms.filt_dy_q8,gain);
    touchx=mul_scale_q8(tmpx,tb_touch_scale_q16[cfg.trackball_speed-1]);
    touchy=mul_scale_q8(tmpy,tb_touch_scale_q16[cfg.trackball_speed-1]);
    cap_vector_q16(&touchx,&touchy,limit);
    ms.mouse_x_q16+=touchx;ms.mouse_y_q16+=touchy;
    outx=consume_q16(&ms.mouse_x_q16);outy=consume_q16(&ms.mouse_y_q16);
    if(outx||outy)rel_trackball_mouse(mouse,source_pad,outx,outy);

    /* Momentum observes raw physical terminal velocity before smoothing and uses
     * one fixed gesture scale at every tracking level. This is the key split:
     * Centipede can have a fast thumb-on cursor without multiplying free-spin
     * inertia by the same sensitivity factor. */
    if(cfg.mouse_mode){
        clear_flick_history();
    }else{
        gain=trackball_gain_q8(raw_mag);
        samplex=mul_scale_q8(dx*gain,TB_FLICK_SCALE_Q8);
        sampley=mul_scale_q8(dy*gain,TB_FLICK_SCALE_Q8);
        cap_vector_q16(&samplex,&sampley,TB_FLICK_MEASURE_LIMIT_Q16);
        step=approx_mag(dx,dy);
        if(click)clear_flick_history();
        else if(step>=RELEASE_SAMPLE_FLOOR)push_flick_sample(samplex,sampley);
        else push_flick_sample(0,0);
        trackball_touch_haptic(hid,touchx,touchy);
    }
}
static void trackball_coast(int hid,int mouse){
    s32 x,y,speed,new_speed,scale_q11,decel;
    int fi=cfg.trackball_friction-1;
    if(cfg.mouse_mode){ms.mouse_x_q16=ms.mouse_y_q16=0;ms.mouse_vx_q16=ms.mouse_vy_q16=0;return;}
    if(ms.trackball_source_pad!=active_trackball_pad){
        ms.mouse_x_q16=ms.mouse_y_q16=0;ms.mouse_vx_q16=ms.mouse_vy_q16=0;return;
    }
    if(ms.trackball_touch_prev||(!ms.mouse_vx_q16&&!ms.mouse_vy_q16))return;
    speed=approx_mag(ms.mouse_vx_q16,ms.mouse_vy_q16);
    decel=tb_roll_friction_q16[fi]+(speed>>TB_VISCOUS_SHIFT);
    if(speed<=decel){
        telemetry_finish_coast(1);ms.mouse_vx_q16=ms.mouse_vy_q16=0;
        ms.coast_haptic_accum_q16=0;ms.coast_haptic_started=0;return;
    }
    ms.mouse_x_q16+=ms.mouse_vx_q16;ms.mouse_y_q16+=ms.mouse_vy_q16;
    x=consume_q16(&ms.mouse_x_q16);y=consume_q16(&ms.mouse_y_q16);
    if(x||y)rel_trackball_mouse(mouse,ms.trackball_source_pad,x,y);
    if(rt.active){rt.coast_ticks++;rt.coast_distance_q16+=speed;rt.coast_emitted_rel+=approx_mag(x,y);}
    trackball_coast_haptic(hid,speed,(x||y));

    /* Constant opposing rolling torque gives linear angular deceleration. The
     * small common viscous term models bearing/air loss at high speed. Q11
     * vector scaling avoids the stair-step direction changes caused by v0.38J's
     * coarse Q8 decay. */
    new_speed=speed-decel;
    if(new_speed<=0){telemetry_finish_coast(1);ms.mouse_vx_q16=ms.mouse_vy_q16=0;return;}
    scale_q11=ratio_q11_wide(new_speed,speed,2048);
    ms.mouse_vx_q16=mul_scale_q11(ms.mouse_vx_q16,scale_q11);
    ms.mouse_vy_q16=mul_scale_q11(ms.mouse_vy_q16,scale_q11);
}

/* -------------------------------------------------------------------------
 * Gamepad output. The "..." / quick-access button is the tuning modifier.
 * L4 is restored as an ordinary virtual gamepad button (BTN_0).
 * ------------------------------------------------------------------------- */
struct gamepad_out_state{
    int valid; s32 a,b,x,y,q,r3,start,r4,r5,r1,sel,l3,steam,l4,l5,l1,r2c,l2c;
    s32 hatx,haty,l2a,r2a,lx,ly,rx,ry;
};
static struct gamepad_out_state gos;static int gp_dirty,gp_batch_count;
static struct input_event gp_batch[32];
static s32 clean_stick(s32 v){return iabs32(v)<1800?0:v;}
static void queue_gamepad(u16 type,u16 code,s32 value){
    struct input_event *e;if(gp_batch_count>=32)return;e=&gp_batch[gp_batch_count++];memset(e,0,sizeof(*e));e->type=type;e->code=code;e->value=value;
}
static void changed(int fd,u16 type,u16 code,s32 v,s32 *old,int force){(void)fd;if(force||v!=*old){queue_gamepad(type,code,v);gp_dirty=1;}*old=v;}
static void emit_gamepad_state(int gp,const u8 *b,int suppress){
    int force=!gos.valid;gp_dirty=0;gp_batch_count=0;
    /* Exact SCWrapper default button report map. */
    s32 a=(b[2]&0x01)!=0,bb=(b[2]&0x02)!=0,x=(b[2]&0x04)!=0,y=(b[2]&0x08)!=0;
    s32 q=(b[2]&0x10)!=0,r3=(b[2]&0x20)!=0,start=(b[2]&0x40)!=0,r4=(b[2]&0x80)!=0;
    s32 r5=(b[3]&0x01)!=0,r1=(b[3]&0x02)!=0,dd=(b[3]&0x04)!=0,dr=(b[3]&0x08)!=0,dl=(b[3]&0x10)!=0,du=(b[3]&0x20)!=0,sel=(b[3]&0x40)!=0,l3=(b[3]&0x80)!=0;
    s32 steam=(b[4]&0x01)!=0,l4=(b[4]&0x02)!=0,l5=(b[4]&0x04)!=0,l1=(b[4]&0x08)!=0,r2c=(b[4]&0x80)!=0,l2c=(b[5]&0x08)!=0;
    s32 hatx=dr?1:(dl?-1:0),haty=dd?1:(du?-1:0),l2a=le16u(b+6),r2a=le16u(b+8),lx=clean_stick(le16s(b+10)),ly=clean_stick(-le16s(b+12));
    /* Steam Controller 2026 report layout, matching SCWrapper:
     * +10/+12 = physical left stick, +14/+16 = physical right stick.
     * Right-trackpad coordinates remain +24/+26 and never enter this emitter. */
    s32 rx=clean_stick(le16s(b+14));
    s32 ry=clean_stick(-le16s(b+16));
    if(!suppress&&gyro.enabled){
        if((gyro.stick_mode&gyro.active_stick_mask)&GYRO_STICK_LEFT)gyro_blend_stick_assist(lx,ly,gyro.stick_lx,gyro.stick_ly,&lx,&ly);
        if((gyro.stick_mode&gyro.active_stick_mask)&GYRO_STICK_RIGHT)gyro_blend_stick_assist(rx,ry,gyro.stick_rx,gyro.stick_ry,&rx,&ry);
    }
    if(suppress){a=bb=x=y=q=r3=start=r4=r5=r1=sel=l3=steam=l4=l5=l1=r2c=l2c=0;hatx=haty=l2a=r2a=lx=ly=rx=ry=0;}
    changed(gp,EV_KEY,BTN_SOUTH,a,&gos.a,force);   /* A */
    changed(gp,EV_KEY,BTN_EAST,bb,&gos.b,force);    /* B */
    changed(gp,EV_KEY,BTN_NORTH,x,&gos.x,force);    /* X */
    changed(gp,EV_KEY,BTN_WEST,y,&gos.y,force);     /* Y */
    changed(gp,EV_KEY,BTN_BASE,q,&gos.q,force);changed(gp,EV_KEY,BTN_THUMBR,r3,&gos.r3,force);
    changed(gp,EV_KEY,BTN_START,start,&gos.start,force);changed(gp,EV_KEY,BTN_1,r4,&gos.r4,force);
    changed(gp,EV_KEY,BTN_3,r5,&gos.r5,force);changed(gp,EV_KEY,BTN_TR,r1,&gos.r1,force);
    changed(gp,EV_KEY,BTN_SELECT,sel,&gos.sel,force);changed(gp,EV_KEY,BTN_THUMBL,l3,&gos.l3,force);
    changed(gp,EV_KEY,BTN_MODE,steam,&gos.steam,force);changed(gp,EV_KEY,BTN_0,l4,&gos.l4,force);changed(gp,EV_KEY,BTN_2,l5,&gos.l5,force);
    changed(gp,EV_KEY,BTN_TL,l1,&gos.l1,force);changed(gp,EV_KEY,BTN_TR2,r2c,&gos.r2c,force);
    changed(gp,EV_KEY,BTN_TL2,l2c,&gos.l2c,force);
    changed(gp,EV_ABS,ABS_HAT0X,hatx,&gos.hatx,force);changed(gp,EV_ABS,ABS_HAT0Y,haty,&gos.haty,force);
    changed(gp,EV_ABS,ABS_Z,l2a,&gos.l2a,force);changed(gp,EV_ABS,ABS_RZ,r2a,&gos.r2a,force);
    changed(gp,EV_ABS,ABS_X,lx,&gos.lx,force);changed(gp,EV_ABS,ABS_Y,ly,&gos.ly,force);
    changed(gp,EV_ABS,ABS_RX,rx,&gos.rx,force);changed(gp,EV_ABS,ABS_RY,ry,&gos.ry,force);
    if(gp_dirty){queue_gamepad(EV_SYN,SYN_REPORT,0);write_bytes(gp,gp_batch,(u32)(gp_batch_count*sizeof(gp_batch[0])));}gos.valid=1;
}
static void neutralize_gamepad(int gp){memset(&gos,0,sizeof(gos));gos.valid=0;emit_gamepad_state(gp,(const u8[28]){0},1);}


static u32 prev_shortcuts;static int tuning_prev,post_tune_block;
static int shortcut_controls_down(const u8 *b){
    return (b[2]&(1|2|4|8|32|64|128))||(b[3]&(1|2|4|8|16|32|64|128))||(b[4]&(1|2|4|8|64|128))||(b[5]&(4|8));
}
static void process_report(int hid,int gp,int mouse,int spinner,int keypad,const u8 *b,int n){
    int ltouch,rtouch,lclick,rclick,left_stick_touch,right_stick_touch,tuning,suppress;
    int gyro_active,gyro_output,gyro_paddle_mode,gyro_paddle_active,gyro_pointer_active,gyro_stick_active_mask;u32 s=0;s32 lx,ly,rx,ry;
    int spinner_touch,trackball_touch,spinner_click,trackball_click,trackball_pad;
    s32 spinner_x,spinner_y,trackball_x,trackball_y;
    if(n<28||(b[0]!=0x42&&b[0]!=0x45&&b[0]!=0x47))return;
    rtouch=(b[4]&32)!=0;rclick=(b[4]&64)!=0;right_stick_touch=(b[4]&16)!=0;
    left_stick_touch=(b[5]&1)!=0;ltouch=(b[5]&2)!=0;lclick=(b[5]&4)!=0;
    lx=le16s(b+18);ly=le16s(b+20);rx=le16s(b+24);ry=le16s(b+26);
    spinner_touch=cfg.pads_swapped?rtouch:ltouch;
    trackball_touch=cfg.pads_swapped?ltouch:rtouch;
    spinner_click=cfg.pads_swapped?rclick:lclick;
    trackball_click=cfg.pads_swapped?lclick:rclick;
    spinner_x=cfg.pads_swapped?rx:lx;spinner_y=cfg.pads_swapped?ry:ly;
    trackball_x=cfg.pads_swapped?lx:rx;trackball_y=cfg.pads_swapped?ly:ry;
    trackball_pad=cfg.pads_swapped?PAD_LEFT:PAD_RIGHT;active_trackball_pad=trackball_pad;
    tuning=(b[2]&16)!=0;
    if(b[4]&8)s|=1u<<0;
    if(b[3]&2)s|=1u<<1;
    if(b[5]&8)s|=1u<<2;
    if(b[4]&128)s|=1u<<3;
    if(b[2]&8)s|=1u<<4;
    if(b[2]&4)s|=1u<<5;
    if(b[2]&2)s|=1u<<6;
    if(spinner_click)s|=1u<<7;
    if(b[3]&16)s|=1u<<8;
    if(b[3]&8)s|=1u<<9;
    if(b[3]&4)s|=1u<<12;
    if(b[3]&32)s|=1u<<13;
    if(b[2]&1)s|=1u<<10;
    if(trackball_click)s|=1u<<11;
    if(b[4]&4)s|=1u<<14;   /* L5: absolute gyro master on/off */
    if(b[2]&128)s|=1u<<15; /* R4: gyro sensitivity down */
    if(b[3]&1)s|=1u<<16;   /* R5: gyro sensitivity up */
    if(b[3]&128)s|=1u<<17; /* L3: gyro-to-left-stick toggle */
    if(b[2]&32)s|=1u<<18;  /* R3: gyro-to-right-stick toggle */
    if(b[4]&2)s|=1u<<19;   /* L4: X/Y inversion profile */
    if(b[2]&64)s|=1u<<20;  /* Start: save complete user profile */
    if(b[3]&64)s|=1u<<21;  /* Select: load complete user profile */
    if(b[4]&1)s|=1u<<22;   /* Steam menu: restore defaults */
    if(tuning){
        if(!tuning_prev){zero_motion();ms.spinner_rearm=spinner_touch;ms.trackball_rearm=trackball_touch;release_trackball_mouse_click(mouse);spinner_primary(spinner,0);spinner_secondary(spinner,0);release_numpad_key(keypad);}
        if((s&(1u<<0))&&!(prev_shortcuts&(1u<<0))){if(cfg.spinner_mode==0)change_level(hid,&cfg.spinner_ipod_speed,-1,spinner_haptic_channel());else change_level(hid,&cfg.spinner_x_speed,-1,spinner_haptic_channel());}
        if((s&(1u<<1))&&!(prev_shortcuts&(1u<<1))){if(cfg.spinner_mode==0)change_level(hid,&cfg.spinner_ipod_speed,1,spinner_haptic_channel());else change_level(hid,&cfg.spinner_x_speed,1,spinner_haptic_channel());}
        if((s&(1u<<2))&&!(prev_shortcuts&(1u<<2)))change_level(hid,&cfg.trackball_speed,-1,trackball_haptic_channel());
        if((s&(1u<<3))&&!(prev_shortcuts&(1u<<3)))change_level(hid,&cfg.trackball_speed,1,trackball_haptic_channel());
        if((s&(1u<<4))&&!(prev_shortcuts&(1u<<4))){cfg.spinner_direction=-cfg.spinner_direction;save_cfg();reverse_cue(hid);}
        if((s&(1u<<5))&&!(prev_shortcuts&(1u<<5))){cfg.haptic_intensity=(cfg.haptic_intensity+1)&3;save_cfg();haptic_intensity_cue(hid,cfg.haptic_intensity);}
        if((s&(1u<<6))&&!(prev_shortcuts&(1u<<6))){
            cfg.numpad_enabled=!cfg.numpad_enabled;save_cfg();zero_motion();release_numpad_key(keypad);numpad_hover_key=numpad_hover_candidate=numpad_hover_reports=0;
            spinner_primary(spinner,0);spinner_secondary(spinner,0);numpad_rearm=spinner_touch?1:0;numpad_hover_key=numpad_hover_candidate=numpad_hover_reports=0;ms.spinner_rearm=spinner_touch;
            numpad_toggle_cue(hid,cfg.numpad_enabled);
            log_text(cfg.numpad_enabled?"SC-Orbit: ...+B -> virtual number pad ENABLED\n":"SC-Orbit: ...+B -> virtual number pad DISABLED\n");
        }
        if((s&(1u<<7))&&!(prev_shortcuts&(1u<<7))){
            if(cfg.numpad_enabled){
                cfg.numpad_layout=!cfg.numpad_layout;save_cfg();release_numpad_key(keypad);numpad_rearm=1;numpad_hover_key=numpad_hover_candidate=numpad_hover_reports=0;
                numpad_layout_cue(hid,cfg.numpad_layout);
                log_text(cfg.numpad_layout?"SC-Orbit: ...+number-pad click -> PHONE 123/456/789 layout\n":"SC-Orbit: ...+number-pad click -> RADIAL 1-9 clockwise layout\n");
            }else{
                zero_motion();ms.spinner_rearm=spinner_touch;spinner_haptic_accum=0;spinner_haptic_started=0;spinner_haptic_last_ms=0;
                if(gyro.enabled){
                    spinner_mode_cue(hid,3);
                    log_text("SC-Orbit: gyro master is ON -> Spinner pad is automatic touch-clutched Gyro Paddle\n");
                }else{
                    cfg.spinner_mode^=1;save_cfg();spinner_mode_cue(hid,cfg.spinner_mode);
                    if(cfg.spinner_mode==0)log_text("SC-Orbit: ...+spinner click -> fluid circular iPod encoder\n");
                    else log_text("SC-Orbit: ...+spinner click -> Horizontal-X drag spinner\n");
                }
            }
        }
        if((s&(1u<<8))&&!(prev_shortcuts&(1u<<8)))change_level(hid,&cfg.trackball_friction,-1,trackball_haptic_channel());
        if((s&(1u<<9))&&!(prev_shortcuts&(1u<<9)))change_level(hid,&cfg.trackball_friction,1,trackball_haptic_channel());
        if((s&(1u<<12))&&!(prev_shortcuts&(1u<<12)))change_momentum_level(hid,-1);
        if((s&(1u<<13))&&!(prev_shortcuts&(1u<<13)))change_momentum_level(hid,1);
        if((s&(1u<<14))&&!(prev_shortcuts&(1u<<14)))gyro_toggle(hid);
        if((s&(1u<<15))&&!(prev_shortcuts&(1u<<15)))gyro_change_level(hid,-1);
        if((s&(1u<<16))&&!(prev_shortcuts&(1u<<16)))gyro_change_level(hid,1);
        if((s&(1u<<17))&&!(prev_shortcuts&(1u<<17)))gyro_toggle_stick_mode(hid,GYRO_STICK_LEFT);
        if((s&(1u<<18))&&!(prev_shortcuts&(1u<<18)))gyro_toggle_stick_mode(hid,GYRO_STICK_RIGHT);
        if((s&(1u<<19))&&!(prev_shortcuts&(1u<<19)))gyro_cycle_inversion(hid);
        if((s&(1u<<20))&&!(prev_shortcuts&(1u<<20))){
            int ok=save_user_profile();profile_action_cue(hid,0,ok);
            log_text(ok?"SC-Orbit: user profile SAVED\n":"SC-Orbit: user profile save FAILED\n");
        }
        if((s&(1u<<21))&&!(prev_shortcuts&(1u<<21))){
            int ok=load_user_profile();
            if(ok){save_cfg();gyro_apply_cfg();zero_motion();release_trackball_mouse_click(mouse);spinner_primary(spinner,0);spinner_secondary(spinner,0);ms.spinner_rearm=1;ms.trackball_rearm=1;release_numpad_key(keypad);numpad_rearm=1;numpad_hover_key=numpad_hover_candidate=numpad_hover_reports=0;}
            profile_action_cue(hid,1,ok);
            log_text(ok?"SC-Orbit: user profile LOADED\n":"SC-Orbit: no valid saved user profile\n");
        }
        if((s&(1u<<22))&&!(prev_shortcuts&(1u<<22))){
            defaults();save_cfg();gyro_apply_cfg();zero_motion();release_trackball_mouse_click(mouse);release_numpad_key(keypad);numpad_hover_key=numpad_hover_candidate=numpad_hover_reports=0;
            spinner_primary(spinner,0);spinner_secondary(spinner,0);ms.spinner_rearm=spinner_touch;ms.trackball_rearm=trackball_touch;numpad_rearm=spinner_touch;
            defaults_cue(hid);log_text("SC-Orbit: ...+Steam -> defaults restored\n");
        }
        if((s&(1u<<11))&&!(prev_shortcuts&(1u<<11))){
            cfg.mouse_mode=!cfg.mouse_mode;save_cfg();zero_motion();
            ms.trackball_rearm=trackball_touch;pointer_mode_cue(hid,cfg.mouse_mode);
            if(cfg.mouse_mode)log_text("SC-Orbit: ...+trackball click -> direct mouse mode\n");
            else log_text("SC-Orbit: ...+trackball click -> physical trackball mode\n");
        }
        if((s&(1u<<10))&&!(prev_shortcuts&(1u<<10))){
            cfg.pads_swapped=!cfg.pads_swapped;save_cfg();zero_motion();
            spinner_touch=cfg.pads_swapped?rtouch:ltouch;trackball_touch=cfg.pads_swapped?ltouch:rtouch;
            spinner_click=cfg.pads_swapped?rclick:lclick;trackball_click=cfg.pads_swapped?lclick:rclick;
            spinner_x=cfg.pads_swapped?rx:lx;spinner_y=cfg.pads_swapped?ry:ly;
            trackball_x=cfg.pads_swapped?lx:rx;trackball_y=cfg.pads_swapped?ly:ry;
            trackball_pad=cfg.pads_swapped?PAD_LEFT:PAD_RIGHT;active_trackball_pad=trackball_pad;
            ms.spinner_rearm=spinner_touch;ms.trackball_rearm=trackball_touch;release_numpad_key(keypad);numpad_rearm=spinner_touch;numpad_hover_key=numpad_hover_candidate=numpad_hover_reports=0;pad_swap_cue(hid,cfg.pads_swapped);
            if(cfg.pads_swapped)log_text("SC-Orbit: ...+A -> spinner right, trackball left\n");
            else log_text("SC-Orbit: ...+A -> spinner left, trackball right\n");
        }
        ms.spinner_x_prev=spinner_x;ms.spinner_y_prev=spinner_y;ms.trackball_x_prev=trackball_x;ms.trackball_y_prev=trackball_y;
    }else{
        if(tuning_prev)post_tune_block=1;
        if(post_tune_block&&!shortcut_controls_down(b))post_tune_block=0;
    }
    spinner_touch=cfg.pads_swapped?rtouch:ltouch;trackball_touch=cfg.pads_swapped?ltouch:rtouch;
    spinner_click=cfg.pads_swapped?rclick:lclick;trackball_click=cfg.pads_swapped?lclick:rclick;
    spinner_x=cfg.pads_swapped?rx:lx;spinner_y=cfg.pads_swapped?ry:ly;
    trackball_x=cfg.pads_swapped?lx:rx;trackball_y=cfg.pads_swapped?ly:ry;
    trackball_pad=cfg.pads_swapped?PAD_LEFT:PAD_RIGHT;active_trackball_pad=trackball_pad;
    suppress=tuning||post_tune_block;
    restore_trackpad_click_haptics(hid,suppress,lclick,rclick);
    gyro_paddle_mode=!cfg.numpad_enabled&&gyro.enabled;
    gyro_paddle_active=gyro_paddle_mode&&spinner_touch;
    gyro_pointer_active=gyro.enabled&&!gyro_paddle_active&&trackball_touch;
    gyro_stick_active_mask=0;
    /* L5 is an absolute gyro master. L3/R3 remember per-stick arming, but no
     * stick, pointer, or paddle path may activate while the master is off. */
    if(gyro.enabled){
        if((gyro.stick_mode&GYRO_STICK_LEFT)&&left_stick_touch)gyro_stick_active_mask|=GYRO_STICK_LEFT;
        if((gyro.stick_mode&GYRO_STICK_RIGHT)&&right_stick_touch)gyro_stick_active_mask|=GYRO_STICK_RIGHT;
    }
    /* Every gyro path is clutched by its physical surface and subordinated
     * to the L5 master. L3/R3 only arm their individual stick paths. */
    gyro_active=gyro_paddle_active||gyro_pointer_active||gyro_stick_active_mask;
    gyro_output=gyro_paddle_active?GYRO_OUTPUT_PADDLE:GYRO_OUTPUT_MOUSE;
    if(suppress){
        release_trackball_mouse_click(mouse);spinner_primary(spinner,0);spinner_secondary(spinner,0);release_numpad_key(keypad);
        process_gyro(hid,mouse,spinner,b,n,0,GYRO_OUTPUT_MOUSE,0,0);
        emit_gamepad_state(gp,b,1);
    }else{
        process_gyro(hid,mouse,spinner,b,n,gyro_active,gyro_output,gyro_pointer_active,gyro_stick_active_mask);
        emit_gamepad_state(gp,b,0);
        if(cfg.numpad_enabled){
            process_spinner(hid,spinner,0,spinner_x,spinner_y);spinner_primary(spinner,0);spinner_secondary(spinner,0);
            process_numpad(hid,keypad,spinner_touch,spinner_click,spinner_x,spinner_y);
        }else{
            release_numpad_key(keypad);process_spinner(hid,spinner,gyro_paddle_mode?0:spinner_touch,spinner_x,spinner_y);
            spinner_primary(spinner,spinner_click);spinner_secondary(spinner,0);
        }
        process_trackball(hid,mouse,trackball_pad,trackball_touch,trackball_click,trackball_x,trackball_y);
        trackball_mouse_click(mouse,trackball_click,trackball_x);
    }
    prev_shortcuts=tuning?s:0;tuning_prev=tuning;
    ms.spinner_touch_prev=(gyro_paddle_mode||cfg.numpad_enabled)?0:spinner_touch;ms.trackball_touch_prev=trackball_touch;
}

static int run_controller(int hid,int gp,int mouse,int spinner,int keypad){
    u8 b[128];long n;int misses=0,active=0,drained;u32 next_coast,next_rescan,next_reassert;
    (void)configure_controller(hid);
    gyro_defaults();
    memset(&voice_rt,0,sizeof(voice_rt));voice_busy=voice_rendering=0;
    memset(&ms,0,sizeof(ms));ms.trackball_source_pad=-1;memset(&gos,0,sizeof(gos));prev_shortcuts=0;tuning_prev=0;post_tune_block=0;mouse_left_state=0;mouse_right_state=0;spinner_primary_state=0;spinner_secondary_state=0;trackball_click_prev=0;trackball_click_button=TRACKBALL_CLICK_NONE;numpad_key_down=0;numpad_click_prev=0;numpad_rearm=0;numpad_hover_key=numpad_hover_candidate=numpad_hover_reports=0;
    next_coast=monotonic_ms()+4U;next_rescan=monotonic_ms()+3000U;next_reassert=monotonic_ms()+5000U;
    for(;;){
        int got=0;
        for(drained=0;drained<64;drained++){
            n=xread(hid,b,sizeof(b));
            if(n>0){
                if(n>=28&&(b[0]==0x42||b[0]==0x45||b[0]==0x47)){
                    got=1;misses=0;
                    if(!active){
                        scan_targeted_physical_events();
                        active=1;log_text("SC-Orbit: verified raw controller; targeted duplicate suppression active; virtual input remains live\n");
                    }
                    process_report(hid,gp,mouse,spinner,keypad,b,(int)n);
                }
                continue;
            }
            if(n<0&&n!=-EAGAIN){neutralize_gamepad(gp);release_trackball_mouse_click(mouse);spinner_primary(spinner,0);spinner_secondary(spinner,0);release_numpad_key(keypad);return -1;}
            break;
        }
        if(!active){if(!got)misses++;if(misses>2000)return -2;}
        if(active){
            u32 now=monotonic_ms();
            if((s32)(now-next_coast)>=0){
                int coast_steps=0;
                while((s32)(now-next_coast)>=0&&coast_steps<3){
                    if(!ms.trackball_touch_prev&&!tuning_prev&&!post_tune_block)trackball_coast(hid,mouse);
                    next_coast+=4U;coast_steps++;
                }
                if((s32)(now-next_coast)>=12)next_coast=now+4U;
            }
            if((s32)(now-next_rescan)>=0){scan_targeted_physical_events();next_rescan=now+3000U;}
            if((s32)(now-next_reassert)>=0){(void)configure_controller(hid);next_reassert=now+5000U;}
        }
        voice_tick(hid,monotonic_ms());
        if(!got)sleep_ms(1);
    }
}
int main(void){
    int gp,mouse,spinner,keypad,hid;load_cfg();active_trackball_pad=cfg.pads_swapped?PAD_LEFT:PAD_RIGHT;save_cfg();gp=create_gamepad();mouse=create_mouse();spinner=create_spinner();keypad=create_keypad();
    if(gp<0||mouse<0||spinner<0||keypad<0){log_text("SC-Orbit: failed to create gamepad, trackball, spinner, or keypad device\n");return 1;}
    log_text("SC-Orbit starting; hybrid analog gyro, no runtime diagnostics, sounds locked\n");
    for(;;){
        int rc;hid=find_hidraw();if(hid<0){if(grab_count)release_grabs();sleep_ms(1000);continue;}
        rc=run_controller(hid,gp,mouse,spinner,keypad);xclose(hid);release_grabs();telemetry_finish_coast(4);memset(&ms,0,sizeof(ms));
        if(rc==-1){log_text("SC-Orbit: controller disconnected; scanning again\n");sleep_ms(500);}
        else{log_text("SC-Orbit: inactive Valve interface skipped\n");sleep_ms(50);}
    }
    return 0;
}

void _start(void){xexit(main());}
