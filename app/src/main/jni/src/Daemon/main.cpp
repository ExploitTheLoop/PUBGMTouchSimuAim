#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
#include "main.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <linux/uinput.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <linux/input.h>
#include <sys/mman.h>
#include <iconv.h>
#include "Offsets.h"

#include "../Socket/ServerSocket.h"
#include "TouchHelper.h"

uintptr_t getRealOffsets(uintptr_t address);
long int entrance;
int openMirror = 0;
const char *uinput_deivce_path = "/dev/uinput";

#define sizeoq 11
#define UNGRAB    0
#define GRAB      1

int x[11],y[11],que[11];
int front=0;
int tid=69;
int fd,fb;
int pid;

struct MAPS
{
    uintptr_t addr;
    uintptr_t taddr;
    int type;
    struct MAPS *next;
};

typedef struct MAPS *PMAPS;

#define LEN sizeof(struct MAPS)

#if defined(__arm__)
int process_vm_readv_syscall = 376;
int process_vm_writev_syscall = 377;
#elif defined(__aarch64__)
int process_vm_readv_syscall = 270;
int process_vm_writev_syscall = 271;
#elif defined(__i386__)
int process_vm_readv_syscall = 347;
int process_vm_writev_syscall = 348;
#else
int process_vm_readv_syscall = 310;
int process_vm_writev_syscall = 311;
#endif

ssize_t process_v(pid_t __pid, const struct iovec *__local_iov, unsigned long __local_iov_count,
                  const struct iovec *__remote_iov, unsigned long __remote_iov_count,
                  unsigned long __flags, bool iswrite)
{
    return syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall), __pid,
                   __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}

/* 进程读写内存 */
bool pvm(void *address, void *buffer, size_t size, bool iswrite)
{
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;

    if (pid < 0)
    {
        return false;
    }

    ssize_t bytes = process_v(pid, local, 1, remote, 1, 0, iswrite);
    return bytes == size;
}

/* 读内存 */
bool vm_readv(uintptr_t address, void *buffer, size_t size)
{
    return pvm(reinterpret_cast < void *>(address), buffer, size, false);
}

/* 写内存 */
bool vm_writev(uintptr_t address, void *buffer, size_t size)
{
    return pvm(reinterpret_cast < void *>(address), buffer, size, true);
}

uintptr_t getdword(uintptr_t addr)
{
    if (addr < 0xFFFFFF)
    {
        return 0;
    }
    uintptr_t var[4] = { 0 };
    memset(var, 0, 4);
    vm_readv(addr, var, 4);
    return var[0];
}

uintptr_t getint(uintptr_t addr)
{
    if (addr < 0xFFFFFF)
    {
        return 0;
    }
    uintptr_t var[4] = { 0 };
    memset(var, 0, 4);
    vm_readv(addr, var, 4);
    return var[0];
}

float getfloat(uintptr_t addr)
{
    if (addr < 0xFFFFFF)
    {
        return 0;
    }
    float var[4] = { 0 };
    memset(var, 0, 4);
    vm_readv(addr, var, 4);
    return var[0];
}

void writeint(uintptr_t addr, uintptr_t data)
{
    vm_writev(addr, &data, 4);
}

void writefloat(uintptr_t addr, float data)
{
    vm_writev(addr, &data, 4);
}

int getPID(const char *packageName)
{
    int id = -1;
    DIR *dir;
    FILE *fp;
    char filename[32];
    char cmdline[256];
    struct dirent *entry;
    dir = opendir("/proc");
    while ((entry = readdir(dir)) != NULL)
    {
        id = atoi(entry->d_name);
        if (id != 0)
        {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp)
            {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);

                if (strcmp(packageName, cmdline) == 0)
                {
                    return id;
                }
            }
        }
    }
    closedir(dir);
    return -1;
}

#define AIM

char nm[64] = { 0 };

struct AimStruct
{
    float x = 0;
    // x
    float y = 0;
    // y
    uintptr_t Ox;
    // 对象x地址

