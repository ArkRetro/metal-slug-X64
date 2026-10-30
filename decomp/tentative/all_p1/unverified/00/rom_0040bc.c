/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0040bc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_40bc(void)

{
  int iVar1;
  int *piVar2;
  undefined4 *puVar3;
  ushort in_D0w;
  undefined4 in_A0;
  int unaff_A6;

  puVar3 = puRam001063d4;
  if (_DAT_001063d8 <= in_D0w) {
    return;
  }
  if (puRam001063d4 != (undefined4 *)0x0) {
    _DAT_001063d8 = _DAT_001063d8 - 1;
  }
  piVar2 = puRam001063d4 + 3;
  puRam001063d4 = (undefined4 *)puRam001063d4[2];
  *piVar2 = unaff_A6;
  *(short *)(puVar3 + 4) = *(short *)(unaff_A6 + 0x10) + 1;
  iVar1 = *(int *)(unaff_A6 + 8);
  puVar3[1] = unaff_A6;
  puVar3[2] = iVar1;
  *(undefined4 **)(iVar1 + 4) = puVar3;
  *(undefined4 **)(unaff_A6 + 8) = puVar3;
  *puVar3 = in_A0;
  *(undefined1 *)(puVar3 + 0x1a) = *(undefined1 *)(unaff_A6 + 0x68);
  return;
}
