#!/usr/bin/env python3
"""Regression cases for the rest-of-Mission-1 tools. Synthetic bytes only:
no ROM data, original program bytes or captures are included."""
import contextlib,csv,io,json,struct,tempfile,unittest
from pathlib import Path
import decode_level_script as dls
from collision_stamp import stamp,template
from vehicle_wreckage import stamp_destroyed_vehicle,FOOTPRINT
from extract_region_collision import extract as extract_region,TEMPLATE_EVENTS
from neo_scene import state,trace_hints
from inventory_mission_rest import descriptor
from measure_camera_rest import rules

class FakeProgram(dls.Program):
    """Program with synthetic fixed/banked bytes (no ROM file)."""
    def __init__(self,fixed,bank):self.fixed=bytes(fixed);self.p2=bytes(0x100000)+bytes(bank);self.bank=1

def program(script,table):
    fixed=bytearray(0x20000);bank=bytearray(0x100000)
    for offset,entry in table.items():struct.pack_into('>I',fixed,dls.SPAWN_TABLE+offset,entry)
    bank[0xd6314:0xd6314+len(script)]=script
    return FakeProgram(fixed,bank)

def spawn(group,flags,offset,x,y,words=()):
    body=struct.pack('>BBHHH',group,flags,offset,x,y)+b''.join(struct.pack('>H',w) for w in words)
    return bytes([7,len(body)+2])+body

class LevelScriptTests(unittest.TestCase):
    def test_grammar_nesting_and_spawn_table(self):
        script=(bytes([0,6])+struct.pack('>H',1920)+b'\xff\xff'      # camera wait
                +bytes([0,14])+struct.pack('>6H',1920,1920,0,0,0,32)  # camera stop with operands
                +bytes([0xa,2])+spawn(0,0x80,0x31c,352,168)+spawn(0,0x81,0x31c,352,168)+bytes([4,2])
                +bytes([8,14])+struct.pack('>4H',0,60,2000,2)+bytes([9,0])+struct.pack('>H',0)
                +bytes([0xb,6,1,2])+struct.pack('>H',1)+spawn(9,0,0x36c,0,0,(60,))
                +bytes([2,6,9,0])+struct.pack('>H',0)+bytes([0xc,2,3,0]))
        prog=program(script,{0x31c:0x000AF17E,0x36c:0x8001913C})
        out=dls.decode(prog)
        self.assertEqual([s['name'] for s in out],['camera_wait','camera_wait','random_choice','runner','condition_wait','camera_unlock','end'])
        self.assertNotIn('lock',out[0]);self.assertEqual(out[1]['lock'],dict(stop=1920,params=[0,0,0,32]))
        alts=out[2]['alternatives'];self.assertEqual([a['variant'] for a in alts],[0,1])
        self.assertTrue(alts[0]['x_screen_relative']);self.assertEqual(alts[0]['constructor'],'0x0AF17E')
        body=out[3]['body'];self.assertEqual(body['name'],'if')
        self.assertEqual(body['condition'],dict(type=1,compare='<',value=1,subject='group1'))
        self.assertEqual(body['then']['constructor'],'0x01913C');self.assertFalse(body['then']['group_counted'])
        self.assertEqual(out[4]['condition']['subject'],'groups0..7')
        self.assertEqual(dls.blocking_addresses(out),[0x2D6314,0x2D631A,0x2D6360,0x2D6368])
    def test_unknown_op_fails(self):
        with self.assertRaisesRegex(AssertionError,'Unknown script op'):
            dls.decode(program(bytes([0x12,2,3,0]),{}))
    def test_unterminated_block_does_not_skip(self):
        # A non-blocking statement at top level is legal; a blocking one inside
        # a random choice is not a statement the walker 01A272 accepts.
        prog=program(bytes([0xa,2,0,6])+struct.pack('>H',5)+b'\xff\xff'+bytes([4,2,3,0]),{})
        out=dls.decode(prog);self.assertEqual(out[0]['alternatives'][0]['name'],'camera_wait')

def ram_with_layer(layer=0x10d7b6,camera=2240):
    ram=bytearray(65536);base=layer-0x100000
    struct.pack_into('>2i',ram,base+0x14,-camera*65536,256*65536)
    struct.pack_into('>2i',ram,0xc72e+0x14,-camera*65536,256*65536)
    struct.pack_into('>2i',ram,0xd7b6+0x14,-camera*65536,256*65536)
    struct.pack_into('>H',ram,0x2036,camera)
    return ram