    float ScreenDistance = 0;
    // 屏幕距离
    float WorldDistance = 0;
    // 世界距离
} Aim[100];



float px, py, bar;
float matrix[16] = { 0 };

float MaxDistance = 300;
uintptr_t MatrixAddress = 0;
uintptr_t UWorldPointer = 0;
uintptr_t GName = 0;
uintptr_t UWorld;
int Fire = 0;

int MaxPlayerCount = 0;
float zm_x, zm_y;
int screenDistance;
int WorldDistance;
int AimCount = 0;
float diff[3] = { 0 };

int FingerMax = 3;
// 相差
double FlyTime = 0.0f;
// 子弹飞行时间

float BulletSpeed = 720.0f;
// 子弹速度(M4:880(720.0 m/s),98K:760(600 m/s))

float LastCoor[3] = { 0 };

// 上一个坐标

float NewCoor[3] = { 0 };

// 下一个坐标

float DropM = 0.0f;
// 下坠

int Gmin = -1;

float SlideX = 0.0f;
float SlideY = 0.0f;
// 划屏位置

float SlideRanges = 70.0f;
// 划屏范围

#define TIME 0.1

float get2dDistance(float x, float y, float x1, float y1)
{
    float xx1 = x - x1;
    float yy2 = y - y1;
    // 取平方根
    return sqrt(xx1 * xx1 + yy2 * yy2);
}

struct D3DVector
{
    float X;
    float Y;
    float Z;
};

