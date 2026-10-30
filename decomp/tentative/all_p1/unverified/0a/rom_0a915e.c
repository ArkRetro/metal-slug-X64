/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a915e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a915e(void)

{
  int iVar1;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  candidate_4256();
  unaff_A6[0x14] = 0xe64e0;
  bVar2 = false;
  candidate_a9c96();
  if (bVar2) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  }
  iVar1 = (uint)*(ushort *)((int)unaff_A6 + 0x86) * 0x100;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    iVar1 = (uint)*(ushort *)((int)unaff_A6 + 0x86) * -0x100;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar1;
  *(undefined2 *)(unaff_A6 + 0x22) = 0;
  *unaff_A6 = 0xa91b0;
  uVar3 = 0;
  if (*(char *)((int)unaff_A6 + 0x95) == '\0') {
    candidate_8ccd4();
  }
  else {
    unaff_A6[0xc] = *(undefined4 *)((int)unaff_A6 + 0x62);
    unaff_A6[0xd] = 0;
    uVar3 = 0;
    candidate_1ac38();
  }
  candidate_427a();
  candidate_a9b1c();
  if ((bool)uVar3) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    unaff_A6[0x14] = 0;
    *unaff_A6 = candidate_a8fc2;
    uVar3 = false;
  }
  else {
    candidate_a9b60();
    if ((bool)uVar3) {
      bVar2 = false;
      candidate_a9c18();
      if (!bVar2) {
        *unaff_A6 = candidate_a927e;
      }
      *(undefined2 *)(unaff_A6 + 0x22) = 0;
      uVar3 = false;
    }
    else {
      *(short *)(unaff_A6 + 0x22) = *(short *)(unaff_A6 + 0x22) + 1;
      if (7 < (short)*(ushort *)(unaff_A6 + 0x22)) {
        *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
        unaff_A6[0x14] = 0;
        *unaff_A6 = candidate_a8fc2;
      }
      uVar3 = *(ushort *)(unaff_A6 + 0x22) < 8;
    }
  }
  candidate_1af9a();
  if ((bool)uVar3) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    unaff_A6[0x14] = 0;
    *unaff_A6 = candidate_a8fc2;
    uVar3 = 0;
  }
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_a93dc;
  }
  candidate_a9a56();
  return;
}
