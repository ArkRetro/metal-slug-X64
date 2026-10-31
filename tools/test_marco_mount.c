/* Test of the mounted player (src/marco_mount.c, ROM 0C3078..0C3260 and the
 * slot hand-off 0C3348..0C3376) against player 1's node 1014C4 in the
 * weapons family traces camelfam (Camel Slug: mounted at 9530, ridden, the
 * rider killed at 10091) and tankfam (SV-001: mounted at 10375, ridden, A+B
 * eject at 11099 and the eject jump): data/mount_fixture.json.gz ->
 * tools/compile_mount_fixture.py -> build/mount_fixture.h.
 *
 * Each update starts from the recorded node bytes and the RAM the original
 * read (slot 10C608, record 10C2E8, 10C300/10C301, input words 10E9EC, the
 * weapon block, 10000E). The translated steps run as the original chains
 * them; the node bytes they own must equal the recorded result, and every
 * slot/record byte the update wrote must equal the native slot. Bytes that
 * the untranslated parts of the same update rewrite afterwards (0143EE and
 * 0427A of the caller, the death 0C2956, the rest of the common
 * processing: +84/+85) are not compared; each case says which. */
#include <stdio.h>
#include <string.h>
#include "../src/marco_mount.h"
typedef struct { uint32_t addr; uint8_t value; } mount_ram_t;
typedef struct { uint8_t source; uint32_t frame,routine; const char *pre,*post; unsigned in0,ins,out0,outs; } mount_update_t;
#include "mount_fixture.h"
static void hex(const char *s,uint8_t *m) {for (unsigned i=0;i<0xc0;i++) {unsigned v;sscanf(s+2*i,"%2x",&v);m[i]=(uint8_t)v;}}
static uint32_t rl(const uint8_t *m,unsigned o) {return (uint32_t)m[o]<<24|(uint32_t)m[o+1]<<16|(uint32_t)m[o+2]<<8|m[o+3];}
static int fails;static unsigned compared,slot_bytes;
static uint8_t mask[0xc0];
static void own(unsigned o,unsigned n,uint8_t bits) {for (unsigned i=0;i<n;i++) mask[o+i]|=bits;}
int main(void) {
    static const char *src[]={"camelfam","tankfam"};
    unsigned counts[8]={0};
    for (unsigned k=0;k<sizeof(mount_updates)/sizeof(mount_updates[0]);k++) {
        const mount_update_t *u=&mount_updates[k];
        uint8_t m[0xc0],post[0xc0];hex(u->pre,m);hex(u->post,post);
        neo_vslots_t v;memset(&v,0,sizeof(v));v.record[0]=v.record[1]=NEO_VSLOT_RECORD;
        marco_mount_in_t in;memset(&in,0,sizeof(in));
        uint8_t ram[0x10];memset(ram,0,sizeof(ram));
        for (unsigned i=0;i<u->ins;i++) {
            uint32_t a=mount_in[u->in0+i].addr;uint8_t x=mount_in[u->in0+i].value;
            if (a>=0x10c608u && a<0x10c650u) {unsigned o=a-0x10c608u;v.slot[o/0x24u].b[o%0x24u]=x;}
            else if (a==0x10000eu) in.video=(uint16_t)(in.video|x<<8);
            else if (a==0x10000fu) in.video=(uint16_t)(in.video|x);
            else if (a==0x10c300u) in.system=x!=0;
            else if (a==0x10c301u) in.halted=x!=0;
            else if (a==0x10e9ecu) in.fresh=(uint16_t)(in.fresh|x<<8);
            else if (a==0x10e9edu) in.fresh=(uint16_t)(in.fresh|x);
            else if (a==0x10e9eeu) in.held=(uint16_t)(in.held|x<<8);
            else if (a==0x10e9efu) in.held=(uint16_t)(in.held|x);
            else if (a==0x10c718u) in.shots=x;
            else if (a==0x10c71au) in.shot_limit=x;
            else if (a==0x10c71cu) in.shot_count=x;
        }
        memset(mask,0,sizeof(mask));
        uint32_t stop=0;const char *path="";
        marco_mount_result_t r;
        if (u->routine==0xc2692u) {
            /* The jump state is the port's; only its common processing's
             * hand-off: the routine it leaves and +9A bit 7. */
            r=marco_mount_handoff(&v,m,&in);
            if (r==MARCO_MOUNT_ENTER) {own(0,4,0xff);path="enter";counts[0]++;}
            else {path="free";counts[1]++;if (rl(post,0)==MARCO_MOUNT_C3078) {printf("FAIL %s %u: mount missed\n",src[u->source],u->frame);fails++;}}
            own(0x9a,1,0x80);
        } else if (u->routine==MARCO_MOUNT_C3078 || u->routine==MARCO_MOUNT_C309C) {
            bool entering=u->routine==MARCO_MOUNT_C3078,followed;
            r=marco_mount_ride(&v,m,&in,&followed,&stop);
            if (entering) {own(0,4,0xff);own(0x7a,2,0xff);own(0x15,1,1);own(0x50,4,0xff);own(0x98,1,0xff);own(0x90,2,0xff);own(0x7c,4,0xff);}
            if (followed) {own(0x15,1,0x40);own(0x28,0x14,0xff);}
            own(0x9a,1,0x80);
            if (r==MARCO_MOUNT_COMMON) r=marco_mount_handoff(&v,m,&in);
            if (r==MARCO_MOUNT_COMMON) {path=entering?"mount":"ride";counts[entering?2:3]++;own(0,4,0xff);}
            else if (r==MARCO_MOUNT_EXIT) {
                uint8_t cause;r=marco_mount_exit(&v,m,&in,&cause,&stop);
                if (r==MARCO_MOUNT_DIE) {
                    /* 0C2956 (the port's death) rewrites the position, the
                     * velocity, the routine and +15. */
                    path="die";counts[4]++;
                    neo_vslot_release(&v,0);       /* 0C2E84 -> 0C2E8A in the same update: 0C2F6C 0AA4C8 */
                    memset(mask,0,sizeof(mask));own(0x5a,1,0xff);own(0x5f,1,0x80);own(0x9a,1,0x80);
                } else if (r==MARCO_MOUNT_EJECT) {
                    /* 0AA5AC's 0143EE moved the player from the vehicle;
                     * then 0C3232/0C3250 in the same update. */
                    path="eject";counts[5]++;
                    memset(mask,0,sizeof(mask));
                    uint32_t saved=marco_mount_eject(&v,m,&in);
                    /* 0143EE (the caller's sweep) decides the ground bit. */
                    m[0x15]=(uint8_t)((m[0x15]&~0x40u)|(post[0x15]&0x40u));
                    marco_mount_eject_end(m,saved);
                    r=marco_mount_handoff(&v,m,&in);
                    own(0,4,0xff);own(0x7c,4,0xff);own(0x90,2,0xff);own(0x98,1,0xff);own(0x30,4,0xff);
                    if (!(saved&0x80000000u)) own(0x34,4,0xff);
                    own(0x5f,1,0x30);own(0x9a,1,0x88);own(0x9b,1,0xff);own(0x15,1,1);
                } else {printf("FAIL %s %u: exit stop %05lx\n",src[u->source],u->frame,(unsigned long)stop);fails++;continue;}
            } else {printf("FAIL %s %u: ride result %d stop %05lx\n",src[u->source],u->frame,(int)r,(unsigned long)stop);fails++;continue;}
        } else if (u->routine==MARCO_MOUNT_C3250) {
            path="jump";counts[6]++;
            uint32_t saved=marco_mount_eject(&v,m,&in);
            m[0x15]=(uint8_t)((m[0x15]&~0x40u)|(post[0x15]&0x40u));   /* 0143EE's ground bit */
            bool landed=marco_mount_eject_end(m,saved);(void)landed;
            own(0,4,0xff);own(0x30,4,0xff);if (!(saved&0x80000000u)) own(0x34,4,0xff);
            own(0x5f,1,0x30);own(0x9a,1,0x88);own(0x9b,1,0xff);own(0x15,1,1);
            /* The common processing after it: 0AA458 again. */
            r=marco_mount_handoff(&v,m,&in);own(0x9a,1,0x80);
        } else {printf("FAIL %s %u: routine %05lx outside the fixture's set\n",src[u->source],u->frame,(unsigned long)u->routine);fails++;continue;}
        for (unsigned o=0;o<0xc0;o++) {
            if (!mask[o]) continue;
            compared++;
            if ((m[o]^post[o])&mask[o]) {printf("FAIL %s %u %s +%02x native %02x original %02x (mask %02x)\n",src[u->source],u->frame,path,o,m[o],post[o],mask[o]);fails++;}
        }
        /* Every slot/record byte the update wrote: the last value. */
        for (unsigned i=0;i<u->outs;i++) {
            uint32_t a=mount_out[u->out0+i].addr;bool last=true;
            for (unsigned j=i+1;j<u->outs;j++) if (mount_out[u->out0+j].addr==a) last=false;
            if (!last) continue;
            uint8_t want=mount_out[u->out0+i].value,got;
            if (a>=0x10c608u && a<0x10c650u) {unsigned o=a-0x10c608u;got=v.slot[o/0x24u].b[o%0x24u];}
            else if (a>=0x10c2e8u && a<0x10c2f0u) {unsigned o=a-0x10c2e8u;got=(uint8_t)(v.record[o/4u]>>(8u*(3u-o%4u)));}
            else continue;
            slot_bytes++;
            if (got!=want) {printf("FAIL %s %u %s RAM %06lx native %02x original %02x\n",src[u->source],u->frame,path,(unsigned long)a,got,want);fails++;}
        }
    }
    printf("mount: %u updates (free %u, enter %u, mount %u, ride %u, die %u, eject %u, jump %u), %u node bytes, %u slot/record bytes, %d failures\n",
        (unsigned)(sizeof(mount_updates)/sizeof(mount_updates[0])),counts[1],counts[0],counts[2],counts[3],counts[4],counts[5],counts[6],compared,slot_bytes,fails);
    return fails?1:0;
}
