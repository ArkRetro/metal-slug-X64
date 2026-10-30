/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a6f30.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_a6f30(void)

{
  short sVar1;
  ushort extraout_D0u;
  int iVar2;
  ushort extraout_D1u;
  int iVar3;
  int extraout_A0;
  int extraout_A0_00;

  candidate_c6f34();
  iVar2 = (uint)extraout_D0u << 0x10;
  iVar3 = (uint)extraout_D1u << 0x10;
  if (*(int *)(extraout_A0 + 0x2e) != 0) {
    candidate_c6bde();
    func_0x000c714e();
    iVar2 = -0x400000 - *(int *)(extraout_A0_00 + 0x14);
    sVar1 = (short)(iVar2 >> 0x14);
    iVar2 = CONCAT22((short)iVar2,sVar1);
    *(short *)(extraout_A0_00 + 0x36) = sVar1;
    iVar3 = 0x800000 - *(int *)(extraout_A0_00 + 0x18);
    sVar1 = (short)(iVar3 >> 0x14);
    iVar3 = CONCAT22((short)iVar3,sVar1);
    *(short *)(extraout_A0_00 + 0x38) = sVar1;
  }
  return CONCAT44(iVar2,iVar3);
}