def raw_template(dx,dy,w,h,material,mask,cells):
    return struct.pack('>bbBBBBI',dx,dy,w,h,material,0,mask)+bytes(cells)

class StampTests(unittest.TestCase):
    def test_generic_stamp_reproduces_the_vehicle_model(self):
        rows=FOOTPRINT['rows_high_to_low'];w,h=FOOTPRINT['width'],FOOTPRINT['height']
        t=template(raw_template(FOOTPRINT['anchor_x_cells'],FOOTPRINT['anchor_y_cells'],w,h,0,0,[c for r in rows for c in r]),0)
        for x in (937,1174,1180):
            ram=bytearray(65536);struct.pack_into('>i',ram,0xc742,-872*65536);struct.pack_into('>i',ram,0xc746,256*65536)
            ram[0xc7b6+((x>>3)-1&63)*64+(~8&63)]=0xcb  # A solid cell under the stamp survives (mask 0).
            a,_=stamp(bytes(ram),0x10c72e,t,x,64);b,_=stamp_destroyed_vehicle(bytes(ram),x*65536,64*65536,0,1)
            self.assertEqual(a,b)
    def test_mask_material_and_window(self):
        ram=ram_with_layer();grid=0xd83e
        ram[grid+((2600>>3)&63)*64+(~8&63)]=0x2b   # shape 11 with high bits
        t=template(raw_template(0,0,1,2,1,1<<11,[15,15]),0)
        out,w=stamp(bytes(ram),0x10d7b6,t,2600,64)
        self.assertEqual(out[grid+((2600>>3)&63)*64+(~8&63)],0x3f)  # bit 4 set, bits 5..7 kept
        self.assertEqual(len(w),2)
        t=template(raw_template(0,0,1,1,0,0,[15]),0)
        out,w=stamp(bytes(ram),0x10d7b6,t,2600,64)
        self.assertEqual(w,[]);self.assertEqual(out,bytes(ram))     # protected shape, mask 0
        out,w=stamp(bytes(ram),0x10d7b6,t,2240+520,64)               # beyond the 64-column window
        self.assertEqual(w,[])
    def test_template_cells_above_15_trap(self):
        with self.assertRaisesRegex(AssertionError,'traps'):template(raw_template(0,0,1,1,0,0,[16]),0)

class TracedStampTests(unittest.TestCase):
    def setUp(self):
        self.temp=tempfile.TemporaryDirectory();self.addCleanup(self.temp.cleanup);self.root=Path(self.temp.name)/'run'
        self.root.mkdir()
        # Synthetic templates at the registered addresses.
        prog=bytearray(0xc0000)
        raise_t,raise_w=TEMPLATE_EVENTS['building_raise']['templates']
        clear_r,clear_w=TEMPLATE_EVENTS['building_clear']['templates']
        prog[raise_t:raise_t+26]=raw_template(-6,11,16,1,1,0xffffffff,[11]*16)
        prog[raise_w:raise_w+22]=raw_template(-7,0,1,12,1,0xffffffff,[15]*12)
        prog[clear_r:clear_r+26]=raw_template(-6,11,16,1,1,0xffffffff,[0]*16)
        prog[clear_w:clear_w+30]=raw_template(-7,0,1,20,1,0xffffffff,[0]*20)
        self.prog=bytes(prog)
    def write_run(self,tamper=False):
        def floor_at(camera):
            r=ram_with_layer(camera=camera)
            for x in range(max(0,camera-64),camera+448,8):r[0xd83e+((x>>3)&63)*64+(~7&63)]=11
            return r
        snaps={i+1:bytes(floor_at(c)) for i,c in enumerate(range(0,2240,320))}  # Earlier views.
        floor=floor_at(2240);snaps[7140]=bytes(floor)
        temps=[template(self.prog,a) for a in TEMPLATE_EVENTS['building_raise']['templates']]
        raised=bytes(floor);writes=[]
        for t in temps:raised,w=stamp(raised,0x10d7b6,t,2632,64);writes+=[(7163,x) for x in w]
        snaps[7170]=raised
        cleared=raised
        for a in TEMPLATE_EVENTS['building_clear']['templates']:
            cleared,w=stamp(cleared,0x10d7b6,template(self.prog,a),2632,64);writes+=[(7312,x) for x in w]
        later=bytearray(cleared)  # The view then advances over the cleared building.
        for base in (0xc72e,0xd7b6):struct.pack_into('>i',later,base+0x14,-2420*65536)
        struct.pack_into('>H',later,0x2036,2420);snaps[7320]=bytes(later)
        if tamper:writes[0]=(7163,dict(writes[0][1],new=writes[0][1]['new']^1))
        for f,r in snaps.items():(self.root/f'ram{f}.bin').write_bytes(r)
        with (self.root/'events.csv').open('w') as e:
            e.write('frame,kind,address,old,new,pc,camera\n')
            for f,w in writes:e.write(f"{f},grid,{w['address']:x},{w['old']},{w['new']},c8982,2240\n")
        with (self.root/'layers.csv').open('w') as l:
            l.write('frame,object,header\n')
            for f in (7163,7312):l.write(f'{f},10d7b6,{floor[0xd7b6:0xd7b6+0x88].hex()}\n')
    def test_traced_raise_and_clear_keep_pre_event_terrain(self):
        self.write_run()
        report=extract_region([self.root],self.prog)
        self.assertEqual([e['kind'] for e in report['events']],['building_raise','building_clear'])
        self.assertEqual(report['events'][0]['anchor_cell'],[329,8])
        data=bytes.fromhex(report['cells_hex'])
        self.assertEqual(data[322*32+8],0)   # Wall cell: static empty, raise 0x1F, clear 0x10.
        self.assertEqual(data[322*32+7],11)  # Untouched floor.
    def test_traced_write_mismatch_fails(self):
        self.write_run(tamper=True)
        with self.assertRaisesRegex(AssertionError,'Unexplained or ambiguous traced stamp'):extract_region([self.root],self.prog)
    def test_traced_writes_need_the_templates(self):
        self.write_run()
        with self.assertRaisesRegex(AssertionError,'need --rom'):extract_region([self.root])