uintptr_t get_module_base(int pid, const char *module_name)
{
    FILE *fp;
    long addr = 0;
    char *pch;
    char filename[32];
    char line[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    fp = fopen(filename, "r");
    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp))
        {
            if (strstr(line, module_name))
            {
                pch = strtok(line, "-");
                addr = strtoul(pch, NULL, 16);
                if (addr == 0x8000)
                    addr = 0;
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}



void getname(int id, char *bbb, uintptr_t addr)
{
    int classname;
    int m = 0;
    if (id > 0 && id < 2000000)
    {
        int ye = id / 16384;
        int xu = id % 16384;
        uintptr_t ddz = getint(addr + ye * 4);
        uintptr_t namedz = getint(ddz + xu * 4);
        int i = 0;
        for (i; i < 8; i++)
        {
            classname = getint(namedz + 8 + i * 4);
            memcpy(&bbb[m], &classname, 4);
            m += 4;
        }
    }
}



void enque(int slot)
{
    for(int i=0;i<sizeoq;i++)
    {
        //printf("entered for \n");
        if(que[i]==-1)
        {
            que[i]=slot;
            front++;
            //printf("\n entered,");
            return ;
        }
    }
    //printf("overflow");
    //return -1;
}

//void* perfgest(void* rp);
void execute_sleep(int duration_msec);
double timediff_msec(struct timespec *t1, struct timespec *t2);

void mainThread(Request &request);

int find(int slot)
{
    for(int i=0;i<sizeoq;i++)
    {
        if(que[i]==slot)
            return i;
    }
    //printf("not found");
    return -1;
}

void deque(int i)
{
    int j;
    if (front == 0)
    {
        //printf("Underflow \n");
        return ;
    }
    else
    {
        j=find(i);
        //printf("Element deleted from the Queue: %d\n", que[j]);
        que[j] = -1;
        x[j]=-1;
        y[j]=-1;
        front--;
        return;
    }
}

void syncit()
{
    struct input_event ie;
    for(int i=0;i<sizeoq;i++)
    {
        if(que[i]>=0)
        {
            if(x[i]!=-1&&y[i]!=-1)
            {
                ie.type=EV_ABS;
                ie.code=ABS_MT_POSITION_X;
                ie.value=x[i];
                write(fd, &ie, sizeof(ie));
                ie.type=EV_ABS;
                ie.code=ABS_MT_POSITION_Y;
                ie.value=y[i];
                write(fd, &ie, sizeof(ie));
                ie.type=EV_ABS;
                ie.code=ABS_MT_TRACKING_ID;
                ie.value=que[i];
                write(fd, &ie, sizeof(ie));
            }
            ie.type=EV_SYN;
            ie.code=SYN_MT_REPORT;
            ie.value=0;
            write(fd, &ie, sizeof(ie));
        }
    }
    ie.type=EV_SYN;
    ie.code=SYN_REPORT;
    ie.value=0;
    write(fd, &ie, sizeof(ie));
}

void initarr()
{
    for(int i=0;i<sizeoq;i++)
    {
        x[i]=-1;
        y[i]=-1;
        que[i]=-1;
    }
}

void *TypeA()
{
    struct input_event ie;
    struct input_event oy;
    int sv = 0, ilsy = 0, ptr = -1, j, lsv = -1, ba = 0;
    while (read(fb, &ie, sizeof(struct input_event)))
    {
        if (ie.code == ABS_MT_SLOT)
        {
            sv = ie.value;
            lsv = sv;
        }
        else if (ie.code == SYN_REPORT)
        {
            ilsy = 1;
            if (ba != 1)
                ptr++;
            else
                ba = 0;
        }
        else if (ilsy == 1 && sv > 0)
        {
            sv = lsv;
            ilsy = 0;
        }
        if (ie.code == ABS_MT_TRACKING_ID)
        {
            if (ie.value == -1)
            {
                deque(sv);
            }
            else
            {
                enque(sv);
            }
        }
        if (ie.code == KEY_APPSELECT)
        {
            oy.type = ie.type;
            oy.code = ie.code;
            oy.value = ie.value;
            write(fd, &oy, sizeof(oy));
            ba = 1;
        }
        else if (ie.code == KEY_BACK)
        {
            // print_event(&ie);
            oy.type = ie.type;
            oy.code = ie.code;
            oy.value = ie.value;
            write(fd, &oy, sizeof(oy));
            ba = 1;
        }
        if (ie.code == ABS_MT_POSITION_X)
        {
            j = find(sv);
            x[j] = ie.value;
        }
        else if (ie.code == ABS_MT_POSITION_Y)
        {
            j = find(sv);
            y[j] = ie.value;
        }
        if (ptr >= front && front != 0)
        {
            ptr = 0;
            syncit();
        }
        else if (front == 0)
        {
            oy.type = EV_SYN;
            oy.code = SYN_MT_REPORT;
            oy.value = 0;
            write(fd, &oy, sizeof(oy));
            oy.type = EV_SYN;
            oy.code = SYN_REPORT;
            oy.value = 0;
            write(fd, &oy, sizeof(oy));
            continue;
        }
    }
    return 0;
}

void TouchDown(int tid, int xx,int yy)
{
    int pl;
    enque(tid);
    pl=find(tid);
    x[pl]=xx;
    y[pl]=yy;
    syncit();
}

void TouchMove(int tid,int xx,int yy)
{
    if (xx >= SlideX + SlideRanges || xx <= SlideX - SlideRanges || yy >= SlideY + SlideRanges
        || yy <= SlideY - SlideRanges)
    {
        return;
    }
    int pl;
    pl=find(tid);
    x[pl]=xx;
    y[pl]=yy;
    // if(front==1)
    syncit();
}

void TouchUp(int tid)
{
    deque(tid);
    syncit();
}

int findminat()
{
    float min = 99999999;
    int minAt = 999;

    for (int i = 0; i < MaxPlayerCount; i++)
    {

        if (Aim[i].ScreenDistance < min && Aim[i].Ox != 0 && Aim[i].ScreenDistance != 0)
        {
            min = Aim[i].ScreenDistance;
            minAt = i;

        }
    }
    if (minAt == 999)
    {
        Gmin = -1;
        return -1;
    }
    Gmin = minAt;

    return minAt;
}

void *AimCalcu()
{
    if (Gmin != -1){
        vm_readv(Aim[Gmin].Ox, LastCoor, sizeof(LastCoor));
        usleep(TIME * 1000000);
        vm_readv(Aim[Gmin].Ox, NewCoor, sizeof(NewCoor));
        for (int i = 0; i < 3; i++)
        {
            diff[i] = NewCoor[i] - LastCoor[i];
        }
    }
}


void *AimBotThread()
{
    bool isDown = false;
    double leenx = 0.0f;
    double leeny = 0.0f;
    double speedx = 19.0f;
    double de = 1.5f;
    double tx = SlideX, ty = SlideY;
    double w = 0.0f, h = 0.0f, cmp = 0.0f;
    double ScreenX = py * 2, ScreenY = px * 2;
    double ScrXH = ScreenX / 2.0f;
    double ScrYH = ScreenY / 2.0f;
    float NowCoor[3];
    findminat();
    if (Gmin != -1){
        FlyTime = WorldDistance / BulletSpeed;
        DropM = 500.0f * FlyTime * FlyTime;
        vm_readv(Aim[Gmin].Ox, NowCoor, sizeof(NowCoor));
        float obj_x = 0, obj_y = 0, obj_z = 0, camear_z = 0;
        obj_x = NowCoor[0] + (FlyTime / TIME * diff[0]);
        obj_y = NowCoor[1] + (FlyTime / TIME * diff[1]);
        obj_z = NowCoor[2] + (FlyTime / TIME * diff[2]) + DropM;
        float r_x = 0, r_y = 0;
        camear_z = matrix[3] * obj_x + matrix[7] * obj_y + matrix[11] * obj_z + matrix[15];
        r_x = px + (matrix[0] * obj_x + matrix[4] * obj_y + matrix[8] * obj_z + matrix[12]) / camear_z * px;
        r_y = py - (matrix[1] * obj_x + matrix[5] * obj_y + matrix[9] * (obj_z + 5) + matrix[13]) / camear_z * py;
        WorldDistance = camear_z / 100.0 - 2.0;
        screenDistance = get2dDistance(px, py, r_x, r_y);
        zm_y = r_x;
        zm_x = py * 2 - r_y;
        if (zm_x != 0 && zm_y != 0){
            if (openMirror == 1 || Fire)
            {
                if (isDown == false)
                {
                    TouchDown(tid, tx, ty);
                    isDown = true;
                }

                if (zm_x >= 0 || zm_x <= ScreenX || zm_y >= 0 || zm_y <= ScreenY){
                    if (zm_x < ScrXH)
                    {
                        w = ScrXH - zm_x;
                        leenx = w / speedx;
                        if (leenx < 3.0f)
                            leenx = screenDistance / (speedx / de);
                        if (zm_y < ScrYH)
                        {
                            h = ScrYH - zm_y;
                            if (w > 1.0f)
                            {
                                TouchMove(tid, tx, ty -= leenx);
                            }

                        }
                        else
                        {
                            h = zm_y - ScrYH;
                            if (w > 1.0f)
                            {
                                TouchMove(tid, tx, ty += leenx);
                            }
                        }
                        cmp = w / h;
                        leeny = h / speedx * cmp;
                        if (leeny < 3.0f)
                            leeny = screenDistance / (speedx / de);
                        if (h > 1.0f)
                        {
                            TouchMove(tid, tx -= leeny, ty);
                        }
                    }
                    else if (zm_x > ScrXH)
                    {
                        w = zm_x - ScrXH;
                        leenx = w / speedx;
                        if (leenx < 3.0f)
                            leenx = screenDistance / (speedx / de);
                        if (zm_y < ScrYH)
                        {
                            h = ScrYH - zm_y;
                            if (w > 1.0f)
                            {
                                TouchMove(tid, tx, ty -= leenx);
                            }
                        }
                        else
                        {
                            h = zm_y - ScrYH;
                            if (w > 1.0f)
                            {
                                TouchMove(tid, tx, ty += leenx);
                            }
                        }
                        cmp = w / h;
                        leeny = h / speedx * cmp;
                        if (leeny < 3.0f)
                            leeny = screenDistance / (speedx / de);
                        if (h > 1.0f)
                        {
                            TouchMove(tid, tx += leeny, ty);
                        }
                    }

                    if (tx >= SlideX + SlideRanges || tx <= SlideX - SlideRanges
                        || ty >= SlideY + SlideRanges || ty <= SlideY - SlideRanges)
                    {
                        tx = SlideX, ty = SlideY;
                        TouchUp(tid);
                        usleep(13000);
                        TouchDown(tid, tx, ty);
                    }
                }
            }
            else if (!openMirror || !Fire)
            {
                if (isDown == true)
                {
                    tx = SlideX, ty = SlideY;
                    TouchUp(tid);
                    isDown = false;
                }
            }
            usleep(13000);
        }
    }
}

void func()
{
    while (1) {
        AimCount = 0;
        long int Uleve = getint(getint(UWorld) + 0x20);
        int quantity = getint(Uleve + 0x74);
        long int arrayaddr = getint(Uleve + 0x70);
        vm_readv(MatrixAddress, matrix, 4 * 16);
        for (int i = 0; i < quantity; i++) {
            long int tmpptr = getint(arrayaddr + 4 * i);
            char name[256] = "";
            int oid = getint(tmpptr + 0x10);
            getname(oid, name, GName);
            long int uMyObject = getint(getint(getint(getint(getint(UWorld) + 0x24) + 0x60) + 0x20) + Offsets::AcknowledgedPawn);
            LOGI("%s", name);
        }
    }
   /**std::thread thr(AimCalcu);
   std::thread thr1(AimBotThread);
   std::thread thr2(TypeA);
    while (1)
    {
        AimCount = 0;
        long int Uleve = getint(getint(UWorld) + 0x20);
        int quantity = getint(Uleve + 0x74);
        long int arrayaddr = getint(Uleve + 0x70);
        vm_readv(MatrixAddress, matrix, 4 * 16);

        for (int i = 0; i < quantity; i++)
        {

            long int tmpptr = getint(arrayaddr + 4 * i);
            char name[256] = "";
            int oid = getint(tmpptr + 0x10);
            getname(oid, name, GName);
            long int uMyObject = getint(getint(getint(getint(getint(UWorld) + 0x24) + 0x60) + 0x20) + 0x320);
            Fire = getint(uMyObject + 0xdf0);
            openMirror = getint(uMyObject + 0xa89);
            LOGI("%s", name);
            if (strstr(name, "Pawn"))
            {
                float camear_z, r_x, r_y, r_w, tmph;
                long int objectCoordinatePointer = getint(tmpptr + 0x140) + 0x150;
                D3DVector Object{};
                vm_readv(objectCoordinatePointer, &Object, sizeof(Object));
                int team = getint(tmpptr + 0x660);
                int teammate;
                if (tmpptr == uMyObject)
                    teammate = team;
                if (teammate == team) continue;
                camear_z =
                        matrix[3] * Object.X + matrix[7] * Object.Y + matrix[11] * Object.Z +
                        matrix[15];
                r_x =
                        px + (matrix[0] * Object.X + matrix[4] * Object.Y +
                              matrix[8] * Object.Z + matrix[12]) / camear_z * px;
                r_y =
                        py - (matrix[1] * Object.X + matrix[5] * Object.Y +
                              matrix[9] * (Object.Z - 5) + matrix[13]) / camear_z * py;
                r_w =
                        py - (matrix[1] * Object.X + matrix[5] * Object.Y +
                              matrix[9] * (Object.Z + 205) + matrix[13]) / camear_z * py;
                tmph = r_y - r_w;
                int ObjectDistance = camear_z / 100.0;
                if (ObjectDistance <= 0 || ObjectDistance >= 400)
                    continue;
                if (tmph > 0){
                    Aim[AimCount].ScreenDistance = get2dDistance(px, py, r_x, r_y);
                    Aim[AimCount].WorldDistance = ObjectDistance;
                    Aim[AimCount].Ox = objectCoordinatePointer;
                    AimCount++;
                }
            }
        }
        MaxPlayerCount = AimCount;
    } **/
}

char msg[0x8000];
uintptr_t libbase = 0;

bool isServerSocketConnected() {
    bool _results = false;
    if (Create() ){
        strcpy(msg, OBFUSCATE("[Server] Socket create!"));
        LOGI("%s", msg);
       if (Connect()){
           strcpy(msg, OBFUSCATE("[Server] Socket connected!"));
           LOGI("%s", msg);
           _results = true;
       } else {
           strcpy(msg, OBFUSCATE("[Server] Socket can't connect!"));
           LOGE("%s", msg);
       }
    } else {
        strcpy(msg, OBFUSCATE("[Server] Socket can't create!"));
        LOGE("%s", msg);
    }
    return _results;
}

uintptr_t getRealOffsets(uintptr_t address){
    if (libbase != 0)
        return libbase + address;
    return 0;
}
int isApkRunning(const char *pkg_name){
    if (getPID(pkg_name) != 0 && getPID(pkg_name) > 0){
        return 1;
    }
    return 0;
}
bool initGameBase(const char* libName) {
    if (pid != 0 || pid > 0)
        libbase = get_module_base(pid, libName);
    sprintf(msg, OBFUSCATE("%s base address: %x"), libName, libbase);
    LOGI("%s", msg);
    return libbase != 0 || libbase > 0;
}
bool initGameHack() {
    if (isApkRunning(OBFUSCATE("com.tencent.ig")) == 1){
        pid = getPID(OBFUSCATE("com.tencent.ig"));
        sprintf(msg, OBFUSCATE("PUBGM Global version is runnning! PID: %d"), pid);
        LOGI("%s", msg);
        return true;
    } else if (isApkRunning(OBFUSCATE("com.vng.pubgmobile")) == 1){
        pid = getPID(OBFUSCATE("com.vng.pubgmobile"));
        sprintf(msg, OBFUSCATE("PUBGM Vietnam version is runnning! PID: %d"), pid);
        LOGI("%s", msg);
        return true;
    } else if (isApkRunning(OBFUSCATE("com.pubg.krmobile")) == 1){
        pid = getPID(OBFUSCATE("com.pubg.krmobile"));
        sprintf(msg, OBFUSCATE("PUBGM Korea version is runnning! PID: %d"), pid);
        LOGI("%s", msg);
        return true;
    } else if (isApkRunning(OBFUSCATE("com.rekoo.pubgm")) == 1){
        pid = getPID(OBFUSCATE("com.rekoo.pubgm"));
        sprintf(msg, OBFUSCATE("PUBGM Taiwan version is runnning! PID: %d"), pid);
        LOGI("%s", msg);
        return true;
    } else {
        sprintf(msg, OBFUSCATE("Can't get game pid!! PID: %d"), pid);
        LOGE("%s", msg);
        return false;
    }
}


float Width = 0.0f, Height = 0.0f;

void initDeviceScreen(Request& request){
    Width = (float) request.ScreenWidth;
    Height = (float) request.ScreenHeight;
    if (Height > Width){
        float t = Width;
        Width = Height;
        Height = t;
    }
    sprintf(msg, OBFUSCATE("\nDevice Screen\nWidth: %d\nHeight: %d\n"), (int) Width, (int) Height);
    LOGI("%s", msg);
}

int getEvent(){
    char l[256];
    sprintf(l, OBFUSCATE("/dev/input/event%d"), TouchHelper::GetEventID());
    fb = open(l, O_RDWR);
    if (fb < 0)
    {
        LOGE("%s NULL", l);
        return NULL;
    }
    LOGI("%s", l);
}

static bool isGrab(){
    if (ioctl(fb, EVIOCGRAB) != 1)
        return true;
    return false;
}

void *startGame(){
    system("su -c am start -n com.tencent.ig/com.epicgames.ue4.GameActivity");
}

int main(int argc, char **argv)
{
    if (isServerSocketConnected()){
        Request request{};
        Response response{};
        receive((void*)&request);
        if (request.Mode == InitMode){
            initDeviceScreen(request);
            if (initGameHack()){
                if (initGameBase(OBFUSCATE("libUE4.so"))){
                    MatrixAddress = getint(getint(libbase + Offsets::ViewWorld) + 0x68) + 0x6d0;
                    UWorld = getRealOffsets(Offsets::GWorld);
                    GName = getint(getRealOffsets(Offsets::GNames));
                    py = Height;
                    px = Width;
                    if (request.Aimbot.Status){
                        TouchHelper::Init(fb, fd, px, py, SlideX, SlideY);
                    }
                    px /= 2;
                    py /= 2;
                    initarr();
                    while (receive((void*)&request) > 0){
                        AimCount = 0;
                        mainThread(request);
                        response.Success = true;
                        send((void*)&response, sizeof(response));
                    }
                }
            }
        }
    }
    return 0;
}

void mainThread(Request &request) {
    if (request.Aimbot.Status){
        std::thread thr(AimCalcu);
        std::thread thr1(AimBotThread);
        std::thread thr2(TypeA);
    }
    long int Uleve = getint(getint(UWorld) + 0x20);
    int quantity = getint(Uleve + 0x74);
    long int arrayaddr = getint(Uleve + 0x70);
    vm_readv(MatrixAddress, matrix, 4 * 16);
    for (int i = 0; i < quantity; i++) {
        long int tmpptr = getint(arrayaddr + 4 * i);
        char name[256] = "";
        int oid = getint(tmpptr + 0x10);
        long int uMyObject = getint(getint(getint(getint(getint(UWorld) + 0x24) + 0x60) + 0x20) + Offsets::AcknowledgedPawn);
        Fire = getint(uMyObject + Offsets::IsFire);
        openMirror = getint(uMyObject + Offsets::IsADS);
        getname(oid, name, GName);
        if (strstr(name, "Pawn"))
        {
            float camear_z, r_x, r_y, r_w, tmph;
            long int objectCoordinatePointer = getint(tmpptr + Offsets::RootComponent) + Offsets::Position;
            D3DVector Object{};
            vm_readv(objectCoordinatePointer, &Object, sizeof(Object));
            int team = getint(tmpptr + Offsets::TeamID);
            int teammate;
            if (tmpptr == uMyObject)
                teammate = team;
            if (teammate == team) continue;
            camear_z =
                    matrix[3] * Object.X + matrix[7] * Object.Y + matrix[11] * Object.Z +
                    matrix[15];
            r_x =
                    px + (matrix[0] * Object.X + matrix[4] * Object.Y +
                          matrix[8] * Object.Z + matrix[12]) / camear_z * px;
            r_y =
                    py - (matrix[1] * Object.X + matrix[5] * Object.Y +
                          matrix[9] * (Object.Z - 5) + matrix[13]) / camear_z * py;
            r_w =
                    py - (matrix[1] * Object.X + matrix[5] * Object.Y +
                          matrix[9] * (Object.Z + 205) + matrix[13]) / camear_z * py;
            tmph = r_y - r_w;
            int ObjectDistance = camear_z / 100.0;
            if (ObjectDistance <= 0 || ObjectDistance >= 400)
                continue;
            if (request.Aimbot.Status) {
                if (tmph > 0){
                    Aim[AimCount].ScreenDistance = get2dDistance(px, py, r_x, r_y);
                    Aim[AimCount].WorldDistance = ObjectDistance;
                    Aim[AimCount].Ox = objectCoordinatePointer;
                    AimCount++;
                }
            }
        }
    }
    MaxPlayerCount = AimCount;
}
