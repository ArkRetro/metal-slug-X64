#include "neo_region.h"
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4), neo_grid_splat (00505A) */
/* The player's C weapon (id 0x0C: homing shells), manual translation of ROM
 * 0635D2..064128 and 0C58D2..0C595A; ROM labels are provenance only.
 *
 * The player's fire callback 0C4372 (gameplay.c) makes, after the list head
 * (00411C), the muzzle flash 0C58D2 (table 0C432A) and the shell 063F32
 * (table 0C42AA; +93 = 2 would take 063F10 from 0C42EA, not translated):
 * +6E angle of 0C4244, +70 player, +5E the player's, position 0C482E.
 *
 * Shell 063F32: program 267F92 (pose by +8A), palette 202, sound 10F5,
 * search row +76 = 063610, speed +62 = 0x80000, +88 turn rate, +86 aim word,
 * +8B random (its sign picks the lowest-health target). Flight 063FDC:
 * 0049E8 with the search row (256x256 around the shell, kind 0: no damage)
 * gives A0 (highest +4E) / A1 (lowest +4E); 01B14A turns +86 towards the
 * centre of the target's first +50 rectangle (none: a random turn); the
 * word angle +6E follows +86 through +88, +8A/+72 from it, the speed grows
 * by a random step (0x80000 again past 0x100000) and the velocity halves
 * towards the polar one; 0143E4, 00427A; a wall (+15 bit 7) or a hit of the
 * contact row 063600 (kind 9, damage 30) makes it explode (064128); out of
 * the rectangle 063814 it releases its projectile (0C0DFC) and goes; every
 * time +74 borrows (-0x70) a trail puff 063D9E follows it (0040BC, 16
 * pixels behind); then 00505A (danger splat).
 * Explosion 064128: 0C0DFC, program 268622 (sound 1025, CALL 0636B0 with
 * payload 0,0, program 22CE6C, row 063690; END): one update. 0636B0: the
 * blast 06371E after the list head, +5E copied, +6E = the row, at the
 * offset (mirrored by +15), +15 = ours & 3, the program; 06371E: palette
 * 363, +66 = 1001; 063740: 01ABC8, 0049E8 with +6E once, then 063756 until
 * its program ends or it leaves 063716.
 * Trail 063D9E: palette 37C, program 26863A, half the shell's velocity, a
 * drift +6E/+72 = polar(0x2000, the shell's +72 byte) mirrored by its +15,
 * random x jitter (sign -> +15 bit 0); 063E48 pulls it towards the node
 * before it in the list (+04: the next newer puff, or the shell) by 1/256
 * of the distance per update, 01AC38, until its program ends.
 * Flash 0C58D2: palette 37C, program 0C586C (up/down; down also flips y)
 * or 0C5806, then 0C5930: velocity decays (x 1/32, y 1/8), 01AC38, until its
 * program ends.
 * The paths that would run the cartridge check (2FFFE0) end in 004188 on
 * genuine hardware: delete. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
static int32_t asr32(int32_t v,unsigned n) {return v<0?(int32_t)~(~(uint32_t)v>>n):(int32_t)((uint32_t)v>>n);}
static int16_t asr16(int16_t v,unsigned n) {return (int16_t)asr32(v,n);}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
/* 0C0DFC with the projectile's player (+70) and its weapon id (the C 0C,
 * the R 3). */
static bool release_id(neo_gactor_t *a,neo_genv_t *e,uint8_t id) {
    if (W(0x70)>=2u) {ga_fail(e,0xc0e04);return false;}           /* trap #15 */
    if (!e->pickup || !e->pickup->weapons) {ga_fail(e,0xc0dfc);return false;}
    marco_ammo_t m=marco_weapons_ammo(e->pickup->weapons,W(0x70));
    marco_ammo_release(&m,id);marco_weapons_set_ammo(e->pickup->weapons,W(0x70),&m);
    return true;
}
static bool release(neo_gactor_t *a,neo_genv_t *e) {return release_id(a,e,0x0c);}
/* 01B14A/01B162: angle from the shell to the centre of the target's first
 * +50 rectangle (the shell's +15 mirrors the offset), 002EB4, & 0xFF. */