class SceneHintTests(unittest.TestCase):
    def test_trace_hint_resolves_vertical_wrap(self):
        v=[0]*34304;ram=bytearray(65536)
        for addr,y,x in ((0xc72e,218,-1920),(0xd7b6,218,-1920),(0xe83e,242,-960)):
            struct.pack_into('>2i',ram,addr+0x14,x*65536,y*65536);struct.pack_into('>2h',ram,addr+0x84,x,y)
        struct.pack_into('>2i',ram,0xd7b6+0x14,-1920*65536,218*65536);struct.pack_into('>2i',ram,0xc72e+0x14,-1920*65536,218*65536)
        slot=106;v[0x8000+slot]=0xfff;v[0x8200+slot]=(((512-(16+26))&511)<<7)|1;v[0x8400+slot]=((slot-105)*16-1920)%512<<7;v[slot*64+1]=0x1140
        with self.assertRaises(AssertionError):state(v,ram,[set()]*4)   # no traced candidate
        trace={6900:dict(sky_x='-960',sky_y='242',mid_x='-1920',mid_y='218',fore_x='-1920',fore_y='218'),
               6899:dict(sky_x='-960',sky_y='254',mid_x='-1920',mid_y='230',fore_x='-1920',fore_y='230')}
        s=state(v,ram,trace_hints(trace,6900))
        self.assertEqual(s['vertical_offsets'][3],26)   # Without the trace 42 would be chosen.

class InventoryAndCameraTests(unittest.TestCase):
    def test_descriptor_box_and_tiles(self):
        data=bytes([2,0x44])+struct.pack('>hhBB',-8,16,1,2)+struct.pack('>2H',1,2)+struct.pack('>hhBB',8,0,1,1)+struct.pack('>H',1)
        d=descriptor(data,0)
        self.assertEqual(d['tiles'],2);self.assertEqual(d['box'],[-8,-16,32,32]);self.assertTrue(d['auto_animation'])
        self.assertEqual(d['tile_codes'],[0x40001,0x40002])
    def test_camera_rules_are_fixed_formulas(self):
        row=dict(frame='10',camera='3376',camera_word='3376',fore_x='-3520',fore_y='240',mid_x='-3520',mid_y='240',sky_x='-1760',sky_y='256',
            obj4_x='-3520',cam_x=str(3520<<16),cam_y=str(16<<16),shake='0',lock=str(3376<<16),lock_params='56 -32768 144 -32768')
        r=rules(row,row)
        self.assertTrue(all(v for v in r.values() if v is not None))
        r=rules(dict(row,sky_x='-1761'),row);self.assertFalse(r['sky_x == (X+1)//2'])
        r=rules(dict(row,fore_x='-3530',mid_x='-3530',obj4_x='-3530',sky_x='-1765',cam_x=str(3530<<16)),row)
        self.assertFalse(r['P+op1 <= X <= P+op3 with stop operands'])

if __name__=='__main__':unittest.main()
