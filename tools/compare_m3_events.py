#!/usr/bin/env python3
"""Mission 3 chain of events and actor inventory from a MAME capture.

Input: debug.log of tools/capture_m3_actors.sh (MAME 0.264, states4/
sec04_117966, frame-exact replay of run4), lines
  ALLOC f n r c cr cam   0040BC node allocation (at 00410E): node n gets
                         routine r (the constructor), creator node c (routine cr)
  FREE f n r             004126 node release
  SPAWN f n entry rec cr cam   01A132 script op 7
  OP f n a0 op cam       01A0A6 non-blocking script op
  WAIT f n a0 op cam     019F7E blocking op of the main script 101414 (on change)
  ANIM f n r p           004256 sprite program start (program p)
  SND f n r w ret        0120F8 sound request (word w)
  ITEM f n k c d         08C380 item constructor: kind +6E, creator +0C, +8A
  CAM f v cam            write of the camera list pointer 10E964
  END f pc n r cam       018F92 / 018870
(cam = camera progress 10E99E.)

Prints the event chain per section (script ops of the main script and its
runners in order, with what each op 7 created, waits and their length, random
choices and the alternative taken, camera-list calls and what they created,
mission end) and the inventory of every constructor allocated (count,
first frame/camera, creators, sprite program roots, sound words, port
status from the host helper build/host/script-native = neo_world_spawn_native).
--json writes the machine-readable summary (data/mission3_actors.json);
--markdown prints the tables of docs/MISION3_ACTORES.md. With --port LOG
(ares log of the reference cartridge, lines `MSX script spawn record R
constructor C native N camera P`) it also compares the records each side
executed, per section, like tools/compare_m2_events.py."""
import argparse,collections,json,re,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parent.parent
# Sections of the camera list 0B8968 (docs/MISION3.md "Reconocimiento del
# mapa"): a section starts when the list pointer passes the given order.
SECTION_ORDERS=((2,0x0B8A50),(3,0x0B8BB8),(4,0x0B8C1C),(5,0x0B8CAC))
SECTION_NAMES={1:'salida',2:'vagones',3:'hueco',4:'tramo final',5:'jefe'}
MAIN_SCRIPT=0x101414
PLAYER_NODE=0x1014C4    # player 1
MISSION_NODE=0x101364   # mission actor (018916 / 0186A0 loop / 018870)
LINE=re.compile(r'^(ALLOC|FREE|SPAWN|OP|WAIT|ANIM|SND|ITEM|CAM|END) (.*)$')
def fields(rest):
    out={}
    for tok in rest.split():
        if '=' in tok:
            k,v=tok.split('=',1)
            out[k]=v
    return out
def parse(path):
    ev=[]
    for line in open(path,errors='replace'):
        m=LINE.match(line.strip())
        if not m:continue
        kind,f=m.group(1),fields(m.group(2))
        e={'kind':kind,'f':int(f['f'])}
        for k,v in f.items():
            if k in ('f','cam'):continue
            e[k]=int(v,16)
        if 'cam' in f:e['cam']=int(f['cam'])
        ev.append(e)
    return ev
def script_index(path):
    """Statement address -> statement (with parent op 10 / op 11 links)."""
    d=json.loads(Path(path).read_text());idx={};parent={}
    def walk(sts,par=None):
        for st in sts:
            a=int(st['address'],16);idx[a]=st;parent[a]=par
            for k in ('alternatives','statements','body','then'):
                if k in st:
                    v=st[k];walk(v if isinstance(v,list) else [v],a)
    for sec in d['sections']:walk(sec['statements'])
    return idx,parent
def camera_list(path):
    try:return {int(o['at'],16):o for o in json.loads(Path(path).read_text())['camera_list']}
    except (OSError,KeyError,ValueError):return {}
