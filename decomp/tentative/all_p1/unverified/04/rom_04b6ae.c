/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04b6ae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4b6ae(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0;
  *unaff_A6 = 0x4b6c8;
  uVar1 = 0;
  candidate_1abc8();
  candidate_4d9de();
  if ((bool)uVar1) {
    *(short *)((int)unaff_A6 + 0x86) = *(short *)((int)unaff_A6 + 0x86) + 1;
    bVar2 = *(ushort *)((int)unaff_A6 + 0x86) < 3;
    if ((short)*(ushort *)((int)unaff_A6 + 0x86) < 4) {
      uVar1 = 0;
      candidate_4256();
    }
    else {
      *(byte *)((int)unaff_A6 + 0x61) = *(byte *)((int)unaff_A6 + 0x61) | 1;
      candidate_1b04a();
      if (bVar2) {
        *unaff_A6 = 0x4b732;
      }
      else {
        *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
        *unaff_A6 = 0x4b58e;
      }
      uVar1 = 0;
    }
  }
  candidate_4d74a();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    *unaff_A6 = 0x4b732;
  }
  return;
}
