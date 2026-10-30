/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cd500.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cd500(void)

{
  ushort uVar1;
  short sVar2;
  int iVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_1b1dc();
  uVar1 = candidate_260c8();
  *(ushort *)(unaff_A6 + 0x2a) = uVar1;
  if ((*(byte *)((int)unaff_A6 + 0x7d) & 1) != 0) {
    uVar1 = -uVar1;
  }
  unaff_A6[0xc] = (int)((uint)uVar1 << 0x10) >> 8;
  iVar3 = 0xceb2c;
  if (*(char *)((int)unaff_A6 + 0xaa) != '\0') {
    iVar3 = 0xceb3c;
  }
  unaff_A6[0x28] = *(undefined4 *)(iVar3 + (short)((*(byte *)((int)unaff_A6 + 0x7e) & 3) << 2));
  *(undefined2 *)(unaff_A6 + 0x29) = 0;
  *(undefined1 *)((int)unaff_A6 + 0xa6) = 0;
  candidate_4256();
  *unaff_A6 = 0xcd56a;
  candidate_21288();
  *(ushort *)((int)unaff_A6 + 0x6e) = (ushort)(*(byte *)((int)unaff_A6 + 0xaa) >> 3);
  candidate_427a();
  sVar2 = candidate_49e8();
  if (sVar2 != 0) {
    *unaff_A6 = candidate_cd7c0;
  }
  uVar4 = false;
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar4 = *(ushort *)((int)unaff_A6 + 0x76) < *(ushort *)((int)unaff_A6 + 0x4e);
    if (!(bool)uVar4) {
      *(ushort *)((int)unaff_A6 + 0x76) =
           *(ushort *)((int)unaff_A6 + 0x76) - *(short *)(unaff_A6 + 0x1e);
      candidate_9f88();
      *(char *)((int)unaff_A6 + 0x73) = *(char *)((int)unaff_A6 + 0x73) + '\x01';
      uVar4 = 0;
      candidate_9ea8();
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_cd7c0;
    uVar4 = 0;
  }
  candidate_19504();
  if ((bool)uVar4) {
    *unaff_A6 = 0xcd7ee;
  }
  return;
}