# Constructors the port runs outside neo_world_spawn_native (other modules),
# the ones only on the unmerged branch work-m3c, and the counted externals.
PORT_MODULE={0x0B8D64:'neo_train.c',0x0607DE:'neo_end.c',0x060B06:'neo_end.c',0x060A72:'neo_end.c',
    0x0C4A58:'neo_landing.c',0x0C5286:'neo_landing.c',0x0308C0:'neo_tally.c',0x030B0A:'neo_tally.c',
    0x030B08:'neo_tally.c',0x06090A:'neo_end.c',0x01A422:'neo_script.c',
    0x040F88:'neo_world.c',0x03BC28:'neo_soldier_effects.c',   # soldier death child / deferred effect
    0x03AED8:'neo_wipe.c',0x03AEF0:'neo_wipe.c'}   # the mission actor's stage wipes (kind 4 transitions, neo_mission.c)
WORK_M3C={0x039382,0x03938A,0x0B911E,0x0B9140}
EXTERNAL={0x08C752}
def port_status(c,nat):
    if nat.get(c)==1:return 'native'
    if c in PORT_MODULE:return 'native-module:'+PORT_MODULE[c]
    if c in WORK_M3C:return 'work-m3c'
    if c in EXTERNAL:return 'external'
    return 'missing'
def family(ctor):
    """Constructor -> its root: the script / camera list / mission actor
    record it descends from ('player' for the player's, 'common' when its
    creators descend from several roots)."""
    memo={}
    def roots(c,stack):
        if c in memo:return memo[c]
        out=set()
        for k in ctor[c]['creators']:
            if k in ('script','mission','01A422'):out.add('%06X'%c)   # 01A422: a script runner
            elif k=='player' or k.startswith('pre:'):out.add('player')
            else:
                kc=int(k,16)
                if kc==c or kc in stack:continue
                out|=roots(kc,stack|{c})
        memo[c]=out or {'%06X'%c};return memo[c]
    return {c:(next(iter(r)) if len(r)==1 else 'common') for c in ctor for r in [roots(c,frozenset())]}
def native_status(ctors):
    helper=ROOT/'build/host/script-native'
    if not ctors:return {}
    # Without the helper every constructor would read as "missing" (the wrong "208 unported" inventory of 11-XI):
    # stop instead, and refuse a helper older than the world sources it was built from.
    if not helper.exists():sys.exit('compare_m3_events: %s missing (make check builds it)'%helper)
    newer=[s.name for s in (ROOT/'src').glob('*.c') if s.stat().st_mtime>helper.stat().st_mtime]
    if newer:sys.exit('compare_m3_events: %s is older than %s (rebuild it: make check)'%(helper,', '.join(sorted(newer)[:5])))
    out=subprocess.run([str(helper)]+['%x'%c for c in sorted(ctors)],capture_output=True,text=True,check=True).stdout
    return {int(a,16):int(b) for a,b in (l.split() for l in out.splitlines())}
def describe(st):
    if st is None:return '?'
    n=st['name']
    if st['op']==0:
        s='camera_wait %d'%st['camera']
        if 'lock' in st:s+=' + lock %d'%st['lock']['stop']
        return s
    if st['op'] in (2,11):
        c=st['condition'];return '%s %s %s %s'%(n,c['subject'],c['compare'],c['value'])
    if st['op']==1:return 'timer_wait %s'%st.get('frames',st.get('timer','?'))
    if st['op']==8:return 'runner period %s count %s limit %s'%(st['period'],st['count'],st['camera_limit'])
    if st['op']==7:return 'spawn %s'%st['constructor'][2:]
    return n
