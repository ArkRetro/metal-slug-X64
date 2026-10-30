/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06c03a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6c03a(void)

{
  short sVar1;
  undefined2 uVar2;
  ushort uVar3;
  undefined4 extraout_A1;
  short *extraout_A1_00;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_6cc68();
  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x21) = uVar2;
  candidate_6ca12();
  *(undefined4 *)((int)unaff_A6 + 0x6e) = extraout_A1;
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0x3c;
  *(undefined2 *)(unaff_A6 + 0x22) = 0;
  *unaff_A6 = 0x6c066;
  sVar1 = *(short *)(unaff_A6 + 0x22) + -1;
  *(short *)(unaff_A6 + 0x22) = sVar1;
  if (sVar1 < 0) {
    *(undefined2 *)(unaff_A6 + 0x22) = 4;
    candidate_6c9aa();
    *(short **)((int)unaff_A6 + 0x6e) = extraout_A1_00;
    if (*extraout_A1_00 == -1) {
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x92) = 0;
      *unaff_A6 = 0x6c0c0;
      uVar4 = 0;
      candidate_6ca80();
      candidate_427a();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_6be00;
        uVar4 = 0;
      }
      goto LAB_0006c1f2;
    }
  }
  sVar1 = *(short *)((int)unaff_A6 + 0x86);
  uVar4 = sVar1 == 0;
  *(short *)((int)unaff_A6 + 0x86) = sVar1 + -1;
  candidate_6ca5c();
  candidate_6ca80();
  candidate_427a();
LAB_0006c1f2:
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0xff;
    uVar3 = candidate_260c8();
    uVar4 = uVar3 < *(ushort *)((int)unaff_A6 + 0x4e);
    if ((short)*(ushort *)((int)unaff_A6 + 0x4e) < (short)uVar3) {
      *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 2;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) & 0xfd;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_6c0d8;
    uVar4 = 0;
  }
  candidate_1945c();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_6c14c;
  }
  candidate_140c0();
  return;
}
