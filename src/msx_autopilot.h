/* Reference build only (included by gameplay.c): the route planner that
 * drives the test's player through Mission 2 stage 3 (docs/MISION2.md,
 * "Fase 2f"). The stage's climbs (ledges 48 px above, one-way platforms,
 * jumps across 64-px gaps, the tower's random pieces) are beyond a blind
 * pad. Each plan reads the player's floors of the collision the player
 * sees (ring included) in a window around it, links them by walks and
 * jumps (the port's jump: 56 px up, about 90 px across) and walks to the
 * jump of the path towards the camera list's goal: the op-4 rectangle it
 * waits for, else the direction of the path mode. Not a game feature: the
 * test turns it on (Start in stage 3). */
#if MSX_REFERENCE_INVULNERABLE
enum { AP_COLS=80, AP_ROWS=72, AP_PER=8, AP_NODES=AP_COLS*AP_PER };
static int ap_reach(int dx,int dy) {
    if (dx<0) dx=-dx;
    if (dy>50) return 0;
    if (dy>40) return dx<=40;
    if (dy>32) return dx<=40;
    if (dy>16) return dx<=64;
    if (dy>=-8) return dx<=72;
    return dx<=72+(-dy)/2;
}
/* The camera list's next wait for a player: an op 4 (0C84BE) at the
 * current command or after progress waits (op 0) and settings, read ahead
 * so the walk does not run the camera past the ledge the rectangle needs
 * (stage 3, progress 0x180: the floor leads left under it). Its rectangle
 * is in layer pixels: the layer's +14/+18 cancel the root's offsets. */