def build(ev,script,clist):
    idx,parent=script
    # Section boundaries from the camera list pointer.
    cam_ptr=[];sec_start={1:None}
    for e in ev:
        if e['kind']=='CAM':
            if not cam_ptr or cam_ptr[-1][1]!=e['v']:cam_ptr.append((e['f'],e['v'],e['cam']))
            for s,o in SECTION_ORDERS:
                if s not in sec_start and e['v']>=o:sec_start[s]=e['f']
    first=min(e['f'] for e in ev);sec_start[1]=first
    starts=sorted((f,s) for s,f in sec_start.items() if f is not None)
    def section(f):
        cur=1
        for sf,s in starts:
            if f>=sf:cur=s
        return cur
    # Nodes -> constructor (allocation), creator.
    node_ctor={};node_cr={};node_creator={};pre={}
    allocs=[];cam_now=None;camptr_now=None
    ctor=collections.OrderedDict()
    def info(c):
        if c not in ctor:ctor[c]={'count':0,'first_frame':None,'first_camera':None,'sections':collections.Counter(),'creators':collections.Counter(),
            'programs':collections.Counter(),'sounds':collections.Counter(),'script_records':collections.Counter(),'camera_calls':collections.Counter(),'freed':0}
        return ctor[c]
    def owner(n,r):
        if n in node_ctor:return node_ctor[n]
        pre.setdefault(n,r);return ('pre',pre[n])
    spawn_rec={}   # node -> record
    executed=set();pending=[]
    items=[]
    chain=[]       # (frame, section, camera, node, address, kind, text)
    last_op_node={}
    for e in ev:
        k=e['kind']
        if k=='CAM':
            # A watchpoint prints the pointer before the write: v is the
            # order being left, i.e. the one that ran. Mission-actor
            # allocations of this frame belong to it; others to the mission
            # actor itself (01860A: start letters, GO arrow; 018870...).
            for j,(pf,pc_,pn) in enumerate(pending):
                f0,s0,cam0,n0,_,_,_=chain[pn]
                if pf==e['f']:
                    chain[pn]=(f0,s0,cam0,n0,e['v'],'camlist','camera list order %06X creates %06X (caller %06X)'%(e['v'],pc_[0],pc_[1]))
                    info(pc_[0])['camera_calls']['%06X'%e['v']]+=1
            pending.clear();camptr_now=e['v'];continue
        if k=='ALLOC':
            c=e['r']&0xffffff;i=info(c);i['count']+=1;s=section(e['f'])
            if i['first_frame'] is None:i['first_frame'],i['first_camera']=e['f'],e['cam']
            i['sections'][s]+=1
            # 00411C (return 004122) inserts after another node: the creator
            # is the A6 it saved (o, routine or).
            cn,crr=(e['o'],0) if e.get('ret')==0x4122 else (e['c'],e['cr'])
            if cn==MISSION_NODE:crn='mission'
            elif cn==PLAYER_NODE:crn='player'
            elif cn==MAIN_SCRIPT:crn='script'
            elif cn in node_ctor:crn='%06X'%node_ctor[cn]
            else:crn='pre:%06X@%06X'%(owner(cn,crr)[1],cn)
            i['creators'][crn]+=1
            if crn=='mission':
                chain.append((e['f'],s,e['cam'],cn,e.get('ret',0),'mission','mission actor creates %06X (caller %06X)'%(c,e.get('ret',0))))
                pending.append((e['f'],(c,e.get('ret',0)),len(chain)-1))
            node_ctor[e['n']]=c;node_cr[e['n']]=crn;node_creator[e['n']]=cn;spawn_rec.pop(e['n'],None)
        elif k=='FREE':
            if e['n'] in node_ctor:info(node_ctor[e['n']])['freed']+=1
        elif k=='SPAWN':
            c=e['entry']&0xffffff;spawn_rec[e['n']]=e['rec'];executed.add(e['rec']);info(c)['script_records']['%06X'%e['rec']]+=1
            # the OP line of this record precedes it: annotate it
            for j in range(len(chain)-1,-1,-1):
                if chain[j][4]==e['rec'] and chain[j][5]=='op':
                    f,s,cam,n,a,kk,t=chain[j];chain[j]=(f,s,cam,n,a,kk,t+' -> node %06X'%e['n']);break
        elif k in ('OP','WAIT'):
            st=idx.get(e['a0']);s=section(e['f'])
            txt=describe(st)
            par=parent.get(e['a0'])
            if par is not None and idx[par]['op']==10:txt+=' (alternative of %06X)'%par
            elif par is not None and idx[par]['op']==11:txt+=' (inside if %06X)'%par
            chain.append((e['f'],s,e['cam'],e['n'],e['a0'],'wait' if k=='WAIT' else 'op',txt))
        elif k=='ANIM':
            o=owner(e['n'],e['r']);c=o if isinstance(o,int) else None
            if c is not None:info(c)['programs']['%06X'%e['p']]+=1
            else:pre_progs.setdefault(o[1],collections.Counter())['%06X'%e['p']]+=1
        elif k=='SND':
            o=owner(e['n'],e['r']);c=o if isinstance(o,int) else None
            if c is not None:info(c)['sounds']['%04X'%e['w']]+=1
            else:pre_sounds.setdefault(o[1],collections.Counter())['%04X'%e['w']]+=1
        elif k=='ITEM':
            # source: the real creator of the item node (ALLOC), else its +0C
            src=node_cr.get(e['n'])
            if src is None:src='%06X?'%e['c']
            cn=node_creator.get(e['n']);srec=spawn_rec.get(cn) if cn is not None else None
            items.append({'frame':e['f'],'section':section(e['f']),'kind':'%02X'%e['k'],'dropped':e['d']!=0,
                'source':src,'record':('%06X'%spawn_rec[e['n']]) if e['n'] in spawn_rec else None,
                'source_record':('%06X'%srec) if srec is not None else None})
        elif k=='END':
            if any(x[5]=='end' and x[4]==e['pc'] for x in chain):continue
            chain.append((e['f'],section(e['f']),e['cam'],e['n'],e['pc'],'end','mission end %06X (node routine %06X)'%(e['pc'],e['r'])))
    # Records never executed: their op 10 / op 11 parent (random branch or
    # condition not taken in this run).
    missed=[]
    for ad,st in idx.items():
        if st['op']==7 and ad not in executed:
            par=parent.get(ad);why='random %06X'%par if par is not None and idx[par]['op']==10 else ('if %06X'%par if par is not None and idx[par]['op']==11 else ('runner/block %06X'%par if par is not None else 'top level'))
            missed.append({'record':'%06X'%ad,'constructor':st['constructor'][2:],'words':st.get('words',[]),'why':why})
    return {'ctor':ctor,'chain':chain,'items':items,'starts':starts,'cam_ptr':cam_ptr,'node_cr':node_cr,'missed':sorted(missed,key=lambda m:m['record']),'executed':executed}
