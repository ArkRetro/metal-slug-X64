/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06fc74.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6fc74(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  short sVar3;
  byte bVar4;
  int unaff_A6;

  uVar1 = *(undefined4 *)(unaff_A6 + 0x30);
  uVar2 = *(undefined4 *)(unaff_A6 + 0x34);
  if (*(char *)(unaff_A6 + 0x87) == '\f') {
    sVar3 = *(short *)((short)((ushort)*(byte *)(unaff_A6 + 0x89) * 2) + 0x29fc);
    bVar4 = *(byte *)(unaff_A6 + 0x89) + 3;
    *(byte *)(unaff_A6 + 0x89) = bVar4;
    *(int *)(unaff_A6 + 0x34) =
         ((int)((uint)(ushort)(*(short *)((short)((ushort)bVar4 * 2) + 0x29fc) - sVar3) << 0x10) >>
         6) + *(int *)(unaff_A6 + 0x34);
  }
  candidate_143ee();
  *(undefined4 *)(unaff_A6 + 0x34) = uVar2;
  *(undefined4 *)(unaff_A6 + 0x30) = uVar1;
  return;
}
