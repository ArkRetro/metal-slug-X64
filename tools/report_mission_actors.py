#!/usr/bin/env python3
"""Actor inventory tables of a mission (docs/MISION5_ACTORES.md) from
data/missionN_actors.json (tools/compare_m3_events.py --json) and
data/missionN_script.json. Prints Markdown: the summary by port status, the
constructors that are not ported (by family and by section, in camera order),
the native ones, the script's constructors by state, the items and the
script records the capture did not run. --previous names the inventories of
earlier missions: a missing constructor already listed there is marked.

usage: report_mission_actors.py N --names 1:calle,2:metro,... [--previous data/mission4_actors.json ...]"""
import argparse,json
from collections import Counter,defaultdict
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('mission',type=int);p.add_argument('--names',required=True)
    p.add_argument('--previous',nargs='*',type=Path,default=[])
    a=p.parse_args();d=json.loads((ROOT/f'data/mission{a.mission}_actors.json').read_text());c=d['constructors']
    names={int(k):v for k,v in (x.split(':') for x in a.names.split(','))}
    before={}
    for q in a.previous:
        for k,v in json.loads(q.read_text())['constructors'].items():
            if v['port']=='missing':before.setdefault(k,q.stem.replace('_actors','').replace('mission','M'))
    kind=lambda v:v['port'].split(':')[0]
    tot=Counter();n=Counter()
    for k,v in c.items():tot[kind(v)]+=v['count'];n[kind(v)]+=1
    print('## Resumen\n\n| | Constructores | Creaciones |\n| --- | --- | --- |')
    for key,label in (('native','nativos (compartidos con misiones anteriores)'),('native-module','nativos en otro módulo (fin de misión, aterrizaje, guion, efectos)'),
                      ('work-m3c','solo en una rama sin fusionar'),('missing','**nuevos, sin portar**'),('external','externos (contados, no creados)')):
        if n[key]:print('| %s | %d | %d |'%(label,n[key],tot[key]))
    print('| total | %d | %d |\n'%(len(c),sum(tot.values())))
    miss={k:v for k,v in c.items() if kind(v)=='missing'}
    shared=[k for k in miss if k in before]
    print('De los %d sin portar, %d ya estaban en el inventario de una misión anterior (los porta su fase 2: %s) y **%d son propios de esta misión**.\n'%(
        len(miss),len(shared),', '.join(sorted(set(before[k] for k in shared))) or '—',len(miss)-len(shared)))
    print('Secciones (fotograma de `run4` en que empieza cada una): '+', '.join('%s %s desde %d'%(s,names[int(s)],f) for s,f in sorted(d['sections'].items()))+'.\n')
    print('Registros del guion que `run4` no ejecutó (ramas `si`/azar; a capturar en la fase 2): %d.\n'%len(d['records_not_executed']))
    row=lambda k,v:'| `%s`%s | %d | %d | %d | %s | %s | %d | %s | %s |'%(k,' ('+before[k]+')' if k in before and kind(v)=='missing' else '',v['count'],v['first_frame'],v['first_camera'],
        ' '.join('%s:%d'%x for x in sorted(v['sections'].items())),', '.join('`%s`×%d'%x for x in sorted(v['creators'].items(),key=lambda x:-x[1])[:3]),
        len(v['programs']),' '.join(v['sounds'][:4]),v['family'])
    head='| Constructor | Creaciones | Primer fotograma | Cámara | Por sección | Creadores | Programas | Sonidos | Familia (raíz) |\n| --- | --- | --- | --- | --- | --- | --- | --- | --- |'
    print('## Nuevos (sin portar): %d constructores, %d creaciones\n'%(len(miss),tot['missing']))
    print('Por familia (raíz = registro del guion, llamada de la lista de cámara o actor de misión del que descienden), en orden de primera aparición:\n')
    fam=defaultdict(list)
    for k,v in sorted(miss.items(),key=lambda x:x[1]['first_frame']):fam[v['family']].append(k)
    for f,ks in sorted(fam.items(),key=lambda x:min(miss[k]['first_frame'] for k in x[1])):
        first=min(ks,key=lambda k:miss[k]['first_frame'])
        print('- `%s` (%d): %s; sección %s, primera cámara %d, %d creaciones'%(f,len(ks),' '.join('`%s`'%k for k in ks),min(miss[first]['sections']),miss[first]['first_camera'],sum(miss[k]['count'] for k in ks)))
    for s in sorted(d['sections']):
        ks=[k for k,v in miss.items() if min(v['sections'])==s]
        if not ks:continue
        print('\n### Sección %s (%s): %d constructores nuevos que aparecen aquí por primera vez, por cámara\n\n%s'%(s,names[int(s)],len(ks),head))
        for k in sorted(ks,key=lambda k:(miss[k]['first_frame'],k)):print(row(k,miss[k]))
    nat={k:v for k,v in c.items() if kind(v)=='native'}
    print('\n## Nativos ya portados (compartidos): %d constructores, %d creaciones\n'%(len(nat),tot['native']))
    direct={k:v for k,v in nat.items() if v['script_records'] or v['camera_calls'] or any(x in v['creators'] for x in ('script','mission','01A422'))}
    print('Los que crea directamente el guion, un corredor del guion o el actor de misión:\n\n'+head)
    for k,v in sorted(direct.items(),key=lambda x:x[1]['first_frame']):print(row(k,v))
    print('\nEl resto (hijos, disparos, efectos, premios): '+' '.join('`%s`'%k for k,v in sorted(nat.items(),key=lambda x:x[1]['first_frame']) if k not in direct)+'\n')
    other=['`%s` (%s)'%(k,v['port'].split(':')[1]) for k,v in sorted(c.items()) if kind(v)=='native-module']
    ext=['`%s`'%k for k,v in sorted(c.items()) if kind(v)=='external']
    print('En otro módulo: '+', '.join(other)+'.'+(' Externo: '+', '.join(ext)+'.' if ext else '')+'\n')
    script=json.loads((ROOT/f'data/mission{a.mission}_script.json').read_text())
    print('## Constructores del guion por estado\n\n| Constructor del guion | Estado | Creaciones en `run4` | Secciones |\n| --- | --- | --- | --- |')
    never=[]
    for k in sorted({x[2:].upper().zfill(6) for x in script['constructors']}):
        v=c.get(k)
        if v is None:never.append(k);continue
        print('| `%s` | %s | %d | %s |'%(k,v['port']+(' ('+before[k]+')' if k in before and kind(v)=='missing' else ''),v['count'],' '.join(sorted(v['sections']))))
    print('\nConstructores del guion que `run4` no creó nunca (solo en ramas no ejecutadas): '+(' '.join('`%s`'%k for k in never) or 'ninguno')+'.\n')
    print('## Premios (`08C380`)\n\n| Tipo `+6E` | Origen | Soltado | Veces | Secciones |\n| --- | --- | --- | --- | --- |')
    for i in d['items']:print('| %s | `%s` | %s | %d | %s |'%(i['kind'],i['source'],'sí' if i['dropped'] else 'no',i['count'],' '.join(map(str,i['sections']))))
    print('\n## Registros no ejecutados (%d)\n\n| Registro | Constructor | Por qué |\n| --- | --- | --- |'%len(d['records_not_executed']))
    for r in d['records_not_executed']:print('| `%s` | `%s` | %s |'%(r['record'],r['constructor'],r['why']))
if __name__=='__main__':main()