pre_progs={};pre_sounds={}
def markdown(r,script,nat):
    """Condensed chain: one row per blocking order of the main script (its
    frame and camera when it began, how long it held), then what ran until
    the next one: op 7 records (record -> constructor), random choices
    (alternative taken), ifs (taken or not), runners' records, camera-list
    and mission-actor creations, mission end."""
    idx,parent=script
    ch=r['chain'];rows=collections.defaultdict(list);cur=None
    def name(c):return '`%06X`'%c+('' if nat.get(c)==1 else '\\*')
    for j,(f,s,cam,n,ad,k,t) in enumerate(ch):
        if k=='wait':
            nxt=next((x[0] for x in ch[j+1:] if x[3]==MAIN_SCRIPT),None)
            st=idx.get(ad);cur=[f,s,cam,describe(st).replace('_',' '),(nxt-f) if nxt else None,[]];rows[s].append(cur);continue
        if cur is None:cur=[f,s,cam,'(inicio)',None,[]];rows[s].append(cur)
        st=idx.get(ad) if k=='op' else None
        if k=='op' and st is not None:
            if st['op']==7:
                c=int(st['constructor'],16)&0xffffff
                who='' if n==MAIN_SCRIPT else ' (corredor)'
                cur[5].append('`%06X`→%s%s'%(ad,name(c),who))
            elif st['op']==10:
                alt=next((x[4] for x in ch[j+1:j+2]),None)
                cur[5].append('azar `%06X`→`%06X`'%(ad,alt) if alt is not None else 'azar `%06X`'%ad)
            elif st['op']==11:
                taken=j+1<len(ch) and parent.get(ch[j+1][4])==ad
                c=st['condition'];cur[5].append('si %s %s %s: %s'%(c['subject'],c['compare'],c['value'],'sí' if taken else 'no'))
            elif st['op']==8:cur[5].append('corredor `%06X` (cada %s, %s veces, hasta %s)'%(ad,st['period'],st['count'],st['camera_limit']))
            elif st['op'] in (12,13,14,16,15):cur[5].append(st['name'].replace('_',' '))
        elif k=='camlist':cur[5].append('cámara `%06X`→%s'%(ad,name(int(t.split('creates ')[1][:6],16))))
        elif k=='mission':cur[5].append('misión→%s'%name(int(t.split('creates ')[1][:6],16)))
        elif k=='end':cur[5].append('**fin `%06X`**'%ad)
    for sec in sorted(rows):
        print('\n#### Sección %d (%s)\n\n| Fotograma | Cámara | Orden que espera | Fotogramas | Después |\n| --- | --- | --- | --- | --- |'%(sec,SECTION_NAMES[sec]))
        for f,s,cam,w,dur,acts in rows[sec]:
            cnt=collections.Counter(acts);seen=[]
            for x in acts:
                if x not in seen:seen.append(x)
            txt='; '.join(x+(' (×%d)'%cnt[x] if cnt[x]>1 else '') for x in seen)
            print('| %d | %d | %s | %s | %s |'%(f,cam,w,dur if dur is not None else '—',txt or '—'))
