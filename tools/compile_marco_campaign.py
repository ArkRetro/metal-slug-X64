#!/usr/bin/env python3
"""Program table of the campaign Marco (src/marco_visual.c) from an audited
Marco catalog capture (tools/mame/run_marco_catalog.py + convert_marco_programs.py
+ extract_marco_controller.py).

Every program the campaign starts is named here with the part (body/weapon)
and the input condition of the capture in which the original selection code
(0C0F32/0C0F62) chose it. The script asserts that evidence against the
capture's recorded table selections, so a role can never point at a program
the original did not select for that condition. Output: the converted command
array (graph marco_script.h) plus the native entry index of each role.
Nothing here is an executable 68000 address."""
import argparse,json
from pathlib import Path
# role, part (0 body, 1 weapon), program address, condition(s) where chosen
ROLES=[
 ('B_IDLE',0,0x24C5EA,('idle_right','stand_fire_right')),
 ('W_IDLE',1,0x24C60E,('idle_right','idle_after_cycles')),
 ('B_RUN',0,0x24DD18,('walk_right','run_cycle_right')),
 ('W_RUN',1,0x24E2C8,('run_cycle_right','run_cycle_left')),
 ('B_TURN_RUN',0,0x24EF0E,('walk_left','run_cycle_left')),
 ('B_TURN',0,0x24EF50,('stand_fire_left','idle_after_cycles')),
 ('W_FIRE',1,0x24C70A,('stand_fire_right','run_fire_right','jump_fire_left')),
 ('W_AIM_UP',1,0x24C6AA,('aim_up_right',)),
 ('W_FIRE_UP',1,0x24C7F2,('aim_up_fire_right','jump_up_fire_left')),
 ('W_FIRE_DOWN',1,0x24EABC,('jump_down_fire_left',)),
 ('B_CROUCH_IN',0,0x24ECD6,('crouch_right','crouch_left')),
 ('B_CROUCH',0,0x24ECF6,('crouch_right','crouch_left')),
 ('B_CROUCH_FIRE',0,0x24EE42,('crouch_fire_right','crouch_fire_left')),
 ('B_JUMP',0,0x24E4E0,('jump_left','jump_fire_left')),
 ('W_JUMP',1,0x24E5BC,('jump_left',)),
 ('B_RUN_JUMP',0,0x24E858,('run_jump_right','run_jump_left')),
 ('W_RUN_JUMP',1,0x24E974,('run_jump_right','run_jump_left')),
 # Grenade (0C3BCC: table 0BF998 standing, running or in the air; crouched
 # 0C2458: body 0BF098, then 0C2294 the after-throw crouch 0BEF98).
 ('W_GRENADE',1,0x24C862,('grenade_stand','grenade_jump')),
 ('B_CROUCH_GRENADE',0,0x24EF98,('grenade_crouch',)),
 ('B_CROUCH_AFTER_GRENADE',0,0x24ED44,('grenade_crouch',)),
]
# Form 1 (+93 = 1, the H: rows +0x10 of the tables), chosen in the H catalog
# capture (tools/mame/plans/marco-hgun.json, run_marco_catalog.py --give-hgun).
ROLES_H=[
 ('W1_IDLE',1,0x253402,('h_idle_right','h_idle_fwd3')),
 ('W1_RUN',1,0x255ABE,('h_walk_right','h_walk_left')),
 ('W1_JUMP',1,0x255CD6,('h_jump_left','h_jump_fire_left')),
 ('W1_RUN_JUMP',1,0x255E96,('h_run_jump_right','h_run_jump_fire_right')),
 ('W1_AIM_UP',1,0x25349E,('h_aim_up_from_idle',)),
 ('W1_AIM_HOLD',1,0x253450,('h_aim_up_right','h_aim_up_from_idle')),
 ('W1_AIM_DOWN',1,0x2534B4,('h_idle_fwd','h_idle_fwd2')),
 ('W1_BURST',1,0x2534FE,('h_stand_fire_right','h_run_fire_right','h_jump_fire_left')),
 ('W1_BURST_UP',1,0x253662,('h_aim_up_fire_right','h_jump_up_fire_left')),
 ('W1_SWEEP_FWD_UP',1,0x2537CE,('h_aim_up_fire_right','h_jump_up_fire_left')),
 ('W1_SWEEP_UP_FWD',1,0x253840,('h_fire_after_up','h_fire_fwd_reset')),
 ('B1_CROUCH_IN',0,0x256BE6,('h_crouch_right',)),
 ('B1_CROUCH',0,0x256C06,('h_crouch_right',)),
 ('B1_CROUCH_FIRE',0,0x256D52,('h_crouch_fire_right',)),
]
# Form 1 grenades and the H down programs (tools/mame/plans/
# marco-hgun-grenade.json, --give-hgun --keep-bombs), and the C (id 0C, also
# form 1: tools/mame/plans/marco-cgun.json, --give-cgun --keep-bombs; its
# fire tables 0C3C10/0C3C50/0C3C90 + 0x10 = C02D8/C0318/C0358, crouched
# C0398). Each role: [(capture option, conditions), ...].
ROLES_EXTRA=[
 ('W1_GRENADE',1,0x253B3A,[('grenade',('h_grenade_stand','h_grenade_jump','h_grenade_run')),('cgun',('c_grenade_stand','c_grenade_jump'))]),
 ('B1_CROUCH_GRENADE',0,0x25707C,[('grenade',('h_grenade_crouch',)),('cgun',('c_grenade_crouch',))]),
 ('B1_CROUCH_AFTER_GRENADE',0,0x256C54,[('grenade',('h_grenade_crouch',)),('cgun',('c_grenade_crouch',))]),
 ('W1_BURST_DOWN',1,0x2560C0,[('grenade',('h_jump_down_fire_right','h_run_jump_down_fire_right','h_jump_down_fire_left'))]),
 ('W1_SWEEP_FWD_DOWN',1,0x2561DC,[('grenade',('h_jump_down_fire_right','h_jump_down_fire_left'))]),
 ('W1C_FIRE',1,0x253A56,[('cgun',('c_stand_fire_right','c_run_fire_right','c_jump_fire_right','c_stand_fire_left'))]),
 ('W1C_FIRE_UP',1,0x253AC6,[('cgun',('c_aim_up_fire_right','c_jump_up_fire_right'))]),
 ('W1C_FIRE_DOWN',1,0x255FDE,[('cgun',('c_jump_down_fire_right',))]),
 ('B1C_CROUCH_FIRE',0,0x256EB6,[('cgun',('c_crouch_fire_right',))]),
]
# The knife (0C3B46: a fire press with +9A bit 0, a hurtbox of type 4/5 in
# reach): weapon table 0BF958 standing (form 1: row +0x10), body table
# 0BF118 crouched. tools/mame/plans/marco-knife-*.json (run_marco_catalog.py
# --plan, each cut placed where +9A bit 0 was set): marco-k1 pistol,
# marco-k2 --give-hgun. The other table 0BF918 (+9A bit 1) did not occur.
# Each role: (capture option, form, conditions).
ROLES_KNIFE=[
 ('W_KNIFE',1,0x24D486,'knife',0,('k_cut_stand_0','k_cut_stand_2')),
 ('B_CROUCH_KNIFE',0,0x2505A6,'knife',0,('k_cut_crouch_1','k_cut_crouch_3')),
 ('W1_KNIFE',1,0x255472,'knife_h',1,('k_cut_stand_0','k_cut_stand_2')),
 ('B1_CROUCH_KNIFE',0,0x258088,'knife_h',1,('k_cut_crouch_1','k_cut_crouch_3')),
]
# The parachute descent (0C15C6/0C162A): body 0BF818 hanging (weapon the
# jump's 0BFC58 24E5BC). tools/mame/plans/marco-descent.json with
# run_marco_catalog.py --first 1830 (marco-dsc0, no input).
ROLES_DESCENT=[('B_HANG',0,0x24E5AA,('d_hang',))]
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--graph',type=Path,help='converted graph directory (default CAPTURE/graph)')
    p.add_argument('--grenade-capture',type=Path,help='form-1 grenade and H down capture (ROLES_EXTRA)')
    p.add_argument('--cgun-capture',type=Path,help='C capture (ROLES_EXTRA)')
    p.add_argument('--hgun-capture',type=Path,help='the H catalog capture (form-1 roles; its programs are in the graph through convert_marco_programs.py --extra-capture)')
    p.add_argument('--knife-capture',type=Path,help='pistol knife capture (ROLES_KNIFE)')
    p.add_argument('--knife-h-capture',type=Path,help='knife capture with the H (ROLES_KNIFE)')
    p.add_argument('--descent-capture',type=Path,help='parachute descent capture (ROLES_DESCENT)')
    a=p.parse_args();c=a.capture
    plan=json.loads((c/'plan.json').read_text())['cases']
    choices=json.loads((c/'controller.json').read_text())['choices']
    gdir=a.graph or c/'graph'
    graph=json.loads((gdir/'graph.json').read_text())
    index={e['source']:e['native_index'] for e in graph['entry_seeds']}
    cases={x['name']:x for x in plan}
    lines=['/* Generated by tools/compile_marco_campaign.py: converted Marco programs',
           ' * and the campaign roles, each checked against the original selections. */',
           '#ifndef MSX_MARCO_CAMPAIGN_H','#define MSX_MARCO_CAMPAIGN_H']
    script=(gdir/'marco_script.h').read_text()
    # neo_animation.h comes from the includer (src/marco_visual.c).
    lines.append(script.replace('#include "neo_animation.h"',''))
    lines.append('enum {')
    for role,part,addr,conds in ROLES:
        for cond in conds:
            case=cases[cond]
            seen=[x for x in choices if case['first']<=x['choice']['frame']<=case['last']
                  and x['choice']['part']==part and x['choice']['program']==addr]
            assert seen,(role,hex(addr),cond)
        assert addr in index,(role,hex(addr))
        lines.append(f'    MARCO_CAMPAIGN_{role}={index[addr]}, /* ROM {addr:06X}, {"weapon" if part else "body"}: {", ".join(conds)} */')
    if a.hgun_capture:
        h=a.hgun_capture
        hplan={x['name']:x for x in json.loads((h/'plan.json').read_text())['cases']}
        hchoices=json.loads((h/'controller.json').read_text())['choices']
        for role,part,addr,conds in ROLES_H:
            for cond in conds:
                case=hplan[cond]
                seen=[x for x in hchoices if case['first']<=x['choice']['frame']<=case['last']
                      and x['choice']['part']==part and x['choice']['program']==addr and x['before']['form']==1]
                assert seen,(role,hex(addr),cond)
            assert addr in index,(role,hex(addr))
            lines.append(f'    MARCO_CAMPAIGN_{role}={index[addr]}, /* ROM {addr:06X}, {"weapon" if part else "body"}, form 1: {", ".join(conds)} */')
    extra={'grenade':a.grenade_capture,'cgun':a.cgun_capture}
    if all(extra.values()):
        loaded={k:({x['name']:x for x in json.loads((v/'plan.json').read_text())['cases']},json.loads((v/'controller.json').read_text())['choices']) for k,v in extra.items()}
        for role,part,addr,evidence in ROLES_EXTRA:
            for key,conds in evidence:
                xplan,xchoices=loaded[key]
                for cond in conds:
                    case=xplan[cond]
                    seen=[x for x in xchoices if case['first']<=x['choice']['frame']<=case['last']
                          and x['choice']['part']==part and x['choice']['program']==addr and x['before']['form']==1]
                    assert seen,(role,hex(addr),key,cond)
            assert addr in index,(role,hex(addr))
            lines.append(f'    MARCO_CAMPAIGN_{role}={index[addr]}, /* ROM {addr:06X}, {"weapon" if part else "body"}, form 1: {", ".join(c for _,cs in evidence for c in cs)} */')
    else:assert not any(extra.values()),'--grenade-capture and --cgun-capture go together'
    knife={'knife':a.knife_capture,'knife_h':a.knife_h_capture}
    if all(knife.values()):
        for role,part,addr,key,form,conds in ROLES_KNIFE:
            k=knife[key];kplan={x['name']:x for x in json.loads((k/'plan.json').read_text())['cases']}
            kchoices=json.loads((k/'controller.json').read_text())['choices']
            for cond in conds:
                case=kplan[cond]
                seen=[x for x in kchoices if case['first']<=x['choice']['frame']<=case['last']
                      and x['choice']['part']==part and x['choice']['program']==addr and x['before']['form']==form]
                assert seen,(role,hex(addr),key,cond)
            assert addr in index,(role,hex(addr))
            lines.append(f'    MARCO_CAMPAIGN_{role}={index[addr]}, /* ROM {addr:06X}, {"weapon" if part else "body"}, form {form}: {", ".join(conds)} */')
    else:assert not any(knife.values()),'--knife-capture and --knife-h-capture go together'
    if a.descent_capture:
        d=a.descent_capture;dplan={x['name']:x for x in json.loads((d/'plan.json').read_text())['cases']}
        dchoices=json.loads((d/'controller.json').read_text())['choices']
        for role,part,addr,conds in ROLES_DESCENT:
            for cond in conds:
                case=dplan[cond]
                seen=[x for x in dchoices if case['first']<=x['choice']['frame']<=case['last'] and x['choice']['part']==part and x['choice']['program']==addr]
                assert seen,(role,hex(addr),cond)
            assert addr in index,(role,hex(addr))
            lines.append(f'    MARCO_CAMPAIGN_{role}={index[addr]}, /* ROM {addr:06X}, {"weapon" if part else "body"}: {", ".join(conds)} */')
    lines+=['};','#endif','']
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text('\n'.join(lines))
    print('Marco campaign roles:',len(ROLES),'commands from',c)
if __name__=='__main__':main()