static uint16_t aim(const neo_gactor_t *a,const neo_ga_target_t *t) {
    int16_t x=(int16_t)t->x,y=(int16_t)t->y;
    if (t->box) {
        int16_t dx=(int16_t)(asr16(t->rect[2],1)+t->rect[0]),dy=(int16_t)(asr16(t->rect[3],1)+t->rect[1]);
        if (B(0x15)&1u) dx=(int16_t)-dx;
        if (B(0x15)&2u) dy=(int16_t)-dy;
        x=(int16_t)(x+dx);y=(int16_t)(y+dy);
    }
    return neo_angle((int16_t)(x-(int16_t)W(0x3c)),(int16_t)(y-(int16_t)W(0x40)))&0xffu;
}
/* 0636B0: the blast 06371E of the explosion program 268622 (its only call;
 * the converted CALL keeps the payload's first word, 0). */
static bool blast(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy,uint32_t program,uint32_t row) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x6371e,true):NULL;
    if (!c) return true;
    ga_sb(c,0x5e,B(0x5e));ga_sl(c,0x6e,row);
    if (B(0x15)&1u) dx=(int16_t)-dx;
    if (B(0x15)&2u) dy=(int16_t)-dy;
    ga_place_at(c,a,e,dx,dy);
    ga_sb(c,0x15,(uint8_t)(B(0x15)&3u));                               /* 01AC94 */
    return ga_anim_start(c,e,program,0x100);                           /* 004260 */
}
static bool chaser_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    if (kind!=NEO_ANIM_CALL || routine!=0x636b0u || parameter) return false;
    /* 268622 (the C): 22CE6C, row 063690; 26860A (the R with +93 = 2,
     * 063D60): 22CF2C, row 0636A0 (their 0636B0 payloads). */
    if (a->program_tag==0x268622u) return blast(a,e,0,0,0x22ce6c,0x63690);
    if (a->program_tag==0x26860au) return blast(a,e,0,0,0x22cf2c,0x636a0);
    return false;
}
/* The R's explosion programs 26858A (level) / 2685CA (vertical): three
 * 0636B0 calls with the payloads (dx, dy, program, row) (0x10/0x20/0x30
 * along the rocket's axis; 22CDD4 / 22CE6C / 22CF2C; rows 063680 / 063690
 * / 0636A0). The converted CALL keeps only dx: the vertical calls are told
 * apart by their order. */
