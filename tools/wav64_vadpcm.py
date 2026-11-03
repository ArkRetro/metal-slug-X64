#!/usr/bin/env python3
"""Host decoder of the WAV64 VADPCM files written by audioconv64 (format 1)
and a check of a pack against its source PCM (B43).

The decoder follows libdragon tools/audioconv64/vadpcm/decode.c (the C
reference of the RSP decoder): 9-byte frames of 16 samples, control byte
(scaling << 4 | predictor), order-2 codebook, 11-bit fixed point, clamp to
16 bits; the channels' frames are interleaved. File layout: conv_wav64.c
(24-byte header, then predictors, order, 6 bytes of padding, four 16-byte
state vectors and the big-endian codebook; the frames start at the header's
start offset).

audioconv64's encoder can fail on a signal: the truck engine 10A5 (B43)
decoded with 281 of its 1157 10 ms windows off by more than the source's own
level (worst window error RMS 15799 over a source RMS of 1091), audible as
bursts. `check` measures that on every pack; usage:
  wav64_vadpcm.py FILE.wav64 SOURCE.wav   (prints the check, exit 1 if bad)"""
import struct,sys
import numpy as np
def decode(path):
    b=open(path,'rb').read()
    magic,ver,fmt,ch,bits,freq,length,loop,start=struct.unpack('>4sbbbbiiii',b[:24])
    assert magic==b'WV64' and fmt==1 and bits==16,(path,magic,fmt,bits)
    pred,order=b[24],b[25]
    cb=np.frombuffer(b[24+8+64:24+8+64+pred*order*ch*16],dtype='>i2').astype(np.int64).reshape(ch,pred,order,8)
    frames=(length+15)//16
    data=np.frombuffer(b[start:start+frames*ch*9],dtype=np.uint8).reshape(frames,ch,9)
    out=np.zeros((frames*16,ch),dtype=np.int16)
    # The C reference keeps int32 accumulators: they wrap (the 10A5 failure).
    def w32(x):return ((x+(1<<31))&0xffffffff)-(1<<31)
    def ext4(x):return np.where(x>7,x-16,x)
    for c in range(ch):
        state=np.zeros(8,dtype=np.int64);book=cb[c];d=data[:,c,:].astype(np.int64)
        ctl=d[:,0].astype(np.int64);nib=np.empty((frames,16),dtype=np.int64)
        nib[:,0::2]=ext4(d[:,1:9]>>4);nib[:,1::2]=ext4(d[:,1:9]&15)
        res=nib<<(ctl>>4)[:,None]
        for f in range(frames):
            p=book[ctl[f]&15]
            assert (ctl[f]&15)<pred,(path,f)
            for v in range(2):
                acc=np.zeros(8,dtype=np.int64)
                for k in range(order):acc=w32(acc+state[8-order+k]*p[k])
                r=w32(res[f,8*v:8*v+8]);last=p[order-1]
                for k in range(8):
                    acc[k]=w32(acc[k]+w32(r[k]<<11))
                    if k<7:acc[k+1:]=w32(acc[k+1:]+w32(r[k]*last[:7-k]))
                s=acc>>11;s=np.where((s<-0x8000)|(s>0x7fff),np.where(s<0,-0x8000,0x7fff),s)
                out[f*16+8*v:f*16+8*v+8,c]=s;state=s
    return out[:length],freq
def check(decoded,source,rate):
    """10 ms windows whose error RMS exceeds max(1000, the source's RMS)."""
    n=min(len(decoded),len(source));e=decoded[:n].astype(float)-source[:n];s=source[:n].astype(float)
    if e.ndim==1:e=e[:,None];s=s[:,None]
    win=rate//100;k=n//win
    er=np.sqrt((e[:k*win].reshape(k,win,-1)**2).mean(axis=(1,2)));sr=np.sqrt((s[:k*win].reshape(k,win,-1)**2).mean(axis=(1,2)))
    bad=int((er>np.maximum(1000,sr)).sum())
    return dict(windows=k,bad_windows=bad,worst_error_rms=round(float(er.max()),1) if k else 0.0,ok=bad==0)
if __name__=='__main__':
    import wave
    pcm,rate=decode(sys.argv[1])
    with wave.open(sys.argv[2]) as w:
        assert w.getframerate()==rate,(w.getframerate(),rate)
        src=np.frombuffer(w.readframes(w.getnframes()),dtype='<i2').reshape(-1,w.getnchannels())
    r=check(pcm,src,rate);print(r);sys.exit(0 if r['ok'] else 1)
