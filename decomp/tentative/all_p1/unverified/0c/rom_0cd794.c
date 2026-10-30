/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cd794.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cd794(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  unaff_A6[0xd] = 0;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0;
  unaff_A6[0xc] = unaff_A6[0xc] << 4;
  *unaff_A6 = 0xcd7ac;
  candidate_cd608();
  candidate_1ac38();
  candidate_427a();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_cd7c0;
  }
  uVar2 = false;
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = *(ushort *)((int)unaff_A6 + 0x76) < *(ushort *)((int)unaff_A6 + 0x4e);
    if (!(bool)uVar2) {
      *(ushort *)((int)unaff_A6 + 0x76) =
           *(ushort *)((int)unaff_A6 + 0x76) - *(short *)(unaff_A6 + 0x1e);
      candidate_9f88();
      *(char *)((int)unaff_A6 + 0x73) = *(char *)((int)unaff_A6 + 0x73) + '\x01';
      uVar2 = 0;
      candidate_9ea8();
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_cd7c0;
    uVar2 = 0;
  }
  candidate_19504();
  if ((bool)uVar2) {
    *unaff_A6 = 0xcd7ee;
  }
  return;
}