static bool rocket_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    if (kind!=NEO_ANIM_CALL || routine!=0x636b0u || (a->program_tag!=0x26858au && a->program_tag!=0x2685cau)) return false;
    static const uint32_t prog[3]={0x22cdd4,0x22ce6c,0x22cf2c},row[3]={0x63680,0x63690,0x636a0};
    bool level=a->program_tag==0x26858au;
    unsigned i;
    if (level) {
        if (parameter!=0x10u && parameter!=0x20u && parameter!=0x30u) {ga_fail(e,0x636b0);return true;}
        i=parameter/0x10u-1u;
    } else {
        /* The calls of one explosion may fall in different updates: count
         * them per node (+00..+BF hold nothing for it). */
        static struct {const neo_gactor_t *a;unsigned n;} seen[8];
        unsigned k,free=8;
        for (k=0;k<8 && seen[k].a!=a;k++) if (!seen[k].a && free==8) free=k;
        if (k==8) {if (free==8) {ga_fail(e,0x636b0);return true;} k=free;seen[k].a=a;seen[k].n=0;}
        i=seen[k].n++;
        if (seen[k].n>=3u) seen[k].a=NULL;
    }
    int16_t d=(int16_t)(0x10*(i+1u));
    return blast(a,e,level?d:0,level?0:d,prog[i],row[i]);
}
static ga_result_t chaser_tick(neo_gactor_t *a,neo_genv_t *e) {
    for (int guard=0;guard<2;guard++) switch (L(0)) {
    /* 063F10, the C with +93 = 2 (table 0C42EA): program 2681A4,
     * palette 86E, then 063F50 as 063F32. */
    case 0x63f10: case 0x63f32: {
        if (L(0)==0x63f10u) {START(0x2681a4);ga_palette(a,0x86e);}
        else {START(0x267f92);ga_palette(a,0x202);}
        SW(0x66,0xe000);SW(0x1e,0xe000);sound(e,0x10f5);
        SL(0x76,0x63610);SB(0x15,(B(0x15)|0x20u)&0xfeu);
        SL(0x62,0x80000);
        uint32_t vx,vy;ga_polar(0x80000,B(0x6e),&vx,&vy);SL(0x30,vx);SL(0x34,vy);
        SW(0x74,0);
        uint16_t d0=(uint16_t)((ga_rand(e)&0x7ffu)-0x400u);
        uint16_t d1=(uint16_t)(0x4000u-W(0x6e));
        if (d1!=0x8000u) d0=(uint16_t)(d0+(uint16_t)asr16((int16_t)d1,3));
        SW(0x88,d0);SW(0x86,W(0x6e));
        SB(0x8b,ga_rand(e));
        SL(0,0x63fdc);
        return GA_MORE;
    }
    case 0x63fdc: {
        ga_attack_targets(a,e,L(0x76));
        if (e->unsupported) return GA_UNSUPPORTED;
        const neo_ga_target_t *t=&e->target[(B(0x8b)&0x80u)?1:0];
        uint16_t d0;
        if (t->valid) {d0=aim(a,t);SB(0x86,d0);}
        else d0=(uint16_t)((ga_rand(e)&0xfffu)-0x800u);
        SW(0x86,W(0x86)+d0);
        int16_t turn=asr16((int16_t)(uint16_t)(W(0x86)-W(0x6e)),1);
        turn=asr16((int16_t)(uint16_t)(turn-W(0x88)),2);
        SW(0x88,W(0x88)+(uint16_t)turn);
        SW(0x6e,W(0x6e)+W(0x88));
        uint8_t angle=B(0x6e);
        SB(0x8a,(uint8_t)(((int8_t)angle>>3)+((angle>>2)&1u))&0x1fu);   /* asr.b #3, addx.b */
        SW(0x72,(uint16_t)(((W(0x88)&0xff00u)|angle)+0x80u));
        uint32_t speed=(ga_rand(e)&0x7fffu)+L(0x62);
        if (speed>=0x100000u) speed=0x80000;
        SL(0x62,speed);
        uint32_t vx,vy;ga_polar(speed,angle,&vx,&vy);
        SL(0x30,L(0x30)+(uint32_t)asr32((int32_t)(vx-L(0x30)),1));
        SL(0x34,L(0x34)+(uint32_t)asr32((int32_t)(vy-L(0x34)),1));
        ga_air(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (B(0x15)&0x80u) SL(0,0x64128);
        if (ga_attack(a,e,0x63600)) SL(0,0x64128);
        if (e->unsupported) return GA_UNSUPPORTED;
        if (ga_cull_rect(a,e,0x63814)) return release(a,e)?GA_DELETE:GA_UNSUPPORTED;
        uint8_t b74=B(0x74);SB(0x74,b74-0x70u);
        if (b74<0x70u) {
            neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x63d9e,0x2a):e->spawn?e->spawn(e->ctx,a,0x63d9e,false):NULL;   /* 0040BC, reserve 0x2A */
            if (c) {
                ga_polar(0xfff00000u,angle,&vx,&vy);
                ga_place_at(c,a,e,(int16_t)(vx>>16),(int16_t)(vy>>16));
            }
        }
        if (e->grid) neo_grid_splat(e->grid,W(0x3c),W(0x40));
        return GA_MORE;
    }
    /* ---- the R's rocket with +93 = 2: 063AEE (id 3) and 063B22 (id 8),
     * table 0C42EA ----
     * 063AEE: palette 202, program 267C74, +66/+1E E000, +8C = 3; 063B22:
     * palette 37C, program 267E86, +66/+1E 1110, +8C = 8. Both 063B52:
     * sound 10F5, contact rows 0635F0, +15 bit 5 set / bit 0 cleared,
     * +62 = 0x40000, the speed 0x20000 along +6E (002E7C), +74 = 0, +88 =
     * random & FFF - 800 + (4000 - +6E) >> 4 (asr), +86 = +6E. */
    case 0x63aee: case 0x63b22: {
        bool r8=L(0)==0x63b22u;
        ga_palette(a,r8?0x37c:0x202);START(r8?0x267e86:0x267c74);
        SW(0x66,r8?0x1110:0xe000);SW(0x1e,r8?0x1110:0xe000);SB(0x8c,r8?8:3);
        sound(e,0x10f5);
        SL(0x76,0x635f0);SB(0x15,(B(0x15)|0x20u)&0xfeu);
        SL(0x62,0x40000);
        uint32_t vx,vy;ga_polar(0x20000,B(0x6e),&vx,&vy);SL(0x30,vx);SL(0x34,vy);
        SW(0x74,0);
        uint16_t d0=(uint16_t)((ga_rand(e)&0xfffu)-0x800u);
        d0=(uint16_t)(d0+(uint16_t)asr16((int16_t)(uint16_t)(0x4000u-W(0x6e)),4));
        SW(0x88,d0);SW(0x86,W(0x6e));
        SL(0,0x63bc0);
        return GA_MORE;
    }
    /* 063BC0: the first target of +76 (0049E8) sets the high byte of +86
     * to its angle (01B14A); no target, no change. Turn (+86 - +6E) >> 2
     * - +88, >> 3 into +88 (asr), +6E += +88; +8A/+72 as 063FDC; +62 +=
     * 0x4000 (no cap); the velocity moves 1/16 towards +62 along +6E. Then
     * as 063FDC: 0143E4, 00427A, +15 bit 7 or a contact of 0635E0 ->
     * 063D60; out of 063814 (019504): 0C0DFC with +8C and 004188 (the
     * cartridge check between them is gone, as for 0638B4); the trail
     * 063D9E every 0x70 of +74; 00505A. */
    case 0x63bc0: {
        ga_attack_targets(a,e,L(0x76));
        if (e->unsupported) return GA_UNSUPPORTED;
        const neo_ga_target_t *t=&e->target[0];
        if (t->valid) SB(0x86,aim(a,t));
        int16_t turn=asr16((int16_t)(uint16_t)(W(0x86)-W(0x6e)),2);
        turn=asr16((int16_t)(uint16_t)(turn-W(0x88)),3);
        SW(0x88,W(0x88)+(uint16_t)turn);
        SW(0x6e,W(0x6e)+W(0x88));
        uint8_t angle=B(0x6e);
        SB(0x8a,(uint8_t)(((int8_t)angle>>3)+((angle>>2)&1u))&0x1fu);   /* asr.b #3, addx.b */
        SW(0x72,(uint16_t)(((W(0x88)&0xff00u)|angle)+0x80u));
        SL(0x62,L(0x62)+0x4000u);
        uint32_t vx,vy;ga_polar(L(0x62),angle,&vx,&vy);
        SL(0x30,L(0x30)+(uint32_t)asr32((int32_t)(vx-L(0x30)),4));
        SL(0x34,L(0x34)+(uint32_t)asr32((int32_t)(vy-L(0x34)),4));
        ga_air(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (B(0x15)&0x80u) SL(0,0x63d60);
        if (ga_attack(a,e,0x635e0)) SL(0,0x63d60);
        if (e->unsupported) return GA_UNSUPPORTED;
        if (ga_cull_rect(a,e,0x63814)) return release_id(a,e,B(0x8c))?GA_DELETE:GA_UNSUPPORTED;
        uint8_t b74=B(0x74);SB(0x74,b74-0x70u);
        if (b74<0x70u) {
            neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x63d9e,0x2a):e->spawn?e->spawn(e->ctx,a,0x63d9e,false):NULL;   /* 0040BC, reserve 0x2A */
            if (c) {
                ga_polar(0xfff00000u,angle,&vx,&vy);
                ga_place_at(c,a,e,(int16_t)(vx>>16),(int16_t)(vy>>16));
            }
        }
        if (e->grid) neo_grid_splat(e->grid,W(0x3c),W(0x40));
        return GA_MORE;
    }
    /* 063D60: 0C0DFC with +8C, the explosion 26860A, 01ABC8 and 00427A
     * until its end (004188). */
    case 0x63d60:
        if (!release_id(a,e,B(0x8c))) return GA_UNSUPPORTED;
        START(0x26860a);SL(0,0x63d86);
        /* fall through */
    case 0x63d86: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,chaser_call);
        if (r<0 || e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    case 0x64128:
        if (!release(a,e)) return GA_UNSUPPORTED;
        START(0x268622);SL(0,0x6414c);
        /* fall through */
    case 0x6414c: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,chaser_call);
        if (r<0 || e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    case 0x6371e:
        ga_palette(a,0x363);SW(0x66,0x1001);SW(0x1e,0x1001);SL(0,0x63740);
        return GA_MORE;
    case 0x63740:
        ga_project(a,e);ga_attack(a,e,L(0x6e));
        if (e->unsupported) return GA_UNSUPPORTED;
        SL(0,0x63756);
        /* fall through */
    case 0x63756: {
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return ga_cull_rect(a,e,0x63716)?GA_DELETE:GA_MORE;
    }
    case 0x63d9e: {
        ga_palette(a,0x37c);SW(0x66,0x1001);SW(0x1e,0x1001);START(0x26863a);
        neo_gactor_t *s=e->creator?e->creator(e->ctx,a):NULL;
        if (!s) {ga_fail(e,0x63dc8);return GA_UNSUPPORTED;}
        SL(0x30,asr32((int32_t)ga_l(s,0x30),1));SL(0x34,asr32((int32_t)ga_l(s,0x34),1));
        uint32_t vx,vy;ga_polar(0x2000,ga_b(s,0x72),&vx,&vy);
        if (ga_b(s,0x15)&1u) vx=0u-vx;
        if (ga_b(s,0x15)&2u) vy=0u-vy;
        SL(0x6e,vx);SL(0x72,vy);
        int32_t r=(int16_t)ga_rand(e);
        if (r<0) SB(0x15,B(0x15)|1u);
        SL(0x30,L(0x30)+(uint32_t)asr32(r,2));
        (void)ga_rand(e);
        SL(0x34,L(0x34)+vy);                                           /* adds d1, not the random word */
        ga_project(a,e);SL(0,0x63e48);
        return GA_MORE;
    }
    case 0x63e48: {
        neo_gactor_t *p=e->previous?e->previous(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x63e48);return GA_UNSUPPORTED;}
        SL(0x30,L(0x30)+(uint32_t)asr32((int32_t)(ga_l(p,0x3c)-L(0x3c)),8)+L(0x6e));
        SL(0x34,L(0x34)+(uint32_t)asr32((int32_t)(ga_l(p,0x40)-L(0x40)),8)+L(0x72));
        ga_move(a,e);
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    /* ---- the R's rocket 06381C and weapon 8's 0641EC ----
     * 0641EC..0644C2 is 06381C..063AEC with the programs 2684D2/2684AA
     * (bigger rocket frames), the contact rows 063640/063650 (0x30 x 0x10
     * instead of 0x18 x 0x08), the cull rectangle 06380C (its first eight
     * bytes are 063804's) and the weapon id 8 handed back by 0C0DFC; the
     * search rows, flight, trail 063D9E and explosion programs are the
     * same (the cartridge check after 0C0DFC is gone: 004188 directly). */
    case 0x6381c: case 0x641ec: {
        bool big=L(0)==0x641ecu;
        sound(e,0x10f5);SW(0x66,0xe000);SW(0x1e,0xe000);SB(0x15,B(0x15)|0x20u);ga_palette(a,0x37c);
        if (!(B(0x6e)&0x7fu)) {
            SL(0x76,0x63660);START(big?0x2684d2:0x2683f2);SL(0x7e,0xffff0000u);SL(0x82,0);
        } else {
            SL(0x76,0x63670);START(big?0x2684aa:0x2683b6);
            if (B(0x6e)&0x80u) SB(0x15,B(0x15)|2u);
        }
        SL(0x34,0);SW(0x74,0);SL(0,big?0x64284:0x638b4);
        return GA_MORE;
    }
    case 0x638b4: case 0x64284: {
        bool big=L(0)==0x64284u;
        /* 0638B4: home on the target of the search row +76 (A0, the
         * highest +4E): level, +34 towards (centre y : target +42) / 4 with
         * the lift +7E (decaying through +82); vertical, +30 towards the
         * target's +3C / 4; then the thrust 0x6000 along the axis. */
        ga_attack_targets(a,e,L(0x76));
        if (e->unsupported) return GA_UNSUPPORTED;
        const neo_ga_target_t *t=&e->target[0];
        bool level=!(B(0x6e)&0x7fu);
        if (level) {
            uint32_t d0=0;
            if (t->valid) {
                int16_t cy=(int16_t)t->y;
                if (t->box) {int16_t dy=(int16_t)(asr16(t->rect[3],1)+t->rect[1]);if (B(0x15)&2u) dy=(int16_t)-dy;cy=(int16_t)(cy+dy);}
                d0=(uint32_t)(uint16_t)cy<<16|t->y_low;
                d0=(uint32_t)asr32((int32_t)(d0-L(0x40)),2);
            }
            d0=(uint32_t)asr32((int32_t)(d0-L(0x34)),6)+L(0x7e);
            SL(0x34,L(0x34)+d0);
            SL(0x82,L(0x82)+L(0x7e));
            d0=(uint32_t)asr32((int32_t)L(0x82),1)+L(0x7e);
            SL(0x7e,L(0x7e)-(uint32_t)asr32((int32_t)d0,2));
            SL(0x30,L(0x30)+((B(0x15)&1u)?0xffffa000u:0x6000u));
        } else {
            uint32_t d0=0;
            if (t->valid) d0=(uint32_t)asr32((int32_t)(((uint32_t)t->x<<16|t->x_low)-L(0x3c)),2);   /* 06391C: the target's +3C long */
            SL(0x30,L(0x30)+(uint32_t)asr32((int32_t)(d0-L(0x30)),6));
            SL(0x34,L(0x34)+((B(0x15)&2u)?0xffffa000u:0x6000u));
        }
        ga_air(a,e);
        if ((B(0x15)&0x80u) && (!(B(0x15)&0x40u) || !(L(0x7e)&0x80000000u))) {SL(0,big?0x643da:0x63a92);continue;}   /* 06398E / 06435E */
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_attack(a,e,big?(level?0x63640:0x63650):(level?0x63620:0x63630))) SL(0,big?0x643da:0x63a92);
        if (e->unsupported) return GA_UNSUPPORTED;
        if (ga_cull_rect(a,e,big?0x6380c:0x63804)) return release_id(a,e,big?8:3)?GA_DELETE:GA_UNSUPPORTED;
        uint8_t b74=B(0x74);SB(0x74,b74-0x70u);
        if (b74<0x70u) {
            neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x63d9e,0x2a):e->spawn?e->spawn(e->ctx,a,0x63d9e,false):NULL;   /* 0040BC (reserve 0x2A), 01ACF2 (0, 0) */
            if (c) ga_place_at(c,a,e,0,0);
        }
        return GA_MORE;
    }
    case 0x63a92: case 0x643da: {
        bool big=L(0)==0x643dau;
        if (!release_id(a,e,big?8:3)) return GA_UNSUPPORTED;
        START((B(0x6e)&0x7fu)?0x2685ca:0x26858a);SL(0,big?0x6441e:0x63ad6);
    }
        /* fall through */
    case 0x63ad6: case 0x6441e: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,rocket_call);
        if (r<0 || e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    case 0xc58d2:
        SW(0x66,0xe000);SW(0x1e,0xe000);ga_palette(a,0x37c);
        if (B(0x6e)&0x40u) {
            if (B(0x6e)&0x80u) SB(0x15,B(0x15)|2u);
            START(0xc586c);
        } else START(0xc5806);
        SL(0,0xc5930);
        /* fall through */
    case 0xc5930: {
        SL(0x30,L(0x30)-(uint32_t)asr32((int32_t)L(0x30),5));
        SL(0x34,L(0x34)-(uint32_t)asr32((int32_t)L(0x34),3));
        ga_move(a,e);
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
bool neo_chaser_routine(uint32_t r) {
    return r==0x63f10 || r==0x63f32 || r==0x63aee || r==0x63b22 || r==0x63bc0 || r==0x63d60 || r==0x63d86 || r==0x63fdc || r==0x64128 || r==0x6414c || r==0x6371e || r==0x63740 || r==0x63756 ||
        r==0x63d9e || r==0x63e48 || r==0xc58d2 || r==0xc5930 ||
        r==0x6381c || r==0x638b4 || r==0x63a92 || r==0x63ad6 ||
        r==0x641ec || r==0x64284 || r==0x643da || r==0x6441e;
}
/* Attackers of the player's side (their rows may hit the boss): the shell's
 * contact row and the blast. */
bool neo_chaser_attacker(uint32_t r) {return r==0x63fdc || r==0x63740 || r==0x638b4 || r==0x64284;}
ga_result_t neo_chaser_tick(neo_gactor_t *a,neo_genv_t *e) {return chaser_tick(a,e);}
