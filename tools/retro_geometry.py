"""Shared offline depth field for technical maps and the N64 mesh lookup."""
import math
from functools import lru_cache

class DepthField:
    def __init__(self,author,grid):
        self.author=author;self.width=grid['columns']*8
        self.cells=bytes.fromhex(grid['cells_hex'])
    @lru_cache(maxsize=8192)
    def ground(self,x):
        if not 0<=x<self.width:return 192.0
        heights=[];u=x%8
        for row in range(32):
            shape=self.cells[(int(x)//8)*32+row]&15;base=row*8
            if 1<=shape<=4:h=base+u/4+(shape-1)*2
            elif 6<=shape<=9:h=base+8-u/4-(shape-6)*2
            elif shape==5:h=base+u
            elif shape==10:h=base+8-u
            elif 11<=shape<=13:h=base+8
            else:continue
            heights.append(h)
        return 248-min(heights,key=lambda h:abs(h-64)) if heights else 192.0
    def sample(self,x,y,layer):
        z=self.author['layer_depths'][layer];dx=dy=0
        if layer>=3:
            ground=self.ground(x)
            if y>=ground:
                span=232-ground;portion=max(0,min(1,(y-ground)/span));z=-6*portion
                slope=(self.ground(x+1)-self.ground(x-1))*.5
                dy=-6/span;dx=6*(232-y)/(span*span)*slope
            for p in self.author['planes']:
                l,t,r,b=p['rect']
                if l<=x<r and t<=y<b:z=p['depth']+(x-l)*p['dx']+(y-t)*p['dy'];dx=p['dx'];dy=p['dy']
        inverse=1/math.sqrt(dx*dx+dy*dy+1)
        return z,(dx*inverse,dy*inverse,inverse)