def lcs(a,b):
    n,m=len(a),len(b);t=[[0]*(m+1) for _ in range(n+1)]
    for i in range(n-1,-1,-1):
        for j in range(m-1,-1,-1):
            t[i][j]=t[i+1][j+1]+1 if a[i]==b[j] else max(t[i+1][j],t[i][j+1])
    return t[0][0]
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('log',type=Path,help='debug.log of tools/capture_m3_actors.sh')
    p.add_argument('--script',type=Path,default=ROOT/'data/mission3_script.json')
    p.add_argument('--stage',type=Path,default=ROOT/'filesystem/mission3/s1/stage.json',help='decoded camera list (private; optional)')
    p.add_argument('--json',type=Path,help='write the summary (data/mission3_actors.json)')
    p.add_argument('--markdown',action='store_true')
    p.add_argument('--chain',action='store_true',help='print the full event chain')
    p.add_argument('--list',action='store_true',help='print one line per constructor')
    p.add_argument('--port',type=Path,help='ares log of the reference cartridge')
    p.add_argument('--sections',help='another mission: "2:0E756E:name,3:..." (section, camera-list order that starts it, name) with "1:0:name" first')
    p.add_argument('--source',help='source line of --json for another mission')
    a=p.parse_args()
    if a.sections:
        global SECTION_ORDERS,SECTION_NAMES
        t=[x.split(':') for x in a.sections.split(',')]
        SECTION_ORDERS=tuple((int(n),int(o,16)) for n,o,_ in t if int(n)>1);SECTION_NAMES={int(n):name for n,_,name in t}
    ev=parse(a.log);script=script_index(a.script);clist=camera_list(a.stage)
    r=build(ev,script,clist);ctor=r['ctor']
    nat=native_status(set(ctor));fam=family(ctor)
    print('sections (start frame):',' '.join('%d@%d'%(s,f) for f,s in r['starts']))
    print('constructors allocated: %d, allocations %d, native %d'%(len(ctor),sum(i['count'] for i in ctor.values()),sum(1 for c in ctor if nat.get(c)==1)))
    if a.chain:
        for f,s,cam,n,ad,k,t in r['chain']:print('%d s%d cam %d node %06X %06X %s %s'%(f,s,cam,n,ad,k,t))
    for c,i in sorted(ctor.items(),key=lambda x:x[1]['first_frame']) if a.list else ():
        print('%06X n=%d first=%d cam=%d sec=%s creators=%s port=%s programs=%s sounds=%s'%(c,i['count'],i['first_frame'],i['first_camera'],
            ','.join('%d:%d'%x for x in sorted(i['sections'].items())),dict(i['creators']),port_status(c,nat),
            ' '.join(sorted(i['programs'])),' '.join('%s×%d'%x for x in sorted(i['sounds'].items()))))
    if a.markdown:markdown(r,script,nat)
    if a.json:
        out={'source':a.source or 'MAME 0.264 mslugx euro, tools/capture_m3_actors.sh from states4/sec04_117966 (frame-exact replay of run4) to the start of Mission 4; tools/compare_m3_events.py',
            'sections':{str(s):f for f,s in r['starts']},
            'port':'native = neo_world_spawn_native (build/host/script-native); native-module:<file> = run by another module; work-m3c = only on the unmerged branch; external = counted, not created; missing',
            'counts':dict(collections.Counter(port_status(c,nat).split(':')[0] for c in ctor),constructors=len(ctor),allocations=sum(i['count'] for i in ctor.values())),
            'constructors':{'%06X'%c:{'count':i['count'],'first_frame':i['first_frame'],'first_camera':i['first_camera'],
                'sections':{str(k):v for k,v in sorted(i['sections'].items())},'creators':dict(i['creators']),
                'script_records':sorted(i['script_records']),'camera_calls':sorted(i['camera_calls']),
                'programs':sorted(i['programs']),'sounds':sorted(i['sounds']),'family':fam[c],'port':port_status(c,nat)} for c,i in sorted(ctor.items())},
            'items':[{'kind':k,'source':src,'dropped':dr,'count':n,'sections':sorted({i['section'] for i in r['items'] if (i['kind'],i['source'],i['dropped'])==(k,src,dr)})}
                for (k,src,dr),n in sorted(collections.Counter((i['kind'],i['source'],i['dropped']) for i in r['items']).items())],
            'records_not_executed':r['missed']}
        # one line per constructor / item / record
        lines=['{']
        for key,val in out.items():
            if isinstance(val,dict) and key=='constructors':
                lines.append(' %s: {'%json.dumps(key))
                lines.append(',\n'.join('  %s: %s'%(json.dumps(k),json.dumps(v,separators=(',',':'))) for k,v in val.items()))
                lines.append(' },')
            elif isinstance(val,list):
                lines.append(' %s: ['%json.dumps(key))
                lines.append(',\n'.join('  '+json.dumps(v,separators=(',',':')) for v in val))
                lines.append(' ],')
            else:lines.append(' %s: %s,'%(json.dumps(key),json.dumps(val)))
        lines[-1]=lines[-1].rstrip(',');lines.append('}')
        txt='\n'.join(lines)
        a.json.write_text(txt+'\n')
    if a.port:
        idx,parent=script
        def slot(rec):
            par=parent.get(rec);return par if par is not None and idx[par]['op']==10 else rec
        arc=collections.defaultdict(list)
        for f,s,cam,n,ad,k,t in r['chain']:
            if k=='op' and idx.get(ad,{}).get('op')==7:arc[s].append(slot(ad))
        port=collections.defaultdict(list)
        # the port has no section pointer: attribute by the arcade's section of the record
        sec_of={slot(ad):s for f,s,cam,n,ad,k,t in r['chain'] if k=='op'}
        for line in a.port.read_text(errors='replace').splitlines():
            m=re.search(r'MSX script spawn record ([0-9a-f]+) constructor ([0-9a-f]+) native (\d) camera (\d+)',line)
            if m:
                rec=slot(int(m.group(1),16));port[sec_of.get(rec,0)].append(rec)
        for s in sorted(set(arc)|set(port)):
            A,P=arc[s],port[s]
            print('section %d: arcade %d records, port %d, in the same order %d; only arcade %s; only port %s'%(s,len(A),len(P),lcs(A,P),
                ' '.join('%06X'%x for x in sorted(set(A)-set(P))) or '-',' '.join('%06X'%x for x in sorted(set(P)-set(A))) or '-'))
if __name__=='__main__':main()
