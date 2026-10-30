/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0190bc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_190bc(void)

{
  uint uVar1;
  uint uVar2;
  int unaff_A5;

  uVar1 = 0;
  uVar2 = 0;
  if (*(char *)(unaff_A5 + 0x7db6) != '\0') {
    uVar1 = (uint)(byte)FUN_0000435c[unaff_A5];
    uVar2 = (uint)*(byte *)(unaff_A5 + 0x435d);
  }
  if (*(char *)(unaff_A5 + 0x7db7) != '\0') {
    uVar1 = (uint)(byte)(*(byte *)(unaff_A5 + 0x4362) | (byte)uVar1);
    uVar2 = (uint)(byte)(*(byte *)(unaff_A5 + 0x4363) | (byte)uVar2);
  }
  return CONCAT44(uVar1,uVar2);
}
