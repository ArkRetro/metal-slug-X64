/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04a380.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4a380(void)

{
  int iVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  if ((*(byte *)(unaff_A6 + 0x28) & 0x40) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x9d) = 7;
    *(undefined1 *)((int)unaff_A6 + 0x9e) = 0xff;
  }
  *(undefined1 *)(unaff_A6 + 0x28) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x8a) = 0x100;
  iVar1 = *(int *)((int)unaff_A6 + 0x62);
  if (*(short *)(unaff_A6 + 0xf) < *(short *)((int)unaff_A6 + 0x8a)) {
    iVar1 = -iVar1;
  }
  unaff_A6[0xc] = iVar1;
  if (*(char *)(unaff_A6 + 0x29) == '\0') {
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x20) = uVar2;
  }
  else {
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x20) = uVar2;
  }
  *(undefined2 *)(unaff_A6 + 0x21) = *(undefined2 *)(unaff_A6 + 0x20);
  *unaff_A6 = 0x4a3f0;
  uVar3 = 0;
  candidate_4d6f2();
  candidate_4d66e();
  candidate_4e008();
  candidate_1ac38();
  candidate_427a();
  candidate_4e04e();
  if ((*(byte *)(unaff_A6 + 0x18) & 1) == 0) {
    candidate_4da38();
    if ((bool)uVar3) {
      if (*(char *)((int)unaff_A6 + 0x9d) == '\b') {
        *unaff_A6 = candidate_4a488;
      }
      uVar3 = *(byte *)((int)unaff_A6 + 0x9d) < 4;
      if (*(byte *)((int)unaff_A6 + 0x9d) == 4) {
        *(byte *)(unaff_A6 + 0x28) = *(byte *)(unaff_A6 + 0x28) | 8;
        *unaff_A6 = candidate_4a668;
        uVar3 = 0;
      }
    }
  }
  else {
    uVar3 = *(ushort *)(unaff_A6 + 0xf) < 0x100;
    if ((short)*(ushort *)(unaff_A6 + 0xf) < 0x101) {
      *(byte *)(unaff_A6 + 0x18) = *(byte *)(unaff_A6 + 0x18) & 0xfe;
      uVar3 = 0;
      candidate_4520();
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
    }
  }
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_4a9d4;
  }
  return;
}
