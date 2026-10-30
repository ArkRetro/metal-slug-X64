/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0729ac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_729ac(void)

{
  byte bVar1;
  undefined2 uVar3;
  uint uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

  *(undefined1 *)((int)unaff_A6 + 0x7a) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x7b) = 0x7f;
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0x1e;
  *unaff_A6 = 0x729c8;
  uVar2 = candidate_72c66();
  if ((uVar2 & 0x20) == 0) {
    if ((uVar2 & 0x10) != 0) {
      *(undefined1 *)((int)unaff_A6 + 0x7a) = 3;
    }
    candidate_72cac();
    candidate_72d52();
    candidate_72bf2();
    candidate_72bfc();
    if (*(short *)((int)unaff_A6 + 0x72) != 0) {
      *(short *)((int)unaff_A6 + 0x72) = *(short *)((int)unaff_A6 + 0x72) + -1;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    }
    bVar1 = *(byte *)((int)unaff_A6 + 0x5f);
    *(byte *)((int)unaff_A6 + 0x5f) = bVar1 & 0xbf;
    if ((bVar1 & 0x40) != 0) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    if ((DAT_0010f000 & 0x80) != 0) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    }
    candidate_72c74();
    bVar5 = false;
    candidate_aa760();
    if (!bVar5) {
      *unaff_A6 = candidate_72930;
    }
    bVar5 = false;
    candidate_aa9e4();
    if ((bVar5) || ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      candidate_aa7fc();
      *unaff_A6 = candidate_72930;
    }
  }
  else {
    candidate_4256();
    candidate_1b1dc();
    candidate_72d52();
    *unaff_A6 = 0x72950;
    candidate_1436a();
    candidate_72bfc();
    uVar4 = CARRY2((ushort)*(byte *)(unaff_A6 + 0x1e) * 2,(ushort)*(byte *)(unaff_A6 + 0x1e) * 2);
    uVar3 = candidate_aa610();
    if ((bool)uVar4) {
      *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar3;
      uVar4 = false;
      uVar3 = candidate_aa760();
      if ((bool)uVar4) {
        *(undefined2 *)(unaff_A6 + 0x1c) = uVar3;
        candidate_aa83a();
        *unaff_A6 = candidate_729ac;
        uVar4 = 0;
      }
    }
    candidate_1945c();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_728a2;
    }
  }
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  return;
}
