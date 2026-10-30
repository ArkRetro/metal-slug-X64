/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c0928.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_c0928(void)

{
  byte bVar1;
  short in_D0w;
  short extraout_D1w;
  byte bVar2;
  ushort uVar3;
  int extraout_A0;
  int unaff_A5;
  bool bVar4;

  bVar4 = false;
  func_0x000040ac();
  if (((bVar4) && (*(char *)(extraout_A0 + 0x93) != '\x03')) &&
     (bVar2 = (byte)extraout_D1w, bVar2 < 0x10)) {
    *(undefined1 *)(extraout_A0 + 0x95) = 0;
    bVar1 = *(byte *)(unaff_A5 + 0x471e + (int)in_D0w);
    *(byte *)(unaff_A5 + 0x471e + (int)in_D0w) = bVar2;
    *(undefined1 *)(extraout_A0 + 0x94) = *(undefined1 *)(extraout_D1w + 0xc0918);
    *(undefined1 *)(unaff_A5 + 0x471c + (int)in_D0w) = *(undefined1 *)(extraout_D1w + 0xc08a8);
    if (bVar2 == bVar1) {
      uVar3 = *(short *)(unaff_A5 + 0x4724 + (int)(short)(in_D0w * 2)) +
              *(short *)((short)(extraout_D1w * 2) + 0xc08d8);
      if (999 < uVar3) {
        uVar3 = 999;
      }
      *(ushort *)(unaff_A5 + 0x4724 + (int)(short)(in_D0w * 2)) = uVar3;
    }
    else {
      *(undefined1 *)(unaff_A5 + 0x471a + (int)in_D0w) = 0;
      *(undefined2 *)(unaff_A5 + 0x4724 + (int)(short)(in_D0w * 2)) =
           *(undefined2 *)((short)(extraout_D1w * 2) + 0xc08b8);
    }
    candidate_120f8();
    return in_D0w;
  }
  return in_D0w;
}
