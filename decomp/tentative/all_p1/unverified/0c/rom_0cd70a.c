/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cd70a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cd70a(void)

{
  short sVar1;
  ushort uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  uVar2 = candidate_29c6();
  *(ushort *)((int)unaff_A6 + 0x92) = (uVar2 & 0x20) + 0x10;
  uVar2 = 0x80;
  if ((*(byte *)((int)unaff_A6 + 0x7d) & 1) == 0) {
    uVar2 = 0xff80;
  }
  unaff_A6[0xc] = (int)((uint)uVar2 << 0x10) >> 8;
  *(ushort *)((int)unaff_A6 + 0x6e) = (ushort)(*(byte *)((int)unaff_A6 + 0xaa) >> 3);
  unaff_A6[0xd] = 0xfffa0000;
  candidate_4256();
  *unaff_A6 = 0xcd764;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_cd794;
  }
  candidate_427a();
  if (*(short *)(unaff_A6 + 0x10) < 0x151) {
    *unaff_A6 = candidate_cd794;
  }
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_cd7c0;
  }
  uVar3 = false;
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = *(ushort *)((int)unaff_A6 + 0x76) < *(ushort *)((int)unaff_A6 + 0x4e);
    if (!(bool)uVar3) {
      *(ushort *)((int)unaff_A6 + 0x76) =
           *(ushort *)((int)unaff_A6 + 0x76) - *(short *)(unaff_A6 + 0x1e);
      candidate_9f88();
      *(char *)((int)unaff_A6 + 0x73) = *(char *)((int)unaff_A6 + 0x73) + '\x01';
      uVar3 = 0;
      candidate_9ea8();
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_cd7c0;
    uVar3 = 0;
  }
  candidate_19504();
  if ((bool)uVar3) {
    *unaff_A6 = 0xcd7ee;
  }
  return;
}
