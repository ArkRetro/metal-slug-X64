/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aa5ac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_aa5ac(void)

{
  ushort uVar1;
  int iVar2;
  int unaff_A5;
  int unaff_A6;
  undefined8 uVar3;

  if (*(short *)(unaff_A6 + 0x70) == 0) {
    iVar2 = unaff_A5 + 0x4608;
  }
  else {
    iVar2 = unaff_A5 + 0x462c;
  }
  *(undefined4 *)(unaff_A6 + 0x38) = *(undefined4 *)(iVar2 + 6);
  *(undefined4 *)(unaff_A6 + 0x28) = *(undefined4 *)(iVar2 + 10);
  *(undefined4 *)(unaff_A6 + 0x2c) = *(undefined4 *)(iVar2 + 0xe);
  uVar1 = *(ushort *)(iVar2 + 4);
  *(uint *)(unaff_A6 + 0x30) = (uint)*(ushort *)(iVar2 + 2) << 0x10;
  *(uint *)(unaff_A6 + 0x34) = (uint)uVar1 << 0x10;
  uVar3 = candidate_143ee();
  *(undefined4 *)(unaff_A6 + 0x30) = 0;
  *(undefined4 *)(unaff_A6 + 0x34) = 0;
  return CONCAT44(CONCAT31((int3)((ulonglong)uVar3 >> 0x28),*(undefined1 *)(iVar2 + 0x1d)),
                  CONCAT31((int3)((ulonglong)uVar3 >> 8),*(undefined1 *)(iVar2 + 0x1e)));
}
