/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07c548.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7c548(void)

{
  ushort uVar1;
  ushort uVar2;
  undefined2 uVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

  *(ushort *)((int)unaff_A6 + 0x8f) = *(ushort *)((int)unaff_A6 + 0x8f) & 3;
  candidate_4256();
  *unaff_A6 = 0x7c564;
  candidate_1abc8();
  candidate_427a();
  *(short *)(unaff_A6 + 0x21) = *(short *)(unaff_A6 + 0x21) + -1;
  uVar4 = false;
  if (*(short *)(unaff_A6 + 0x21) < 1) {
    uVar2 = candidate_7cd3c();
    if (!(bool)uVar4) {
      uVar3 = 1;
      uVar1 = *(ushort *)((int)unaff_A6 + 0x8f);
      uVar4 = uVar2 < uVar1;
      if (uVar2 == uVar1) {
        candidate_7cdc4();
        if (!(bool)uVar4) {
          *unaff_A6 = candidate_7c63c;
          uVar4 = 0;
        }
      }
      else {
        if ((short)uVar2 < (short)uVar1) {
          uVar3 = 0xffff;
        }
        *(undefined2 *)(unaff_A6 + 0x24) = uVar3;
        *unaff_A6 = candidate_7c5ba;
        uVar4 = 0;
      }
    }
  }
  func_0x0007cce0();
  candidate_4c24();
  if ((bool)uVar4) {
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
  bVar5 = false;
  if ((*(char *)(unaff_A6 + 0x26) != '\0') &&
     (bVar5 = *(char *)(unaff_A6[3] + 0x60) != -1, *(char *)(unaff_A6[3] + 0x60) == -1)) {
    *unaff_A6 = candidate_7c72e;
    bVar5 = false;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_7c85a;
    bVar5 = false;
  }
  candidate_19504();
  if (bVar5) {
    *unaff_A6 = 0x7c946;
  }
  return;
}