static bool ap_rect(const neo_camera_t *c,int r[4]) {
    unsigned a=c->command;
    for (unsigned i=0;i<12 && c->list && a+4u<=c->list_size;i++) {
        unsigned op=c->list[a]<<8|c->list[a+1],n=c->list[a+2]<<8|c->list[a+3];
        if (op==4 && a+16u<=c->list_size) {
            for (unsigned k=0;k<4;k++) r[k]=(int16_t)(c->list[a+8+2*k]<<8|c->list[a+9+2*k]);
            return true;
        }
        if ((op!=0 && op<0x0c) || op==0x14 || n<4) return false;
        a+=n;
    }
    return false;
}
static int ap_score(const msx_game_t *g,const int *rect,int x,int y,bool *inside) {
    *inside=false;
    if (rect) {
        int d=(x<rect[0]+8?rect[0]+8-x:x>rect[0]+rect[2]-8?x-(rect[0]+rect[2]-8):0)+(y<rect[1]+4?rect[1]+4-y:y>rect[1]+rect[3]-4?y-(rect[1]+rect[3]-4):0);
        *inside=!d;
        int m=g->world.camera.mode==1?-x:g->world.camera.mode==2?y:x;
        return 8192-8*d+m/4;
    }
    switch (g->world.camera.mode) {
    case 1: return -x;
    case 2: return 2*y;
    case 3: return -2*y;
    case 5: return x+2*y;
    case 7: return x-2*y;
    default: return x;
    }
}
static void ap_step(msx_game_t *g,int *direction,bool *jump) {
    /* On the stack (3.3 KB): no program-image RAM (docs/RENDIMIENTO.md). */
    int16_t ap_h[AP_NODES],ap_prev[AP_NODES];uint8_t ap_cost[AP_NODES],ap_n[AP_COLS];
    if (!g->pilot.on || !g->world_enabled || g->life.phase!=MARCO_ALIVE || g->complete || g->world.ga_flags.player_handlers) return;
    int X=g->x>>16,Y=(int)((56*MSX_UNIT-g->y)>>16);
    *jump=false;
    if (X==g->pilot.last_x) {if (g->pilot.still<0xffff) g->pilot.still++;} else g->pilot.still=0;
    g->pilot.last_x=(uint16_t)X;
    /* The boss 026254 (neo_m2boss.c) rises behind the climb and the camera
     * with it: keep climbing, with Down held in the air (the downward
     * shot) while the pad fires. */
    bool boss=false;
    for (unsigned i=0;i<NEO_WORLD_GACTORS && !boss;i++) boss=g->world.gactors[i].active && neo_m2boss_routine(ga_l(&g->world.gactors[i],0));
    if (boss) g->crouch=!g->grounded;
    if (!g->grounded) {*direction=Y>=g->pilot.air_y?g->pilot.air_dir:g->pilot.pre_dir;g->pilot.kind=0;return;}
    /* A plan holds for 16 steps (one plan costs milliseconds on the N64). */
    if (g->pilot.kind && (uint16_t)(g->ticks-g->pilot.plan_tick)<16u) {
        int tx=g->pilot.kind==2?g->pilot.take_x:g->pilot.goal_x;
        if (g->pilot.kind==1 || abs(X-tx)>3) {*direction=X<tx-2?1:X>tx+2?-1:0;return;}
        g->pilot.air_dir=g->pilot.jdir;g->pilot.pre_dir=g->pilot.jpre;g->pilot.air_y=g->pilot.jair_y;
        *direction=g->pilot.pre_dir;*jump=true;g->pilot.jumps++;g->pilot.kind=0;g->pilot.why=1;g->pilot.from_x=(int16_t)X;g->pilot.from_y=(int16_t)Y;return;
    }
    g->pilot.kind=0;g->pilot.plan_tick=(uint16_t)g->ticks;
    /* The floors: columns around the player, rows from 24 below it. */
    int c0=X/8-AP_COLS/2,r0=Y/8-24,cam=g->camera,ry=g->world.camera_native?(int16_t)(uint16_t)((uint32_t)neo_camera_root_y(&g->world.camera)>>16):0;
    int self=-1,best=-1,best_score=-0x7fffffff;
    for (int i=0;i<AP_COLS;i++) {
        ap_n[i]=0;int x=(c0+i)*8+4;
        if (x<cam+28 || x>cam+292) continue;          /* the screen's player box */
        for (int r=r0;r<r0+AP_ROWS && ap_n[i]<AP_PER;r++) {
            neo_surface_t s;
            if (r<0 || !neo_player_surface(x*MSX_UNIT,r,&s)) continue;
            int h=s.height>>16;
            if (h+ry<0x108) continue;                  /* below the screen: a fall */
            if (neo_collision_cell(x,h+8) && (neo_collision_cell(x,h+8)&15)<11) continue;   /* a slope above: no room */
            int n=i*AP_PER+ap_n[i]++;ap_h[n]=(int16_t)h;ap_prev[n]=-1;ap_cost[n]=255;
            if (i==X/8-c0 && (self<0 || abs(h-Y)<abs(ap_h[self]-Y))) self=n;
        }
        /* Mission 3: the platform 0B9286 over the gap (dynamic map 0E6896)
         * is a floor too, where it is now (it moves 1 px per update). */
        uint8_t obj;int32_t top;
        if (g->flow.mission==2u && ap_n[i]<AP_PER &&
            platform_floor(g,x*MSX_UNIT,(r0+AP_ROWS)*8*MSX_UNIT,-AP_ROWS*8*MSX_UNIT,&obj,&top) && obj!=NEO_KOBJ_TRAIN) {
            int h=top>>16;
            if (h+ry>=0x108) {
                int n=i*AP_PER+ap_n[i]++;ap_h[n]=(int16_t)h;ap_prev[n]=-1;ap_cost[n]=255;
                if (i==X/8-c0 && (self<0 || abs(h-Y)<abs(ap_h[self]-Y))) self=n;
            }
        }
    }
    if (self<0 || abs(ap_h[self]-Y)>8) {
        *direction=g->pilot.pre_dir?g->pilot.pre_dir:1;
        if (g->pilot.still>40) *jump=(g->ticks&8)!=0;
        if (*jump) {g->pilot.why=2;g->pilot.from_x=(int16_t)X;g->pilot.from_y=(int16_t)Y;g->pilot.to_x=g->pilot.to_y=0;}
        return;
    }
    /* Cheapest paths (walk 1, jump 3) by cost levels. */
    /* Dial's buckets: the count of nodes per cost skips empty levels. */
    uint16_t at_level[256];memset(at_level,0,sizeof at_level);
    ap_cost[self]=0;at_level[0]=1;int pending=1;
    for (int level=0;level<=250 && pending>0;level++) {
        if (!at_level[level]) continue;
        for (int a=0;a<AP_COLS;a++) for (int k=0;k<ap_n[a];k++) {
            int na=a*AP_PER+k;if (ap_cost[na]!=level) continue;
            pending--;
            for (int b=a-11;b<=a+11;b++) {
                if (b<0 || b>=AP_COLS || b==a) continue;
                for (int m=0;m<ap_n[b];m++) {
                    int nb=b*AP_PER+m,dy=ap_h[nb]-ap_h[na];
                    int step=(abs(b-a)==1 && abs(dy)<=9)?1:ap_reach(8*(b-a),dy)?4+abs(b-a):0;
                    if (!step || level+step>=ap_cost[nb] || level+step>250) continue;
                    if (ap_cost[nb]!=255) at_level[ap_cost[nb]]--; else pending++;
                    ap_cost[nb]=(uint8_t)(level+step);ap_prev[nb]=(int16_t)na;at_level[level+step]++;
                }
            }
        }
    }
    int rect[4];bool has_rect=g->world.camera_native && ap_rect(&g->world.camera,rect);
    bool best_inside=false;
    for (int a=0;a<AP_COLS;a++) for (int k=0;k<ap_n[a];k++) {
        int n=a*AP_PER+k;if (ap_cost[n]==255) continue;
        bool inside;int s=ap_score(g,has_rect?rect:NULL,(c0+a)*8+4,ap_h[n],&inside)-ap_cost[n];
        if (s>best_score) {best_score=s;best=n;best_inside=inside;}
    }
    g->pilot.plans++;
    if (best<0 || best==self) {
        *direction=0;
        if (!best_inside && g->pilot.still>90) {
            *direction=(g->ticks&256)?1:-1;*jump=(g->ticks&16)!=0;
            if (*jump) {g->pilot.why=3;g->pilot.from_x=(int16_t)X;g->pilot.from_y=(int16_t)Y;g->pilot.to_x=g->pilot.to_y=0;}
        }
        return;
    }
    /* The first jump of the path: walk to its take-off, then jump. */
    int jfrom=-1,jto=-1;
    for (int n=best;ap_prev[n]>=0;n=ap_prev[n]) {
        int p=ap_prev[n],dx=n/AP_PER-p/AP_PER,dy=ap_h[n]-ap_h[p];
        if (!(abs(dx)==1 && abs(dy)<=9)) {jfrom=p;jto=n;}
    }
    int gx=(c0+best/AP_PER)*8+4;
    g->pilot.goal_x=(int16_t)gx;g->pilot.goal_y=ap_h[best];
    if (jfrom<0) {g->pilot.kind=1;*direction=X<gx-2?1:X>gx+2?-1:0;return;}
    int tx=(c0+jfrom/AP_PER)*8+4,lx=(c0+jto/AP_PER)*8+4,dy=ap_h[jto]-ap_h[jfrom];
    /* Jump: up a ledge close above, rise first; else the whole way across. */
    int dir=lx>tx+2?1:lx<tx-2?-1:0;
    g->pilot.take_x=(int16_t)tx;g->pilot.kind=2;g->pilot.jdir=(int8_t)dir;g->pilot.to_x=(int16_t)lx;g->pilot.to_y=ap_h[jto];
    g->pilot.jpre=(int8_t)(dy>24 && abs(lx-tx)<=40?0:dir);g->pilot.jair_y=(int16_t)(ap_h[jto]+4);
    if (abs(X-tx)>3) {*direction=X<tx?1:-1;return;}
    g->pilot.air_dir=g->pilot.jdir;g->pilot.pre_dir=g->pilot.jpre;g->pilot.air_y=g->pilot.jair_y;g->pilot.kind=0;
    *direction=g->pilot.pre_dir;*jump=true;g->pilot.jumps++;g->pilot.why=1;g->pilot.from_x=(int16_t)X;g->pilot.from_y=(int16_t)Y;
}
/* Blocked on the ground (a soldier's solid body 0E1B4E at an edge): jump
 * the way it walks. */
void msx_game_autopilot(msx_game_t *g,int *direction,bool *jump) {
    bool on=g->pilot.on && g->world_enabled && g->life.phase==MARCO_ALIVE && !g->complete,ground=g->grounded;
    ap_step(g,direction,jump);
    if (on && ground && *direction && !*jump && g->pilot.still>40) {
        /* Straight up, across near the top: over the body, not past the
         * ledge (stage 3's stair at x 415: a long jump left fell to the
         * floor under the camera, which never comes back). */
        *jump=true;g->pilot.air_dir=(int8_t)*direction;g->pilot.pre_dir=0;
        g->pilot.why=4;g->pilot.from_x=(int16_t)((g->x)>>16);g->pilot.from_y=(int16_t)((56*MSX_UNIT-g->y)>>16);g->pilot.to_x=g->pilot.to_y=0;
        g->pilot.air_y=(int16_t)(((56*MSX_UNIT-g->y)>>16)+40);g->pilot.still=0;g->pilot.kind=0;*direction=0;
    }
}
#else
void msx_game_autopilot(msx_game_t *g,int *direction,bool *jump) {(void)g;(void)direction;(void)jump;}
#endif
