/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07c63c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7c63c(void)

{
  ushort uVar1;
  undefined2 uVar2;
  ushort uVar3;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x86) = uVar2;
  bVar4 = false;
  candidate_7cdc4();
  if (bVar4) {
    *(ushort *)((int)unaff_A6 + 0x8f) = *(ushort *)((int)unaff_A6 + 0x8f) & 3;
    candidate_4256();
    *unaff_A6 = 0x7c564;
    candidate_1abc8();
    candidate_427a();
    *(short *)(unaff_A6 + 0x21) = *(short *)(unaff_A6 + 0x21) + -1;
    uVar5 = false;
    if ((*(short *)(unaff_A6 + 0x21) < 1) && (uVar3 = candidate_7cd3c(), !(bool)uVar5)) {
      uVar2 = 1;
      uVar1 = *(ushort *)((int)unaff_A6 + 0x8f);
      uVar5 = uVar3 < uVar1;
      if (uVar3 == uVar1) {
        candidate_7cdc4();
        if (!(bool)uVar5) {
          *unaff_A6 = candidate_7c63c;
          uVar5 = 0;
        }
      }
      else {
        if ((short)uVar3 < (short)uVar1) {
          uVar2 = 0xffff;
        }
        *(undefined2 *)(unaff_A6 + 0x24) = uVar2;
        *unaff_A6 = candidate_7c5ba;
        uVar5 = 0;
      }
    }
  }
  else {
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x21) = uVar2;
    *(ushort *)((int)unaff_A6 + 0x8f) = *(ushort *)((int)unaff_A6 + 0x8f) & 3;
    if (*(short *)((int)unaff_A6 + 0x86) < 1) {
      candidate_4256();
    }
    else if (*(short *)(unaff_A6 + 0x21) < 0x2d) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *unaff_A6 = 0x7c6de;
    uVar5 = 0;
    candidate_1abc8();
    candidate_7cd8a();
    candidate_427a();
    if ((bool)uVar5) {
      *unaff_A6 = 0x7c64c;
      *(short *)((int)unaff_A6 + 0x86) = *(short *)((int)unaff_A6 + 0x86) + -1;
      uVar5 = 0;
      if (*(short *)((int)unaff_A6 + 0x86) < 1) {
        uVar2 = candidate_260c8();
        *(undefined2 *)(unaff_A6 + 0x21) = uVar2;
        uVar3 = candidate_29c6();
        *(ushort *)(unaff_A6 + 0x21) = (uVar3 & 0x1f) + *(short *)(unaff_A6 + 0x21);
        *unaff_A6 = candidate_7c548;
        uVar5 = 0;
      }
    }
  }
  func_0x0007cce0();
  candidate_4c24();
  if ((bool)uVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(undefined1 *)(unaff_A6 + 0x22) = 10;
  }
  *(char *)(unaff_A6 + 0x22) = *(char *)(unaff_A6 + 0x22) + -1;
  if (*(char *)(unaff_A6 + 0x22) < '\x01') {
    *(undefined1 *)(unaff_A6 + 0x22) = 0;
  }
  if (*(char *)(unaff_A6 + 0x22) != '\0') {
    *(char *)((int)unaff_A6 + 0x8a) = *(char *)((int)unaff_A6 + 0x8a) + '\x01';
    *(short *)(unaff_A6 + 0xb) =
         *(short *)((short)((*(byte *)((int)unaff_A6 + 0x8a) & 3) * 2) + 0xe4658) +
         *(short *)(unaff_A6 + 0xb);
  }
  bVar4 = false;
  if ((*(char *)(unaff_A6 + 0x26) != '\0') &&
     (bVar4 = *(char *)(unaff_A6[3] + 0x60) != -1, *(char *)(unaff_A6[3] + 0x60) == -1)) {
    *unaff_A6 = candidate_7c72e;
    bVar4 = false;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_7c85a;
    bVar4 = false;
  }
  candidate_19504();
  if (bVar4) {
    *unaff_A6 = 0x7c946;
  }
  return;
}
