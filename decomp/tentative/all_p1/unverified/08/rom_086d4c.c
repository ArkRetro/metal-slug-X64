/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x086d4c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_86d4c(void)

{
  char cVar1;
  uint uVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  uVar2 = candidate_29c6();
  if ((*(byte *)((int)unaff_A6 + 0x8b) & 0x40) == 0) {
    if ((*(byte *)((int)unaff_A6 + 0x8b) & 0x80) == 0) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xdf;
      if ((uVar2 & 1) == 0) {
        unaff_A6[0xd] = 0x99999;
        *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffff851f;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
          unaff_A6[0xc] = 0xffff3334;
        }
        else {
          unaff_A6[0xc] = 0xcccc;
        }
      }
      else {
        unaff_A6[0xd] = 0x8cccc;
        *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffff8f5d;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
          unaff_A6[0xc] = 0xfffecccd;
        }
        else {
          unaff_A6[0xc] = 0x13333;
        }
      }
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
      if ((uVar2 & 1) == 0) {
        unaff_A6[0xd] = 0x44444;
        *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffff6e5e;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
          unaff_A6[0xc] = 0xfffeeeef;
        }
        else {
          unaff_A6[0xc] = 0x11111;
        }
      }
      else {
        unaff_A6[0xd] = 0x66666;
        *(undefined4 *)((int)unaff_A6 + 0x72) = 0xfffeb852;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
          unaff_A6[0xc] = 0;
        }
        else {
          unaff_A6[0xc] = 0;
        }
      }
    }
  }
  else if ((*(byte *)((int)unaff_A6 + 0x8b) & 0x80) == 0) {
    unaff_A6[0xd] = 0x66666;
    *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffff5c29;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      unaff_A6[0xc] = 0x26666;
    }
    else {
      unaff_A6[0xc] = 0xfffd999a;
    }
  }
  else {
    unaff_A6[0xd] = 0x66666;
    *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffff92c6;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      unaff_A6[0xc] = 0xfff9999a;
    }
    else {
      unaff_A6[0xc] = 0x66666;
    }
  }
  if (-1 < *(int *)((int)unaff_A6 + 0x72)) {
    *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffffb000;
  }
  candidate_4256();
  unaff_A6[0x14] = 0x87c9e;
  *unaff_A6 = 0x86ee6;
  candidate_87892();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_86f04;
  }
  cVar1 = *(char *)((int)unaff_A6 + 0x87);
  *(char *)((int)unaff_A6 + 0x87) = cVar1 + -1;
  if (cVar1 == '\0') {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0;
  }
  *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
  if (*(short *)(unaff_A6 + 0x20) < 0) {
    *(undefined2 *)(unaff_A6 + 0x20) = 0;
  }
  bVar3 = false;
  candidate_4c24();
  if ((bVar3) && ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_871aa;
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_873c0;
  }
  bVar3 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar3) {
    *unaff_A6 = candidate_873c0;
  }
  return;
}
